/*==============================================================================
** Copyright (C) 2024-2027 WingSummer
**
** You can redistribute this file and/or modify it under the terms of the
** BSD 3-Clause.
**
** THIS FILE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
** =============================================================================
*/

#ifndef IWINGPLUGIN_H
#define IWINGPLUGIN_H

#include "WingPlugin/iwingplugincalls.h"
#include "hexeditorcontext.h"
#include "iwingpluginbase.h"
#include "wingeditorviewwidget.h"
#include "wingplugin_global.h"

namespace WingHex {

class IWingPlugin;

struct WINGPLUGIN_EXPORT SenderInfo {
    QString plgcls;
    QString puid;
    QVariant meta;
};

class SettingPage;

class WINGPLUGIN_EXPORT IWingPlugin : public IWingPluginBase,
                                      public IWingPluginCalls,
                                      public IWingHexEditorInterface {
    Q_OBJECT
public:
    IWingPlugin();

public:
    enum class RegisteredEvent : uint {
        None,
        AppReady = 1u,
        AppClosing = 1u << 1,
        SelectionChanged = 1u << 2,
        CursorPositionChanged = 1u << 3,
        FileOpened = 1u << 4,
        FileSaved = 1u << 5,
        FileSwitched = 1u << 6,
        FileClosed = 1u << 7,
        ScriptPragma = 1u << 8,
        PluginFileOpened = 1u << 9,
        PluginFileClosed = 1u << 10,
        ScriptPragmaInit = 1u << 11,
        HexEditorViewPaint = 1u << 12
    };
    Q_DECLARE_FLAGS(RegisteredEvents, RegisteredEvent)

    enum class PluginFileEvent {
        Opened,
        Saved,
        Exported,
        Switched,
        Closed,
        PluginOpened,
        PluginClosed
    };

    enum class FileType { Invalid, File, Extension };
    Q_ENUM(FileType)

public:
    ScriptCallError generateScriptCallError(int errCode, const QString &msg);

    QVariant getScriptCallError(int errCode, const QString &msg);

public:
    virtual ~IWingPlugin() = default;

    virtual RegisteredEvents registeredEvents() const;

public:
    template <typename... Event>
    inline static RegisteredEvents packupEvent(Event... evs) {
        static_assert((std::is_same_v<Event, RegisteredEvent> && ...),
                      "All arguments must be RegisteredEvent!");
        return WingHex::packup<QFlags, RegisteredEvent>(
            std::forward<Event>(evs)...);
    }

public:
    virtual QList<WingRibbonToolBoxInfo> registeredRibbonTools() const;
    virtual QList<SettingPage *> registeredSettingPages() const;

    virtual QList<QSharedPointer<WingEditorViewWidget::Creator>>
    registeredEditorViewWidgets() const;

public:
    virtual void eventSelectionChanged(const QByteArrayList &selections,
                                       bool isPreview);

    virtual void eventCursorPositionChanged(const WingHex::HexPosition &pos);

    virtual void eventPluginFile(PluginFileEvent e, FileType type,
                                 const QUrl &newfileName, int handle,
                                 const QUrl &oldfileName);

    virtual void eventReady();

    virtual bool eventClosing();

    virtual void onPaintHexEditorView(QPainter *painter, QWidget *w,
                                      HexEditorContext *context);

public:
    virtual std::optional<PragmaResult>
    eventOnScriptPragma(const QString &script, const QStringList &comments);

    virtual void eventOnScriptPragmaInit();

    // WingPluginCallConvertor interface
protected:
    virtual const QObject *getSender() const override;

    // WingPluginCalls interface
protected:
    virtual CallTable callTable() const override;
    virtual QObject *callReceiver() const override;

private:
    WingPluginCallsCore *_core;
};

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::SenderInfo)
Q_DECLARE_INTERFACE(WingHex::IWingPlugin, "com.wingsummer.iwingplugin")

#endif // IWINGPLUGIN_H

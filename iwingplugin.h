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
#include "hexeditorpalette.h"
#include "iwingpluginbase.h"
#include "wingeditorviewwidget.h"
#include "wingplugin_global.h"

#include <QShortcut>
#include <QToolButton>

namespace WingHex {

class IWingPlugin;

struct WINGPLUGIN_EXPORT SenderInfo {
    QString plgcls;
    QString puid;
    QVariant meta;
};

struct WINGPLUGIN_EXPORT WingRibbonCatagories {
    inline static QString FILE = QStringLiteral("File");
    inline static QString EDIT = QStringLiteral("Edit");
    inline static QString VIEW = QStringLiteral("View");
    inline static QString SCRIPT = QStringLiteral("Script");
    inline static QString PLUGIN = QStringLiteral("Plugin");
    inline static QString SETTING = QStringLiteral("Setting");
    inline static QString ABOUT = QStringLiteral("About");
};

struct WINGPLUGIN_EXPORT WingRibbonToolBoxInfo {
    QString catagory;
    QString displayName;

    struct WINGPLUGIN_EXPORT Toolbox {
        QString name;
        QList<QToolButton *> tools;
    };
    QList<Toolbox> toolboxs;
};

class SettingPage;

class WINGPLUGIN_EXPORT IWingPlugin : public IWingPluginBase,
                                      public IWingPluginCalls {
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
    // some helper class to make it more C++
    inline static RegisteredEvents packupEvent(RegisteredEvent evs...) {
        return WingHex::packup<QFlags, RegisteredEvent>(evs);
    }

    inline static WingRibbonToolBoxInfo
    createRibbonToolBox(QString catagory, QString displayName) {
        WingRibbonToolBoxInfo info;
        info.catagory = catagory;
        info.displayName = displayName;
        return info;
    }

    inline static WingRibbonToolBoxInfo
    createRibbonToolBox(QString catagory, QString displayName,
                        const WingRibbonToolBoxInfo::Toolbox &toolbox...) {
        WingRibbonToolBoxInfo info;
        info.catagory = catagory;
        info.displayName = displayName;
        info.toolboxs =
            WingHex::packup<QList, WingRibbonToolBoxInfo::Toolbox>(toolbox);
        return info;
    }

    inline static WingRibbonToolBoxInfo
    createRibbonToolBox(QString catagory,
                        const WingRibbonToolBoxInfo::Toolbox &toolbox...) {
        WingRibbonToolBoxInfo info;
        info.catagory = catagory;
        info.toolboxs =
            WingHex::packup<QList, WingRibbonToolBoxInfo::Toolbox>(toolbox);
        return info;
    }

    inline static WingRibbonToolBoxInfo::Toolbox
    createToolBox(const QString &name, QToolButton *tools...) {
        WingRibbonToolBoxInfo::Toolbox tb;
        tb.name = name;
        tb.tools = WingHex::packup<QList, QToolButton *>(tools);
        return tb;
    }

    template <typename... Args>
    using CompatibleToolButtonSlotArgs = std::enable_if_t<std::conjunction_v<
        std::disjunction<
            std::is_same<Args, Qt::ConnectionType>,
            std::negation<std::is_convertible<Args, QKeySequence>>>...,
        std::negation<std::is_convertible<Args, QIcon>>...,
        std::negation<std::is_convertible<Args, const char *>>...,
        std::negation<std::is_convertible<Args, QString>>...>>;

    template <typename... Args,
              typename = CompatibleToolButtonSlotArgs<Args...>>
    inline static QToolButton *
    createToolButton(const QIcon &icon, const QString &text,
                     const QKeySequence &shortcut, Args &&...args) {
        auto tb = createToolButton(icon, text, std::forward<Args>(args)...);
        if (!shortcut.isEmpty()) {
            auto shortCut = new QShortcut(shortcut, tb);
            shortCut->setContext(Qt::WindowShortcut);
            connect(shortCut, &QShortcut::activated, tb, &QToolButton::click);
        }
        return tb;
    }

    template <typename... Args,
              typename = CompatibleToolButtonSlotArgs<Args...>>
    inline static QToolButton *
    createToolButton(const QIcon &icon, const QString &text, Args &&...args) {
        auto tb = new QToolButton;
        tb->setIcon(icon);
        tb->setText(text);
        connect(tb, &QToolButton::clicked, std::forward<Args>(args)...);
        return tb;
    }

public:
    virtual QMenu *registeredHexContextMenu() const;
    virtual QList<WingRibbonToolBoxInfo> registeredRibbonTools() const;
    virtual QList<SettingPage *> registeredSettingPages() const;

    virtual QList<QSharedPointer<WingEditorViewWidget::Creator>>
    registeredEditorViewWidgets() const;

public:
    virtual void eventSelectionChanged(const QByteArrayList &selections,
                                       bool isPreview);

    virtual void eventCursorPositionChanged(const WingHex::HexPosition &pos);

    virtual void eventPluginFile(PluginFileEvent e, FileType type,
                                 const QString &newfileName, int handle,
                                 const QString &oldfileName);

    virtual void eventReady();

    virtual bool eventClosing();

    virtual void onPaintHexEditorView(QPainter *painter,
                                      HexEditorPalette *palette);

public:
    virtual bool eventOnScriptPragma(const QString &script,
                                     const QStringList &comments);

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

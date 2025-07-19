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

#ifndef IWINGHEXEDITORPLUGIN_H
#define IWINGHEXEDITORPLUGIN_H

#include "WingPlugin/hexeditorcontext.h"
#include "WingPlugin/iwingpluginbase.h"

#include <QPoint>

namespace WingHex {

class SettingPage;

class WINGPLUGIN_EXPORT IWingHexEditorPlugin : public IWingPluginCoreBase {
    Q_OBJECT

public:
    explicit IWingHexEditorPlugin();

public:
    virtual const QString comment() const = 0;

    virtual QMenu *registeredHexContextMenu() const;
    virtual QList<WingRibbonToolBoxInfo> registeredRibbonTools() const;
    virtual QList<SettingPage *> registeredSettingPages() const;

    // additional offset that applies HexEditor
    virtual QMargins contentMargins(WingHex::HexEditorContext *context) const;

public:
    virtual void onPaintEvent(QPainter *painter, const QWidget *w,
                              WingHex::HexEditorContext *context);

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

}; // namespace WingHex

Q_DECLARE_INTERFACE(WingHex::IWingHexEditorPlugin, "com.wingsummer.iwinghexext")

#endif // IWINGHEXEDITORPLUGIN_H

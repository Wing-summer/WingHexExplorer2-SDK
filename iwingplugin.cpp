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

#include "iwingplugin.h"

using namespace WingHex;

IWingPlugin::IWingPlugin()
    : IWingPluginBase(), IWingPluginCalls(), _core(new WingPluginCallsCore) {
    this->installEventFilter(_core);
}

QVariant IWingPlugin::getScriptCallError(int errCode, const QString &msg) {
    ScriptCallError err;

    err.errorCode = errCode;
    err.errmsg = msg;

    return QVariant::fromValue(err);
}

IWingPlugin::RegisteredEvents IWingPlugin::registeredEvents() const {
    return RegisteredEvent::None;
}

QMenu *IWingPlugin::registeredHexContextMenu() const { return nullptr; }

QList<WingRibbonToolBoxInfo> IWingPlugin::registeredRibbonTools() const {
    return {};
}

QList<SettingPage *> IWingPlugin::registeredSettingPages() const { return {}; }

QList<QSharedPointer<WingEditorViewWidget::Creator>>
IWingPlugin::registeredEditorViewWidgets() const {
    return {};
}

void IWingPlugin::eventSelectionChanged(const QByteArrayList &selections,
                                        bool isPreview) {
    Q_UNUSED(selections);
    Q_UNUSED(isPreview);
}

void IWingPlugin::eventCursorPositionChanged(const HexPosition &pos) {
    Q_UNUSED(pos);
}

void IWingPlugin::eventPluginFile(PluginFileEvent e, FileType type,
                                  const QString &newfileName, int handle,
                                  const QString &oldfileName) {
    Q_UNUSED(e);
    Q_UNUSED(newfileName);
    Q_UNUSED(oldfileName);
    Q_UNUSED(handle);
    Q_UNUSED(type);
}

void IWingPlugin::eventReady() {}

bool IWingPlugin::eventClosing() { return true; }

void IWingPlugin::onPaintHexEditorView(QPainter *painter, QWidget *w,
                                       HexEditorContext *context) {
    Q_UNUSED(painter);
    Q_UNUSED(w);
    Q_UNUSED(context);
}

bool IWingPlugin::eventOnScriptPragma(const QString &script,
                                      const QStringList &comments) {
    Q_UNUSED(script);
    Q_UNUSED(comments);
    return false;
}

void IWingPlugin::eventOnScriptPragmaInit() {}

const QObject *IWingPlugin::getSender() const { return this; }

CallTable IWingPlugin::callTable() const { return _core->callTable(); }

QObject *IWingPlugin::callReceiver() const { return _core->callReceiver(); }

ScriptCallError IWingPlugin::generateScriptCallError(int errCode,
                                                     const QString &msg) {
    ScriptCallError err;

    err.errorCode = errCode;
    err.errmsg = msg;

    return err;
}

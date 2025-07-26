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

#include "iwinghexeditorplugin.h"

using namespace WingHex;

IWingHexEditorPlugin::IWingHexEditorPlugin()
    : IWingPluginCoreBase(), _core(new WingPluginCallsCore) {
    this->installEventFilter(_core);
}

QList<WingRibbonToolBoxInfo>
IWingHexEditorPlugin::registeredRibbonTools() const {
    return {};
}

QList<SettingPage *> IWingHexEditorPlugin::registeredSettingPages() const {
    return {};
}

QMargins IWingHexEditorPlugin::contentMargins(HexEditorContext *context) const {
    Q_UNUSED(context);
    return {};
}

bool IWingHexEditorPlugin::disableOriginPainting() const { return false; }

void IWingHexEditorPlugin::onPaintEvent(QPainter *painter, const QWidget *w,
                                        HexEditorContext *context) {
    Q_UNUSED(painter);
    Q_UNUSED(w);
    Q_UNUSED(context);
}

const QObject *IWingHexEditorPlugin::getSender() const { return this; }

CallTable IWingHexEditorPlugin::callTable() const { return _core->callTable(); }

QObject *IWingHexEditorPlugin::callReceiver() const {
    return _core->callReceiver();
}

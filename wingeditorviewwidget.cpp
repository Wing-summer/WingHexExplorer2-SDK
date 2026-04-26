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

#include "wingeditorviewwidget.h"
#include "WingPlugin/wingplugincalls.h"

WingHex::WingEditorViewWidget::WingEditorViewWidget(const Creator *creator,
                                                    QWidget *parent)
    : QWidget(parent), IWingEditorViewCalls(), _core(new WingPluginCallsCore),
      _creator(creator) {
    this->installEventFilter(_core);
}

bool WingHex::WingEditorViewWidget::isWorkSpace() const {
    return viewProperty("isWorkSpace").toBool();
}

bool WingHex::WingEditorViewWidget::isNewFile() const {
    return viewProperty("isNewFile").toBool();
}

bool WingHex::WingEditorViewWidget::isCloneFile() const {
    return viewProperty("isCloneFile").toBool();
}

bool WingHex::WingEditorViewWidget::isExtensionFile() const {
    return viewProperty("isExtensionFile").toBool();
}

bool WingHex::WingEditorViewWidget::isCommonFile() const {
    return viewProperty("isCommonFile").toBool();
}

bool WingHex::WingEditorViewWidget::hasCloneChildren() const {
    return viewProperty("hasCloneChildren").toBool();
}

void WingHex::WingEditorViewWidget::raiseView() {
    auto ptr = callReceiver();
    auto id = _creator->id();
    if (ptr && !id.isEmpty()) {
        QMetaObject::invokeMethod(ptr, "raiseAndSwitchView",
                                  Qt::DirectConnection, id);
    }
}

void WingHex::WingEditorViewWidget::toggled(bool isVisible) {
    Q_UNUSED(isVisible);
}

void WingHex::WingEditorViewWidget::loadState(const QByteArray &state) {
    Q_UNUSED(state);
}

bool WingHex::WingEditorViewWidget::hasUnsavedState() { return false; }

QByteArray WingHex::WingEditorViewWidget::saveState() { return {}; }

void WingHex::WingEditorViewWidget::setSaved() {}

bool WingHex::WingEditorViewWidget::onClosing() { return true; }

void WingHex::WingEditorViewWidget::onWorkSpaceNotify(bool isWorkSpace) {
    Q_UNUSED(isWorkSpace);
}

QVariant
WingHex::WingEditorViewWidget::viewProperty(const char *property) const {
    return callReceiver()->property(property);
}

const QObject *WingHex::WingEditorViewWidget::getSender() const { return this; }

QObject *WingHex::WingEditorViewWidget::callReceiver() const {
    return _core->callReceiver();
}

WingHex::CallTable WingHex::WingEditorViewWidget::callTable() const {
    return _core->callTable();
}

const WingHex::WingEditorViewWidget::Creator *
WingHex::WingEditorViewWidget::creator() const {
    return _creator;
}

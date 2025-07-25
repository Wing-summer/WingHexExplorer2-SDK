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

#include "iwingpluginbase.h"

#include <QCoreApplication>
#include <QDialog>
#include <QMetaMethod>

using namespace WingHex;

IWingPluginBase::IWingPluginBase() : IWingPluginCoreBase() {}

QIcon IWingPluginBase::pluginIcon() const { return {}; }

QList<WingDockWidgetInfo> IWingPluginBase::registeredDockWidgets() const {
    return {};
}

QString WingHex::PLUGINDIR() {
    return QCoreApplication::applicationDirPath() + QStringLiteral("/plugin");
}

QString WingHex::HOSTRESPIMG(const QString &name, const QString &suffix) {
    return QStringLiteral(":/com.wingsummer.winghex/images/") + name + suffix;
}

IWingPluginCoreBase::IWingPluginCoreBase() : IWingPluginBaseCalls() {}

QString IWingPluginCoreBase::retranslate(const QString &str) { return str; }

void IWingPluginCoreBase::onRegisterScriptObj(IWingAngel *o) { Q_UNUSED(o); }

IWingHexEditorInterface::IWingHexEditorInterface() {}

QMenu *IWingHexEditorInterface::registeredHexContextMenu() const {
    return nullptr;
}

void IWingHexEditorInterface::prepareCallEditorContext(
    HexEditorContext *context) {
    Q_UNUSED(context);
}

void IWingHexEditorInterface::finishCallEditorContext(
    HexEditorContext *context) {
    Q_UNUSED(context);
}

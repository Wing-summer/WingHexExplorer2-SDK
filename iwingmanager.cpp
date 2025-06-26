#include "iwingmanager.h"

using namespace WingHex;

IWingManager::IWingManager()
    : IWingPluginCoreBase(), _core(new WingPluginCallsCore) {
    this->installEventFilter(_core);
}

QList<WingRibbonToolBoxInfo> IWingManager::registeredRibbonTools() const {
    return {};
}

QList<SettingPage *> IWingManager::registeredSettingPages() const { return {}; }

bool IWingManager::enterGuard(const QMetaObject *sender, const QString &sig,
                              const QVariantList &params) {
    Q_UNUSED(sender);
    Q_UNUSED(sig);
    Q_UNUSED(params);
    return true;
}

bool IWingManager::onLoadingPlugin(const QString &fileName,
                                   const PluginInfo &info) {
    Q_UNUSED(fileName);
    Q_UNUSED(info);
    return true;
}

const QObject *IWingManager::getSender() const { return this; }

CallTable IWingManager::callTable() const { return _core->callTable(); }

QObject *IWingManager::callReceiver() const { return _core->callReceiver(); }

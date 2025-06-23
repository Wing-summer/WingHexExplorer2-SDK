#include "iwingmanager.h"

using namespace WingHex;

IWingManager::IWingManager() : QObject(), IWingPluginBaseCalls() {}

bool IWingManager::enterGuard(const QMetaObject *sender, const QString &sig,
                              const QVariantList &params) {
    Q_UNUSED(sender);
    Q_UNUSED(sig);
    Q_UNUSED(params);
    return true;
}

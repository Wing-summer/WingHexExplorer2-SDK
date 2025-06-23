#ifndef IWINGMANAGER_H
#define IWINGMANAGER_H

#include "iwingpluginbasecalls.h"

namespace WingHex {

/**
 * @brief The IWingManager class
 * @note a singleton monitor for API with PluginSystem
 */
class WINGPLUGIN_EXPORT IWingManager : public QObject,
                                       public IWingPluginBaseCalls {
    Q_OBJECT
public:
    IWingManager();

public slots:
    bool enterGuard(const QMetaObject *sender, const QString &sig,
                    const QVariantList &params);
};

} // namespace WingHex

Q_DECLARE_INTERFACE(WingHex::IWingManager, "com.wingsummer.iwingmanager")

#endif // IWINGMANAGER_H

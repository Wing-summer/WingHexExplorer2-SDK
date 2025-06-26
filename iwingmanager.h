#ifndef IWINGMANAGER_H
#define IWINGMANAGER_H

#include "WingPlugin/iwingplugin.h"
#include "WingPlugin/settingpage.h"

namespace WingHex {

/**
 * @brief The IWingManager class
 * @note a singleton monitor for API with PluginSystem
 */
class WINGPLUGIN_EXPORT IWingManager : public IWingPluginCoreBase {
    Q_OBJECT
public:
    IWingManager();

public:
    virtual const QString comment() const = 0;

    virtual QList<WingRibbonToolBoxInfo> registeredRibbonTools() const;

    virtual QList<SettingPage *> registeredSettingPages() const;

public slots:
    virtual bool enterGuard(const QMetaObject *sender, const QString &sig,
                            const QVariantList &params);

    virtual bool onLoadingPlugin(const QString &fileName,
                                 const WingHex::PluginInfo &info);

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

Q_DECLARE_INTERFACE(WingHex::IWingManager, "com.wingsummer.iwingmanager")

#endif // IWINGMANAGER_H

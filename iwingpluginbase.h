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

#ifndef IWINGPLUGINBASE_H
#define IWINGPLUGINBASE_H

#include "WingPlugin/iwingpluginbasecalls.h"
#include "wingplugin_global.h"

#include <QSettings>

namespace WingHex {

WINGPLUGIN_EXPORT QString PLUGINDIR();

WINGPLUGIN_EXPORT QString HOSTRESPIMG(
    const QString &name, const QString &suffix = QStringLiteral(".png"));

class PluginPage;
class IWingAngel;

class WINGPLUGIN_EXPORT IWingPluginCoreBase : public QObject,
                                              public IWingPluginBaseCalls {
public:
    IWingPluginCoreBase();

public:
    virtual bool init(const std::unique_ptr<QSettings> &set) = 0;
    virtual void unload(std::unique_ptr<QSettings> &set) = 0;

    virtual QString retranslate(const QString &str);

public:
    virtual void onRegisterScriptObj(WingHex::IWingAngel *o);
};

class WINGPLUGIN_EXPORT IWingPluginBase : public IWingPluginCoreBase {
    Q_OBJECT

public:
    IWingPluginBase();
    virtual ~IWingPluginBase() = default;

public:
    virtual const QString pluginName() const = 0;
    virtual QIcon pluginIcon() const;
    virtual const QString pluginComment() const = 0;

public:
    virtual QList<WingDockWidgetInfo> registeredDockWidgets() const;
};

} // namespace WingHex

#endif // IWINGPLUGINBASE_H

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

#ifndef WINGPLUGIN_GLOBAL_H
#define WINGPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WINGPLUGIN_LIBRARY)
#define WINGPLUGIN_EXPORT Q_DECL_EXPORT
#else
#define WINGPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMetaMethod>
#include <QString>
#include <QVersionNumber>
#include <QWidget>

#ifdef WING_SERVICE
#undef WING_SERVICE
#endif

#define WING_SERVICE Q_INVOKABLE

#ifndef Q_MOC_RUN
#define WING_API
#endif

namespace WingHex {

Q_DECL_UNUSED constexpr auto SDKVERSION = 18;

struct WINGPLUGIN_EXPORT WingDockWidgetInfo {
    QString widgetName;
    QString displayName;
    QWidget *widget = nullptr;
    Qt::DockWidgetArea area = Qt::DockWidgetArea::NoDockWidgetArea;
};

struct WINGPLUGIN_EXPORT WingDependency {
    QString puid;
    QVersionNumber version;
};

struct WINGPLUGIN_EXPORT PluginInfo {
    QString id;
    int SDKVersion;
    QVersionNumber version;
    QString vendor;
    QList<WingDependency> dependencies;
    QString author;
    QString license;
    QString url;
};

enum class AppTheme { Invalid, Dark, Light };

struct WINGPLUGIN_EXPORT FunctionSig {
    QByteArray fnName;
    QVarLengthArray<int, 8> types;

    bool operator==(const FunctionSig &other) const {
        return fnName == other.fnName && types == other.types;
    }
};

inline size_t qHash(const FunctionSig &c, size_t seed) noexcept {
    return qHashMulti(seed, c.fnName, c.types);
}

using CallTable = QHash<FunctionSig, QMetaMethod>;

Q_NAMESPACE_EXPORT(WINGPLUGIN_EXPORT)

enum ErrFile : int {
    Success = 0,
    Error = -1,
    UnSaved = -2,
    Permission = -3,
    NotExist = -4,
    AlreadyOpened = -5,
    IsNewFile = -6,
    WorkSpaceUnSaved = -7,
    ClonedFile = -8,
    InvalidFormat = -9,
    TooManyOpenedFile = -10,
    NotAllowedInNoneGUIThread = -11,
    DevNotFound = -12,
};
Q_ENUM_NS(ErrFile)

enum class SelectionMode : int { Add, Remove, Single };
Q_ENUM_NS(SelectionMode)

struct WINGPLUGIN_EXPORT ScriptCallError {
    int errorCode;
    QString errmsg;
};

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::ScriptCallError)
Q_DECLARE_METATYPE(WingHex::AppTheme)
Q_DECLARE_METATYPE(WingHex::CallTable)

#endif // WINGPLUGIN_GLOBAL_H

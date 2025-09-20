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

inline static WingDockWidgetInfo createWingDockWidget(
    QString widgetName, QString displayName, QWidget *widget,
    Qt::DockWidgetArea area = Qt::DockWidgetArea::NoDockWidgetArea) {
    return WingDockWidgetInfo{widgetName, displayName, widget, area};
}

inline static WingDockWidgetInfo createWingDockWidget(
    QString widgetName, QWidget *widget,
    Qt::DockWidgetArea area = Qt::DockWidgetArea::NoDockWidgetArea) {
    return WingDockWidgetInfo{widgetName, {}, widget, area};
}

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

// copy from angelscript.h
enum class asRetCodes {
    asSUCCESS = 0,
    asERROR = -1,
    asCONTEXT_ACTIVE = -2,
    asCONTEXT_NOT_FINISHED = -3,
    asCONTEXT_NOT_PREPARED = -4,
    asINVALID_ARG = -5,
    asNO_FUNCTION = -6,
    asNOT_SUPPORTED = -7,
    asINVALID_NAME = -8,
    asNAME_TAKEN = -9,
    asINVALID_DECLARATION = -10,
    asINVALID_OBJECT = -11,
    asINVALID_TYPE = -12,
    asALREADY_REGISTERED = -13,
    asMULTIPLE_FUNCTIONS = -14,
    asNO_MODULE = -15,
    asNO_GLOBAL_VAR = -16,
    asINVALID_CONFIGURATION = -17,
    asINVALID_INTERFACE = -18,
    asCANT_BIND_ALL_FUNCTIONS = -19,
    asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20,
    asWRONG_CONFIG_GROUP = -21,
    asCONFIG_GROUP_IS_IN_USE = -22,
    asILLEGAL_BEHAVIOUR_FOR_TYPE = -23,
    asWRONG_CALLING_CONV = -24,
    asBUILD_IN_PROGRESS = -25,
    asINIT_GLOBAL_VARS_FAILED = -26,
    asOUT_OF_MEMORY = -27,
    asMODULE_IS_IN_USE = -28
};
Q_ENUM_NS(asRetCodes)

struct WINGPLUGIN_EXPORT ScriptCallError {
    int errorCode;
    QString errmsg;
};

struct WINGPLUGIN_EXPORT PragmaResult {
    QString warn;
    QString info;
    QString error;
};

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::ScriptCallError)
Q_DECLARE_METATYPE(WingHex::AppTheme)
Q_DECLARE_METATYPE(WingHex::PluginInfo)
Q_DECLARE_METATYPE(WingHex::CallTable)

#endif // WINGPLUGIN_GLOBAL_H

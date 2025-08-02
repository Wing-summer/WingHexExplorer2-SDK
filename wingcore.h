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

#ifndef WINGCORE_H
#define WINGCORE_H

#include "wingplugin_global.h"

#include <QDebug>
#include <QList>
#include <QMetaType>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QShortcut>
#include <QString>
#include <QToolButton>

#include <tuple>
#include <type_traits>
#include <utility>

namespace WingHex {

constexpr auto CALL_TABLE_PROPERTY = "__CALL_TABLE__";
constexpr auto CALL_POINTER_PROPERTY = "__CALL_POINTER__";

using MetaCallInfo = std::tuple<const char *, Qt::ConnectionType, qsizetype,
                                const void *const *, const char *const *,
                                const QtPrivate::QMetaTypeInterface *const *>;

using UNSAFE_RET =
    std::variant<std::monostate, bool, quint8, quint16, quint32, quint64, float,
                 double, void *, ScriptCallError>;
using UNSAFE_SCFNPTR = std::function<UNSAFE_RET(const QList<void *> &)>;

template <typename T>
static inline QVector<void *> normalizePackedVector(QVector<T> &buffer) {
    QVector<void *> ret;
    if constexpr (std::is_arithmetic_v<T>) {
        auto len = buffer.size();
        ret.resize(len);
        for (int i = 0; i < len; ++i) {
            auto &ptr = ret[i];
            reinterpret_cast<T &>(ptr) = buffer.at(i);
        }
    } else {
        for (auto &item : buffer) {
            ret.append(&item);
        }
    }
    return ret;
}

template <typename T>
static inline QList<void *> normalizePackedList(QList<T> &buffer) {
    static_assert(std::is_same_v<QList<void *>, QVector<void *>>);
    return normalizePackedVector(buffer);
}

enum MetaType : uint {
    Meta_Void,

    Meta_Bool,
    Meta_Int,
    Meta_Int32 = Meta_Int,
    Meta_UInt,
    Meta_UInt32 = Meta_UInt,
    Meta_Int8,
    Meta_UInt8,
    Meta_Int16,
    Meta_UInt16,
    Meta_Int64,
    Meta_UInt64,

    Meta_Float,
    Meta_Double,

    Meta_String,
    Meta_Char,
    Meta_Byte,
    Meta_Color,

    Meta_Map,  // QVariantMap -> dictionary
    Meta_Hash, // QVariantHash -> dictionary

    MetaMax, // reserved
    MetaTypeMask = 0xFFFFF,
    Meta_Array = 0x100000, // QVector<?> -> array<?>
    Meta_List = 0x200000,  // QList<?> -> array<?>
};

static_assert(MetaType::MetaMax < MetaType::Meta_Array);

struct WINGPLUGIN_EXPORT HexPosition {
    qsizetype line;
    int column;
    quint8 lineWidth;
    int nibbleindex;

    HexPosition();
    Q_REQUIRED_RESULT qsizetype offset() const;
    qsizetype operator-(const HexPosition &rhs) const;
    bool operator==(const HexPosition &rhs) const;
    bool operator!=(const HexPosition &rhs) const;
};

struct WINGPLUGIN_EXPORT WingRibbonCatagories {
    inline static QString FILE = QStringLiteral("File");
    inline static QString EDIT = QStringLiteral("Edit");
    inline static QString VIEW = QStringLiteral("View");
    inline static QString SCRIPT = QStringLiteral("Script");
    inline static QString PLUGIN = QStringLiteral("Plugin");
    inline static QString SETTING = QStringLiteral("Setting");
    inline static QString ABOUT = QStringLiteral("About");
};

struct WINGPLUGIN_EXPORT WingRibbonToolBoxInfo {
    QString catagory;
    QString displayName;

    struct WINGPLUGIN_EXPORT Toolbox {
        QString name;
        QList<QToolButton *> tools;
    };
    QList<Toolbox> toolboxs;
};

template <class Func>
inline WingHex::FunctionSig getFunctionSig(Func &&, const char *fn) {
    typedef QtPrivate::FunctionPointer<std::decay_t<Func>> FnPointerType;
    const int *types =
        QtPrivate::ConnectionTypes<typename FnPointerType::Arguments>::types();

    Q_ASSERT(fn);

    WingHex::FunctionSig sig;
    sig.fnName = fn;

    if constexpr (FnPointerType::ArgumentCount > 0) {
        Q_ASSERT(types);
        if (types) {
            sig.types.resize(FnPointerType::ArgumentCount);
            std::copy(types, types + FnPointerType::ArgumentCount,
                      sig.types.begin());
        } else {
            return {};
        }
    }

    return sig;
}

inline QByteArray getFunctionSig(const WingHex::FunctionSig &fn) {
    Q_ASSERT(!fn.fnName.isEmpty());
    auto len = fn.types.size();
    if (len > 0) {
        QByteArrayList args;
        for (qsizetype i = 0; i < len; ++i) {
            QMetaType type(fn.types[i]);
            if (type.isValid()) {
                args.append(type.name());
            }
        }
        return fn.fnName + '(' + args.join(',') + ')';
    } else {
        return fn.fnName + QByteArray("()");
    }
}

#define SETUP_CALL_CONTEXT(FN)                                                 \
    QMetaMethod m;                                                             \
    do {                                                                       \
        static auto CALL = getFunctionSig(FN, __func__);                       \
        if (CALL.fnName.isEmpty()) {                                           \
            qWarning(                                                          \
                "[InvokeCall] The parameters of '%s' contains "                \
                "unregistered type. Please use Q_DECLARE_METATYPE and "        \
                "qRegisterMetaType to make the type known to Qt meta system.", \
                __func__);                                                     \
        } else {                                                               \
            auto fnMap = callTable();                                          \
            if (fnMap.contains(CALL)) {                                        \
                m = fnMap.value(CALL);                                         \
                Q_ASSERT(m.isValid());                                         \
            } else {                                                           \
                auto sig = getFunctionSig(CALL);                               \
                qDebug("[InvokeCall] '%s' is not found in call table.",        \
                       sig.constData());                                       \
            }                                                                  \
        }                                                                      \
    } while (0)

template <template <typename> class Container, typename T, typename... Args>
Container<T> packup(Args &&...args) {
    // ensure every Arg is convertible to T
    static_assert((std::is_convertible_v<Args, T> && ...),
                  "All arguments must be convertible to T!");

    Container<T> c;
    if constexpr (std::is_same_v<Container<T>, QFlags<T>>) {
        (c.setFlag(std::forward<Args>(args)), ...);
    } else {
        static_assert(std::is_same_v<Container<T>, QList<T>> ||
                          std::is_same_v<Container<T>, QVector<T>>,
                      "only QList and QVector are supported!");
        (c.append(std::forward<Args>(args)), ...);
    }
    return c;
}

inline static WingRibbonToolBoxInfo createRibbonToolBox(QString catagory,
                                                        QString displayName) {
    WingRibbonToolBoxInfo info;
    info.catagory = catagory;
    info.displayName = displayName;
    return info;
}

inline static WingRibbonToolBoxInfo
createRibbonToolBox(QString catagory, QString displayName,
                    const WingRibbonToolBoxInfo::Toolbox &toolbox...) {
    WingRibbonToolBoxInfo info;
    info.catagory = catagory;
    info.displayName = displayName;
    info.toolboxs =
        WingHex::packup<QList, WingRibbonToolBoxInfo::Toolbox>(toolbox);
    return info;
}

inline static WingRibbonToolBoxInfo
createRibbonToolBox(QString catagory,
                    const WingRibbonToolBoxInfo::Toolbox &toolbox...) {
    WingRibbonToolBoxInfo info;
    info.catagory = catagory;
    info.toolboxs =
        WingHex::packup<QList, WingRibbonToolBoxInfo::Toolbox>(toolbox);
    return info;
}

inline static WingRibbonToolBoxInfo::Toolbox
createToolBox(const QString &name, QToolButton *tools...) {
    WingRibbonToolBoxInfo::Toolbox tb;
    tb.name = name;
    tb.tools = WingHex::packup<QList, QToolButton *>(tools);
    return tb;
}

template <typename... Args>
using CompatibleToolButtonSlotArgs = std::enable_if_t<std::conjunction_v<
    std::disjunction<std::is_same<Args, Qt::ConnectionType>,
                     std::negation<std::is_convertible<Args, QKeySequence>>>...,
    std::negation<std::is_convertible<Args, QIcon>>...,
    std::negation<std::is_convertible<Args, const char *>>...,
    std::negation<std::is_convertible<Args, QString>>...>>;

template <typename... Args, typename = CompatibleToolButtonSlotArgs<Args...>>
inline static QToolButton *
createToolButton(const QIcon &icon, const QString &text,
                 const QKeySequence &shortcut, Args &&...args) {
    auto tb = createToolButton(icon, text, std::forward<Args>(args)...);
    if (!shortcut.isEmpty()) {
        auto shortCut = new QShortcut(shortcut, tb);
        shortCut->setContext(Qt::WindowShortcut);
        QObject::connect(shortCut, &QShortcut::activated, tb,
                         &QToolButton::click);
    }
    return tb;
}

template <typename... Args, typename = CompatibleToolButtonSlotArgs<Args...>>
inline static QToolButton *
createToolButton(const QIcon &icon, const QString &text, Args &&...args) {
    auto tb = new QToolButton;
    tb->setIcon(icon);
    tb->setText(text);
    QObject::connect(tb, &QToolButton::clicked, std::forward<Args>(args)...);
    return tb;
}

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::MetaCallInfo);

#endif // WINGCORE_H

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

#include <QCache>
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

using MetaCallInfo = std::tuple<const char *, Qt::ConnectionType, qsizetype,
                                const void *const *, const char *const *,
                                const QtPrivate::QMetaTypeInterface *const *>;

class WINGPLUGIN_EXPORT ValueObjValue {
public:
    ValueObjValue(void *ptr, std::function<void(void *)> deleter) noexcept
        : _ptr(ptr), _deleter(deleter) {}
    ~ValueObjValue() noexcept {
        if (_deleter) {
            _deleter(_ptr);
        }
    }

    inline void *data() const { return _ptr; }

private:
    void *_ptr;
    std::function<void(void *)> _deleter;

    Q_DISABLE_COPY(ValueObjValue)
};

using ValueObjPtr = std::shared_ptr<ValueObjValue>;

template <typename T, typename = std::enable_if<!std::is_same_v<T, void>>>
inline ValueObjPtr makeValueObjPtr(T *ptr) {
    return makeValueObjPtr(
        ptr, [](void *memory) { delete static_cast<T *>(memory); });
}

template <typename T, typename... _Args>
inline ValueObjPtr makeValueObject(_Args &&...__args) {
    return std::make_shared<ValueObjValue>(
        new T(std::forward<_Args>(__args)...),
        [](void *memory) { delete static_cast<T *>(memory); });
}

inline ValueObjPtr makeValueObjPtr(void *ptr,
                                   std::function<void(void *)> deleter = {}) {
    return std::make_shared<ValueObjValue>(ptr, deleter);
}

using RefObjPtr = void *;

template <typename T>
inline RefObjPtr makeRefObjPtr(T *value) {
    return static_cast<void *>(value);
}

template <typename T, typename... _Args>
inline RefObjPtr makeRefObject(_Args &&...__args) {
    return static_cast<void *>(new T(std::forward<_Args>(__args)...));
}

using UNSAFE_RET =
    std::variant<std::monostate, bool, quint8, quint16, quint32, quint64, float,
                 double, RefObjPtr, ValueObjPtr, ScriptCallError>;
using UNSAFE_SCFNPTR = std::function<UNSAFE_RET(const QList<void *> &)>;

template <typename T>
inline UNSAFE_RET makeUnsafeRet(const T &v) {
    using rT = std::remove_cv_t<T>;

    if constexpr (std::is_integral_v<rT>) {
        if constexpr (std::is_same_v<rT, bool>) {
            return v;
        }

        constexpr auto op = [](const T &v) -> UNSAFE_RET {
            switch (sizeof(T)) {
            case sizeof(quint8):
                return quint8(v);
            case sizeof(quint16):
                return quint16(v);
            case sizeof(quint32):
                return quint32(v);
            case sizeof(quint64):
                return quint64(v);
            }
            return {};
        };

        return op(v);
    } else if constexpr (std::is_floating_point_v<rT>) {
        if constexpr (std::is_same_v<rT, float>) {
            return float(v);
        } else {
            return double(v); // long double -> double
        }
    } else if constexpr (std::is_same_v<RefObjPtr, rT> ||
                         std::is_same_v<ValueObjPtr, rT> ||
                         std::is_same_v<ScriptCallError, T>) {
        return v;
    } else {
        static_assert(false, "Unacceptable type with UNSAFE_RET");
        return {};
    }
}

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

#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
template <class Func>
inline constexpr WingHex::FunctionSig getFunctionSig(Func &&, const char *fn) {
    typedef QtPrivate::FunctionPointer<std::decay_t<Func>> FnPointerType;
    int const *types =
        QtPrivate::ConnectionTypes<typename FnPointerType::Arguments>::types();

    Q_ASSERT(fn);

    WingHex::FunctionSig sig;
    sig.fnName = fn;
    sig.types.fill(0);

    if constexpr (FnPointerType::ArgumentCount > 0) {
        Q_ASSERT(types);
        if (types) {
            static_assert(sig.types.size() >= FnPointerType::ArgumentCount);
            std::copy(types, types + FnPointerType::ArgumentCount,
                      sig.types.begin());
            sig.typesCount = FnPointerType::ArgumentCount;
        } else {
            return {};
        }
    }

    return sig;
}
#else
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
#endif

inline QByteArray getFunctionSig(const WingHex::FunctionSig &fn) {
    Q_ASSERT(!fn.fnName.isEmpty());
    auto len = fn.types.size();
    QByteArray sig;
    if (len > 0) {
        QByteArrayList args;
        args.reserve(len);
        for (qsizetype i = 0; i < len; ++i) {
            QMetaType type(fn.types[i]);
            if (type.isValid()) {
                args.append(type.name());
            }
        }
        sig = fn.fnName + QByteArrayLiteral("(") + args.join(',') +
              QByteArrayLiteral(")");
    } else {
        sig = fn.fnName + QByteArrayLiteral("()");
    }
    return sig;
}

// lighter QCache but without cost and ownership taking
template <class Key, class T>
class Cache {
    struct Chain {
        Chain() noexcept : prev(this), next(this) {}
        Chain *prev;
        Chain *next;
    };

    struct Node : public Chain {
        using KeyType = Key;
        using ValueType = T;

        Key key;
        T value;

        Node(const Key &k,
             T &&t) noexcept(std::is_nothrow_move_assignable_v<Key>)
            : Chain(), key(k), value(t) {}
        Node(Key &&k, T t) noexcept(std::is_nothrow_move_assignable_v<Key>)
            : Chain(), key(std::move(k)), value(t) {}
        static void createInPlace(Node *n, const Key &k, T o) {
            new (n) Node{Key(k), o};
        }
        void emplace(T o) { value = o; }

        Node(Node &&other)
            : Chain(other), key(std::move(other.key)),
              value(std::move(other.value)) {
            Q_ASSERT(this->prev);
            Q_ASSERT(this->next);
            this->prev->next = this;
            this->next->prev = this;
        }

    private:
        Q_DISABLE_COPY(Node)
    };

    using Data = QHashPrivate::Data<Node>;

    mutable Chain chain;
    Data d;
    qsizetype mx = 0;
    qsizetype total = 0;

    void unlink(Node *n) noexcept(std::is_nothrow_destructible_v<Node>) {
        Q_ASSERT(n->prev);
        Q_ASSERT(n->next);
        n->prev->next = n->next;
        n->next->prev = n->prev;
        total--;
        auto it = d.findBucket(n->key);
        d.erase(it);
    }
    T relink(const Key &key) const noexcept {
        if (isEmpty())
            return T{};

        Node *n = d.findNode(key);
        if (!n)
            return T{};

        if (chain.next != n) {
            Q_ASSERT(n->prev);
            Q_ASSERT(n->next);
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n->next = chain.next;
            chain.next->prev = n;
            n->prev = &chain;
            chain.next = n;
        }
        return n->value;
    }

    void trim(qsizetype m) noexcept(std::is_nothrow_destructible_v<Node>) {
        while (chain.prev != &chain && total > m) {
            Node *n = static_cast<Node *>(chain.prev);
            unlink(n);
        }
    }

    Q_DISABLE_COPY(Cache)

public:
    inline explicit Cache(qsizetype maxCost) noexcept : mx(maxCost) {}
    inline ~Cache() {
        static_assert(std::is_nothrow_destructible_v<Key>,
                      "Types with throwing destructors are not supported in Qt "
                      "containers.");
        static_assert(std::is_nothrow_destructible_v<T>,
                      "Types with throwing destructors are not supported in Qt "
                      "containers.");

        clear();
    }

    inline qsizetype maxCost() const noexcept { return mx; }
    inline void
    setMaxCost(qsizetype m) noexcept(std::is_nothrow_destructible_v<Node>) {
        mx = m;
        trim(mx);
    }
    inline qsizetype totalCost() const noexcept { return total; }

    inline qsizetype size() const noexcept { return qsizetype(d.size); }
    inline qsizetype count() const noexcept { return qsizetype(d.size); }
    inline bool isEmpty() const noexcept { return !d.size; }

    inline void clear() noexcept(std::is_nothrow_destructible_v<Node>) {
        d.clear();
        total = 0;
        chain.next = &chain;
        chain.prev = &chain;
    }

    inline bool insert(const Key &key, T object) {
        if (1 > mx) {
            remove(key);
            return false;
        }
        trim(mx - 1);
        auto result = d.findOrInsert(key);
        Node *n = result.it.node();
        if (result.initialized) {
            result.it.node()->emplace(object);
            relink(key);
        } else {
            Node::createInPlace(n, key, object);
            n->prev = &chain;
            n->next = chain.next;
            chain.next->prev = n;
            chain.next = n;
        }
        total++;
        return true;
    }
    inline T object(const Key &key) const noexcept { return relink(key); }
    inline bool contains(const Key &key) const noexcept {
        return !isEmpty() && d.findNode(key) != nullptr;
    }

    inline bool
    remove(const Key &key) noexcept(std::is_nothrow_destructible_v<Node>) {
        if (isEmpty())
            return false;
        Node *n = d.findNode(key);
        if (!n) {
            return false;
        } else {
            unlink(n);
            return true;
        }
    }
};

#define SETUP_CALL_CONTEXT(FN)                                                 \
    QMetaMethod m;                                                             \
    do {                                                                       \
        static Cache<WingPluginCalls const *, QMetaMethod> cache(10);          \
        m = cache.object(this);                                                \
        if (!m.isValid()) {                                                    \
            static auto CALL = getFunctionSig(FN, __func__);                   \
            if (CALL.fnName.isEmpty()) {                                       \
                qWarning(                                                      \
                    "[InvokeCall] The parameters of '%s' contains "            \
                    "unregistered type. Please use Q_DECLARE_METATYPE and "    \
                    "qRegisterMetaType to make the type known to Qt meta "     \
                    "system.",                                                 \
                    __func__);                                                 \
            } else {                                                           \
                auto fnMap = callTable();                                      \
                if (fnMap.contains(CALL)) {                                    \
                    m = fnMap.value(CALL);                                     \
                    Q_ASSERT(m.isValid());                                     \
                    cache.insert(this, m);                                     \
                } else {                                                       \
                    auto sig = getFunctionSig(CALL);                           \
                    qDebug("[InvokeCall] '%s' is not found in call table.",    \
                           sig.constData());                                   \
                }                                                              \
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

inline static WingRibbonToolBoxInfo
createRibbonToolBox(const QString &catagory, const QString &displayName) {
    WingRibbonToolBoxInfo info;
    info.catagory = catagory;
    info.displayName = displayName;
    return info;
}

inline static WingRibbonToolBoxInfo
createRibbonToolBox(const QString &catagory, const QString &displayName,
                    const WingRibbonToolBoxInfo::Toolbox &toolbox...) {
    WingRibbonToolBoxInfo info;
    info.catagory = catagory;
    info.displayName = displayName;
    info.toolboxs =
        WingHex::packup<QList, WingRibbonToolBoxInfo::Toolbox>(toolbox);
    return info;
}

inline static WingRibbonToolBoxInfo
createRibbonToolBox(const QString &catagory,
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

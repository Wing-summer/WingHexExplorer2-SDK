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
#include <QMetaType>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QString>

#include <tuple>

namespace WingHex {

constexpr auto CALL_TABLE_PROPERTY = "__CALL_TABLE__";
constexpr auto CALL_POINTER_PROPERTY = "__CALL_POINTER__";

using MetaCallInfo = std::tuple<const char *, Qt::ConnectionType, qsizetype,
                                const void *const *, const char *const *,
                                const QtPrivate::QMetaTypeInterface *const *>;

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
                "unregistered type. Please use Q_DECLARE_INTERFACE and "       \
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

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::MetaCallInfo);

#endif // WINGCORE_H

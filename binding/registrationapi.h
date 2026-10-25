/*==============================================================================
** Copyright (C) 2026-2029 WingSummer
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

#ifndef BINDING_REGISTRATION_API_H
#define BINDING_REGISTRATION_API_H

#include "value.h"

#include <QAnyStringView>
#include <QFlags>

namespace WingScriptBind {

class ScopeBuilder;
using ScopeId = quint64;

enum class ClassFlag : quint32 {
    None = 0,
    Extensible = 1u << 0,
    AllowOverridingMethods = 1u << 1,
    VisibleMetatables = 1u << 2
};
Q_DECLARE_FLAGS(ClassFlags, ClassFlag)

inline bool hasFlag(ClassFlags flags, ClassFlag flag) noexcept {
    return flags.testFlag(flag);
}

class ICallable {
public:
    virtual ~ICallable() = default;
    virtual bool accepts(const QList<Value> &arguments) const = 0;
    virtual Value invoke(const ObjectValue &receiver,
                         const QList<Value> &arguments) = 0;
    virtual bool returnsVoid() const noexcept = 0;
};
using Callable = QSharedPointer<ICallable>;

struct BaseInfo {
    TypeInfo type;
    void *(*cast)(void *) = nullptr;
};

class IRegistrationApi {
public:
    virtual ~IRegistrationApi() = default;
    virtual ScopeId globalScope() const noexcept = 0;
    virtual ScopeId parentScope(ScopeId scope) const = 0;
    virtual void retainNamespaceScope(ScopeId root) noexcept = 0;
    virtual void releaseNamespaceScope(ScopeId root) noexcept = 0;
    virtual ScopeId beginNamespace(ScopeId parent, QAnyStringView name) = 0;
    virtual ScopeId beginClass(ScopeId parent, const TypeInfo &type,
                               QAnyStringView name,
                               const QList<BaseInfo> &bases,
                               ClassFlags flags = {}) = 0;
    virtual void registerFunction(ScopeId scope, QAnyStringView name,
                                  const QList<Callable> &overloads,
                                  bool isStatic = false) = 0;
    virtual void registerProperty(ScopeId scope, QAnyStringView name,
                                  Callable getter, Callable setter = {},
                                  bool isStatic = false) = 0;
    virtual void registerVariable(ScopeId scope, QAnyStringView name,
                                  const Value &value) = 0;
    virtual void registerDefinitionFile(ScopeId scope,
                                        QAnyStringView fileName) = 0;
};

class IScriptEngine {
public:
    virtual ~IScriptEngine() = default;
    ScopeBuilder global();
    virtual IRegistrationApi &registrations() noexcept = 0;
    virtual void collectGarbage() = 0;
};

} // namespace WingScriptBind

Q_DECLARE_OPERATORS_FOR_FLAGS(WingScriptBind::ClassFlags)

#endif

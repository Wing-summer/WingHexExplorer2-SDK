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

#ifndef BINDING_BUILDER_H
#define BINDING_BUILDER_H

#include "actions.h"

namespace WingScriptBind {

template <class T>
class ClassBuilder;
template <class T>
class EnumBuilderT;

namespace detail {
class NamespaceScopeLease final {
public:
    NamespaceScopeLease(IRegistrationApi &api, ScopeId root)
        : api_(&api), root_(root) {
        api_->retainNamespaceScope(root_);
    }
    ~NamespaceScopeLease() { api_->releaseNamespaceScope(root_); }
    ScopeId root() const noexcept { return root_; }
    NamespaceScopeLease(const NamespaceScopeLease &) = delete;
    NamespaceScopeLease &operator=(const NamespaceScopeLease &) = delete;

private:
    IRegistrationApi *api_;
    ScopeId root_;
};
} // namespace detail

class ScopeBuilder {
public:
    ScopeBuilder(IRegistrationApi &api, ScopeId scope, ScopeId,
                 QSharedPointer<detail::NamespaceScopeLease> lease = {})
        : api_(&api), scope_(scope), lease_(std::move(lease)) {}

    ScopeId id() const noexcept { return scope_; }
    IRegistrationApi &api() const noexcept { return *api_; }

    ScopeBuilder beginNamespace(QAnyStringView name) const {
        auto child = api_->beginNamespace(scope_, name);
        auto lease = lease_;
        if (!lease && scope_ == api_->globalScope()) {
            lease = QSharedPointer<detail::NamespaceScopeLease>::create(*api_,
                                                                        child);
        }
        return ScopeBuilder(*api_, child, scope_, std::move(lease));
    }
    ScopeBuilder beginTable(QAnyStringView name) const {
        return beginNamespace(name);
    }
    ScopeBuilder endNamespace() const {
        auto parent = api_->parentScope(scope_);
        auto lease = lease_;
        if (lease && lease->root() == scope_) {
            lease.clear();
        }
        return ScopeBuilder(*api_, parent, parent, std::move(lease));
    }
    ScopeBuilder endTable() const { return endNamespace(); }

    template <class T>
    ClassBuilder<T> beginClass(QAnyStringView name,
                               ClassFlags flags = {}) const;
    template <class T, class... Bases>
    ClassBuilder<T> deriveClass(QAnyStringView name,
                                ClassFlags flags = {}) const;
    template <class T>
    EnumBuilderT<T> beginEnum(QAnyStringView name) const;

    template <class... Functions>
    ScopeBuilder &addFunction(QAnyStringView name, Functions &&...functions) {
        static_assert(sizeof...(Functions) > 0);
        api_->registerFunction(
            scope_, name,
            {detail::makeCallable(std::forward<Functions>(functions))...});
        return *this;
    }
    template <class Getter>
    ScopeBuilder &addProperty(QAnyStringView name, Getter &&getter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)));
        return *this;
    }
    template <class Getter, class Setter>
    ScopeBuilder &addProperty(QAnyStringView name, Getter &&getter,
                              Setter &&setter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)),
            detail::makeCallable(std::forward<Setter>(setter)));
        return *this;
    }
    template <class T>
    ScopeBuilder &addVariable(QAnyStringView name, const T &value) {
        api_->registerVariable(scope_, name, detail::toValue(value));
        return *this;
    }
    template <class T>
    ScopeBuilder &addConstant(QAnyStringView name, const T &value) {
        return addVariable(name, value);
    }

private:
    IRegistrationApi *api_;
    ScopeId scope_;
    QSharedPointer<detail::NamespaceScopeLease> lease_;
};

template <class T>
class ClassBuilder {
public:
    ClassBuilder(IRegistrationApi &api, ScopeId scope, ScopeBuilder parent)
        : api_(&api), scope_(scope), parent_(std::move(parent)) {}
    ScopeId id() const noexcept { return scope_; }

    template <class... Signatures>
    ClassBuilder &addConstructor() {
        static_assert(sizeof...(Signatures) > 0);
        api_->registerFunction(scope_, "__constructor",
                               {detail::Constructor<T, Signatures>::make()...},
                               true);
        return *this;
    }
    template <class... Signatures>
    ClassBuilder &addConstructors() {
        return addConstructor<Signatures...>();
    }
    template <class... Functions>
    ClassBuilder &addFunction(QAnyStringView name, Functions &&...functions) {
        api_->registerFunction(
            scope_, name,
            {detail::makeCallable(std::forward<Functions>(functions))...});
        return *this;
    }
    template <class... Functions>
    ClassBuilder &addMetaFunction(QAnyStringView name,
                                  Functions &&...functions) {
        return addFunction(name, std::forward<Functions>(functions)...);
    }
    template <class... Functions>
    ClassBuilder &addStaticFunction(QAnyStringView name,
                                    Functions &&...functions) {
        api_->registerFunction(
            scope_, name,
            {detail::makeCallable(std::forward<Functions>(functions))...},
            true);
        return *this;
    }
    template <class Getter>
    ClassBuilder &addProperty(QAnyStringView name, Getter &&getter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)));
        return *this;
    }
    template <class Getter, class Setter>
    ClassBuilder &addProperty(QAnyStringView name, Getter &&getter,
                              Setter &&setter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)),
            detail::makeCallable(std::forward<Setter>(setter)));
        return *this;
    }
    template <class Field>
    ClassBuilder &addData(QAnyStringView name, Field T::*field) {
        Callable getter =
            QSharedPointer<detail::FieldGetter<T, Field>>::create(field);
        Callable setter;
        if constexpr (!std::is_const_v<Field>) {
            setter =
                QSharedPointer<detail::FieldSetter<T, Field>>::create(field);
        }
        api_->registerProperty(scope_, name, getter, setter);
        return *this;
    }
    template <class Field>
    ClassBuilder &addPropertyReadWrite(QAnyStringView name, Field T::*field) {
        return addData(name, field);
    }
    template <class Getter>
    ClassBuilder &addStaticProperty(QAnyStringView name, Getter &&getter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)),
            {}, true);
        return *this;
    }
    template <class Getter, class Setter>
    ClassBuilder &addStaticProperty(QAnyStringView name, Getter &&getter,
                                    Setter &&setter) {
        api_->registerProperty(
            scope_, name, detail::makeCallable(std::forward<Getter>(getter)),
            detail::makeCallable(std::forward<Setter>(setter)), true);
        return *this;
    }
    ScopeBuilder endClass() const { return parent_; }

private:
    IRegistrationApi *api_;
    ScopeId scope_;
    ScopeBuilder parent_;
};

template <class T>
class EnumBuilderT {
public:
    EnumBuilderT(IRegistrationApi &, ScopeBuilder parent, QString name)
        : parent_(std::move(parent)), name_(std::move(name)) {
        static_assert(std::is_enum_v<T>);
    }
    EnumBuilderT &addValue(QAnyStringView name, T value) {
        values_.append(qMakePair(name.toString(), value));
        return *this;
    }
    ScopeBuilder endEnum() {
        auto scope = parent_.beginNamespace(name_);
        for (const auto &entry : values_) {
            scope.addConstant(entry.first, entry.second);
        }
        return parent_;
    }

private:
    ScopeBuilder parent_;
    QString name_;
    QList<QPair<QString, T>> values_;
};

template <class T>
ClassBuilder<T> ScopeBuilder::beginClass(QAnyStringView name,
                                         ClassFlags flags) const {
    return ClassBuilder<T>(
        *api_, api_->beginClass(scope_, typeInfo<T>(), name, {}, flags), *this);
}

template <class T, class... Bases>
ClassBuilder<T> ScopeBuilder::deriveClass(QAnyStringView name,
                                          ClassFlags flags) const {
    static_assert(sizeof...(Bases) > 0);
    QList<BaseInfo> bases{{typeInfo<Bases>(), [](void *object) -> void * {
                               return static_cast<Bases *>(
                                   static_cast<T *>(object));
                           }}...};
    return ClassBuilder<T>(
        *api_, api_->beginClass(scope_, typeInfo<T>(), name, bases, flags),
        *this);
}

template <class T>
EnumBuilderT<T> ScopeBuilder::beginEnum(QAnyStringView name) const {
    return EnumBuilderT<T>(*api_, *this, name.toString());
}
inline ScopeBuilder global(IRegistrationApi &api) {
    return ScopeBuilder(api, api.globalScope(), api.globalScope());
}
inline ScopeBuilder IScriptEngine::global() {
    return WingScriptBind::global(registrations());
}

} // namespace WingScriptBind
#endif

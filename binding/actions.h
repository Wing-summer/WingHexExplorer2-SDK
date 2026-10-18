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

#ifndef BINDING_ACTIONS_H
#define BINDING_ACTIONS_H

#include "registrationapi.h"

#include <functional>
#include <tuple>

namespace WingScriptBind::detail {

template <class T>
struct FunctionTraits : FunctionTraits<decltype(&T::operator())> {};
template <class Return, class... Args>
struct FunctionTraits<Return (*)(Args...)> {
    using Result = Return;
    using Arguments = std::tuple<std::decay_t<Args>...>;
    static constexpr bool member = false;
};
template <class Return, class... Args>
struct FunctionTraits<Return (*)(Args...) noexcept>
    : FunctionTraits<Return (*)(Args...)> {};
template <class Return, class Owner, class... Args>
struct FunctionTraits<Return (Owner::*)(Args...)>
    : FunctionTraits<Return (*)(Args...)> {
    using Class = Owner;
    static constexpr bool member = true;
};
template <class Return, class Owner, class... Args>
struct FunctionTraits<Return (Owner::*)(Args...) const>
    : FunctionTraits<Return (Owner::*)(Args...)> {};
template <class Return, class Owner, class... Args>
struct FunctionTraits<Return (Owner::*)(Args...) noexcept>
    : FunctionTraits<Return (Owner::*)(Args...)> {};
template <class Return, class Owner, class... Args>
struct FunctionTraits<Return (Owner::*)(Args...) const noexcept>
    : FunctionTraits<Return (Owner::*)(Args...)> {};

template <class Function>
class CallableAdapter final : public ICallable {
    using Traits = FunctionTraits<Function>;
    using Arguments = typename Traits::Arguments;
    using Result = typename Traits::Result;
    static constexpr auto count = std::tuple_size_v<Arguments>;

    template <std::size_t... Index>
    static Arguments unpack(const QList<Value> &arguments,
                            std::index_sequence<Index...>) {
        return Arguments{fromValue<std::tuple_element_t<Index, Arguments>>(
            arguments[Index])...};
    }

public:
    explicit CallableAdapter(Function function)
        : function_(std::move(function)) {}

    bool accepts(const QList<Value> &arguments) const override {
        if (arguments.size() != count) {
            return false;
        }
        try {
            unpack(arguments, std::make_index_sequence<count>{});
            return true;
        } catch (const ScriptException &error) {
            if (error.getError() == Exception::TypeMismatch) {
                return false;
            }
            throw;
        }
    }

    Value invoke(const ObjectValue &receiver,
                 const QList<Value> &arguments) override {
        if (arguments.size() != count) {
            conversionError();
        }
        auto values = unpack(arguments, std::make_index_sequence<count>{});
        auto call = [&](auto &...args) -> Result {
            if constexpr (std::is_member_function_pointer_v<Function>) {
                using Owner = typename Traits::Class;
                if (!sameType(receiver.type, typeInfo<Owner>()) ||
                    !receiver.pointer) {
                    conversionError();
                }
                return std::invoke(
                    function_, static_cast<Owner *>(receiver.pointer), args...);
            } else {
                return std::invoke(function_, args...);
            }
        };
        if constexpr (std::is_void_v<Result>) {
            std::apply(call, values);
            return {};
        } else {
            return toValue(std::apply(call, values));
        }
    }

    bool returnsVoid() const noexcept override {
        return std::is_void_v<Result>;
    }

private:
    Function function_;
};

template <class Function>
Callable makeCallable(Function &&function) {
    using Adapter = CallableAdapter<std::decay_t<Function>>;
    return QSharedPointer<Adapter>::create(std::forward<Function>(function));
}

template <class T, class Signature>
struct Constructor;
template <class T, class Return, class... Args>
struct Constructor<T, Return (*)(Args...)> {
    static Callable make() {
        return makeCallable([](Args... arguments) { return T(arguments...); });
    }
};

template <class T, class Field>
class FieldGetter final : public ICallable {
public:
    explicit FieldGetter(Field T::*field) : field_(field) {}
    bool accepts(const QList<Value> &arguments) const override {
        return arguments.isEmpty();
    }
    Value invoke(const ObjectValue &receiver, const QList<Value> &) override {
        if (!sameType(receiver.type, typeInfo<T>()) || !receiver.pointer) {
            conversionError();
        }
        return toValue(static_cast<T *>(receiver.pointer)->*field_);
    }
    bool returnsVoid() const noexcept override { return false; }

private:
    Field T::*field_;
};

template <class T, class Field>
class FieldSetter final : public ICallable {
public:
    explicit FieldSetter(Field T::*field) : field_(field) {}
    bool accepts(const QList<Value> &arguments) const override {
        if (arguments.size() != 1) {
            return false;
        }
        try {
            fromValue<Field>(arguments.front());
            return true;
        } catch (const ScriptException &error) {
            if (error.getError() == Exception::TypeMismatch) {
                return false;
            }
            throw;
        }
    }
    Value invoke(const ObjectValue &receiver,
                 const QList<Value> &arguments) override {
        if (!sameType(receiver.type, typeInfo<T>()) || !receiver.pointer ||
            arguments.size() != 1) {
            conversionError();
        }
        static_cast<T *>(receiver.pointer)->*field_ =
            fromValue<Field>(arguments.front());
        return {};
    }
    bool returnsVoid() const noexcept override { return true; }

private:
    Field T::*field_;
};

} // namespace WingScriptBind::detail
#endif

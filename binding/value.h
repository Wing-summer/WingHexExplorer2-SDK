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

#ifndef BINDING_VALUE_H
#define BINDING_VALUE_H

#include "exception.h"

#include <QHash>
#include <QList>
#include <QMap>
#include <QQueue>
#include <QSet>
#include <QSharedPointer>
#include <QStack>
#include <QStringList>

#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <qtdeprecationdefinitions.h>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace WingScriptBind {

struct TypeInfo {
    const std::type_info *rtti = nullptr;
    const char *name = "<unknown>";
    std::size_t size = 0;
    std::size_t alignment = 0;
};

template <class T>
const TypeInfo &typeInfo() {
    using ValueType = std::remove_cv_t<std::remove_reference_t<T>>;
    static const TypeInfo info{&typeid(ValueType), typeid(ValueType).name(),
                               sizeof(ValueType), alignof(ValueType)};
    return info;
}

inline bool sameType(const TypeInfo &left, const TypeInfo &right) {
    return left.rtti && right.rtti && *left.rtti == *right.rtti;
}

struct ObjectValue {
    TypeInfo type;
    std::shared_ptr<void> storage;
    void *pointer = nullptr;
};

struct Value {
    enum class Kind {
        Nil,
        Boolean,
        Signed,
        Unsigned,
        Number,
        Bytes,
        Object,
        Sequence,
        Table
    };
    Kind kind = Kind::Nil;
    bool boolean = false;
    qint64 signedNumber = 0;
    quint64 unsignedNumber = 0;
    double number = 0;
    QByteArray bytes;
    ObjectValue object;
    QList<Value> sequence;
    QList<QPair<Value, Value>> entries;
};

namespace detail {

template <class T>
struct SequenceTraits {
    static constexpr bool enabled = std::is_same_v<T, QStringList>;
    using Element = QString;
};
template <class T>
struct SequenceTraits<QList<T>> {
    static constexpr bool enabled = true;
    using Element = T;
};
template <class T>
struct SequenceTraits<QQueue<T>> : SequenceTraits<QList<T>> {};
template <class T>
struct SequenceTraits<QStack<T>> : SequenceTraits<QList<T>> {};
template <class T>
struct SequenceTraits<QSet<T>> : SequenceTraits<QList<T>> {};
template <class T>
struct IsSet : std::false_type {};
template <class T>
struct IsSet<QSet<T>> : std::true_type {};

template <class T>
struct MapTraits {
    static constexpr bool enabled = false;
};
template <class KeyType, class ValueType>
struct MapTraits<QMap<KeyType, ValueType>> {
    static constexpr bool enabled = true;
    using Key = KeyType;
    using Mapped = ValueType;
};
template <class KeyType, class ValueType>
struct MapTraits<QHash<KeyType, ValueType>>
    : MapTraits<QMap<KeyType, ValueType>> {};

inline void conversionError() {
    throw ScriptException(Exception::TypeMismatch);
}

template <class T>
Value toValue(const T &source);
template <class T>
T fromValue(const Value &source);

inline qsizetype sequenceIndex(const Value &key, qsizetype size) {
    long double index;
    if (key.kind == Value::Kind::Signed) {
        index = key.signedNumber;
    } else if (key.kind == Value::Kind::Unsigned) {
        index = key.unsignedNumber;
    } else if (key.kind == Value::Kind::Number) {
        index = key.number;
    } else {
        conversionError();
    }
    if (!std::isfinite(index) || index < 1 || index > size ||
        std::trunc(index) != index) {
        conversionError();
    }
    return qsizetype(index) - 1;
}

template <class T>
Value toValue(const T &source) {
    using Plain = std::remove_cv_t<std::remove_reference_t<T>>;
    static_assert(
        !std::is_pointer_v<Plain>,
        "Binding values must be copied; raw pointers are unsupported");
    Value result;
    if constexpr (std::is_same_v<Plain, bool>) {
        result.kind = Value::Kind::Boolean;
        result.boolean = source;
    } else if constexpr (std::is_enum_v<Plain>) {
        return toValue(static_cast<std::underlying_type_t<Plain>>(source));
    } else if constexpr (std::is_integral_v<Plain> && std::is_signed_v<Plain>) {
        result.kind = Value::Kind::Signed;
        result.signedNumber = source;
    } else if constexpr (std::is_integral_v<Plain>) {
        result.kind = Value::Kind::Unsigned;
        result.unsignedNumber = source;
    } else if constexpr (std::is_floating_point_v<Plain>) {
        result.kind = Value::Kind::Number;
        result.number = source;
    } else if constexpr (std::is_same_v<Plain, QByteArray>) {
        result.kind = Value::Kind::Bytes;
        result.bytes = source;
    } else if constexpr (SequenceTraits<Plain>::enabled) {
        result.kind = Value::Kind::Sequence;
        result.sequence.reserve(source.size());
        for (const auto &element : source) {
            result.sequence.append(toValue(element));
        }
    } else if constexpr (MapTraits<Plain>::enabled) {
        result.kind = Value::Kind::Table;
        result.entries.reserve(source.size());
        for (auto entry = source.cbegin(); entry != source.cend(); ++entry) {
            result.entries.append(
                qMakePair(toValue(entry.key()), toValue(entry.value())));
        }
    } else {
        static_assert(std::is_copy_constructible_v<Plain>,
                      "Bound objects must be copy constructible");
        result.kind = Value::Kind::Object;
        auto *copy = new Plain(source);
        result.object = {
            typeInfo<Plain>(),
            std::shared_ptr<void>(
                copy,
                [](void *object) { delete static_cast<Plain *>(object); }),
            copy};
    }
    return result;
}

template <class T>
T fromValue(const Value &source) {
    static_assert(
        !std::is_pointer_v<T>,
        "Binding values must be copied; raw pointers are unsupported");
    if constexpr (std::is_same_v<T, bool>) {
        if (source.kind != Value::Kind::Boolean) {
            conversionError();
        }
        return source.boolean;
    } else if constexpr (std::is_enum_v<T>) {
        return static_cast<T>(fromValue<std::underlying_type_t<T>>(source));
    } else if constexpr (std::is_integral_v<T>) {
        if (source.kind == Value::Kind::Signed) {
            if constexpr (std::is_signed_v<T>) {
                if (source.signedNumber < std::numeric_limits<T>::lowest() ||
                    source.signedNumber > std::numeric_limits<T>::max()) {
                    conversionError();
                }
            } else if (source.signedNumber < 0 ||
                       quint64(source.signedNumber) >
                           std::numeric_limits<T>::max()) {
                conversionError();
            }
            return static_cast<T>(source.signedNumber);
        }
        if (source.kind == Value::Kind::Unsigned) {
            if (source.unsignedNumber >
                quint64(std::numeric_limits<T>::max())) {
                conversionError();
            }
            return static_cast<T>(source.unsignedNumber);
        }
        if (source.kind != Value::Kind::Number) {
            conversionError();
        }
        const double upper = std::ldexp(1.0, std::numeric_limits<T>::digits);
        const double lower = std::is_signed_v<T> ? -upper : 0;
        if (!std::isfinite(source.number) ||
            std::trunc(source.number) != source.number ||
            source.number < lower || source.number >= upper) {
            conversionError();
        }
        return static_cast<T>(source.number);
    } else if constexpr (std::is_floating_point_v<T>) {
        long double number;
        if (source.kind == Value::Kind::Signed) {
            number = source.signedNumber;
        } else if (source.kind == Value::Kind::Unsigned) {
            number = source.unsignedNumber;
        } else if (source.kind == Value::Kind::Number) {
            number = source.number;
        } else {
            conversionError();
        }
        if (std::isfinite(number) &&
            std::abs(number) > std::numeric_limits<T>::max()) {
            conversionError();
        }
        return static_cast<T>(number);
    } else if constexpr (std::is_same_v<T, QByteArray>) {
        if (source.kind != Value::Kind::Bytes) {
            conversionError();
        }
        return source.bytes;
    } else if constexpr (SequenceTraits<T>::enabled) {
        QList<Value> elements;
        if (source.kind == Value::Kind::Sequence) {
            elements = source.sequence;
        } else if (source.kind == Value::Kind::Table) {
            elements.resize(source.entries.size());
            QSet<qsizetype> seen;
            for (const auto &entry : source.entries) {
                const auto index = sequenceIndex(entry.first, elements.size());
                if (seen.contains(index)) {
                    conversionError();
                }
                seen.insert(index);
                elements[index] = entry.second;
            }
        } else {
            conversionError();
        }
        T result;
        for (const auto &element : elements) {
            auto converted =
                fromValue<typename SequenceTraits<T>::Element>(element);
            if constexpr (IsSet<T>::value) {
                result.insert(std::move(converted));
            } else {
                result.append(std::move(converted));
            }
        }
        return result;
    } else if constexpr (MapTraits<T>::enabled) {
        if (source.kind != Value::Kind::Table &&
            source.kind != Value::Kind::Sequence) {
            conversionError();
        }
        T result;
        auto insert = [&](const Value &key, const Value &value) {
            auto convertedKey = fromValue<typename MapTraits<T>::Key>(key);
            if (result.contains(convertedKey)) {
                conversionError();
            }
            result.insert(std::move(convertedKey),
                          fromValue<typename MapTraits<T>::Mapped>(value));
        };
        if (source.kind == Value::Kind::Sequence) {
            for (qsizetype index = 0; index < source.sequence.size(); ++index) {
                insert(toValue(index + 1), source.sequence[index]);
            }
        } else {
            for (const auto &entry : source.entries) {
                insert(entry.first, entry.second);
            }
        }
        return result;
    } else {
        if (source.kind != Value::Kind::Object ||
            !sameType(source.object.type, typeInfo<T>()) ||
            !source.object.pointer) {
            conversionError();
        }
        return *static_cast<const T *>(source.object.pointer);
    }
}

} // namespace detail
} // namespace WingScriptBind

#endif

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

#ifndef BINDING_EXCEPTION_H
#define BINDING_EXCEPTION_H

#include <QByteArray>
#include <QString>
#include <QStringView>
#include <QtGlobal>

#include <exception>

namespace WingScriptBind {

// Compact, Qt-native error taxonomy. These values are part of the public C++
// API; callers should branch on getError() instead of parsing text.
enum class Exception : quint16 {
    None = 0,
    InvalidArgument,
    InvalidScope,
    InvalidScopeKind,
    DuplicateName,
    TypeMismatch,
    DerivedClassAlreadyInitialized,
    WrongThread,
    CompileError,
    RuntimeError,
    RegistrationFailed,
    OutOfMemory,
    Unknown
};

// Static, allocation-free description of an error code. The returned view
// refers to static storage and remains valid for the program lifetime.
inline QStringView getErrorString(Exception error) noexcept {
    switch (error) {
    case Exception::None:
        return QStringLiteral("No error");
    case Exception::InvalidArgument:
        return QStringLiteral("Invalid argument");
    case Exception::InvalidScope:
        return QStringLiteral("Invalid registration scope");
    case Exception::InvalidScopeKind:
        return QStringLiteral(
            "This scope does not support the requested operation");
    case Exception::DuplicateName:
        return QStringLiteral("A registration with this name already exists");
    case Exception::TypeMismatch:
        return QStringLiteral(
            "The registered type does not match the existing type");
    case Exception::DerivedClassAlreadyInitialized:
        return QStringLiteral("A derived class can only be initialized once; "
                              "use beginClass() to reopen it");
    case Exception::WrongThread:
        return QStringLiteral(
            "ScriptBinding must be used from its creating thread");
    case Exception::CompileError:
        return QStringLiteral("Luau compilation or loading failed");
    case Exception::RuntimeError:
        return QStringLiteral("Luau script execution failed");
    case Exception::RegistrationFailed:
        return QStringLiteral("Script binding registration failed");
    case Exception::OutOfMemory:
        return QStringLiteral("Out of memory");
    case Exception::Unknown:
        return QStringLiteral("Unknown ScriptBinding error");
    }
    return QStringLiteral("Unknown ScriptBinding error");
}

// Error object used by the public API. The enum is the efficient, stable way
// to branch; QString text is materialized only when an error is actually
// thrown. what() is provided for logging and for interoperability with
// std::exception-based diagnostic code.
class ScriptException final : public std::exception {
public:
    explicit ScriptException(Exception error, QString detail = {})
        : error_(error) {
        errorString_ = WingScriptBind::getErrorString(error_).toString();
        if (!detail.isEmpty()) {
            errorString_.append(QStringLiteral(": "));
            errorString_.append(detail);
        }
        utf8What_ = errorString_.toUtf8();
    }

    Exception getError() const noexcept { return error_; }
    const QString &getErrorString() const noexcept { return errorString_; }
    const char *what() const noexcept override { return utf8What_.constData(); }

private:
    Exception error_ = Exception::Unknown;
    QString errorString_;
    QByteArray utf8What_;
};

} // namespace WingScriptBind

#endif

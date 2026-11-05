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

#ifndef WINGHEXDOCUMENT_H
#define WINGHEXDOCUMENT_H

#include "iwingplugincalls.h"

namespace WingHex {

class WingHexDocumentPrivate;
class WingHexDocumentUsagePrivate;

class WINGPLUGIN_EXPORT WingHexDocument final : public IWingPluginCallsOp {
public:
    class WINGPLUGIN_EXPORT UsingEnv final {
    public:
        ~UsingEnv();
        UsingEnv(UsingEnv &&other) noexcept;
        UsingEnv &operator=(UsingEnv &&other) noexcept;
        explicit operator bool() const;

    private:
        Q_DISABLE_COPY(UsingEnv)
        friend class WingHexDocument;
        explicit UsingEnv(WingHexDocumentUsagePrivate *data);
        WingHexDocumentUsagePrivate *d_ptr;
        Q_DECLARE_PRIVATE_D(d_ptr, WingHexDocumentUsage)
    };

    Q_REQUIRED_RESULT UsingEnv use();

    explicit WingHexDocument(IWingPlugin *plugin);
    ~WingHexDocument() override;
    WingHexDocument(WingHexDocument &&other) noexcept;
    WingHexDocument &operator=(WingHexDocument &&other) noexcept;

public:
    Q_REQUIRED_RESULT int openFile(const QString &filename);
    Q_REQUIRED_RESULT int openExt(const QString &ext, const QString &file);
    Q_REQUIRED_RESULT int openWorkSpace(const QString &filename);
    Q_REQUIRED_RESULT int close(bool force = false);
    Q_REQUIRED_RESULT int closeHandle();

    Q_REQUIRED_RESULT bool isValid() const;

public:
    Q_REQUIRED_RESULT DocumentInfo documentInfo() const;
    Q_REQUIRED_RESULT bool activate();

    // Inherited document operations require an active use() environment.

private:
    Q_DISABLE_COPY(WingHexDocument)

private:
    int openUrl(const QUrl &url);
    void release();

private:
    const QObject *getSender() const override;
    CallTable callTable() const override;
    QObject *callReceiver() const override;

private:
    using IWingPluginCallsOp::existsServiceHost;
    using IWingPluginCallsOp::invokeService;

private:
    WingHexDocumentPrivate *d_ptr;
    Q_DECLARE_PRIVATE(WingHexDocument)
};

} // namespace WingHex

#endif // WINGHEXDOCUMENT_H

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

#include "winghexdocument.h"
#include "iwingplugin.h"

#include <QHash>
#include <QPointer>
#include <QSharedPointer>
#include <QThread>
#include <atomic>
#include <utility>

using namespace WingHex;

namespace {
struct DocumentUsageState {
    QPointer<IWingPlugin> plugin;
    std::atomic_uint users{0};
    std::atomic_bool enabled{true};

    bool isActive() const {
        return plugin && enabled.load() && users.load() > 0;
    }
};
// Fresh scopes are registered only after switchDocument() accepts the call.
thread_local QHash<IWingPlugin *, QWeakPointer<DocumentUsageState>>
    activeUsages;
} // namespace

class WingHex::WingHexDocumentUsagePrivate {
public:
    explicit WingHexDocumentUsagePrivate(
        const QSharedPointer<DocumentUsageState> &usage)
        : state(usage) {
        state->users.fetch_add(1);
    }

    QSharedPointer<DocumentUsageState> state;

    ~WingHexDocumentUsagePrivate() { state->users.fetch_sub(1); }
};

WingHexDocument::UsingEnv::UsingEnv(WingHexDocumentUsagePrivate *data)
    : d_ptr(data) {}

WingHexDocument::UsingEnv::~UsingEnv() { delete d_ptr; }

WingHexDocument::UsingEnv::UsingEnv(UsingEnv &&other) noexcept
    : d_ptr(std::exchange(other.d_ptr, nullptr)) {}

WingHexDocument::UsingEnv &
WingHexDocument::UsingEnv::operator=(UsingEnv &&other) noexcept {
    if (this != &other) {
        delete d_ptr;
        d_ptr = std::exchange(other.d_ptr, nullptr);
    }
    return *this;
}

WingHexDocument::UsingEnv::operator bool() const {
    Q_D(const WingHexDocumentUsage);
    return d && d->state->plugin && d->state->enabled.load();
}

class WingHex::WingHexDocumentPrivate {
public:
    explicit WingHexDocumentPrivate(IWingPlugin *owner) : plugin(owner) {}

    QPointer<IWingPlugin> plugin;
    int handle = -1;
    QSharedPointer<DocumentUsageState> usage;

    void invalidateUsage() {
        if (usage) {
            usage->enabled = false;
        }
        usage.clear();
    }
    void resetHandle() {
        invalidateUsage();
        handle = -1;
    }

    bool inUse() const {
        return plugin && handle >= 0 && usage && usage->isActive();
    }

    // Called only for a valid document without an active scope of its own.
    bool beginUsage() {
        const auto owner = plugin.data();
        const auto it = activeUsages.constFind(owner);
        if (it != activeUsages.cend()) {
            const auto active = it->toStrongRef();
            if (active && active->isActive()) {
                return false;
            }
        } else {
            // Repeated scopes for a known plugin only need the lookup above.
            // Retire unrelated stale entries when registering a new plugin.
            for (auto stale = activeUsages.begin();
                 stale != activeUsages.end();) {
                const auto state = stale->toStrongRef();
                if (!state || !state->isActive()) {
                    stale = activeUsages.erase(stale);
                } else {
                    ++stale;
                }
            }
        }
        if (!plugin->switchDocument(handle)) {
            return false;
        }
        if (!usage || !usage->enabled.load()) {
            usage = QSharedPointer<DocumentUsageState>::create();
            usage->plugin = plugin;
        }
        activeUsages.insert(owner, usage);
        return true;
    }
};

WingHexDocument::WingHexDocument(IWingPlugin *plugin)
    : d_ptr(new WingHexDocumentPrivate(plugin)) {}

WingHexDocument::~WingHexDocument() {
    release();
    delete d_ptr;
}

WingHexDocument::WingHexDocument(WingHexDocument &&other) noexcept
    : d_ptr(std::exchange(other.d_ptr, nullptr)) {}

WingHexDocument &WingHexDocument::operator=(WingHexDocument &&other) noexcept {
    if (this != &other) {
        release();
        delete d_ptr;
        d_ptr = std::exchange(other.d_ptr, nullptr);
    }
    return *this;
}

WingHexDocument::UsingEnv WingHexDocument::use() {
    Q_D(WingHexDocument);
    if (documentInfo().documentType == DocumentType::InValid) {
        return UsingEnv(nullptr);
    }
    if (!d->inUse() && !d->beginUsage()) {
        return UsingEnv(nullptr);
    }
    return UsingEnv(new WingHexDocumentUsagePrivate(d->usage));
}

int WingHexDocument::openFile(const QString &filename) {
    return openUrl(WingHex::fileUrl(filename));
}

int WingHexDocument::openExt(const QString &ext, const QString &file) {
    return openUrl(WingHex::deviceUrl(ext, file));
}

int WingHexDocument::openWorkSpace(const QString &filename) {
    return openUrl(WingHex::workspaceUrl(filename));
}

int WingHexDocument::openUrl(const QUrl &url) {
    Q_D(WingHexDocument);
    if (!d) {
        return ErrFile::Error;
    }
    if (!d->plugin || !d->plugin->callReceiver()) {
        return ErrFile::Error;
    }
    release();
    const auto handle = d->plugin->openFile(url);
    if (handle < 0) {
        return handle;
    }
    d->handle = handle;
    return ErrFile::Success;
}

DocumentInfo WingHexDocument::documentInfo() const {
    Q_D(const WingHexDocument);
    if (!d) {
        return DocumentInfo{};
    }
    return d->plugin && d->handle >= 0 ? d->plugin->documentInfo(d->handle)
                                       : DocumentInfo{};
}

bool WingHexDocument::activate() {
    Q_D(WingHexDocument);
    if (!d) {
        return false;
    }
    return d->plugin->raiseDocument(d->handle);
}

int WingHexDocument::close(bool force) {
    Q_D(WingHexDocument);
    if (!d || d->handle < 0) {
        return ErrFile::NotExist;
    }
    if (!d->plugin || !d->plugin->callReceiver()) {
        d->resetHandle();
        return ErrFile::NotExist;
    }
    const auto result = d->plugin->closeFile(d->handle, force);
    if (result == ErrFile::Success || result == ErrFile::NotExist) {
        d->resetHandle();
    }
    return result;
}

int WingHexDocument::closeHandle() {
    Q_D(WingHexDocument);
    if (!d || d->handle < 0) {
        return ErrFile::NotExist;
    }
    if (!d->plugin || !d->plugin->callReceiver()) {
        d->resetHandle();
        return ErrFile::NotExist;
    }
    const auto result = d->plugin->closeFileHandle(d->handle);
    if (result == ErrFile::Success || result == ErrFile::NotExist) {
        d->resetHandle();
    }
    return result;
}

void WingHexDocument::release() {
    Q_D(WingHexDocument);
    if (!d) {
        return;
    }
    d->invalidateUsage();
    const auto handle = std::exchange(d->handle, -1);
    if (handle < 0 || !d->plugin || !d->plugin->callReceiver()) {
        return;
    }
    if (QThread::currentThread() == d->plugin->thread()) {
        Q_UNUSED(d->plugin->closeFileHandle(handle));
    } else {
        // Off-thread destruction queues handle release to the GUI thread.
        auto plugin = d->plugin;
        QMetaObject::invokeMethod(
            d->plugin,
            [plugin, handle] {
                if (plugin && plugin->callReceiver()) {
                    Q_UNUSED(plugin->closeFileHandle(handle));
                }
            },
            Qt::QueuedConnection);
    }
}

const QObject *WingHexDocument::getSender() const {
    Q_D(const WingHexDocument);
    return d ? d->plugin.data() : nullptr;
}

CallTable WingHexDocument::callTable() const {
    Q_D(const WingHexDocument);
    return d && d->inUse() ? d->plugin->callTable() : CallTable{};
}

QObject *WingHexDocument::callReceiver() const {
    Q_D(const WingHexDocument);
    return d && d->plugin ? d->plugin->callReceiver() : nullptr;
}

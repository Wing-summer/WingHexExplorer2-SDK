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

#include "wingplugincalls.h"

#include "wingcore.h"
#include "wingplugincalls_p.h"

#include <QDynamicPropertyChangeEvent>
#include <QEvent>

using namespace WingHex;

Q_DECLARE_METATYPE(QMetaMethod)

WingPluginCallsCore::WingPluginCallsCore()
    : QObject(), d_ptr(new WingPluginCallsCorePrivate) {}

WingPluginCallsCore::~WingPluginCallsCore() { delete d_ptr; }

QObject *WingPluginCallsCore::callReceiver() const { return d_ptr->_fnCaller; }

CallTable WingPluginCallsCore::callTable() const { return d_ptr->_fnTable; }

bool WingPluginCallsCore::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::DynamicPropertyChange) {
        auto e = static_cast<QDynamicPropertyChangeEvent *>(event);
        if (e) {
            auto ppname = e->propertyName();
            if (ppname == CALL_POINTER_PROPERTY) {
                Q_D(WingPluginCallsCore);
                if (d->_fnCaller) {
                    std::abort();
                }

                auto var = watched->property(CALL_POINTER_PROPERTY);
                if (!var.canConvert<quintptr>()) {
                    std::abort();
                }

                d->_fnCaller =
                    reinterpret_cast<QObject *>(var.value<quintptr>());
            }

            if (ppname == CALL_TABLE_PROPERTY) {
                Q_D(WingPluginCallsCore);
                if (!d->_fnTable.isEmpty()) {
                    std::abort();
                }

                auto var = watched->property(CALL_TABLE_PROPERTY);
                if (!var.canConvert<QHash<FunctionSig, QMetaMethod>>()) {
                    std::abort();
                }

                d->_fnTable = var.value<QHash<FunctionSig, QMetaMethod>>();
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

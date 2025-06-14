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

#include "wingpluginwidget.h"

#include "WingPlugin/wingcore.h"
#include "WingPlugin/wingplugincalls_p.h"
#include "iwingplugin.h"

using namespace WingHex;

WingPluginWidget::WingPluginWidget(IWingPlugin *plg, QWidget *parent)
    : QWidget(parent), WingHex::IWingPluginAPICalls(),
      d_ptr(new WingPluginCallsCorePrivate) {
    Q_ASSERT(plg);

    {
        auto var = plg->property(CALL_POINTER_PROPERTY);
        if (!var.canConvert<quintptr>()) {
            std::abort();
        }

        d_ptr->_fnCaller = reinterpret_cast<QObject *>(var.value<quintptr>());
    }

    {
        auto var = plg->property(CALL_TABLE_PROPERTY);
        if (!var.canConvert<QHash<FunctionSig, QMetaMethod>>()) {
            std::abort();
        }

        d_ptr->_fnTable = var.value<QHash<FunctionSig, QMetaMethod>>();
    }
}

WingPluginWidget::~WingPluginWidget() { delete d_ptr; }

const QObject *WingPluginWidget::getSender() const { return this; }

CallTable WingPluginWidget::callTable() const { return d_ptr->_fnTable; }

QObject *WingPluginWidget::callReceiver() const { return d_ptr->_fnCaller; }

WingPluginDialog::WingPluginDialog(IWingPlugin *plg, QWidget *parent)
    : QDialog(parent), WingHex::IWingPluginAPICalls(),
      d_ptr(new WingPluginCallsCorePrivate) {
    Q_ASSERT(plg);

    {
        auto var = plg->property(CALL_POINTER_PROPERTY);
        if (!var.canConvert<quintptr>()) {
            std::abort();
        }

        d_ptr->_fnCaller = reinterpret_cast<QObject *>(var.value<quintptr>());
    }

    {
        auto var = plg->property(CALL_TABLE_PROPERTY);
        if (!var.canConvert<QHash<FunctionSig, QMetaMethod>>()) {
            std::abort();
        }

        d_ptr->_fnTable = var.value<QHash<FunctionSig, QMetaMethod>>();
    }
}

WingPluginDialog::~WingPluginDialog() { delete d_ptr; }

const QObject *WingPluginDialog::getSender() const { return this; }

CallTable WingPluginDialog::callTable() const { return d_ptr->_fnTable; }

QObject *WingPluginDialog::callReceiver() const { return d_ptr->_fnCaller; }

WingPluginWindow::WingPluginWindow(IWingPlugin *plg, QWindow *parent)
    : QWindow(parent), WingHex::IWingPluginAPICalls() {
    init(plg);
}

WingPluginWindow::WingPluginWindow(IWingPlugin *plg, QScreen *parent)
    : QWindow(parent), WingHex::IWingPluginAPICalls() {
    init(plg);
}

WingPluginWindow::~WingPluginWindow() { delete d_ptr; }

const QObject *WingPluginWindow::getSender() const { return this; }

CallTable WingPluginWindow::callTable() const { return d_ptr->_fnTable; }

QObject *WingPluginWindow::callReceiver() const { return d_ptr->_fnCaller; }

void WingPluginWindow::init(IWingPlugin *plg) {
    Q_ASSERT(plg);
    d_ptr = new WingPluginCallsCorePrivate;
    {
        auto var = plg->property(CALL_POINTER_PROPERTY);
        if (!var.canConvert<quintptr>()) {
            std::abort();
        }

        d_ptr->_fnCaller = reinterpret_cast<QObject *>(var.value<quintptr>());
    }

    {
        auto var = plg->property(CALL_TABLE_PROPERTY);
        if (!var.canConvert<QHash<FunctionSig, QMetaMethod>>()) {
            std::abort();
        }

        d_ptr->_fnTable = var.value<QHash<FunctionSig, QMetaMethod>>();
    }
}

WingPluginMainWindow::WingPluginMainWindow(IWingPlugin *plg, QWidget *parent)
    : QMainWindow(parent), WingHex::IWingPluginAPICalls(),
      d_ptr(new WingPluginCallsCorePrivate) {
    Q_ASSERT(plg);

    {
        auto var = plg->property(CALL_POINTER_PROPERTY);
        if (!var.canConvert<quintptr>()) {
            std::abort();
        }

        d_ptr->_fnCaller = reinterpret_cast<QObject *>(var.value<quintptr>());
    }

    {
        auto var = plg->property(CALL_TABLE_PROPERTY);
        if (!var.canConvert<QHash<FunctionSig, QMetaMethod>>()) {
            std::abort();
        }

        d_ptr->_fnTable = var.value<QHash<FunctionSig, QMetaMethod>>();
    }
}

WingPluginMainWindow::~WingPluginMainWindow() { delete d_ptr; }

const QObject *WingPluginMainWindow::getSender() const { return this; }

CallTable WingPluginMainWindow::callTable() const { return d_ptr->_fnTable; }

QObject *WingPluginMainWindow::callReceiver() const { return d_ptr->_fnCaller; }

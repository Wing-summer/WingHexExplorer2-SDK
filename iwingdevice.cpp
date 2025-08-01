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

#include "iwingdevice.h"

using namespace WingHex;

IWingDevice::IWingDevice(QObject *parent)
    : IWingPluginBase(), _core(new WingPluginCallsCore) {
    this->installEventFilter(_core);
}

QIcon IWingDevice::supportedFileIcon() const { return {}; }

QString IWingDevice::onOpenFileBegin() { return {}; }

const QObject *IWingDevice::getSender() const { return this; }

CallTable IWingDevice::callTable() const { return _core->callTable(); }

QObject *IWingDevice::callReceiver() const { return _core->callReceiver(); }

WingIODevice::WingIODevice(QObject *parent) : QIODevice(parent) {}

bool WingIODevice::keepSize() const { return false; }

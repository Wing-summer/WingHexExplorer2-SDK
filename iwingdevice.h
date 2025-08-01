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

#ifndef IWINGDEVICE_H
#define IWINGDEVICE_H

#include "iwingpluginbase.h"

#include <QIODevice>

namespace WingHex {

class WINGPLUGIN_EXPORT WingIODevice : public QIODevice {
    Q_OBJECT

public:
    WingIODevice(QObject *parent = nullptr);

    // can not change size during editing (default: changing-abled)
    virtual bool keepSize() const;
};

class WINGPLUGIN_EXPORT IWingDevice : public IWingPluginBase {
    Q_OBJECT
public:
    explicit IWingDevice(QObject *parent = nullptr);

public:
    virtual QString supportedFileExtDisplayName() const = 0;

    virtual QIcon supportedFileIcon() const;

public:
    virtual QString onOpenFileBegin();

    virtual WingIODevice *onOpenFile(const QString &path) = 0;

    virtual bool onCloseFile(WingIODevice *dev) = 0;

    // WingPluginCallConvertor interface
protected:
    virtual const QObject *getSender() const override;

    // WingPluginCalls interface
protected:
    virtual CallTable callTable() const override;
    virtual QObject *callReceiver() const override;

private:
    WingPluginCallsCore *_core;
};

} // namespace WingHex

Q_DECLARE_INTERFACE(WingHex::IWingDevice, "com.wingsummer.iwingdevice")

#endif // IWINGDEVICE_H

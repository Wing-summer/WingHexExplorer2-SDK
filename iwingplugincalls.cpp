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

#include "iwingplugincalls.h"
#include "WingPlugin/wingcore.h"

using namespace WingHex;

HexPosition::HexPosition() : line(0), column(0), lineWidth(0), nibbleindex(0) {}

qsizetype HexPosition::offset() const {
    return static_cast<qsizetype>(line * lineWidth) + column;
}

qsizetype HexPosition::operator-(const HexPosition &rhs) const {
    return qsizetype(this->offset() - rhs.offset());
}

bool HexPosition::operator==(const HexPosition &rhs) const {
    return (line == rhs.line) && (column == rhs.column) &&
           (nibbleindex == rhs.nibbleindex);
}

bool HexPosition::operator!=(const HexPosition &rhs) const {
    return (line != rhs.line) || (column != rhs.column) ||
           (nibbleindex != rhs.nibbleindex);
}

//=================================

IWingPluginCalls::IWingPluginCalls() : WingHex::IWingPluginCallsOp() {}

IWingPluginCallsOp::IWingPluginCallsOp() : WingHex::WingPluginCalls() {}

bool IWingPluginCallsOp::existsServiceHost(const QString &puid) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::existsServiceHost);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             puid);
    return ret;
}

bool IWingPluginCalls::isCurrentDocEditing() {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::isCurrentDocEditing);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

QString IWingPluginCallsOp::currentDocFilename() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::currentDocFilename);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::isReadOnly() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::isReadOnly);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::isInsertionMode() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::isInsertionMode);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::isKeepSize() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::isKeepSize);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::isLocked() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::isLocked);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::documentLines() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::documentLines);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::documentBytes() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::documentBytes);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::currentRow() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::currentRow);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::currentColumn() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::currentColumn);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::currentOffset() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::currentOffset);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::selectedLength() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectedLength);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

QByteArray IWingPluginCallsOp::selectedBytes(qsizetype index) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectedBytes);
    QByteArray ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             index);
    return ret;
}

QByteArrayList IWingPluginCallsOp::selectionBytes() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectionBytes);
    QByteArrayList ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qsizetype IWingPluginCallsOp::selectionLength(qsizetype index) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectionLength);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             index);
    return ret;
}

qsizetype IWingPluginCallsOp::selectionCount() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectionCount);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::stringVisible() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::stringVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::addressVisible() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::addressVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::headerVisible() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::headerVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

quintptr IWingPluginCallsOp::addressBase() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::addressBase);
    quintptr ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::isModified() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::isModified);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

qint8 IWingPluginCallsOp::readInt8(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readInt8);
    qint8 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

qint16 IWingPluginCallsOp::readInt16(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readInt16);
    qint16 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

qint32 IWingPluginCallsOp::readInt32(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readInt32);
    qint32 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

qint64 IWingPluginCallsOp::readInt64(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readInt64);
    qint64 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

quint8 IWingPluginCallsOp::readUInt8(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readUInt8);
    quint8 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

quint16 IWingPluginCallsOp::readUInt16(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readUInt16);
    quint16 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

quint32 IWingPluginCallsOp::readUInt32(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readUInt32);
    quint32 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

quint64 IWingPluginCallsOp::readUInt64(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readUInt64);
    quint64 ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

float IWingPluginCallsOp::readFloat(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readFloat);
    float ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

double IWingPluginCallsOp::readDouble(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readDouble);
    double ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

QString IWingPluginCallsOp::readString(qsizetype offset,
                                       const QString &encoding) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readString);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, encoding);
    return ret;
}

QByteArray IWingPluginCallsOp::readBytes(qsizetype offset, qsizetype count) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::readBytes);
    QByteArray ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, count);
    return ret;
}

qsizetype IWingPluginCallsOp::findNext(qsizetype begin, const QByteArray &ba) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::findNext);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             begin, ba);
    return ret;
}

qsizetype IWingPluginCallsOp::findPrevious(qsizetype begin,
                                           const QByteArray &ba) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::findPrevious);
    qsizetype ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             begin, ba);
    return ret;
}

QString IWingPluginCallsOp::bookMarkComment(qsizetype pos) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::bookMarkComment);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             pos);
    return ret;
}

bool IWingPluginCallsOp::existBookMark(qsizetype pos) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::existBookMark);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             pos);
    return ret;
}

bool IWingPluginCalls::switchDocument(int handle) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::switchDocument);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle);
    return ret;
}

bool IWingPluginCalls::raiseDocument(int handle) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::raiseDocument);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle);
    return ret;
}

bool IWingPluginCallsOp::setLockedFile(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setLockedFile);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setKeepSize(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setKeepSize);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setStringVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setStringVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setAddressVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setAddressVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setHeaderVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setHeaderVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setAddressBase(quintptr base) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setAddressBase);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             base);
    return ret;
}

bool IWingPluginCallsOp::beginMarco(const QString &txt) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::beginMarco);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             txt);
    return ret;
}

bool IWingPluginCallsOp::endMarco() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::endMarco);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::writeInt8(qsizetype offset, qint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeInt16(qsizetype offset, qint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeInt32(qsizetype offset, qint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeInt64(qsizetype offset, qint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeUInt8(qsizetype offset, quint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeUInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeUInt16(qsizetype offset, quint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeUInt32(qsizetype offset, quint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeUInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeUInt64(qsizetype offset, quint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeUInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeFloat(qsizetype offset, float value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeFloat);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeDouble(qsizetype offset, double value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeDouble);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::writeString(qsizetype offset, const QString &value,
                                     const QString &encoding) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeString);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value, encoding);
    return ret;
}

bool IWingPluginCallsOp::writeBytes(qsizetype offset, const QByteArray &data) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::writeBytes);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, data);
    return ret;
}

bool IWingPluginCallsOp::insertInt8(qsizetype offset, qint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertInt16(qsizetype offset, qint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertInt32(qsizetype offset, qint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertInt64(qsizetype offset, qint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertUInt8(qsizetype offset, quint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertUInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertUInt16(qsizetype offset, quint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertUInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertUInt32(qsizetype offset, quint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertUInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertUInt64(qsizetype offset, quint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertUInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertFloat(qsizetype offset, float value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertFloat);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertDouble(qsizetype offset, double value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertDouble);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value);
    return ret;
}

bool IWingPluginCallsOp::insertString(qsizetype offset, const QString &value,
                                      const QString &encoding) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertString);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, value, encoding);
    return ret;
}

bool IWingPluginCallsOp::insertBytes(qsizetype offset, const QByteArray &data) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::insertBytes);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, data);
    return ret;
}

bool IWingPluginCallsOp::appendInt8(qint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendInt16(qint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendInt32(qint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendInt64(qint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendUInt8(quint8 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendUInt8);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendUInt16(quint16 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendUInt16);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendUInt32(quint32 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendUInt32);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendUInt64(quint64 value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendUInt64);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendFloat(float value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendFloat);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendDouble(double value) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendDouble);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value);
    return ret;
}

bool IWingPluginCallsOp::appendString(const QString &value,
                                      const QString &encoding) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendString);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             value, encoding);
    return ret;
}

bool IWingPluginCallsOp::appendBytes(const QByteArray &data) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::appendBytes);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             data);
    return ret;
}

bool IWingPluginCallsOp::removeBytes(qsizetype offset, qsizetype len) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::removeBytes);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, len);
    return ret;
}

bool IWingPluginCallsOp::moveTo(qsizetype line, qsizetype column,
                                int nibbleindex, bool clearSelection) {
    SETUP_CALL_CONTEXT((QOverload<qsizetype, qsizetype, int, bool>::of(
        &IWingPluginCallsOp::moveTo)));
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             line, column, nibbleindex, clearSelection);
    return ret;
}

bool IWingPluginCallsOp::moveTo(qsizetype offset, bool clearSelection) {
    SETUP_CALL_CONTEXT(
        (QOverload<qsizetype, bool>::of(&IWingPluginCallsOp::moveTo)));
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, clearSelection);
    return ret;
}

bool IWingPluginCallsOp::select(qsizetype offset, qsizetype length,
                                SelectionMode mode) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::select);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset, length, mode);
    return ret;
}

bool IWingPluginCallsOp::setInsertionMode(bool isinsert) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setInsertionMode);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             isinsert);
    return ret;
}

bool IWingPluginCallsOp::foreground(qsizetype begin, qsizetype length,
                                    const QColor &fgcolor) {
    return metadata(begin, length, fgcolor, {}, {});
}

bool IWingPluginCallsOp::background(qsizetype begin, qsizetype length,
                                    const QColor &bgcolor) {
    return metadata(begin, length, {}, bgcolor, {});
}

bool IWingPluginCallsOp::comment(qsizetype begin, qsizetype length,
                                 const QString &comment) {
    return metadata(begin, length, {}, {}, comment);
}

bool IWingPluginCallsOp::metadata(qsizetype begin, qsizetype length,
                                  const QColor &fgcolor, const QColor &bgcolor,
                                  const QString &comment) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::metadata);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             begin, length, fgcolor, bgcolor, comment);
    return ret;
}

bool IWingPluginCallsOp::removeMetadata(qsizetype offset) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::removeMetadata);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             offset);
    return ret;
}

bool IWingPluginCallsOp::clearMetadata() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::clearMetadata);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::setMetaVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setMetaVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setMetafgVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setMetafgVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setMetabgVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setMetabgVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::setMetaCommentVisible(bool b) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::setMetaCommentVisible);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             b);
    return ret;
}

bool IWingPluginCallsOp::addBookMark(qsizetype pos, const QString &comment) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::addBookMark);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             pos, comment);
    return ret;
}

bool IWingPluginCallsOp::modBookMark(qsizetype pos, const QString &comment) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::modBookMark);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             pos, comment);
    return ret;
}

bool IWingPluginCallsOp::removeBookMark(qsizetype pos) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::removeBookMark);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             pos);
    return ret;
}

bool IWingPluginCallsOp::clearBookMark() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::clearBookMark);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

bool IWingPluginCallsOp::invokeServiceImpl(const QString &puid,
                                           const MetaCallInfo &infos) const {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::invokeServiceImpl);
    bool ret = false;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             puid, infos);
    return ret;
}

bool IWingPluginCalls::closeAllFiles() {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::closeAllFiles);
    bool ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

ErrFile IWingPluginCalls::openWorkSpace(const QString &filename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::openWorkSpace);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             filename);
    return ret;
}

ErrFile IWingPluginCalls::saveAsCurrent(const QString &savename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::saveAsCurrent);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             savename);
    return ret;
}

ErrFile IWingPluginCalls::exportCurrent(const QString &savename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::exportCurrent);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             savename);
    return ret;
}

ErrFile IWingPluginCalls::saveCurrent() {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::saveCurrent);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

ErrFile IWingPluginCalls::closeCurrent(bool force) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::closeCurrent);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             force);
    return ret;
}

ErrFile IWingPluginCalls::openCurrent() {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::openCurrent);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

ErrFile IWingPluginCalls::saveAsFile(int handle, const QString &savename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::saveAsFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle, savename);
    return ret;
}

ErrFile IWingPluginCalls::exportFile(int handle, const QString &savename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::exportFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle, savename);
    return ret;
}

ErrFile IWingPluginCalls::saveFile(int handle) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::saveFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle);
    return ret;
}

ErrFile IWingPluginCalls::closeFile(int handle, bool force) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::closeFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle, force);
    return ret;
}

ErrFile IWingPluginCalls::closeHandle(int handle) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::closeHandle);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             handle);
    return ret;
}

ErrFile IWingPluginCalls::openExtFile(const QString &ext, const QString &file) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::openExtFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             ext, file);
    return ret;
}

ErrFile IWingPluginCalls::openFile(const QString &filename) {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::openFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             filename);
    return ret;
}

ErrFile IWingPluginCalls::newFile() {
    SETUP_CALL_CONTEXT(&IWingPluginCalls::newFile);
    ErrFile ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

HexPosition IWingPluginCallsOp::selectionEnd(qsizetype index) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectionEnd);
    HexPosition ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             index);
    return ret;
}

HexPosition IWingPluginCallsOp::selectionStart(qsizetype index) {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::selectionStart);
    HexPosition ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             index);
    return ret;
}

HexPosition IWingPluginCallsOp::currentPos() {
    SETUP_CALL_CONTEXT(&IWingPluginCallsOp::currentPos);
    HexPosition ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret),
             getSender());
    return ret;
}

IWingPluginAPICalls::IWingPluginAPICalls()
    : IWingPluginCalls(), IWingPluginBaseCalls() {}

IWingEditorViewCalls::IWingEditorViewCalls()
    : IWingPluginCallsOp(), IWingPluginBaseCalls() {}

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

#ifndef IWINGPLUGINCALLS_H
#define IWINGPLUGINCALLS_H

#include "WingPlugin/iwingpluginbasecalls.h"
#include "WingPlugin/wingcore.h"

#include <QAbstractItemModel>
#include <QGenericReturnArgument>
#include <QMetaObject>

namespace WingHex {

class IWingPlugin;
class WingHexDocument;

struct WINGPLUGIN_EXPORT DocumentInfo {
    QUrl fileNameUrl;
    QString workSpaceName;
    DocumentType documentType = DocumentType::InValid;
    bool isSaved = false;
    bool isReadOnly = false;
};

struct WINGPLUGIN_EXPORT MetadataInfo {
    qsizetype begin;
    qsizetype end;
    QColor foreground;
    QColor background;
    QString comment;
};

class WINGPLUGIN_EXPORT IWingPluginCallsOp : public WingPluginCalls {
public:
    explicit IWingPluginCallsOp();

public:
    bool existsServiceHost(const QString &puid) const;

public:
#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method, Qt::ConnectionType c,
                  QMetaMethodReturnArgument r, Args &&...arguments) const {
        auto h =
            QtPrivate::invokeMethodHelper(r, std::forward<Args>(arguments)...);
        return invokeServiceImpl(
            puid,
            std::make_tuple(method, c, h.parameterCount(), h.parameters.data(),
                            h.typeNames.data(), h.metaTypes.data()));
    }

    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method, Qt::ConnectionType c,
                  Args &&...arguments) const {
        return invokeService(puid, method, c, QMetaMethodReturnArgument{},
                             std::forward<Args>(arguments)...);
    }

    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method,
                  QMetaMethodReturnArgument r, Args &&...arguments) const {
        return invokeService(puid, method, Qt::DirectConnection, r,
                             std::forward<Args>(arguments)...);
    }

    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method,
                  Args &&...arguments) const {
        return invokeService(puid, method, Qt::DirectConnection,
                             std::forward<Args>(arguments)...);
    }
#else
    template <typename T, typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method, Qt::ConnectionType c,
                  QTemplatedMetaMethodReturnArgument<T> r,
                  Args &&...arguments) const {
        auto h =
            QtPrivate::invokeMethodHelper(r, std::forward<Args>(arguments)...);
        return invokeServiceImpl(
            puid,
            std::make_tuple(method, c, h.parameterCount(), h.parameters.data(),
                            h.typeNames.data(), h.metaTypes.data()));
    }

    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method, Qt::ConnectionType c,
                  Args &&...arguments) const {
        return invokeService(puid, method, c,
                             QTemplatedMetaMethodReturnArgument<void>{},
                             std::forward<Args>(arguments)...);
    }

    template <typename T, typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method,
                  QTemplatedMetaMethodReturnArgument<T> r,
                  Args &&...arguments) const {
        return invokeService(puid, method, Qt::DirectConnection, r,
                             std::forward<Args>(arguments)...);
    }

    template <typename... Args>
    QtPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
    invokeService(const QString &puid, const char *method,
                  Args &&...arguments) const {
        return invokeService(puid, method, Qt::DirectConnection,
                             std::forward<Args>(arguments)...);
    }
#endif

public:
    Q_REQUIRED_RESULT QUrl currentDocFile() const;
    Q_REQUIRED_RESULT QString currentDocFileName() const;
    Q_REQUIRED_RESULT QUrl currentDocWorkSpace() const;
    Q_REQUIRED_RESULT QString currentDocWorkSpaceName() const;

    // document
    Q_REQUIRED_RESULT bool isReadOnly() const;
    Q_REQUIRED_RESULT bool isInsertionMode() const;
    Q_REQUIRED_RESULT bool isKeepSize() const;
    Q_REQUIRED_RESULT bool isLocked() const;
    Q_REQUIRED_RESULT qsizetype documentLines() const;
    Q_REQUIRED_RESULT qsizetype documentBytes() const;
    Q_REQUIRED_RESULT WingHex::HexPosition currentPos() const;
    Q_REQUIRED_RESULT qsizetype currentRow() const;
    Q_REQUIRED_RESULT qsizetype currentColumn() const;
    Q_REQUIRED_RESULT qsizetype currentOffset() const;
    Q_REQUIRED_RESULT qsizetype selectedLength() const;

    Q_REQUIRED_RESULT QByteArray selectedBytes(qsizetype index) const;
    Q_REQUIRED_RESULT QByteArrayList selectionBytes() const;

    Q_REQUIRED_RESULT WingHex::HexPosition
    selectionStart(qsizetype index) const;
    Q_REQUIRED_RESULT WingHex::HexPosition selectionEnd(qsizetype index) const;
    Q_REQUIRED_RESULT qsizetype selectionLength(qsizetype index) const;
    Q_REQUIRED_RESULT qsizetype selectionCount() const;

    Q_REQUIRED_RESULT bool stringVisible() const;
    Q_REQUIRED_RESULT bool addressVisible() const;
    Q_REQUIRED_RESULT bool headerVisible() const;
    Q_REQUIRED_RESULT quintptr addressBase() const;
    Q_REQUIRED_RESULT bool isModified() const;

    Q_REQUIRED_RESULT qint8 readInt8(qsizetype offset) const;
    Q_REQUIRED_RESULT qint16 readInt16(qsizetype offset) const;
    Q_REQUIRED_RESULT qint32 readInt32(qsizetype offset) const;
    Q_REQUIRED_RESULT qint64 readInt64(qsizetype offset) const;
    Q_REQUIRED_RESULT quint8 readUInt8(qsizetype offset) const;
    Q_REQUIRED_RESULT quint16 readUInt16(qsizetype offset) const;
    Q_REQUIRED_RESULT quint32 readUInt32(qsizetype offset) const;
    Q_REQUIRED_RESULT quint64 readUInt64(qsizetype offset) const;
    Q_REQUIRED_RESULT float readFloat(qsizetype offset) const;
    Q_REQUIRED_RESULT double readDouble(qsizetype offset) const;
    Q_REQUIRED_RESULT QString readString(qsizetype offset,
                                         const QString &encoding = {}) const;
    Q_REQUIRED_RESULT QByteArray readBytes(qsizetype offset,
                                           qsizetype count) const;

    Q_REQUIRED_RESULT qsizetype findNext(qsizetype begin,
                                         const QByteArray &ba) const;
    Q_REQUIRED_RESULT qsizetype findPrevious(qsizetype begin,
                                             const QByteArray &ba) const;

    bool setLockedFile(bool b);
    bool setKeepSize(bool b);
    bool setStringVisible(bool b);
    bool setAddressVisible(bool b);
    bool setHeaderVisible(bool b);
    bool setAddressBase(quintptr base);

    Q_REQUIRED_RESULT bool beginMarco(const QString &txt = {});
    bool endMarco();
    Q_REQUIRED_RESULT bool isMacroEmpty() const;
    bool resetMarco();

    bool writeInt8(qsizetype offset, qint8 value);
    bool writeInt16(qsizetype offset, qint16 value);
    bool writeInt32(qsizetype offset, qint32 value);
    bool writeInt64(qsizetype offset, qint64 value);
    bool writeUInt8(qsizetype offset, quint8 value);
    bool writeUInt16(qsizetype offset, quint16 value);
    bool writeUInt32(qsizetype offset, quint32 value);
    bool writeUInt64(qsizetype offset, quint64 value);
    bool writeFloat(qsizetype offset, float value);
    bool writeDouble(qsizetype offset, double value);
    bool writeString(qsizetype offset, const QString &value,
                     const QString &encoding = QString());
    bool writeBytes(qsizetype offset, const QByteArray &data);

    bool insertInt8(qsizetype offset, qint8 value);
    bool insertInt16(qsizetype offset, qint16 value);
    bool insertInt32(qsizetype offset, qint32 value);
    bool insertInt64(qsizetype offset, qint64 value);
    bool insertUInt8(qsizetype offset, quint8 value);
    bool insertUInt16(qsizetype offset, quint16 value);
    bool insertUInt32(qsizetype offset, quint32 value);
    bool insertUInt64(qsizetype offset, quint64 value);
    bool insertFloat(qsizetype offset, float value);
    bool insertDouble(qsizetype offset, double value);
    bool insertString(qsizetype offset, const QString &value,
                      const QString &encoding = {});
    bool insertBytes(qsizetype offset, const QByteArray &data);

    bool appendInt8(qint8 value);
    bool appendInt16(qint16 value);
    bool appendInt32(qint32 value);
    bool appendInt64(qint64 value);
    bool appendUInt8(quint8 value);
    bool appendUInt16(quint16 value);
    bool appendUInt32(quint32 value);
    bool appendUInt64(quint64 value);
    bool appendFloat(float value);
    bool appendDouble(double value);
    bool appendString(const QString &value, const QString &encoding = {});
    bool appendBytes(const QByteArray &data);

    bool removeBytes(qsizetype offset, qsizetype len);

    // cursor
    bool moveTo(qsizetype line, qsizetype column, int nibbleindex = 1,
                bool clearSelection = true);
    bool moveTo(qsizetype offset, bool clearSelection = true);
    bool select(qsizetype offset, qsizetype length,
                SelectionMode mode = SelectionMode::Add);
    bool setInsertionMode(bool isinsert);

    // metadata
    bool foreground(qsizetype begin, qsizetype length, const QColor &fgcolor);
    bool background(qsizetype begin, qsizetype length, const QColor &bgcolor);
    bool comment(qsizetype begin, qsizetype length, const QString &comment);
    bool metadata(qsizetype begin, qsizetype length, const QColor &fgcolor,
                  const QColor &bgcolor, const QString &comment);
    bool removeMetadata(qsizetype offset);
    bool clearMetadata();

    bool setMetaVisible(bool b);
    bool setMetafgVisible(bool b);
    bool setMetabgVisible(bool b);
    bool setMetaCommentVisible(bool b);

    Q_REQUIRED_RESULT WingHex::MetadataInfo metadataInfo(qsizetype pos) const;
    Q_REQUIRED_RESULT WingHex::MetadataInfo
    metadataInfoByIndex(qsizetype index) const;
    Q_REQUIRED_RESULT qint64 metadataCount() const;

    // bookmark
    Q_REQUIRED_RESULT QString bookMarkComment(qsizetype pos) const;
    Q_REQUIRED_RESULT bool existBookMark(qsizetype pos) const;
    Q_REQUIRED_RESULT qint64 bookMarkPos(qsizetype index) const;
    Q_REQUIRED_RESULT qint64 bookMarkCount() const;

    bool addBookMark(qsizetype pos, const QString &comment);
    bool modBookMark(qsizetype pos, const QString &comment);
    bool removeBookMark(qsizetype pos);
    bool clearBookMark();

private slots:
    bool invokeServiceImpl(const QString &puid,
                           const WingHex::MetaCallInfo &infos) const;
};

class WINGPLUGIN_EXPORT IWingEditorViewCalls : public IWingPluginCallsOp,
                                               public IWingPluginBaseCalls {
public:
    explicit IWingEditorViewCalls();
};

class WINGPLUGIN_EXPORT IWingPluginCalls : public IWingPluginCallsOp {
public:
    explicit IWingPluginCalls();

public:
    Q_REQUIRED_RESULT bool isCurrentDocEditing() const;

public:
    // document
    Q_REQUIRED_RESULT bool switchDocument(int handle);
    bool raiseDocument(int handle);

    Q_REQUIRED_RESULT WingHex::DocumentInfo documentInfo(int handle) const;

    // mainwindow
    Q_REQUIRED_RESULT int openFile(const QUrl &file);
    int closeFile(int handle, bool force);
    int closeFileHandle(int handle);

    Q_REQUIRED_RESULT int openCurrent();
    int closeCurrent();
};

class WINGPLUGIN_EXPORT IWingPluginAPICalls : public IWingPluginCalls,
                                              public IWingPluginBaseCalls {
public:
    explicit IWingPluginAPICalls();
};

} // namespace WingHex

Q_DECLARE_METATYPE(const char *);
Q_DECLARE_METATYPE(QModelIndex)
Q_DECLARE_METATYPE(WingHex::HexPosition)
Q_DECLARE_METATYPE(WingHex::MetadataInfo)

Q_DECLARE_METATYPE(WingHex::DocumentInfo)

#endif // IWINGPLUGINCALLS_H

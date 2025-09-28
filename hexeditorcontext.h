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

#ifndef HEXEDITORCONTEXT_H
#define HEXEDITORCONTEXT_H

#include "WingPlugin/wingcore.h"
#include "WingPlugin/wingplugin_global.h"

namespace WingHex {

class WINGPLUGIN_EXPORT HexEditorContext : public QObject {
    Q_OBJECT

public:
    explicit HexEditorContext(QObject *parent = nullptr);

public:
    virtual QString docFileName() const = 0;
    virtual QFontMetricsF fontMetrics() const = 0;

public:
    // color
    virtual QColor headerColor() const = 0;
    virtual QColor addressColor() const = 0;
    virtual QColor bytesBackground() const = 0;
    virtual QColor bytesAlterBackground() const = 0;
    virtual QColor bytesColor() const = 0;
    virtual QColor selectionColor() const = 0;
    virtual QColor selBackgroundColor() const = 0;
    virtual QColor borderColor() const = 0;

    virtual bool stringAreaVisible() const = 0;
    virtual bool addressAreaVisible() const = 0;
    virtual bool headerAreaVisible() const = 0;

public:
    virtual qsizetype documentLastLine() const = 0;
    virtual qsizetype documentLastColumn() const = 0;
    virtual qsizetype documentLines() const = 0;
    virtual int documentWidth() const = 0;
    virtual int lineHeight() const = 0;
    virtual int borderSize() const = 0;
    virtual int hexLineWidth() const = 0;
    virtual int areaIndent() const = 0;

    virtual int addressWidth() const = 0;
    virtual int headerHeight() const = 0;
    virtual int hexColumnX() const = 0;
    virtual int stringColumnX() const = 0;
    virtual int endColumnX() const = 0;
    virtual qreal cellWidth() const = 0;
    virtual int nCellsWidth(int n) const = 0;
    virtual QRect lineRect(qsizetype line, qsizetype firstline) const = 0;

public:
    // HexCursor
    virtual HexPosition position() const = 0;

    virtual qsizetype selectionCount() const = 0;
    virtual HexPosition selectionStart(qsizetype index) const = 0;
    virtual HexPosition selectionEnd(qsizetype index) const = 0;
    virtual qsizetype selectionLength(qsizetype index) const = 0;

    virtual bool isInInsertionMode() const = 0;

    virtual qsizetype currentLine() const = 0;
    virtual int currentColumn() const = 0;
    virtual int currentNibble() const = 0;
    virtual qsizetype currentSelectionLength() const = 0;

    virtual bool isLineSelected(qsizetype line) const = 0;
    virtual bool isSelected(const HexPosition &pos) const = 0;
    virtual bool hasSelection() const = 0;
    virtual bool hasInternalSelection() const = 0;

public:
    // current rendering status
    virtual qsizetype beginLine() const = 0;
    virtual qsizetype endLine() const = 0;
    virtual qsizetype firstVisibleLine() const = 0;
    virtual int currentHorizontalOffset() const = 0;

public:
    // current document
    virtual QByteArray read(qsizetype offset, qsizetype len = -1) const = 0;
    virtual char readAt(qsizetype offset) const = 0;
    virtual quintptr baseAddress() const = 0;

    virtual bool metafgVisible() const = 0;
    virtual bool metabgVisible() const = 0;
    virtual bool metaCommentVisible() const = 0;

    virtual bool isReadOnly() const = 0;
    virtual bool isKeepSize() const = 0;
    virtual bool isLocked() const = 0;
    virtual bool lockKeepSize() const = 0;

public slots:
    virtual void update() = 0;
    virtual void update(const QRect &r) = 0;
    virtual void update(const QRegion &r) = 0;

public:
    // helper
    void renderHexBackground(QPainter *painter, const QPoint &topLeft,
                             qsizetype width);

    using RenderFunc = std::function<void(
        QPainter *painter, const QRect &lineRect, qsizetype curLine)>;
    void renderContent(QPainter *painter, const QPoint &topLeft,
                       qsizetype width, const RenderFunc &rfunc);
};

} // namespace WingHex

#endif // HEXEDITORCONTEXT_H

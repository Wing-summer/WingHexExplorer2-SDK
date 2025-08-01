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

#include "hexeditorcontext.h"

#include <QPainter>

using namespace WingHex;

HexEditorContext::HexEditorContext(QObject *parent) : QObject(parent) {}

void HexEditorContext::renderHexBackground(QPainter *painter,
                                           const QPoint &topLeft,
                                           qsizetype width) {
    auto begin = this->beginLine();
    auto end = this->endLine();
    auto firstline = this->firstVisibleLine();
    auto documentLines = this->documentLines();
    for (qsizetype line = begin; line < std::min(end, documentLines); line++) {
        QRect linerect =
            QRect(topLeft.x(),
                  topLeft.y() +
                      int((line - firstline + (headerAreaVisible() ? 1 : 0)) *
                          this->lineHeight()),
                  width, lineHeight());
        if (line % 2)
            painter->fillRect(linerect, this->bytesBackground());
        else
            painter->fillRect(linerect, this->bytesAlterBackground());
    }
}

void HexEditorContext::renderContent(QPainter *painter, const QPoint &topLeft,
                                     qsizetype width, const RenderFunc &rfunc) {
    if (!rfunc) {
        return;
    }

    auto begin = this->beginLine();
    auto end = this->endLine();
    auto firstline = this->firstVisibleLine();
    auto documentLines = this->documentLines();
    for (qsizetype line = begin; line < std::min(end, documentLines); line++) {
        QRect linerect =
            QRect(topLeft.x(),
                  topLeft.y() +
                      int((line - firstline + (headerAreaVisible() ? 1 : 0)) *
                          this->lineHeight()),
                  width, lineHeight());
        rfunc(painter, linerect, line);
    }
}

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

#ifndef IWINGPLUGINBASECALLS_H
#define IWINGPLUGINBASECALLS_H

#include "WingPlugin/iwingangel.h"
#include "WingPlugin/wingplugin_global.h"
#include "WingPlugin/wingplugincalls.h"

#include <QFileDialog>
#include <QIcon>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QObject>

namespace WingHex {

class IWingPluginBase;

class WINGPLUGIN_EXPORT IWingPluginBaseCalls : public WingPluginCalls {
public:
    IWingPluginBaseCalls();

public:
    void toast(const QPixmap &icon, const QString &message) const;
    void logTrace(const QString &message) const;
    void logDebug(const QString &message) const;
    void logWarn(const QString &message) const;
    void logError(const QString &message) const;
    void logInfo(const QString &message) const;

    bool raiseDockWidget(QWidget *w);

    // theme
    WingHex::AppTheme currentAppTheme() const;

    // not available for AngelScript
    // only for plugin UI extenstion

    QDialog *createDialog(QWidget *content) const;

public:
    void msgAboutQt(QWidget *parent = nullptr,
                    const QString &title = QString()) const;

    QMessageBox::StandardButton
    msgInformation(QWidget *parent, const QString &title, const QString &text,
                   QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                   QMessageBox::StandardButton defaultButton =
                       QMessageBox::NoButton) const;

    QMessageBox::StandardButton msgQuestion(
        QWidget *parent, const QString &title, const QString &text,
        QMessageBox::StandardButtons buttons =
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
        QMessageBox::StandardButton defaultButton =
            QMessageBox::NoButton) const;

    QMessageBox::StandardButton
    msgWarning(QWidget *parent, const QString &title, const QString &text,
               QMessageBox::StandardButtons buttons = QMessageBox::Ok,
               QMessageBox::StandardButton defaultButton =
                   QMessageBox::NoButton) const;

    QMessageBox::StandardButton
    msgCritical(QWidget *parent, const QString &title, const QString &text,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton =
                    QMessageBox::NoButton) const;

    void msgAbout(QWidget *parent, const QString &title,
                  const QString &text) const;

    QMessageBox::StandardButton
    msgbox(QWidget *parent, QMessageBox::Icon icon, const QString &title,
           const QString &text,
           QMessageBox::StandardButtons buttons = QMessageBox::NoButton,
           QMessageBox::StandardButton defaultButton =
               QMessageBox::NoButton) const;

public:
    Q_REQUIRED_RESULT QString
    dlgGetText(QWidget *parent, const QString &title, const QString &label,
               QLineEdit::EchoMode echo = QLineEdit::Normal,
               const QString &text = QString(), bool *ok = nullptr,
               Qt::InputMethodHints inputMethodHints = Qt::ImhNone) const;
    Q_REQUIRED_RESULT QString dlgGetMultiLineText(
        QWidget *parent, const QString &title, const QString &label,
        const QString &text = QString(), bool *ok = nullptr,
        Qt::InputMethodHints inputMethodHints = Qt::ImhNone) const;

    Q_REQUIRED_RESULT QString
    dlgGetItem(QWidget *parent, const QString &title, const QString &label,
               const QStringList &items, int current = 0, bool editable = true,
               bool *ok = nullptr,
               Qt::InputMethodHints inputMethodHints = Qt::ImhNone) const;

    Q_REQUIRED_RESULT int dlgGetInt(QWidget *parent, const QString &title,
                                    const QString &label, int value = 0,
                                    int minValue = -2147483647,
                                    int maxValue = 2147483647, int step = 1,
                                    bool *ok = nullptr) const;

    Q_REQUIRED_RESULT double
    dlgGetDouble(QWidget *parent, const QString &title, const QString &label,
                 double value = 0, double minValue = -2147483647,
                 double maxValue = 2147483647, int decimals = 1,
                 bool *ok = nullptr, double step = 1) const;

public:
    Q_REQUIRED_RESULT QString dlgGetExistingDirectory(
        QWidget *parent = nullptr, const QString &caption = QString(),
        const QString &dir = QString(),
        QFileDialog::Options options = QFileDialog::ShowDirsOnly) const;

    Q_REQUIRED_RESULT QString dlgGetOpenFileName(
        QWidget *parent = nullptr, const QString &caption = QString(),
        const QString &dir = QString(), const QString &filter = QString(),
        QString *selectedFilter = nullptr,
        QFileDialog::Options options = QFileDialog::Options()) const;

    Q_REQUIRED_RESULT QStringList dlgGetOpenFileNames(
        QWidget *parent = nullptr, const QString &caption = QString(),
        const QString &dir = QString(), const QString &filter = QString(),
        QString *selectedFilter = nullptr,
        QFileDialog::Options options = QFileDialog::Options()) const;

    Q_REQUIRED_RESULT QString dlgGetSaveFileName(
        QWidget *parent = nullptr, const QString &caption = QString(),
        const QString &dir = QString(), const QString &filter = QString(),
        QString *selectedFilter = nullptr,
        QFileDialog::Options options = QFileDialog::Options()) const;

public:
    Q_REQUIRED_RESULT QColor dlgGetColor(const QString &caption,
                                         QWidget *parent = nullptr) const;

public:
    Q_REQUIRED_RESULT IWingGeneric *__createParamContext(void *ctx) const;

    void __raiseContextException(const QString &exception,
                                 bool allowCatch) const;
};

} // namespace WingHex

Q_DECLARE_METATYPE(QMessageBox::StandardButtons)
Q_DECLARE_METATYPE(QMessageBox::StandardButton)
Q_DECLARE_METATYPE(bool *)
Q_DECLARE_METATYPE(QString *)

#define WING_DECLARE_STATIC_API                                                \
    static std::function<WingHex::IWingGeneric *(void *)> createParamContext;  \
    static std::function<void(const QString &, bool)> raiseContextException;

#define WING_INIT_STATIC_API                                                   \
    do {                                                                       \
        createParamContext =                                                   \
            std::bind(&IWingPluginBaseCalls::__createParamContext, this,       \
                      std::placeholders::_1);                                  \
        raiseContextException =                                                \
            std::bind(&IWingPluginBaseCalls::__raiseContextException, this,    \
                      std::placeholders::_1, std::placeholders::_2);           \
    } while (0)

#endif // IWINGPLUGINBASECALLS_H

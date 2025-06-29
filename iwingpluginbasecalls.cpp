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

#include "iwingpluginbasecalls.h"

#include "WingPlugin/wingcore.h"

using namespace WingHex;

IWingPluginBaseCalls::IWingPluginBaseCalls() {}

void IWingPluginBaseCalls::toast(const QPixmap &icon,
                                 const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::toast);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), icon, message);
}

void IWingPluginBaseCalls::logTrace(const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::logTrace);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), message);
}

void IWingPluginBaseCalls::logDebug(const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::logDebug);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), message);
}

void IWingPluginBaseCalls::logWarn(const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::logWarn);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), message);
}

void IWingPluginBaseCalls::logError(const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::logError);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), message);
}

void IWingPluginBaseCalls::logInfo(const QString &message) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::logInfo);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), message);
}

bool IWingPluginBaseCalls::raiseDockWidget(QWidget *w) {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::raiseDockWidget);
    bool ret = false;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             w);
    return ret;
}

QDialog *IWingPluginBaseCalls::createDialog(QWidget *content) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::createDialog);
    QDialog *ret = nullptr;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             content);
    return ret;
}

void IWingPluginBaseCalls::msgAboutQt(QWidget *parent,
                                      const QString &title) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgAboutQt);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), parent, title);
}

QMessageBox::StandardButton IWingPluginBaseCalls::msgInformation(
    QWidget *parent, const QString &title, const QString &text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgInformation);
    QMessageBox::StandardButton ret = QMessageBox::NoButton;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, text, buttons, defaultButton);
    return ret;
}

QMessageBox::StandardButton IWingPluginBaseCalls::msgQuestion(
    QWidget *parent, const QString &title, const QString &text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgQuestion);
    QMessageBox::StandardButton ret = QMessageBox::NoButton;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, text, buttons, defaultButton);
    return ret;
}

QMessageBox::StandardButton IWingPluginBaseCalls::msgWarning(
    QWidget *parent, const QString &title, const QString &text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgWarning);
    QMessageBox::StandardButton ret = QMessageBox::NoButton;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, text, buttons, defaultButton);
    return ret;
}

QMessageBox::StandardButton IWingPluginBaseCalls::msgCritical(
    QWidget *parent, const QString &title, const QString &text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgCritical);
    QMessageBox::StandardButton ret = QMessageBox::NoButton;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, text, buttons, defaultButton);
    return ret;
}

void IWingPluginBaseCalls::msgAbout(QWidget *parent, const QString &title,
                                    const QString &text) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgAbout);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), parent, title,
             text);
}

QMessageBox::StandardButton
IWingPluginBaseCalls::msgbox(QWidget *parent, QMessageBox::Icon icon,
                             const QString &title, const QString &text,
                             QMessageBox::StandardButtons buttons,
                             QMessageBox::StandardButton defaultButton) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::msgbox);
    QMessageBox::StandardButton ret = QMessageBox::NoButton;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, icon, title, text, buttons, defaultButton);
    return ret;
}

QString
IWingPluginBaseCalls::dlgGetText(QWidget *parent, const QString &title,
                                 const QString &label, QLineEdit::EchoMode echo,
                                 const QString &text, bool *ok,
                                 Qt::InputMethodHints inputMethodHints) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetText);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, label, echo, text, ok, inputMethodHints);
    return ret;
}

QString IWingPluginBaseCalls::dlgGetMultiLineText(
    QWidget *parent, const QString &title, const QString &label,
    const QString &text, bool *ok,
    Qt::InputMethodHints inputMethodHints) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetMultiLineText);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, label, text, ok, inputMethodHints);
    return ret;
}

QString
IWingPluginBaseCalls::dlgGetItem(QWidget *parent, const QString &title,
                                 const QString &label, const QStringList &items,
                                 int current, bool editable, bool *ok,
                                 Qt::InputMethodHints inputMethodHints) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetItem);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, label, items, current, editable, ok,
             inputMethodHints);
    return ret;
}

int IWingPluginBaseCalls::dlgGetInt(QWidget *parent, const QString &title,
                                    const QString &label, int value,
                                    int minValue, int maxValue, int step,
                                    bool *ok) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetInt);
    int ret = 0;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, label, value, minValue, maxValue, step, ok);
    return ret;
}

double IWingPluginBaseCalls::dlgGetDouble(QWidget *parent, const QString &title,
                                          const QString &label, double value,
                                          double minValue, double maxValue,
                                          int decimals, bool *ok,
                                          double step) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetDouble);
    double ret = 0;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, title, label, value, minValue, maxValue, decimals, ok,
             step);
    return ret;
}

QString IWingPluginBaseCalls::dlgGetExistingDirectory(
    QWidget *parent, const QString &caption, const QString &dir,
    QFileDialog::Options options) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetExistingDirectory);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, caption, dir, options);
    return ret;
}

QString IWingPluginBaseCalls::dlgGetOpenFileName(
    QWidget *parent, const QString &caption, const QString &dir,
    const QString &filter, QString *selectedFilter,
    QFileDialog::Options options) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetOpenFileName);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, caption, dir, filter, selectedFilter, options);
    return ret;
}

QStringList IWingPluginBaseCalls::dlgGetOpenFileNames(
    QWidget *parent, const QString &caption, const QString &dir,
    const QString &filter, QString *selectedFilter,
    QFileDialog::Options options) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetOpenFileNames);
    QStringList ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, caption, dir, filter, selectedFilter, options);
    return ret;
}

QString IWingPluginBaseCalls::dlgGetSaveFileName(
    QWidget *parent, const QString &caption, const QString &dir,
    const QString &filter, QString *selectedFilter,
    QFileDialog::Options options) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetSaveFileName);
    QString ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             parent, caption, dir, filter, selectedFilter, options);
    return ret;
}

QColor IWingPluginBaseCalls::dlgGetColor(const QString &caption,
                                         QWidget *parent) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::dlgGetColor);
    QColor ret;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             caption, parent);
    return ret;
}

IWingGeneric *IWingPluginBaseCalls::__createParamContext(void *ctx) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::__createParamContext);
    IWingGeneric *ret = nullptr;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(ret), getSender(),
             ctx);
    return ret;
}

void IWingPluginBaseCalls::__raiseContextException(const QString &exception,
                                                   bool allowCatch) const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::__raiseContextException);
    m.invoke(callReceiver(), Qt::DirectConnection, getSender(), exception,
             allowCatch);
}

AppTheme IWingPluginBaseCalls::currentAppTheme() const {
    SETUP_CALL_CONTEXT(&IWingPluginBaseCalls::currentAppTheme);
    WingHex::AppTheme theme = AppTheme::Invalid;
    m.invoke(callReceiver(), Qt::DirectConnection, qReturnArg(theme),
             getSender());
    return theme;
}

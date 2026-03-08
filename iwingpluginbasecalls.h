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
#include "WingPlugin/scriptobjects.h"
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

    Q_REQUIRED_RESULT ASScriptArray *
    __createScriptArray(const QString &type) const;
    Q_REQUIRED_RESULT ASScriptDictionary *__createScriptDictionary() const;
    Q_REQUIRED_RESULT ASScriptAny *__createScriptAny() const;
    Q_REQUIRED_RESULT ASScript2DArray *
    __createScript2DArray(const QString &type) const;
};

template <typename Accessor, typename... CallArgs>
auto runGlobalAPI(Accessor accessor, CallArgs &&...args)
    -> decltype((*accessor())(std::forward<CallArgs>(args)...)) {
    auto p = accessor(); // call accessor to obtain pointer-to-callable
    if (!p)
        throw std::runtime_error("global-static accessor returned null");
    auto &fn = *p;
    if (!fn)
        throw std::runtime_error(
            "std::function inside global static is not initialized");
    return fn(std::forward<CallArgs>(args)...);
}

using CREATE_PARAM_CONTEXT_FN = std::function<WingHex::IWingGeneric *(void *)>;
using RAISE_CONTEXT_EXCEPTION_FN = std::function<void(const QString &, bool)>;
using CREATE_SCRIPTARRAY_FN =
    std::function<WingHex::ASScriptArray *(const QString &)>;
using CREATE_SCRIPTDICTIONARY_FN =
    std::function<WingHex::ASScriptDictionary *()>;
using CREATE_SCRIPTANY_FN = std::function<WingHex::ASScriptAny *()>;
using CREATE_SCRIPT2DARRAY_FN =
    std::function<WingHex::ASScript2DArray *(const QString &)>;

#define WING_DECLARE_STATIC_API                                                \
    Q_GLOBAL_STATIC(WingHex::CREATE_PARAM_CONTEXT_FN, CreateParamContext)      \
    Q_GLOBAL_STATIC(WingHex::RAISE_CONTEXT_EXCEPTION_FN,                       \
                    RaiseContextException)                                     \
    Q_GLOBAL_STATIC(WingHex::CREATE_SCRIPTARRAY_FN, CreateScriptArray)         \
    Q_GLOBAL_STATIC(WingHex::CREATE_SCRIPTDICTIONARY_FN,                       \
                    CreateScriptDictionary)                                    \
    Q_GLOBAL_STATIC(WingHex::CREATE_SCRIPTANY_FN, CreateScriptAny)             \
    Q_GLOBAL_STATIC(WingHex::CREATE_SCRIPT2DARRAY_FN, CreateScript2DArray)

#define WING_INIT_STATIC_API                                                   \
    do {                                                                       \
        *CreateParamContext =                                                  \
            std::bind(&IWingPluginBaseCalls::__createParamContext, this,       \
                      std::placeholders::_1);                                  \
        *RaiseContextException =                                               \
            std::bind(&IWingPluginBaseCalls::__raiseContextException, this,    \
                      std::placeholders::_1, std::placeholders::_2);           \
        *CreateScriptArray =                                                   \
            std::bind(&IWingPluginBaseCalls::__createScriptArray, this,        \
                      std::placeholders::_1);                                  \
        *CreateScriptDictionary =                                              \
            std::bind(&IWingPluginBaseCalls::__createScriptDictionary, this);  \
        *CreateScriptAny =                                                     \
            std::bind(&IWingPluginBaseCalls::__createScriptAny, this);         \
        *CreateScript2DArray =                                                 \
            std::bind(&IWingPluginBaseCalls::__createScript2DArray, this,      \
                      std::placeholders::_1);                                  \
    } while (0)

} // namespace WingHex

Q_DECLARE_METATYPE(QMessageBox::StandardButtons)
Q_DECLARE_METATYPE(QMessageBox::StandardButton)
Q_DECLARE_METATYPE(bool *)
Q_DECLARE_METATYPE(QString *)

#endif // IWINGPLUGINBASECALLS_H

/************************************************************************
 **
 **  @file   vapplication.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vapplication.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../ifc/exception/vexceptioninvalidnotch.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../version.h"
#include "../vmisc/vmath.h"
#include "../qmuparser/qmuparsererror.h"
#include "../mainwindow.h"
#include "../vmisc/qt_dispatch/qt_dispatch.h"

#include <QtDebug>
#include <QDir>
#include <QProcess>
#include <QTemporaryFile>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QThread>
#include <QDateTime>
#include <QtXmlPatterns>
#include <QIcon>

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vApp, "v.application")

QT_WARNING_POP

Q_DECL_CONSTEXPR auto DAYS_TO_KEEP_LOGS = 3;

//---------------------------------------------------------------------------------------------------------------------
inline void noisyFailureMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // only the GUI thread should display message boxes.  If you are
    // writing a multithreaded application and the error happens on
    // a non-GUI thread, you'll have to queue the message to the GUI
    QCoreApplication *instance = QCoreApplication::instance();
    const bool isGuiThread = instance && (QThread::currentThread() == instance->thread());

    if (not isGuiThread)
    {
        auto Handler = [](QtMsgType type, const QMessageLogContext &context, const QString &msg)
        {
            noisyFailureMsgHandler(type, context, msg);
        };

        q_dispatch_async_main(Handler, type, context, msg);
        return;
    }

    // Why on earth didn't Qt want to make failed signal/slot connections qWarning?
    if ((type == QtDebugMsg) && msg.contains(QStringLiteral("::connect")))
    {
        type = QtWarningMsg;
    }

#if defined(V_NO_ASSERT)
    // I have decided to hide this annoing message for release builds.
    if ((type == QtWarningMsg) && msg.contains(QStringLiteral("QSslSocket: cannot resolve")))
    {
        type = QtDebugMsg;
    }

    if ((type == QtWarningMsg) && msg.contains(QStringLiteral("setGeometry: Unable to set geometry")))
    {
        type = QtDebugMsg;
    }
#endif //defined(V_NO_ASSERT)

#if defined(Q_OS_MAC)
#   if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0) && QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
        // Try hide very annoying, Qt related, warnings in Mac OS X
        // QNSView mouseDragged: Internal mouse button tracking invalid (missing Qt::LeftButton)
        // https://bugreports.qt.io/browse/QTBUG-42846
        if ((type == QtWarningMsg) && msg.contains(QStringLiteral("QNSView")))
        {
            type = QtDebugMsg;
        }
#   endif

    // Hide Qt bug 'Assertion when reading an icns file'
    // https://bugreports.qt.io/browse/QTBUG-45537
    // Remove after Qt fix will be released
    if ((type == QtWarningMsg) && msg.contains(QStringLiteral("QICNSHandler::read()")))
    {
        type = QtDebugMsg;
    }

    // See issue #568
    if (msg.contains(QStringLiteral("Error receiving trust for a CA certificate")))
    {
        type = QtDebugMsg;
    }
#endif

    // this is another one that doesn't make sense as just a debug message.  pretty serious
    // sign of a problem
    // http://www.developer.nokia.com/Community/Wiki/QPainter::begin:Paint_device_returned_engine_%3D%3D_0_(Known_Issue)
    if ((type == QtDebugMsg) && msg.contains(QStringLiteral("QPainter::begin"))
            && msg.contains(QStringLiteral("Paint device returned engine")))
    {
        type = QtWarningMsg;
    }

    // This qWarning about "Cowardly refusing to send clipboard message to hung application..."
    // is something that can easily happen if you are debugging and the application is paused.
    // As it is so common, not worth popping up a dialog.
    if ((type == QtWarningMsg) && msg.contains(QStringLiteral("QClipboard::event"))
            && msg.contains(QStringLiteral("Cowardly refusing")))
    {
        type = QtDebugMsg;
    }

    QString logMsg = msg;
    const bool isPatternMessage = qApp->IsPatternMessage(msg);
    if (isPatternMessage)
    {
        logMsg = logMsg.remove(VAbstractApplication::patternMessageSignature);
    }

    {
        QString debugdate = "[" + QDateTime::currentDateTime().toString(QStringLiteral("yyyy.MM.dd hh:mm:ss"));

        switch (type)
        {
            case QtDebugMsg:
                debugdate += QStringLiteral(":DEBUG:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                             .arg(context.function, context.category, logMsg);
                vStdOut() << QApplication::translate("vNoisyHandler", "DEBUG:") << logMsg << "\n";
                break;
            case QtWarningMsg:
                if (isPatternMessage)
                {
                    qApp->PostPatternMessage(logMsg, type);
                }
                debugdate += QStringLiteral(":WARNING:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                             .arg(context.function, context.category, logMsg);
                vStdErr() << QApplication::translate("vNoisyHandler", "WARNING:") << logMsg << "\n";
                break;
            case QtCriticalMsg:
                if (isPatternMessage)
                {
                    qApp->PostPatternMessage(logMsg, type);
                }
                debugdate += QStringLiteral(":CRITICAL:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                             .arg(context.function, context.category, logMsg);
                vStdErr() << QApplication::translate("vNoisyHandler", "CRITICAL:") << logMsg << "\n";
                break;
            case QtFatalMsg:
                debugdate += QStringLiteral(":FATAL:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                             .arg(context.function, context.category, logMsg);
                vStdErr() << QApplication::translate("vNoisyHandler", "FATAL:") << logMsg << "\n";
                break;
            #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            case QtInfoMsg:
                if (isPatternMessage)
                {
                    qApp->PostPatternMessage(logMsg, type);
                }
                debugdate += QStringLiteral(":INFO:%1(%2)] %3: %4: %5").arg(context.file).arg(context.line)
                             .arg(context.function, context.category, logMsg);
                vStdOut() << QApplication::translate("vNoisyHandler", "INFO:") << logMsg << "\n";
                break;
            #endif
            default:
                break;
        }
        vStdOut().flush();
        vStdErr().flush();

        (*qApp->LogFile()) << debugdate <<  endl;
    }

    if (isGuiThread)
    {
        if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
        {
            if (VApplication::IsGUIMode())
            {
                //fixme: trying to make sure there are no save/load dialogs are opened, because error message during
                // them will lead to crash
                const bool topWinAllowsPop = (QApplication::activeModalWidget() == nullptr) ||
                        !QApplication::activeModalWidget()->inherits("QFileDialog");

                if (topWinAllowsPop && (not isPatternMessage || (type == QtCriticalMsg || type == QtFatalMsg)))
                {
                    QMessageBox messageBox;
                    switch (type)
                    {
                        case QtWarningMsg:
                            messageBox.setWindowTitle(QApplication::translate("vNoisyHandler", "Warning"));
                            messageBox.setIcon(QMessageBox::Warning);
                            break;
                        case QtCriticalMsg:
                            messageBox.setWindowTitle(QApplication::translate("vNoisyHandler", "Critical error"));
                            messageBox.setIcon(QMessageBox::Critical);
                            break;
                        case QtFatalMsg:
                            messageBox.setWindowTitle(QApplication::translate("vNoisyHandler", "Fatal error"));
                            messageBox.setIcon(QMessageBox::Critical);
                            break;
                        #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                        case QtInfoMsg:
                            messageBox.setWindowTitle(QApplication::translate("vNoisyHandler", "Information"));
                            messageBox.setIcon(QMessageBox::Information);
                            break;
                        #endif
                        case QtDebugMsg:
                        default:
                            break;
                    }

                    messageBox.setText(VAbstractApplication::ClearMessage(logMsg));
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.setWindowModality(Qt::ApplicationModal);
                    messageBox.setModal(true);
                #ifndef QT_NO_CURSOR
                    QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
                #endif
                    messageBox.exec();
                #ifndef QT_NO_CURSOR
                    QGuiApplication::restoreOverrideCursor();
                #endif
                }
            }
        }

        if (QtFatalMsg == type)
        {
            abort();
        }
    }
    else
    {
        if (type != QtDebugMsg)
        {
            abort(); // be NOISY unless overridden!
        }
    }
}

#define DefWidth 1.2//mm

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VApplication constructor.
 * @param argc number arguments.
 * @param argv command line.
 */
VApplication::VApplication(int &argc, char **argv)
    : VAbstractApplication(argc, argv),
      trVars(nullptr),
      autoSaveTimer(nullptr),
      lockLog(),
      out(nullptr)
{
    setApplicationDisplayName(VER_PRODUCTNAME_STR);
    setApplicationName(VER_INTERNALNAME_STR);
    setOrganizationName(VER_COMPANYNAME_STR);
    setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    // Setting the Application version
    setApplicationVersion(APP_VERSION_STR);
    // making sure will create new instance...just in case we will ever do 2 objects of VApplication
    VCommandLine::Reset();
}

//---------------------------------------------------------------------------------------------------------------------
VApplication::~VApplication()
{
    qCDebug(vApp, "Application closing.");
    qInstallMessageHandler(nullptr); // Resore the message handler
    delete trVars;
    VCommandLine::Reset();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewValentina start Valentina in new process, send path to pattern file in argument.
 * @param fileName path to pattern file.
 */
void VApplication::NewValentina(const QString &fileName)
{
    qCDebug(vApp, "Open new detached process.");
    if (fileName.isEmpty())
    {
        qCDebug(vApp, "New process without arguments. program = %s",
                qUtf8Printable(QCoreApplication::applicationFilePath()));
        // Path can contain spaces.
        if (QProcess::startDetached("\""+QCoreApplication::applicationFilePath()+"\""))
        {
            qCDebug(vApp, "The process was started successfully.");
        }
        else
        {
            qCWarning(vApp, "Could not run process. The operation timed out or an error occurred.");
        }
    }
    else
    {
        const QString run = QStringLiteral("\"%1\" \"%2\"").arg(QCoreApplication::applicationFilePath(), fileName);
        qCDebug(vApp, "New process with arguments. program = %s", qUtf8Printable(run));
        if (QProcess::startDetached(run))
        {
            qCDebug(vApp, "The process was started successfully.");
        }
        else
        {
            qCWarning(vApp, "Could not run process. The operation timed out or an error occurred.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief notify Reimplemented from QApplication::notify().
 * @param receiver receiver.
 * @param event event.
 * @return value that is returned from the receiver's event handler.
 */
// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file. Program will be terminated.")), //-V807
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionBadId &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error bad id. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionToolWasDeleted &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s",
                   qUtf8Printable("Unhadled deleting tool. Continue use object after deleting"),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch(const VExceptionInvalidNotch &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Invalid notch.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VException &e)
    {
        qCCritical(vApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Something's wrong!!")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return true;
    }
    // These last two cases are special. I found that we can't show here a modal dialog with an error message.
    // Somehow program doesn't wait until an error dialog will be closed. But if ignore the exception the program will
    // hang.
    catch (const qmu::QmuParserError &e)
    {
        qCCritical(vApp, "%s", qUtf8Printable(tr("Parser error: %1. Program will be terminated.").arg(e.GetMsg())));
        exit(V_EX_DATAERR);
    }
    catch (std::exception& e)
    {
        qCCritical(vApp, "%s", qUtf8Printable(tr("Exception thrown: %1. Program will be terminated.").arg(e.what())));
        exit(V_EX_SOFTWARE);
    }
    return false;
}

void VApplication::ActivateDarkMode()
{
     VSettings *settings = qApp->ValentinaSettings();
     if (settings->GetDarkMode())
     {
         QFile f(QStringLiteral(":qdarkstyle/style.qss"));
         if (!f.exists())
         {
             qDebug()<<"Unable to set stylesheet, file not found\n";
         }
         else
         {
             f.open(QFile::ReadOnly | QFile::Text);
             QTextStream ts(&f);
             qApp->setStyleSheet(ts.readAll());
         }

     }
}


//---------------------------------------------------------------------------------------------------------------------
QString VApplication::TapeFilePath() const
{
    const QString tape = QStringLiteral("tape");
#ifdef Q_OS_WIN
    QFileInfo tapeFile(QCoreApplication::applicationDirPath() + "/" + tape + ".exe");
    if (tapeFile.exists())
    {
        return tapeFile.absoluteFilePath();
    }
    else
    {
        return QCoreApplication::applicationDirPath() + "/../../tape/bin/" + tape + ".exe";
    }
#elif defined(Q_OS_MAC)
    QFileInfo tapeFile(QCoreApplication::applicationDirPath() + "/" + tape);
    if (tapeFile.exists())
    {
        return tapeFile.absoluteFilePath();
    }
    else
    {
        QFileInfo file(QCoreApplication::applicationDirPath() + "/../../tape/bin/" + tape);
        if (file.exists())
        {
            return file.absoluteFilePath();
        }
        else
        {
            return tape;
        }
    }
#else // Unix
    QFileInfo file(QCoreApplication::applicationDirPath() + "/../../tape/bin/" + tape);
    if (file.exists())
    {
        return file.absoluteFilePath();
    }
    else
    {
        QFileInfo tapeFile(QCoreApplication::applicationDirPath() + "/" + tape);
        if (tapeFile.exists())
        {
            return tapeFile.absoluteFilePath();
        }
        else
        {
            return tape;
        }
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::LogDirPath() const
{
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    const QString logDirPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(),
                                                      QStandardPaths::LocateDirectory) + "Valentina";
#else
    const QString logDirPath = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(),
                                                      QStandardPaths::LocateDirectory)
            + QCoreApplication::organizationName();
#endif
    return logDirPath;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::LogPath() const
{
    return QStringLiteral("%1/valentina-pid%2.log").arg(LogDirPath()).arg(applicationPid());
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::CreateLogDir() const
{
    QDir logDir(LogDirPath());
    if (logDir.exists() == false)
    {
        return logDir.mkpath(QChar('.')); // Create directory for log if need
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::BeginLogging()
{
    VlpCreateLock(lockLog, LogPath(), [this](){return new QFile(LogPath());});

    if (lockLog->IsLocked())
    {
        if (lockLog->GetProtected()->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            out.reset(new QTextStream(lockLog->GetProtected().data()));
            qInstallMessageHandler(noisyFailureMsgHandler);
            qCDebug(vApp, "Log file %s was locked.", qUtf8Printable(LogPath()));
        }
        else
        {
            qCDebug(vApp, "Error opening log file \'%s\'. All debug output redirected to console.",
                    qUtf8Printable(LogPath()));
        }
    }
    else
    {
        qCDebug(vApp, "Failed to lock %s", qUtf8Printable(LogPath()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::ClearOldLogs() const
{
    const QString workingDirectory = QDir::currentPath();// Save the app working directory
    QDir logsDir(LogDirPath());
    logsDir.setNameFilters(QStringList("*.log"));
    logsDir.setCurrent(LogDirPath());

    const QStringList allFiles = logsDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    if (allFiles.isEmpty() == false)
    {
        qCDebug(vApp, "Clearing old logs");
        for (auto &fn : allFiles)
        {
            QFileInfo info(fn);
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
            const QDateTime created = info.birthTime();
#else
            const QDateTime created = info.created();
#endif
            if (created.daysTo(QDateTime::currentDateTime()) >= DAYS_TO_KEEP_LOGS)
            {
                VLockGuard<QFile> tmp(info.absoluteFilePath(), [&fn](){return new QFile(fn);});
                if (tmp.GetProtected() != nullptr)
                {
                    if (tmp.GetProtected()->remove())
                    {
                        qCDebug(vApp, "Deleted %s", qUtf8Printable(info.absoluteFilePath()));
                    }
                    else
                    {
                        qCDebug(vApp, "Could not delete %s", qUtf8Printable(info.absoluteFilePath()));
                    }
                }
                else
                {
                    qCDebug(vApp, "Failed to lock %s", qUtf8Printable(info.absoluteFilePath()));
                }
            }
        }
    }
    else
    {
        qCDebug(vApp, "There are no old logs.");
    }

    QDir::setCurrent(workingDirectory); // Restore working directory
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitOptions()
{
    OpenSettings();

    // Run creation log after sending crash report
    StartLogging();

    qDebug()<<"Version:"<<APP_VERSION_STR;
    qDebug()<<"Build revision:"<<BUILD_REVISION;
    qDebug()<<buildCompatibilityString();
    qDebug()<<"Built on"<<__DATE__<<"at"<<__TIME__;
    qDebug()<<"Command-line arguments:"<<arguments();
    qDebug()<<"Process ID:"<<applicationPid();

    LoadTranslation(QLocale().name());// By default the console version uses system locale

    // Create command line parser after loading translations to show localized version.
    VCommandLine::Get(*this);

    if (VApplication::IsGUIMode())// By default console version uses system locale
    {
        LoadTranslation(ValentinaSettings()->GetLocale());
    }

    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    if (QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK) == false)
    {
        //If there is no default working icon theme then we should
        //use an icon theme that we provide via a .qrc file
        //This case happens under Windows and Mac OS X
        //This does not happen under GNOME or KDE
        QIcon::setThemeName(QStringLiteral("win.icon.theme"));
    }
    ActivateDarkMode();
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VApplication::LabelLanguages()
{
    QStringList list {"de",  // German
                      "en",  // English
                      "fr",  // French
                      "ru",  // Russian
                      "uk",  // Ukrainian
                      "hr",  // Croatian
                      "sr",  // Serbian
                      "bs",  // Bosnian
                      "cs"   // Czech
                     };
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::StartLogging()
{
    if (CreateLogDir())
    {
        BeginLogging();
        ClearOldLogs();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QTextStream *VApplication::LogFile()
{
    return out.get();
}

//---------------------------------------------------------------------------------------------------------------------
const VTranslateVars *VApplication::TrVars()
{
    return trVars;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitTrVars()
{
    if (trVars != nullptr)
    {
        trVars->Retranslate();
    }
    else
    {
        trVars = new VTranslateVars();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::event(QEvent *e)
{
    switch(e->type())
    {
        // In Mac OS X the QFileOpenEvent event is generated when user perform "Open With" from Finder (this event is
        // Mac specific).
        case QEvent::FileOpen:
        {
            QFileOpenEvent *fileOpenEvent = static_cast<QFileOpenEvent *>(e);
            const QString macFileOpen = fileOpenEvent->file();
            if(not macFileOpen.isEmpty())
            {
                MainWindow *window = qobject_cast<MainWindow*>(mainWindow);
                if (window)
                {
                    window->LoadPattern(macFileOpen);  // open file in existing window
                }
                return true;
            }
            break;
        }
#if defined(Q_OS_MAC)
        case QEvent::ApplicationActivate:
        {
            if (mainWindow && not mainWindow->isMinimized())
            {
                mainWindow->show();
            }
            return true;
        }
#endif //defined(Q_OS_MAC)
        default:
            return VAbstractApplication::event(e);
    }
    return VAbstractApplication::event(e);
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::AboutToQuit()
{
    // If try to use the method QApplication::exit program can't sync settings and show warning about QApplication
    // instance. Solution is to call sync() before quit.
    // Connect this slot with VApplication::aboutToQuit.
    Settings()->sync();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief OpenSettings get acsses to application settings.
 *
 * Because we can create object in constructor we open file separately.
 */
void VApplication::OpenSettings()
{
    settings = new VSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(),
                             QCoreApplication::applicationName(), this);
}

//---------------------------------------------------------------------------------------------------------------------
VSettings *VApplication::ValentinaSettings()
{
    SCASSERT(settings != nullptr)
    return qobject_cast<VSettings *>(settings);
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::IsGUIMode()
{
    return (VCommandLine::instance != nullptr) && VCommandLine::instance->IsGuiEnabled();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsAppInGUIMode little hack that allow to have access to application state from VAbstractApplication class.
 */
bool VApplication::IsAppInGUIMode() const
{
    return IsGUIMode();
}

//---------------------------------------------------------------------------------------------------------------------
bool VApplication::IsPedantic() const
{
    return (VCommandLine::instance != nullptr) && VCommandLine::instance->IsPedantic();
}

//---------------------------------------------------------------------------------------------------------------------
const VCommandLinePtr VApplication::CommandLine() const
{
    return VCommandLine::instance;
}

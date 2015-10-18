/************************************************************************
 **
 **  @file   mapplication.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
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

#include "mapplication.h"
#include "version.h"
#include "tmainwindow.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../vmisc/logging.h"
#include "../vmisc/vsysexits.h"
#include "../qmuparser/qmuparsererror.h"

#include <QDir>
#include <QFileOpenEvent>
#include <QLocalSocket>
#include <QResource>
#include <QTranslator>
#include <QPointer>
#include <QLocalServer>
#include <QMessageBox>
#include <iostream>
#include <QGridLayout>
#include <QSpacerItem>
#include <QThread>

Q_LOGGING_CATEGORY(mApp, "m.application")

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../vmisc/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif

//---------------------------------------------------------------------------------------------------------------------
inline void noisyFailureMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    // Why on earth didn't Qt want to make failed signal/slot connections qWarning?
    if ((type == QtDebugMsg) && msg.contains("::connect"))
    {
        type = QtWarningMsg;
    }

    // this is another one that doesn't make sense as just a debug message.  pretty serious
    // sign of a problem
    // http://www.developer.nokia.com/Community/Wiki/QPainter::begin:Paint_device_returned_engine_%3D%3D_0_(Known_Issue)
    if ((type == QtDebugMsg) && msg.contains("QPainter::begin") && msg.contains("Paint device returned engine"))
    {
        type = QtWarningMsg;
    }

    // This qWarning about "Cowardly refusing to send clipboard message to hung application..."
    // is something that can easily happen if you are debugging and the application is paused.
    // As it is so common, not worth popping up a dialog.
    if ((type == QtWarningMsg) && QString(msg).contains("QClipboard::event")
            && QString(msg).contains("Cowardly refusing"))
    {
        type = QtDebugMsg;
    }

    // only the GUI thread should display message boxes.  If you are
    // writing a multithreaded application and the error happens on
    // a non-GUI thread, you'll have to queue the message to the GUI
    QCoreApplication *instance = QCoreApplication::instance();
    const bool isGuiThread = instance && (QThread::currentThread() == instance->thread());

    switch (type)
    {
        case QtDebugMsg:
            vStdOut() << QApplication::translate("mNoisyHandler", "DEBUG:") << msg << "\n";
            return;
        case QtWarningMsg:
            vStdErr() << QApplication::translate("mNoisyHandler", "WARNING:") << msg << "\n";
            break;
        case QtCriticalMsg:
            vStdErr() << QApplication::translate("mNoisyHandler", "CRITICAL:") << msg << "\n";
            break;
        case QtFatalMsg:
            vStdErr() << QApplication::translate("mNoisyHandler", "FATAL:") << msg << "\n";
            break;
        #if QT_VERSION > QT_VERSION_CHECK(5, 4, 2)
        case QtInfoMsg:
            vStdOut() << QApplication::translate("mNoisyHandler", "INFO:") << msg << "\n";
            break;
        #endif
        default:
            break;
    }

    if (isGuiThread)
    {
        //fixme: trying to make sure there are no save/load dialogs are opened, because error message during them will
        //lead to crash
        const bool topWinAllowsPop = (qApp->activeModalWidget() == nullptr) ||
                !qApp->activeModalWidget()->inherits("QFileDialog");
        QMessageBox messageBox;
        switch (type)
        {
            case QtWarningMsg:
                messageBox.setWindowTitle(QApplication::translate("mNoisyHandler", "Warning."));
                messageBox.setIcon(QMessageBox::Warning);
                break;
            case QtCriticalMsg:
                messageBox.setWindowTitle(QApplication::translate("mNoisyHandler", "Critical error."));
                messageBox.setIcon(QMessageBox::Critical);
                break;
            case QtFatalMsg:
                messageBox.setWindowTitle(QApplication::translate("mNoisyHandler", "Fatal error."));
                messageBox.setIcon(QMessageBox::Critical);
                break;
            #if QT_VERSION > QT_VERSION_CHECK(5, 4, 2)
            case QtInfoMsg:
                messageBox.setWindowTitle(QApplication::translate("mNoisyHandler", "Information."));
                messageBox.setIcon(QMessageBox::Information);
                break;
            #endif
            case QtDebugMsg:
            default:
                break;
        }

        if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
        {
            if (not qApp->IsTestMode())
            {
                if (topWinAllowsPop)
                {
                    messageBox.setText(msg);
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.setWindowModality(Qt::ApplicationModal);
                    messageBox.setModal(true);
                #ifndef QT_NO_CURSOR
                    QApplication::setOverrideCursor(Qt::ArrowCursor);
                #endif
                    messageBox.exec();
                #ifndef QT_NO_CURSOR
                    QApplication::restoreOverrideCursor();
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

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress uninitMemberVar
MApplication::MApplication(int &argc, char **argv)
    :VAbstractApplication(argc, argv),
      mainWindows(),
      localServer(nullptr),
      trVars(nullptr),
      dataBase(QPointer<DialogMDataBase>()),
      testMode(false)
{
    setApplicationDisplayName(VER_PRODUCTNAME_STR);
    setApplicationName(VER_INTERNALNAME_STR);
    setOrganizationName(VER_COMPANYNAME_STR);
    setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    // Setting the Application version
    setApplicationVersion(APP_VERSION_STR);
    setWindowIcon(QIcon(":/tapeicon/64x64/logo.png"));
}

//---------------------------------------------------------------------------------------------------------------------
MApplication::~MApplication()
{
    for (int i = 0; i < mainWindows.size(); ++i)
    {
        TMainWindow *window = mainWindows.at(i);
        delete window;
    }

    delete trVars;
    if (not dataBase.isNull())
    {
        delete dataBase;
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
bool MApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionBadId &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error bad id. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id. Program will be terminated.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        exit(V_EX_DATAERR);
    }
    catch (const VException &e)
    {
        qCCritical(mApp, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Something's wrong!!")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return true;
    }
    // These last two cases special. I found that we can't show here modal dialog with error message.
    // Somehow program doesn't waite untile an error dialog will be closed. But if ignore this program will hang.
    catch (const qmu::QmuParserError &e)
    {
        qCCritical(mApp, "%s", qUtf8Printable(tr("Parser error: %1. Program will be terminated.").arg(e.GetMsg())));
        exit(V_EX_DATAERR);
    }
    catch (std::exception &e)
    {
        qCCritical(mApp, "%s", qUtf8Printable(tr("Exception thrown: %1. Program will be terminated.").arg(e.what())));
        exit(V_EX_SOFTWARE);
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool MApplication::IsTestMode() const
{
    return testMode;
}

//---------------------------------------------------------------------------------------------------------------------
TMainWindow *MApplication::MainWindow()
{
    Clean();
    if (mainWindows.isEmpty())
    {
        NewMainWindow();
    }
    return mainWindows[0];
}

//---------------------------------------------------------------------------------------------------------------------
QList<TMainWindow *> MApplication::MainWindows()
{
    Clean();
    QList<TMainWindow*> list;
    for (int i = 0; i < mainWindows.count(); ++i)
    {
        list.append(mainWindows.at(i));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::InitOptions()
{
    qInstallMessageHandler(noisyFailureMsgHandler);

    OpenSettings();

    qCDebug(mApp, "Version: %s", qUtf8Printable(APP_VERSION_STR));
    qCDebug(mApp, "Build revision: %s", BUILD_REVISION);
    qCDebug(mApp, "%s", qUtf8Printable(buildCompatibilityString()));
    qCDebug(mApp, "Built on %s at %s", __DATE__, __TIME__);
    qCDebug(mApp, "Command-line arguments: %s", qUtf8Printable(this->arguments().join(", ")));
    qCDebug(mApp, "Process ID: %s", qUtf8Printable(QString().setNum(this->applicationPid())));

    LoadTranslation(QLocale::system().name());// By default the console version uses system locale

    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    if (QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK) == false)
    {
       //If there is no default working icon theme then we should
       //use an icon theme that we provide via a .qrc file
       //This case happens under Windows and Mac OS X
       //This does not happen under GNOME or KDE
       QIcon::setThemeName("win.icon.theme");
    }

    QResource::registerResource(diagramsPath());
}

//---------------------------------------------------------------------------------------------------------------------
const VTranslateVars *MApplication::TrVars()
{
    return trVars;
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::InitTrVars()
{
    if (trVars != nullptr)
    {
        trVars->Retranslate();
    }
    else
    {
        trVars = new VTranslateVars(TapeSettings()->GetOsSeparator());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::OpenSettings()
{
    settings = new VTapeSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                                 QApplication::applicationName(), this);
}

//---------------------------------------------------------------------------------------------------------------------
VTapeSettings *MApplication::TapeSettings()
{
    SCASSERT(settings != nullptr);
    return qobject_cast<VTapeSettings *>(settings);
}

//---------------------------------------------------------------------------------------------------------------------
QString MApplication::translationsPath() const
{
    const QString trPath = QStringLiteral("/translations");
#ifdef Q_OS_WIN
    QDir dir(QApplication::applicationDirPath() + trPath);
    if (dir.exists())
    {
        return dir.absolutePath();
    }
    else
    {
        return QApplication::applicationDirPath() + "/../../valentina/bin" + trPath;
    }
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + trPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir(QApplication::applicationDirPath() + trPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/translations");
        }
    }
#else // Unix
    QDir dir1(QApplication::applicationDirPath() + trPath);
    if (dir1.exists())
    {
        return dir1.absolutePath();
    }

    QDir dir2(QApplication::applicationDirPath() + "/../../valentina/bin" + trPath);
    if (dir2.exists())
    {
        return dir2.absolutePath();
    }
    else
    {
        return QStringLiteral("/usr/share/valentina/translations");
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString MApplication::diagramsPath() const
{
    const QString dPath = QStringLiteral("/diagrams.rcc");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + dPath;
#elif defined(Q_OS_MAC)
    QFileInfo fileBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + dPath);
    if (fileBundle.exists())
    {
        return fileBundle.absoluteFilePath();
    }
    else
    {
        QFileInfo file(QApplication::applicationDirPath() + dPath);
        if (file.exists())
        {
            return file.absoluteFilePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina") + dPath;
        }
    }
#else // Unix
    QFileInfo file(QApplication::applicationDirPath() + dPath);
    if (file.exists())
    {
        return file.absoluteFilePath();
    }
    else
    {
        return QStringLiteral("/usr/share/valentina") + dPath;
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::ShowDataBase()
{
    if (dataBase.isNull())
    {
        dataBase = new DialogMDataBase();
        dataBase->setAttribute(Qt::WA_DeleteOnClose, true);
        dataBase->setModal(false);
        dataBase->show();
    }
    else
    {
        dataBase->activateWindow();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::RetranslateGroups()
{
    if (not dataBase.isNull())
    {
        dataBase->RetranslateGroups();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::RetranslateTables()
{
    QList<TMainWindow*> list = MainWindows();
    for (int i=0; i < list.size(); ++i)
    {
        list.at(i)->RetranslateTable();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::ParseCommandLine(const SocketConnection &connection, const QStringList &arguments)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Valentina's measurements editor."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", tr("The measurement file."));
    //-----
    QCommandLineOption heightOption(QStringList() << "e" << "height",
            tr("Open with the base height. Valid values: %1cm.")
                                    .arg(VMeasurement::WholeListHeights(Unit::Cm).join(", ")),
            tr("The base height"));
    parser.addOption(heightOption);
    //-----
    QCommandLineOption sizeOption(QStringList() << "s" << "size",
            tr("Open with the base size. Valid values: %1cm.").arg(VMeasurement::WholeListSizes(Unit::Cm).join(", ")),
            tr("The base size"));
    parser.addOption(sizeOption);
    //-----
    QCommandLineOption unitOption(QStringList() << "u" << "unit",
            tr("Set pattern file unit: cm, mm, inch."),
            tr("The pattern unit"));
    parser.addOption(unitOption);
    //-----
    QCommandLineOption testOption(QStringList() << "test",
            tr("Use for unit testing. Run the program and open a file without showing a window."));
    parser.addOption(testOption);
    //-----
    parser.process(arguments);

    bool flagHeight = false;
    bool flagSize = false;
    bool flagUnit = false;

    int size = 0;
    int height = 0;
    Unit unit = Unit::Cm;

    if (parser.isSet(heightOption))
    {
        const QString heightValue = parser.value(heightOption);
        if (VMeasurement::IsGradationHeightValid(heightValue))
        {
            flagHeight = true;
            height = heightValue.toInt();
        }
        else
        {
            qCCritical(mApp, "%s\n",
                    qPrintable(tr("Invalid base height argument. Must be %1cm.")
                               .arg(VMeasurement::WholeListHeights(Unit::Cm).join(", "))));
            parser.showHelp(V_EX_USAGE);
        }
    }

    if (parser.isSet(sizeOption))
    {
        const QString sizeValue = parser.value(sizeOption);
        if (VMeasurement::IsGradationSizeValid(sizeValue))
        {
            flagSize = true;
            size = sizeValue.toInt();
        }
        else
        {
            qCCritical(mApp, "%s\n",
                    qPrintable(tr("Invalid base size argument. Must be %1cm.")
                               .arg(VMeasurement::WholeListSizes(Unit::Cm).join(", "))));
            parser.showHelp(V_EX_USAGE);
        }
    }

    {
    const QString unitValue = parser.value(unitOption);
    if (not unitValue.isEmpty())
    {

        const QStringList units = QStringList() << VDomDocument::UnitMM
                                                << VDomDocument::UnitCM
                                                << VDomDocument::UnitINCH;
        if (units.contains(unitValue))
        {
            flagUnit = true;
            unit = VDomDocument::StrToUnits(unitValue);
        }
        else
        {
            qCCritical(mApp, "%s\n", qPrintable(tr("Invalid base size argument. Must be cm, mm or inch.")));
            parser.showHelp(V_EX_USAGE);
        }
    }
    }

    testMode = parser.isSet(testOption);

    if (not testMode && connection == SocketConnection::Client)
    {
        const QString serverName = QCoreApplication::applicationName();
        QLocalSocket socket;
        socket.connectToServer(serverName);
        if (socket.waitForConnected(1000))
        {
            qCDebug(mApp, "Connected to the server '%s'", qUtf8Printable(serverName));
            QTextStream stream(&socket);
            stream << QCoreApplication::arguments().join(";;");
            stream.flush();
            socket.waitForBytesWritten();
            qApp->exit(V_EX_OK);
            return;
        }

        qCDebug(mApp, "Can't establish connection to the server '%s'", qUtf8Printable(serverName));

        localServer = new QLocalServer(this);
        connect(localServer, &QLocalServer::newConnection, this, &MApplication::NewLocalSocketConnection);
        if (not localServer->listen(serverName))
        {
            qCDebug(mApp, "Can't begin to listen for incoming connections on name '%s'",
                    qUtf8Printable(serverName));
            if (localServer->serverError() == QAbstractSocket::AddressInUseError)
            {
                QLocalServer::removeServer(serverName);
                if (not localServer->listen(serverName))
                {
                    qCWarning(mApp, "%s",
                     qUtf8Printable(tr("Can't begin to listen for incoming connections on name '%1'").arg(serverName)));
                }
            }
        }

        LoadTranslation(TapeSettings()->GetLocale());
    }

    const QStringList args = parser.positionalArguments();
    if (args.count() > 0)
    {
        if (testMode && args.count() > 1)
        {
            qCCritical(mApp, "%s\n", qPrintable(tr("Test mode doesn't support openning several files.")));
            parser.showHelp(V_EX_USAGE);
        }

        for (int i = 0; i < args.size(); ++i)
        {
            NewMainWindow();
            if (not MainWindow()->LoadFile(args.at(i)))
            {
                if (testMode)
                {
                    return; // process only one input file
                }
                delete MainWindow();
                continue;
            }

            if (flagSize)
            {
                MainWindow()->SetBaseMSize(size);
            }

            if (flagHeight)
            {
                MainWindow()->SetBaseMHeight(height);
            }

            if (flagUnit)
            {
                MainWindow()->SetPUnit(unit);
            }
        }
    }
    else
    {
        if (not testMode)
        {
            NewMainWindow();
        }
        else
        {
            qCCritical(mApp, "%s\n", qPrintable(tr("Please, provide one input file.")));
            parser.showHelp(V_EX_USAGE);
        }
    }

    if (testMode)
    {
        qApp->exit(V_EX_OK); // close program after processing in console mode
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_WS_MAC)
bool MApplication::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ApplicationActivate:
        {
            Clean();
            if (!mainWindows.isEmpty())
            {
                TMainWindow *mw = MainWindow();
                if (mw && !mw->isMinimized())
                {
                    MainWindow()->show();
                }
                return true;
            }
        }
        default:
            break;
    }
    return QApplication::event(event);
}
#endif

//---------------------------------------------------------------------------------------------------------------------
TMainWindow *MApplication::NewMainWindow()
{
    TMainWindow *tape = new TMainWindow();
    mainWindows.prepend(tape);
    if (not qApp->IsTestMode())
    {
        tape->show();
    }
    return tape;
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::ProcessCMD()
{
    ParseCommandLine(SocketConnection::Client, arguments());
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::OpenFile(const QString &path)
{
    MainWindow()->LoadFile(path);
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::NewLocalSocketConnection()
{
    QLocalSocket *socket = localServer->nextPendingConnection();
    if (not socket)
    {
        return;
    }
    socket->waitForReadyRead(1000);
    QTextStream stream(socket);
    const QString arg = stream.readAll();
    if (not arg.isEmpty())
    {
        ParseCommandLine(SocketConnection::Server, arg.split(";;"));
    }
    delete socket;
    MainWindow()->raise();
    MainWindow()->activateWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void MApplication::Clean()
{
    // cleanup any deleted main windows first
    for (int i = mainWindows.count() - 1; i >= 0; --i)
    {
        if (mainWindows.at(i).isNull())
        {
            mainWindows.removeAt(i);
        }
    }
}

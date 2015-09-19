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

#include <QDir>
#include <QFileOpenEvent>
#include <QLocalSocket>
#include <QResource>
#include <QTranslator>
#include <QPointer>
#include <QLocalServer>
#include <QMessageBox>
#include <iostream>

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../../libs/vmisc/backport/qcommandlineparser.h"
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

    if (isGuiThread)
    {
        //fixme: trying to make sure there are no save/load dialogs are opened, because error message during them will
        //lead to crash
        const bool topWinAllowsPop = (qApp->activeModalWidget() == nullptr) ||
                !qApp->activeModalWidget()->inherits("QFileDialog");
        QMessageBox messageBox;
        switch (type)
        {
            case QtDebugMsg:
                std::cerr << msg.toUtf8().constData() << std::endl;
                return;
            case QtWarningMsg:
                messageBox.setIcon(QMessageBox::Warning);
                break;
            case QtCriticalMsg:
                messageBox.setIcon(QMessageBox::Critical);
                break;
            case QtFatalMsg:
                messageBox.setIcon(QMessageBox::Critical);
                break;
            default:
                break;
        }

        if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
        {
            if (topWinAllowsPop)
            {
                messageBox.setInformativeText(msg);
                messageBox.setStandardButtons(QMessageBox::Ok);
                messageBox.setWindowModality(Qt::ApplicationModal);
                messageBox.setModal(true);
                messageBox.exec();
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
MApplication::MApplication(int &argc, char **argv)
    :VAbstractApplication(argc, argv),
      mainWindows(),
      localServer(nullptr),
      trVars(nullptr),
      dataBase(QPointer<DialogMDataBase>())
{
    setApplicationDisplayName(VER_PRODUCTNAME_STR);
    setApplicationName(VER_INTERNALNAME_STR);
    setOrganizationName(VER_COMPANYNAME_STR);
    setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    // Setting the Application version
    setApplicationVersion(APP_VERSION_STR);
    setWindowIcon(QIcon(":/tapeicon/64x64/logo.png"));

    const QString serverName = QCoreApplication::applicationName();
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500))
    {
        QTextStream stream(&socket);
        stream << QCoreApplication::arguments().join(";;");
        stream.flush();
        socket.waitForBytesWritten();
        return;
    }

    localServer = new QLocalServer(this);
    connect(localServer, &QLocalServer::newConnection, this, &MApplication::NewLocalSocketConnection);
    if (!localServer->listen(serverName))
    {
        if (localServer->serverError() == QAbstractSocket::AddressInUseError
                && QFile::exists(localServer->serverName()))
        {
            QFile::remove(localServer->serverName());
            localServer->listen(serverName);
        }
    }
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
bool MApplication::IsTheOnly() const
{
    return (localServer != 0);
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

    qDebug()<<"Version:"<<APP_VERSION_STR;
    qDebug()<<"Build revision:"<<BUILD_REVISION;
    qDebug()<<buildCompatibilityString();
    qDebug()<<"Built on"<<__DATE__<<"at"<<__TIME__;
    qDebug()<<"Command-line arguments:"<<this->arguments();
    qDebug()<<"Process ID:"<<this->applicationPid();

    LoadTranslation(TapeSettings()->GetLocale());

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
void MApplication::ParseCommandLine(const QStringList &arguments)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Valentina's measurements editor."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "The measurement file."));
    //-----
    QCommandLineOption heightOption(QStringList() << "e" << "height",
    QCoreApplication::translate("main", "Open with the base height: 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, "
                                "152, 158, 164, 170, 176, 182 or 188 cm."),
                                QCoreApplication::translate("main", "The base height"));
    parser.addOption(heightOption);
    //-----
    QCommandLineOption sizeOption(QStringList() << "s" << "size",
    QCoreApplication::translate("main", "Open with the base size: 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, "
                                "48, 50, 52, 54 or 56 cm."),
                                QCoreApplication::translate("main", "The base size"));
    parser.addOption(sizeOption);
    //-----
    QCommandLineOption unitOption(QStringList() << "u" << "unit",
    QCoreApplication::translate("main", "Set pattern file unit: cm, mm, inch."),
                                QCoreApplication::translate("main", "The pattern unit"));
    parser.addOption(unitOption);
    //-----
    parser.process(arguments);

    bool flagHeight = false;
    bool flagSize = false;
    bool flagUnit = false;

    int size = 0;
    int height = 0;
    Unit unit = Unit::Cm;

    {
    const QString heightValue = parser.value(heightOption);
    if (not heightValue.isEmpty())
    {
        const QStringList heights = VMeasurement::WholeListHeights(Unit::Cm);
        if (heights.contains(heightValue))
        {
            flagHeight = true;
            height = heightValue.toInt();
        }
        else
        {
            fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main",
            "Error: Invalid base height argument. Must be 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, 152, 158, "
            "164, 170, 176, 182 or 188 cm.")));
            parser.showHelp(1);
        }
    }
    }

    {
    const QString sizeValue = parser.value(sizeOption);
    if (not sizeValue.isEmpty())
    {
        const QStringList sizes = VMeasurement::WholeListSizes(Unit::Cm);
        if (sizes.contains(sizeValue))
        {
            flagSize = true;
            size = sizeValue.toInt();
        }
        else
        {
            fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main",
            "Error: Invalid base size argument. Must be 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, "
            "52, 54 or 56 cm.")));
            parser.showHelp(1);
        }
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
            fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main",
            "Error: Invalid base size argument. Must be cm, mm or inch.")));
            parser.showHelp(1);
        }
    }
    }

    const QStringList args = parser.positionalArguments();
    if (args.count() > 0)
    {
        for (int i = 0; i < args.size(); ++i)
        {
            NewMainWindow();
            MainWindow()->LoadFile(args.at(i));

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
        NewMainWindow();
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
    tape->show();
    return tape;
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
        ParseCommandLine(arg.split(";;"));
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

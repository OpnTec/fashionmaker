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
#include <QLibraryInfo>
#include <QLocalSocket>
#include <QResource>
#include <QTranslator>
#include <QPointer>

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
        QStringList args = QCoreApplication::arguments();
        if (args.count() > 1)
        {
            args.removeFirst();
            const QString arguments = args.join(";;");
            stream << arguments;
        }
        else
        {
            stream << QString();
        }
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
    OpenSettings();

    qDebug()<<"Version:"<<APP_VERSION_STR;
    qDebug()<<"Build revision:"<<BUILD_REVISION;
    qDebug()<<buildCompatibilityString();
    qDebug()<<"Built on"<<__DATE__<<"at"<<__TIME__;
    qDebug()<<"Command-line arguments:"<<this->arguments();
    qDebug()<<"Process ID:"<<this->applicationPid();

    const QString checkedLocale = TapeSettings()->GetLocale();
    qDebug()<<"Checked locale:"<<checkedLocale;

    QTranslator *qtTranslator = new QTranslator(this);
#if defined(Q_OS_WIN)
    qtTranslator->load("qt_" + checkedLocale, translationsPath());
#else
    qtTranslator->load("qt_" + checkedLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    installTranslator(qtTranslator);

    QTranslator *qtxmlTranslator = new QTranslator(this);
#if defined(Q_OS_WIN)
    qtxmlTranslator->load("qtxmlpatterns_" + checkedLocale, translationsPath());
#else
    qtxmlTranslator->load("qtxmlpatterns_" + checkedLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    installTranslator(qtxmlTranslator);

    QTranslator *appTranslator = new QTranslator(this);
    appTranslator->load("valentina_" + checkedLocale, translationsPath());
    installTranslator(appTranslator);

    InitTrVars();//Very important do it after load QM files.

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
    trVars = new VTranslateVars(TapeSettings()->GetOsSeparator());
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
        return QApplication::applicationDirPath() + "../../valentina/bin" + trPath;
    }
#else
    #ifdef QT_DEBUG
        QDir dir(QApplication::applicationDirPath() + trPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QApplication::applicationDirPath() + "../../valentina/bin" + trPath;
        }
    #else
        QDir dir1(QApplication::applicationDirPath() + trPath);
        if (dir1.exists())
        {
            return dir1.absolutePath();
        }

        QDir dir2(QApplication::applicationDirPath() + "../../valentina/bin" + trPath);
        if (dir2.exists())
        {
            return dir2.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/translations");
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString MApplication::diagramsPath() const
{
    const QString dPath = QStringLiteral("/diagrams.rcc");
#ifdef Q_OS_WIN
    QFileInfo file(QApplication::applicationDirPath()  + dPath);
    if (file.exists())
    {
        return file.absoluteFilePath();
    }
    else
    {
        return QApplication::applicationDirPath() + "../../app/tape/bin" + dPath;
    }
#else
    #ifdef QT_DEBUG
        QFileInfo file(QApplication::applicationDirPath() + dPath);
        if (file.exists())
        {
            return file.absoluteFilePath();
        }
        else
        {
            return QApplication::applicationDirPath() + "../../app/tape/bin" + dPath;
        }
    #else
        QFileInfo file1(QApplication::applicationDirPath() + dPath);
        if (file1.exists())
        {
            return file1.absoluteFilePath();
        }

        QFileInfo file2(QApplication::applicationDirPath() + "../../app/tape/bin" + dPath);
        if (file2.exists())
        {
            return file2.absoluteFilePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina");
        }
    #endif
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
    QString path;
    stream >> path;
    if (not path.isEmpty())
    {
        const QStringList args = path.split(";;");
        for (int i = 0; i < args.size(); ++i)
        {
            NewMainWindow();
            OpenFile(args.at(i));
        }
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

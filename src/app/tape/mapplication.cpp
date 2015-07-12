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

#include <QFileOpenEvent>
#include <QLocalSocket>

//---------------------------------------------------------------------------------------------------------------------
MApplication::MApplication(int &argc, char **argv)
    :QApplication(argc, argv),
      mainWindows(),
      localServer(nullptr)
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
MApplication *MApplication::instance()
{
    return (static_cast<MApplication *>(QCoreApplication::instance()));
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

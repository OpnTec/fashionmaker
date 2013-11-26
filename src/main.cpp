/************************************************************************
 **
 **  @file   main.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "mainwindow.h"
#include "widgets/vapplication.h"
#include <QTextCodec>
#include <QtCore>
#include "tablewindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            abort();
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    VApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
#ifdef Q_OS_WIN32
    appTranslator.load("valentina_" + QLocale::system().name(), ".");
#else
    #ifdef QT_DEBUG
        appTranslator.load("valentina_" + QLocale::system().name(), ".");
    #else
        appTranslator.load("valentina_" + QLocale::system().name(), "/usr/share/valentina/translations");
    #endif
#endif
    app.installTranslator(&appTranslator);

    MainWindow w;
    w.setWindowState(w.windowState() ^ Qt::WindowMaximized);
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    TableWindow table;
    QObject::connect(&w, &MainWindow::ModelChosen, &table, &TableWindow::ModelChosen);
    QObject::connect(&table, &TableWindow::closed, &w, &MainWindow::tableClosed);

    const QStringList args = app.arguments();
    QString fileName;
    QRegExp rxArgOpenFile("-o");//parameter open file

    if (args.size()>1)
    {
        for (int i = 1; i < args.size(); ++i)
        {
            if (rxArgOpenFile.indexIn(args.at(i)) != -1 )
            {
                if (args.at(i+1).isEmpty() == false)
                {
                    fileName =  args.at(i+1);
                    qDebug() << args.at(i)<< ":" << fileName;
                    w.OpenPattern(fileName);
                }
                w.show();
                break;
            }
            else
            {
                qDebug() << "Uknown arg:" << args.at(i);
                w.show();
                break;
            }
        }
    }
    else
    {
        w.show();
    }
    return app.exec();
}

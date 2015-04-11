/************************************************************************
 **
 **  @file   main.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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
#include "core/vapplication.h"
#include "core/vsettings.h"
#include "tablewindow.h"
#include "version.h"

#include <QTextCodec>
#include <QMessageBox>
#include <QThread>
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../core/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif
#include <QtXml>
#include <QLibraryInfo>

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(cursor);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(theme);
    Q_INIT_RESOURCE(flags);
    Q_INIT_RESOURCE(icons);

    QT_REQUIRE_VERSION(argc, argv, "5.0.0");

    VApplication app(argc, argv);

    app.setApplicationDisplayName(VER_PRODUCTNAME_STR);
    app.setApplicationName(VER_INTERNALNAME_STR);
    app.setOrganizationName(VER_COMPANYNAME_STR);
    app.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    // Setting the Application version
    app.setApplicationVersion(APP_VERSION);

    app.OpenSettings();

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    // Catch and send report
    VApplication::DrMingw();
    app.CollectReports();
#endif

    // Run creation log after sending crash report
    app.StartLogging();

    qDebug()<<"Version:"<<APP_VERSION;
    qDebug()<<"Build revision:"<<BUILD_REVISION;
    qDebug()<<buildCompatibilityString();
    qDebug()<<"Built on"<<__DATE__<<"at"<<__TIME__;
    qDebug()<<"Command-line arguments:"<<app.arguments();
    qDebug()<<"Process ID:"<<app.applicationPid();

    const QString checkedLocale = qApp->getSettings()->GetLocale();
    qDebug()<<"Checked locale:"<<checkedLocale;

    QTranslator qtTranslator;
#if defined(Q_OS_WIN)
    qtTranslator.load("qt_" + checkedLocale, QCoreApplication::applicationDirPath());
#else
    qtTranslator.load("qt_" + checkedLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("valentina_" + checkedLocale, qApp->translationsPath());
    app.installTranslator(&appTranslator);

    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    if (QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK) == false)
    {
        //If there is no default working icon theme then we should
        //use an icon theme that we provide via a .qrc file
        //This case happens under Windows and Mac OS X
        //This does not happen under GNOME or KDE
        QIcon::setThemeName("win.icon.theme");
    }

    MainWindow w;
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    app.setMainWindow(&w);
    TableWindow table;
    QObject::connect(&w, &MainWindow::ModelChosen, &table, &TableWindow::ModelChosen);
    QObject::connect(&table, &TableWindow::closed, &w, &MainWindow::tableClosed);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Pattern making program."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Pattern file."));
    parser.process(app);
    QStringList args = parser.positionalArguments();

    //Before we load pattern show window.
    w.show();

    w.ReopenFilesAfterCrash(args);

    for (int i=0;i<args.size();++i)
    {
        w.LoadPattern(args.at(i));
    }

    return app.exec();
}

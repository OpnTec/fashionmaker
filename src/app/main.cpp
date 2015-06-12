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

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../core/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(cursor);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(theme);
    Q_INIT_RESOURCE(flags);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(toolicon);

    QT_REQUIRE_VERSION(argc, argv, "5.0.0");

    VApplication app(argc, argv);
    app.InitOptions();

    MainWindow w;
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    app.setMainWindow(&w);

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

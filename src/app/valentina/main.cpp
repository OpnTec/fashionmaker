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
#include <QMessageBox> // For QT_REQUIRE_VERSION

// Lock producing random attribute order in XML
 // https://stackoverflow.com/questions/27378143/qt-5-produce-random-attribute-order-in-xml
 extern Q_CORE_EXPORT QBasicAtomicInt qt_qhash_seed;

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

    qt_qhash_seed.store(0); // Lock producing random attribute order in XML

    VApplication app(argc, argv);

    app.InitOptions();

    MainWindow w;
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
    app.setMainWindow(&w);

    auto args = app.CommandLine()->OptInputFileNames();

    //Before we load pattern show window.
    if (VApplication::CheckGUI())
    {
        w.show();
        w.ReopenFilesAfterCrash(args);
    }

    for (size_t i=0, sz = args.size(); i < sz;++i)
    {
        w.LoadPattern(args.at(static_cast<int>(i)), app.CommandLine()->OptMeasurePath());
        if (app.CommandLine()->IsExportEnabled())
        {            
            w.DoExport(app.CommandLine());
            break;
        }
    }

    return (VApplication::CheckGUI()) ? app.exec() : 0; // single return point is always better than more
}

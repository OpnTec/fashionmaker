/************************************************************************
 **
 **  @file   main.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 7, 2015
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

#include "tmainwindow.h"
#include "mapplication.h"

#include <QMessageBox> // For QT_REQUIRE_VERSION

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(icon);

    QT_REQUIRE_VERSION(argc, argv, "5.0.0");

    MApplication app(argc, argv);
    if (not app.IsTheOnly())
    {
        return 0;
    }

    QStringList args = QCoreApplication::arguments();
    if (args.count() > 1)
    {
        args.removeFirst();
        for (int i = 0; i < args.size(); ++i)
        {
            app.NewMainWindow();
            app.MainWindow()->LoadFile(args.at(i));
        }
    }
    else
    {
        app.NewMainWindow();
    }
    return app.exec();
}

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
#include "../fervor/fvupdater.h"

#include <QMessageBox> // For QT_REQUIRE_VERSION
#include <QTimer>

// Lock producing random attribute order in XML
// https://stackoverflow.com/questions/27378143/qt-5-produce-random-attribute-order-in-xml
extern Q_CORE_EXPORT QBasicAtomicInt qt_qhash_seed;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(tapeicon);
    Q_INIT_RESOURCE(theme);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(flags);

    QT_REQUIRE_VERSION(argc, argv, "5.0.0");

    qt_qhash_seed.store(0); // Lock producing random attribute order in XML

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
#endif

    MApplication app(argc, argv);
    app.InitOptions();

    // Set feed URL before doing anything else
    FvUpdater::sharedUpdater()->SetFeedURL(defaultFeedURL);

    QTimer::singleShot(0, &app, SLOT(ProcessCMD()));

    return app.exec();
}

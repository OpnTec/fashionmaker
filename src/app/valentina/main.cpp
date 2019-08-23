/************************************************************************
 **
 **  @file   main.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../fervor/fvupdater.h"
#include "../vpatterndb/vpiecenode.h"

#include <QMessageBox> // For QT_REQUIRE_VERSION
#include <QTimer>

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
    Q_INIT_RESOURCE(style);

    QT_REQUIRE_VERSION(argc, argv, "5.4.0")// clazy:exclude=qstring-arg,qstring-allocations

#if defined(Q_OS_WIN)
    VAbstractApplication::WinAttachConsole();
#endif

    // Need to internally move a node inside a piece main path
    qRegisterMetaTypeStreamOperators<VPieceNode>("VPieceNode");
    // Need to internally move a node inside a custom seam allowance path
    qRegisterMetaTypeStreamOperators<CustomSARecord>("CustomSARecord");

#ifndef Q_OS_MAC // supports natively
    InitHighDpiScaling(argc, argv);
#endif //Q_OS_MAC

    VApplication app(argc, argv);

    app.InitOptions();

    if (FvUpdater::IsStaledTestBuild())
    {
        qWarning() << QApplication::translate("Valentina",
                                              "This test build is older than %1 days. To provide you with better "
                                              "quality service we restrict the lifetime you can use a test build. "
                                              "To continue using Valentina please update to newer test build. The "
                                              "application will be shut down.")
                       .arg(FvUpdater::testBuildLifetime);
        return V_EX_UNAVAILABLE;
    }

    if (VApplication::IsGUIMode())
    {
        // Set feed URL before doing anything else
        FvUpdater::sharedUpdater()->SetFeedURL(FvUpdater::CurrentFeedURL());

        // Check for updates automatically
        FvUpdater::sharedUpdater()->CheckForUpdatesSilent();
    }

    MainWindow w;
#if !defined(Q_OS_MAC)
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
#endif // !defined(Q_OS_MAC)
    app.setMainWindow(&w);

    int msec = 0;
    //Before we load pattern show window.
    if (VApplication::IsGUIMode())
    {
        w.show();
        msec = 15; // set delay for correct the first fitbest zoom
    }

    QTimer::singleShot(msec, &w, &MainWindow::ProcessCMD);

    return app.exec();
}

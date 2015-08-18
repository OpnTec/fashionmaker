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

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
#   include "../../libs/vmisc/backport/qcommandlineparser.h"
#else
#   include <QCommandLineParser>
#endif

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

    MApplication app(argc, argv);
    if (not app.IsTheOnly())
    {
        return 0;
    }
    app.InitOptions();

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Valentina's measurements editor."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "The measurement file."));
    //-----
    QCommandLineOption heightOption(QStringList() << "h" << "height",
    QCoreApplication::translate("main", "Open with the base height: 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, "
                                "152, 158, 164, 170, 176, 182, 188 (default is measurement file value)."),
                                QCoreApplication::translate("main", "The base height"));
    parser.addOption(heightOption);
    //-----
    QCommandLineOption sizeOption(QStringList() << "s" << "size",
    QCoreApplication::translate("main", "Open with the base size: 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, "
                                "48, 50, 52, 54, 56 (default is measurement file value)."),
                                QCoreApplication::translate("main", "The Base size"));
    parser.addOption(sizeOption);
    //-----
    QCommandLineOption unitOption(QStringList() << "u" << "unit",
    QCoreApplication::translate("main", "Set pattern file unit: cm, mm, inch (default is measurement file value)."),
                                QCoreApplication::translate("main", "The pattern unit"));
    parser.addOption(unitOption);
    //-----
    parser.process(app);

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
                                                                           "164, 170, 176, 182 or 188.")));
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
                                                                           "52, 54 or 56.")));
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
            app.NewMainWindow();
            app.MainWindow()->LoadFile(args.at(i));

            if (flagSize)
            {
                app.MainWindow()->SetBaseMSize(size);
            }

            if (flagHeight)
            {
                app.MainWindow()->SetBaseMHeight(height);
            }

            if (flagUnit)
            {
                app.MainWindow()->SetPUnit(unit);
            }
        }
    }
    else
    {
        app.NewMainWindow();
    }
    return app.exec();
}

/************************************************************************
 **
 **  @file   tst_vdomdocument.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 1, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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
#include "tst_vdomdocument.h"

#include <QtTest>
#include "../ifc/xml/vdomdocument.h"

//---------------------------------------------------------------------------------------------------------------------
TST_VDomDocument::TST_VDomDocument(QObject *parent)
    :QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VDomDocument::TestCompareDomElements_data()
{
    QTest::addColumn<QString>("element1Content");
    QTest::addColumn<QString>("element2Content");
    QTest::addColumn<bool>("compare");

    QTest::newRow("Base point")
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"1\" name=\"Т\" mx=\"0.132292\" "
               "my=\"0.264583\"/>"
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"1\" name=\"Т\" mx=\"0.132292\" "
                                      "my=\"0.264583\"/>"
            << true;

    QTest::newRow("Base point. Change attribute order")
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"1\" name=\"Т\" mx=\"0.132292\" "
               "my=\"0.264583\"/>"
            << "<point type=\"single\" id=\"1\" name=\"Т\" x=\"0.819903\" y=\"23.1895\"  mx=\"0.132292\" "
                                      "my=\"0.264583\"/>"
            << true;

    QTest::newRow("Base point. Change attribute value")
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"1\" name=\"Т\" mx=\"0.132292\" "
               "my=\"0.264583\"/>"
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"2\" name=\"Т\" mx=\"0.132292\" "
                                      "my=\"0.264583\"/>"
            << false;

    QTest::newRow("Dirrent types")
            << "<point type=\"single\" x=\"0.819903\" y=\"23.1895\" id=\"1\" name=\"Т\" mx=\"0.132292\" "
               "my=\"0.264583\"/>"
            << "<spline angle1=\"229.381\" angle2=\"41.6325\" color=\"black\" id=\"4\" kAsm1=\"0.962941\" "
               "kAsm2=\"1.00054\" kCurve=\"1\" point1=\"3\" point4=\"2\" type=\"simple\"/>"
            << false;

    QTest::newRow("Equal details")
            << "<detail closed=\"1\" forbidFlipping=\"false\" forceFlipping=\"false\" hideMainPath=\"false\" id=\"24\" "
               "mx=\"-11.7117\" my=\"4.05406\" name=\"Case 2\" seamAllowance=\"true\" version=\"2\" width=\"1\">"
               "<data annotation=\"\" foldPosition=\"\" fontSize=\"0\" height=\"1\" letter=\"\" mx=\"0\" my=\"0\" "
               "onFold=\"false\" orientation=\"\" quantity=\"1\" rotation=\"0\" rotationWay=\"\" tilt=\"\" "
               "visible=\"false\" width=\"1\"/>"
               "<patternInfo fontSize=\"0\" height=\"1\" mx=\"0\" my=\"0\" rotation=\"0\" visible=\"false\" "
               "width=\"1\"/>"
               "<grainline arrows=\"0\" length=\"1\" mx=\"0\" my=\"0\" rotation=\"90\" visible=\"false\"/>"
               "<nodes>"
                   "<node idObject=\"19\" type=\"NodePoint\"/>"
                   "<node idObject=\"20\" type=\"NodePoint\"/>"
                   "<node idObject=\"21\" reverse=\"0\" type=\"NodeSpline\"/>"
                   "<node idObject=\"22\" type=\"NodePoint\"/>"
               "</nodes>"
               "</detail>"
            << "<detail closed=\"1\" forbidFlipping=\"false\" forceFlipping=\"false\" hideMainPath=\"false\" id=\"24\" "
               "mx=\"-11.7117\" my=\"4.05406\" name=\"Case 2\" seamAllowance=\"true\" version=\"2\" width=\"1\">"
               "<data annotation=\"\" foldPosition=\"\" fontSize=\"0\" height=\"1\" letter=\"\" mx=\"0\" my=\"0\" "
               "onFold=\"false\" orientation=\"\" quantity=\"1\" rotation=\"0\" rotationWay=\"\" tilt=\"\" "
               "visible=\"false\" width=\"1\"/>"
               "<patternInfo fontSize=\"0\" height=\"1\" mx=\"0\" my=\"0\" rotation=\"0\" visible=\"false\" "
               "width=\"1\"/>"
               "<grainline arrows=\"0\" length=\"1\" mx=\"0\" my=\"0\" rotation=\"90\" visible=\"false\"/>"
               "<nodes>"
                   "<node idObject=\"19\" type=\"NodePoint\"/>"
                   "<node idObject=\"20\" type=\"NodePoint\"/>"
                   "<node idObject=\"21\" reverse=\"0\" type=\"NodeSpline\"/>"
                   "<node idObject=\"22\" type=\"NodePoint\"/>"
               "</nodes>"
               "</detail>"
            << true;
    QTest::newRow("Not equal details")
            << "<detail closed=\"1\" forbidFlipping=\"false\" forceFlipping=\"false\" hideMainPath=\"false\" id=\"24\" "
               "mx=\"-11.7117\" my=\"4.05406\" name=\"Case 2\" seamAllowance=\"true\" version=\"2\" width=\"1\">"
               "<data annotation=\"\" foldPosition=\"\" fontSize=\"0\" height=\"1\" letter=\"\" mx=\"0\" my=\"0\" "
               "onFold=\"false\" orientation=\"\" quantity=\"1\" rotation=\"0\" rotationWay=\"\" tilt=\"\" "
               "visible=\"false\" width=\"1\"/>"
               "<patternInfo fontSize=\"0\" height=\"1\" mx=\"0\" my=\"0\" rotation=\"0\" visible=\"false\" "
               "width=\"1\"/>"
               "<grainline arrows=\"0\" length=\"1\" mx=\"0\" my=\"0\" rotation=\"90\" visible=\"false\"/>"
               "<nodes>"
                   "<node idObject=\"19\" type=\"NodePoint\"/>"
                   "<node idObject=\"20\" type=\"NodePoint\"/>"
                   "<node idObject=\"21\" reverse=\"0\" type=\"NodeSpline\"/>"
                   "<node idObject=\"22\" type=\"NodePoint\"/>"
               "</nodes>"
               "</detail>"
            << "<detail closed=\"1\" forbidFlipping=\"false\" forceFlipping=\"false\" hideMainPath=\"false\" id=\"35\" "
               "mx=\"7.71397\" my=\"-3.94145\" name=\"Case 1\" seamAllowance=\"true\" version=\"2\" width=\"2\">"
               "<data annotation=\"\" foldPosition=\"\" fontSize=\"0\" height=\"1\" letter=\"\" mx=\"0\" my=\"0\" "
               "onFold=\"false\" orientation=\"\" quantity=\"1\" rotation=\"0\" rotationWay=\"\" tilt=\"\" "
               "visible=\"false\" width=\"1\"/>"
               "<patternInfo fontSize=\"0\" height=\"1\" mx=\"0\" my=\"0\" rotation=\"0\" visible=\"false\" "
               "width=\"1\"/>"
               "<grainline arrows=\"0\" length=\"1\" mx=\"0\" my=\"0\" rotation=\"0\" visible=\"false\"/>"
               "<nodes>"
                   "<node idObject=\"30\" type=\"NodePoint\"/>"
                   "<node idObject=\"31\" reverse=\"0\" type=\"NodeSpline\"/>"
                   "<node idObject=\"32\" type=\"NodePoint\"/>"
                   "<node idObject=\"33\" type=\"NodePoint\"/>"
               "</nodes>"
               "</detail>"
            << false;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VDomDocument::TestCompareDomElements()
{
    QFETCH(QString, element1Content);
    QFETCH(QString, element2Content);
    QFETCH(bool, compare);

    QDomDocument xmlDoc;

    QVERIFY(xmlDoc.setContent(element1Content));
    QDomElement element1 = xmlDoc.firstChild().toElement();

    QVERIFY(xmlDoc.setContent(element2Content));
    QDomElement element2 = xmlDoc.firstChild().toElement();

    const bool result = VDomDocument::Compare(element1, element2);
    QCOMPARE(compare, result);
}

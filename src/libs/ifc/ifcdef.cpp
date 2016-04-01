/************************************************************************
 **
 **  @file   ifcdef.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2015
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

#include "ifcdef.h"

const QString CustomMSign    = QStringLiteral("@");
const QString CustomIncrSign = QStringLiteral("#");

#define DefWidth 1.2//mm

//---------------------------------------------------------------------------------------------------------------------
qreal WidthMainLine(Unit patternUnit)
{
    qreal _widthMainLine = DefWidth;
    switch (patternUnit)
    {
        case Unit::Mm:
            _widthMainLine = DefWidth;
            break;
        case Unit::Cm:
            _widthMainLine = DefWidth/10.0;
            break;
        case Unit::Inch:
            _widthMainLine = DefWidth/25.4;
            break;
        default:
            _widthMainLine = DefWidth;
            break;
    }
    return _widthMainLine;
}

//---------------------------------------------------------------------------------------------------------------------
qreal WidthHairLine(Unit patternUnit)
{
    return WidthMainLine(patternUnit)/3.0;
}

const QString AttrType        = QStringLiteral("type");
const QString AttrMx          = QStringLiteral("mx");
const QString AttrMy          = QStringLiteral("my");
const QString AttrName        = QStringLiteral("name");
const QString AttrMx1         = QStringLiteral("mx1");
const QString AttrMy1         = QStringLiteral("my1");
const QString AttrName1       = QStringLiteral("name1");
const QString AttrMx2         = QStringLiteral("mx2");
const QString AttrMy2         = QStringLiteral("my2");
const QString AttrName2       = QStringLiteral("name2");
const QString AttrBaseLineP1  = QStringLiteral("baseLineP1");
const QString AttrBaseLineP2  = QStringLiteral("baseLineP2");
const QString AttrDartP1      = QStringLiteral("dartP1");
const QString AttrDartP2      = QStringLiteral("dartP2");
const QString AttrDartP3      = QStringLiteral("dartP3");
const QString AttrX           = QStringLiteral("x");
const QString AttrY           = QStringLiteral("y");
const QString AttrTypeLine    = QStringLiteral("typeLine");
const QString AttrLength      = QStringLiteral("length");
const QString AttrBasePoint   = QStringLiteral("basePoint");
const QString AttrFirstPoint  = QStringLiteral("firstPoint");
const QString AttrSecondPoint = QStringLiteral("secondPoint");
const QString AttrThirdPoint  = QStringLiteral("thirdPoint");
const QString AttrCenter      = QStringLiteral("center");
const QString AttrRadius      = QStringLiteral("radius");
const QString AttrAngle       = QStringLiteral("angle");
const QString AttrAngle1      = QStringLiteral("angle1");
const QString AttrAngle2      = QStringLiteral("angle2");
const QString AttrLength1     = QStringLiteral("length1");
const QString AttrLength2     = QStringLiteral("length2");
const QString AttrP1Line      = QStringLiteral("p1Line");
const QString AttrP2Line      = QStringLiteral("p2Line");
const QString AttrP1Line1     = QStringLiteral("p1Line1");
const QString AttrP2Line1     = QStringLiteral("p2Line1");
const QString AttrP1Line2     = QStringLiteral("p1Line2");
const QString AttrP2Line2     = QStringLiteral("p2Line2");
const QString AttrPShoulder   = QStringLiteral("pShoulder");
const QString AttrPoint1      = QStringLiteral("point1");
const QString AttrPoint2      = QStringLiteral("point2");
const QString AttrPoint3      = QStringLiteral("point3");
const QString AttrPoint4      = QStringLiteral("point4");
const QString AttrKAsm1       = QStringLiteral("kAsm1");
const QString AttrKAsm2       = QStringLiteral("kAsm2");
const QString AttrKCurve      = QStringLiteral("kCurve");
const QString AttrDuplicate   = QStringLiteral("duplicate");
const QString AttrPathPoint   = QStringLiteral("pathPoint");
const QString AttrPSpline     = QStringLiteral("pSpline");
const QString AttrAxisP1      = QStringLiteral("axisP1");
const QString AttrAxisP2      = QStringLiteral("axisP2");
const QString AttrCurve       = QStringLiteral("curve");
const QString AttrCurve1      = QStringLiteral("curve1");
const QString AttrCurve2      = QStringLiteral("curve2");
const QString AttrLineColor   = QStringLiteral("lineColor");
const QString AttrColor       = QStringLiteral("color");
const QString AttrFirstArc    = QStringLiteral("firstArc");
const QString AttrSecondArc   = QStringLiteral("secondArc");
const QString AttrCrossPoint  = QStringLiteral("crossPoint");
const QString AttrVCrossPoint = QStringLiteral("vCrossPoint");
const QString AttrHCrossPoint = QStringLiteral("hCrossPoint");
const QString AttrC1Center    = QStringLiteral("c1Center");
const QString AttrC2Center    = QStringLiteral("c2Center");
const QString AttrC1Radius    = QStringLiteral("c1Radius");
const QString AttrC2Radius    = QStringLiteral("c2Radius");
const QString AttrCCenter     = QStringLiteral("cCenter");
const QString AttrTangent     = QStringLiteral("tangent");
const QString AttrCRadius     = QStringLiteral("cRadius");
const QString AttrArc         = QStringLiteral("arc");

const QString TypeLineNone           = QStringLiteral("none");
const QString TypeLineLine           = QStringLiteral("hair");
const QString TypeLineDashLine       = QStringLiteral("dashLine");
const QString TypeLineDotLine        = QStringLiteral("dotLine");
const QString TypeLineDashDotLine    = QStringLiteral("dashDotLine");
const QString TypeLineDashDotDotLine = QStringLiteral("dashDotDotLine");

const QString ColorBlack     = QStringLiteral("black");
const QString ColorGreen     = QStringLiteral("green");
const QString ColorBlue      = QStringLiteral("blue");
const QString ColorDarkRed   = QStringLiteral("darkRed");
const QString ColorDarkGreen = QStringLiteral("darkGreen");
const QString ColorDarkBlue  = QStringLiteral("darkBlue");
const QString ColorYellow    = QStringLiteral("yellow");

//variables
const QString line_         = QStringLiteral("Line_");
const QString angleLine_    = QStringLiteral("AngleLine_");
const QString spl_          = QStringLiteral(SPL_);
const QString arc_          = QStringLiteral(ARC_);
const QString splPath       = QStringLiteral("SplPath");
const QString radius_V      = QStringLiteral("Radius");
const QString radiusArc_    = radius_V + arc_;
const QString angle1_V      = QStringLiteral("Angle1");
const QString angle2_V      = QStringLiteral("Angle2");
const QString angle1Arc_    = angle1_V + arc_;
const QString angle2Arc_    = angle2_V + arc_;
const QString angle1Spl_    = angle1_V + spl_;
const QString angle2Spl_    = angle2_V + spl_;
const QString angle1SplPath = angle1_V + splPath;
const QString angle2SplPath = angle2_V + splPath;
const QString seg_          = QStringLiteral("Seg_");

const QStringList builInVariables = QStringList() << line_
                                                  << angleLine_
                                                  << arc_
                                                  << spl_
                                                  << splPath
                                                  << radius_V
                                                  << radiusArc_
                                                  << angle1_V
                                                  << angle2_V
                                                  << angle1Arc_
                                                  << angle2Arc_
                                                  << angle1Spl_
                                                  << angle2Spl_
                                                  << angle1SplPath
                                                  << angle2SplPath
                                                  << seg_;

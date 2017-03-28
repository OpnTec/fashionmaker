/************************************************************************
 **
 **  @file   ifcdef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 12, 2014
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

#ifndef IFCDEF_H
#define IFCDEF_H

#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "../vmisc/def.h"

extern const QString CustomMSign;
extern const QString CustomIncrSign;

#ifdef Q_OS_WIN32
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    #include <windows.h>
#endif /*Q_OS_WIN32*/

static const quint32 null_id = 0;

#define NULL_ID null_id//use this value for initialization variables that keeps id values. 0 mean uknown id value.
#define NULL_ID_STR "0"

// Detect whether the compiler supports C++11 noexcept exception specifications.
#  if   defined(__clang__)
#    if __has_feature(cxx_noexcept)
#      define V_NOEXCEPT_EXPR(x) noexcept(x) // Clang 3.0 and above have noexcept
#    endif
#  elif defined(__GNUC__)
#    if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#      define V_NOEXCEPT_EXPR(x) noexcept(x) // GCC 4.7 and following have noexcept
#    endif
#  elif defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
#    define V_NOEXCEPT_EXPR(x) noexcept(x) // Visual Studio 2015 and following have noexcept
#  else
#    define V_NOEXCEPT_EXPR(x)
#  endif

qreal WidthMainLine(Unit patternUnit);
qreal WidthHairLine(Unit patternUnit);

extern const QString AttrType;
extern const QString AttrMx;
extern const QString AttrMy;
extern const QString AttrName;
extern const QString AttrMx1;
extern const QString AttrMy1;
extern const QString AttrName1;
extern const QString AttrMx2;
extern const QString AttrMy2;
extern const QString AttrName2;
extern const QString AttrBaseLineP1;
extern const QString AttrBaseLineP2;
extern const QString AttrDartP1;
extern const QString AttrDartP2;
extern const QString AttrDartP3;
extern const QString AttrX;
extern const QString AttrY;
extern const QString AttrTypeLine;
extern const QString AttrLength;
extern const QString AttrBasePoint;
extern const QString AttrFirstPoint;
extern const QString AttrSecondPoint;
extern const QString AttrThirdPoint;
extern const QString AttrCenter;
extern const QString AttrRadius;
extern const QString AttrRadius1;
extern const QString AttrRadius2;
extern const QString AttrAngle;
extern const QString AttrAngle1;
extern const QString AttrAngle2;
extern const QString AttrRotationAngle;
extern const QString AttrLength1;
extern const QString AttrLength2;
extern const QString AttrP1Line;
extern const QString AttrP2Line;
extern const QString AttrP1Line1;
extern const QString AttrP2Line1;
extern const QString AttrP1Line2;
extern const QString AttrP2Line2;
extern const QString AttrPShoulder;
extern const QString AttrPoint1;
extern const QString AttrPoint2;
extern const QString AttrPoint3;
extern const QString AttrPoint4;
extern const QString AttrKAsm1;// TODO. Delete if minimal supported version is 0.2.7
extern const QString AttrKAsm2;// TODO. Delete if minimal supported version is 0.2.7
extern const QString AttrKCurve;// TODO. Delete if minimal supported version is 0.2.7
extern const QString AttrDuplicate;
extern const QString AttrPathPoint;
extern const QString AttrPSpline;
extern const QString AttrAxisP1;
extern const QString AttrAxisP2;
extern const QString AttrCurve;
extern const QString AttrCurve1;
extern const QString AttrCurve2;
extern const QString AttrLineColor;
extern const QString AttrColor;
extern const QString AttrFirstArc;
extern const QString AttrSecondArc;
extern const QString AttrCrossPoint;
extern const QString AttrVCrossPoint;
extern const QString AttrHCrossPoint;
extern const QString AttrAxisType;
extern const QString AttrC1Center;
extern const QString AttrC2Center;
extern const QString AttrC1Radius;
extern const QString AttrC2Radius;
extern const QString AttrCCenter;
extern const QString AttrTangent;
extern const QString AttrCRadius;
extern const QString AttrArc;
extern const QString AttrSuffix;
extern const QString AttrIdObject;
extern const QString AttrInLayout;
extern const QString AttrClosed;

extern const QString TypeLineNone;
extern const QString TypeLineLine;
extern const QString TypeLineDashLine;
extern const QString TypeLineDotLine;
extern const QString TypeLineDashDotLine;
extern const QString TypeLineDashDotDotLine;

extern const QString ColorBlack;
extern const QString ColorGreen;
extern const QString ColorBlue;
extern const QString ColorDarkRed;
extern const QString ColorDarkGreen;
extern const QString ColorDarkBlue;
extern const QString ColorYellow;
extern const QString ColorLightSalmon;
extern const QString ColorGoldenRod;
extern const QString ColorOrange;
extern const QString ColorDeepPink;
extern const QString ColorViolet;
extern const QString ColorDarkViolet;
extern const QString ColorMediumSeaGreen;
extern const QString ColorLime;
extern const QString ColorDeepSkyBlue;
extern const QString ColorCornFlowerBlue;


// variables name
// Hacks for avoiding the linker error "undefined reference to"
#define SPL_ "Spl_"
#define ARC_ "Arc_"
#define EARC_ "ElArc_"

extern const QString measurement_;
extern const QString increment_;
extern const QString line_;
extern const QString angleLine_;
extern const QString arc_;
extern const QString spl_;
extern const QString splPath;
extern const QString radius_V;
extern const QString radiusArc_;
extern const QString angle1_V;
extern const QString angle2_V;
extern const QString c1Length_V;
extern const QString c2Length_V;
extern const QString c1LengthSpl_;
extern const QString c2LengthSpl_;
extern const QString c1LengthSplPath;
extern const QString c2LengthSplPath;
extern const QString angle1Arc_;
extern const QString angle2Arc_;
extern const QString angle1Spl_;
extern const QString angle2Spl_;
extern const QString angle1SplPath;
extern const QString angle2SplPath;
extern const QString seg_;
extern const QString currentLength;
extern const QString currentSeamAllowance;

extern const QStringList builInVariables;

#endif // IFCDEF_H

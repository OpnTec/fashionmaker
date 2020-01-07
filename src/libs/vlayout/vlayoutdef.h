/************************************************************************
 **
 **  @file   vlayoutdef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#ifndef VLAYOUTDEF_H
#define VLAYOUTDEF_H

#include <QSize>
#include <QTransform>
#include <ciso646>

#include "../vmisc/typedef.h"

enum class LayoutExportFormats : qint8
{
    SVG = 0,
    PDF = 1,
    PNG = 2,
    OBJ = 3,              /* Wavefront OBJ*/
    PS  = 4,
    EPS = 5,
    DXF_AC1006_Flat = 6,  /* R10. */
    DXF_AC1009_Flat = 7,  /* R11 & R12. */
    DXF_AC1012_Flat = 8,  /* R13. */
    DXF_AC1014_Flat = 9,  /* R14. */
    DXF_AC1015_Flat = 10, /* ACAD 2000. */
    DXF_AC1018_Flat = 11, /* ACAD 2004. */
    DXF_AC1021_Flat = 12, /* ACAD 2007. */
    DXF_AC1024_Flat = 13, /* ACAD 2010. */
    DXF_AC1027_Flat = 14, /* ACAD 2013. */
    DXF_AC1006_AAMA = 15, /* R10. */
    DXF_AC1009_AAMA = 16, /* R11 & R12. */
    DXF_AC1012_AAMA = 17, /* R13. */
    DXF_AC1014_AAMA = 18, /* R14. */
    DXF_AC1015_AAMA = 19, /* ACAD 2000. */
    DXF_AC1018_AAMA = 20, /* ACAD 2004. */
    DXF_AC1021_AAMA = 21, /* ACAD 2007. */
    DXF_AC1024_AAMA = 22, /* ACAD 2010. */
    DXF_AC1027_AAMA = 23, /* ACAD 2013. */
    DXF_AC1006_ASTM = 24, /* R10. */
    DXF_AC1009_ASTM = 25, /* R11 & R12. */
    DXF_AC1012_ASTM = 26, /* R13. */
    DXF_AC1014_ASTM = 27, /* R14. */
    DXF_AC1015_ASTM = 28, /* ACAD 2000. */
    DXF_AC1018_ASTM = 29, /* ACAD 2004. */
    DXF_AC1021_ASTM = 30, /* ACAD 2007. */
    DXF_AC1024_ASTM = 31, /* ACAD 2010. */
    DXF_AC1027_ASTM = 32, /* ACAD 2013. */
    PDFTiled = 33,
    NC = 34,              /*G-code. Reserved for future*/
    COUNT                 /*Use only for validation*/
};

enum class LayoutErrors : qint8
{
    NoError,
    PrepareLayoutError,
    ProcessStoped,
    EmptyPaperError,
    Timeout,
    TerminatedByException
};

enum class BestFrom : qint8
{
    Rotation = 0,
    Combine = 1
};

struct VBestSquareResData
{
    QSizeF        bestSize{INT_MAX, INT_MAX};
    // cppcheck-suppress unusedStructMember
    int           globalI{0}; // Edge of global contour
    // cppcheck-suppress unusedStructMember
    int           detJ{0}; // Edge of detail
    QTransform    resMatrix{}; // Matrix for rotation and translation detail
    // cppcheck-suppress unusedStructMember
    bool          resMirror{false};
    BestFrom      type{BestFrom::Rotation};
    // cppcheck-suppress unusedStructMember
    qreal         depthPosition{INT_MAX};
    // cppcheck-suppress unusedStructMember
    qreal         sidePosition{0};
};

struct VCachedPositions
{
    QRectF boundingRect{};
    QPainterPath layoutAllowancePath{};
};

#endif // VLAYOUTDEF_H

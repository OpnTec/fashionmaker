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

enum class LayoutErrors : char
{
    NoError,
    PrepareLayoutError,
    ProcessStoped,
    EmptyPaperError,
    Timeout,
    TerminatedByException
};

enum class BestFrom : char
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

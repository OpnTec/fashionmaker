/************************************************************************
 **
 **  @file   vlayoutdef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#ifndef VLAYOUTDEF_H
#define VLAYOUTDEF_H

enum class EquidistantType : char { OpenEquidistant, CloseEquidistant };

enum class LayoutErrors : char
{
    NoError,
    PrepareLayoutError,
    PaperSizeError,
    ProcessStoped,
    EmptyPaperError
};

enum class BestFrom : char
{
    Rotation = 0,
    Combine = 1
};

//#define LAYOUT_DEBUG // Enable debug mode

#ifdef LAYOUT_DEBUG
#   define SHOW_VERTICES // Show contour vertices
#   define SHOW_DIRECTION   // Show contour direction
#   define ARRANGED_DETAILS // Show already arranged details
//#   define SHOW_CANDIDATE
//#   define SHOW_ROTATION
//#   define SHOW_COMBINE
//#   define SHOW_MIRROR
//#   define SHOW_CANDIDATE_BEST
#   define SHOW_BEST
#endif//LAYOUT_DEBUG

#endif // VLAYOUTDEF_H

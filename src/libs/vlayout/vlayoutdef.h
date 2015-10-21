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

#include <ciso646> //for msvc

enum class EquidistantType : char { OpenEquidistant, CloseEquidistant };

enum class LayoutErrors : char
{
    NoError,
    PrepareLayoutError,
    ProcessStoped,
    EmptyPaperError
};

enum class BestFrom : char
{
    Rotation = 0,
    Combine = 1
};

/* Warning! Debugging doesn't work stable in debug mode. If you need big allocation use release mode. Or disable
 * Address Sanitizer. See page https://bitbucket.org/dismine/valentina/wiki/developers/Address_Sanitizer
 */
//#define LAYOUT_DEBUG // Enable debug mode

// This block help rule debug mode. Don't turn all options at the same time!
#ifdef LAYOUT_DEBUG
// Nice looking
#   define SHOW_VERTICES         // Show contour vertices
#   define SHOW_DIRECTION        // Show contour direction
#   define ARRANGED_DETAILS      // Show already arranged details
//#   define SHOW_SHEET            // Show sheet rect

// Debugging
#   define SHOW_CANDIDATE        // Show each position
//#   define SHOW_ROTATION         // For each position show rotation part
//#   define SHOW_COMBINE          // For each position show edge combine part
//#   define SHOW_MIRROR           // For each position show mirror part
//#   define SHOW_CANDIDATE_BEST   // For only correct positions that pass checks
#   define SHOW_BEST             // Show only best position for workpiece
#endif//LAYOUT_DEBUG

#endif // VLAYOUTDEF_H

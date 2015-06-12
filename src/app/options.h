/************************************************************************
 **
 **  @file   options.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses.
 **
 *************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include "../libs/ifc/ifcdef.h"
#include "../libs/vgeometry/vgeometrydef.h"
#include "../libs/qmuparser/qmudef.h"
#include "../../utils/def.h"

#ifdef Q_OS_WIN32
#   if defined( Q_CC_MSVC )        // MSVC USED
#       ifndef NOMINMAX
#           define NOMINMAX        // DISABLE min/max MACROS
#       endif
#   endif /*Q_CC_MSVC*/
#   include <windows.h>

#endif /*Q_OS_WIN32*/

class QString;
class QStringList;

#define SceneSize 50000
#define DefPointRadius 1.5//mm

extern const QString degreeSymbol;

extern const QString cursorArrowOpenHand;
extern const QString cursorArrowCloseHand;

enum class SceneObject : char { Point, Line, Spline, Arc, SplinePath, Detail, Unknown };


enum class Source : char { FromGui, FromFile, FromTool };
enum class Contour : char { OpenContour, CloseContour };

#endif // OPTIONS_H

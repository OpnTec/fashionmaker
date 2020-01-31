/************************************************************************
 **
 **  @file   typedef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 3, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <QtGlobal>

using vidtype = quint32;

static const vidtype null_id = 0;

#define NULL_ID null_id//use this value for initialization variables that keeps id values. 0 mean uknown id value.
#define NULL_ID_STR QChar('0')

#endif // TYPEDEF_H

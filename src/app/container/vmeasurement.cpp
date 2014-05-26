/************************************************************************
 **
 **  @file   vstandardtablecell.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vmeasurement.h"

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::VMeasurement()
    :base(0), ksize(50.0), kheight(176.0), gui_text(QString()), number(QString()), _tagName(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::VMeasurement(const qreal &base, const qreal &ksize, const qreal &kgrowth,
                           const QString &gui_text, const QString &number, const QString &tagName)
    :base(base), ksize(ksize), kheight(kgrowth), gui_text(gui_text), number(number), _tagName(tagName)
{}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::VMeasurement(const qreal &base, const QString &gui_text, const QString &number, const QString &tagName)
    :base(base), ksize(50.0), kheight(176.0), gui_text(gui_text), number(number), _tagName(tagName)
{}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurement::GetValue() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurement::GetValue(const qreal &size, const qreal &height) const
{
    const qreal k_size    = ( size - 50.0 ) / 2.0;
    const qreal k_height  = ( height - 176.0 ) / 6.0;
    return base + k_size * ksize + k_height * kheight;
}

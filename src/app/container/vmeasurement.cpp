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
/**
 * @brief VMeasurement create empty measurement
 */
VMeasurement::VMeasurement()
    :base(0), ksize(50.0), kheight(176.0), gui_text(QString()), description(QString()), _tagName(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMeasurement create measurement for standard table
 * @param base value in base size and growth
 * @param ksize increment in sizes
 * @param kheight increment in heights
 * @param gui_text shor tooltip for user
 * @param description measurement full description
 * @param tagName measurement's tag name in file
 */
VMeasurement::VMeasurement(const qreal &base, const qreal &ksize, const qreal &kheight,
                           const QString &gui_text, const QString &description, const QString &tagName)
    :base(base), ksize(ksize), kheight(kheight), gui_text(gui_text), description(description), _tagName(tagName)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMeasurement create measurement for individual table
 * @param base value in base size and growth
 * @param gui_text shor tooltip for user
 * @param description measurement full description
 * @param tagName measurement's tag name in file
 */
VMeasurement::VMeasurement(const qreal &base, const QString &gui_text, const QString &description,
                           const QString &tagName)
    :base(base), ksize(50.0), kheight(176.0), gui_text(gui_text), description(description), _tagName(tagName)
{}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::VMeasurement(const VMeasurement &m)
    :base(m.GetBase()), ksize(m.GetKsize()), kheight(m.GetKheight()), gui_text(m.GetGuiText()),
      description(m.GetDescription()), _tagName(m.TagName())
{}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement &VMeasurement::operator=(const VMeasurement &m)
{
    this->base = m.GetBase();
    this->ksize = m.GetKsize();
    this->kheight = m.GetKheight();
    this->gui_text = m.GetGuiText();
    this->description = m.GetDescription();
    this->_tagName = m.TagName();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurement::GetValue(const qreal &size, const qreal &height) const
{
    const qreal k_size    = ( size - 50.0 ) / 2.0;
    const qreal k_height  = ( height - 176.0 ) / 6.0;
    return base + k_size * ksize + k_height * kheight;
}

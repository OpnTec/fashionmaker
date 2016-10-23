/************************************************************************
 **
 **  @file   vgraphicsfillitem.cpp
 **  @author Bojan Kverh
 **  @date   October 16, 2016
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

#include "vgraphicsfillitem.h"

//---------------------------------------------------------------------------------------------------------------------
VGraphicsFillItem::VGraphicsFillItem()
    :QGraphicsPathItem()
{}

//---------------------------------------------------------------------------------------------------------------------
VGraphicsFillItem::~VGraphicsFillItem()
{}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsFillItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setBrush(painter->pen().color());
    painter->drawPath(path());
    painter->restore();
}

//---------------------------------------------------------------------------------------------------------------------


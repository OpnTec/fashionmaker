/************************************************************************
 **
 **  @file   vgraphicsfillitem.h
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

#ifndef VGRAPHICSFILLITEM_H
#define VGRAPHICSFILLITEM_H

#include <QGraphicsPathItem>
#include <QPainter>

class VGraphicsFillItem : public QGraphicsPathItem
{
public:
    /**
     * @brief VGraphicsFillItem Constructor
     */
    explicit VGraphicsFillItem(QGraphicsItem *parent = nullptr);
    /**
     * @brief ~VGraphicsFillItem Destructor
     */
    ~VGraphicsFillItem();
    /**
     * @brief paint Paints the item, filling the inside surface
     * @param painter pointer to the painter object
     * @param option unused
     * @param widget unused
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // VGRAPHICSFILLITEM_H

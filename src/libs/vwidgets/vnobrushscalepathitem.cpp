/************************************************************************
 **
 **  @file   vnobrushscalepathitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vnobrushscalepathitem.h"

#include <QBrush>
#include <QMatrix>
#include <QPainter>

class QGraphicsItem;
class QStyleOptionGraphicsItem;
class QWidget;

//---------------------------------------------------------------------------------------------------------------------
VNoBrushScalePathItem::VNoBrushScalePathItem(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void VNoBrushScalePathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*
     * how to avoid transformation of QBrush
     * http://www.qtforum.org/article/23942/how-to-avoid-transformation-of-qbrush-texture.html
     * ?s=b4ba78dd6758da78fe395d8f6bb7512511a0833e#post84983
     *
     * Not Scaling QBrush style fro qgraphics view item
     * http://www.qtcentre.org/archive/index.php/t-13950.html
     *
     * You'll have to scale the brush down. The QStyleOptionGraphicsItem option passed in paint() will give you the
     * transform being used, and you can set a matrix on a QBrush. Put the two together and you can scale the brush
     * opposite of the item.
     */
    QBrush brush = this->brush();
    brush.setMatrix(painter->combinedMatrix().inverted());
    this->setBrush(brush);
    QGraphicsPathItem::paint(painter, option, widget);
}

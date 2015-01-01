/************************************************************************
 **
 **  @file   vitem.cpp
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

#include "vitem.h"

#include <QGraphicsScene>
#include <QDebug>
#include "../core/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VItem constructor.
 * @param path detail path.
 * @param numInList index in list of details.
 * @param parent parent object.
 */
VItem::VItem (const QPainterPath & path, int numInList, QGraphicsItem * parent )
    :QGraphicsPathItem ( path, parent ), numInOutList(numInList), paper(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VItem default constructor.
 */
VItem::VItem():numInOutList(0), paper(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VItem constructor.
 * @param numInList index in list of details.
 * @param parent parent object.
 */
VItem::VItem(int numInList, QGraphicsItem *parent):QGraphicsPathItem (parent), numInOutList(numInList),
    paper(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief checkItemChange check item change. If detail create colission or moved out paper emit signal.
 */
void VItem::checkItemChange()
{
    QRectF rect;
    if (paper == nullptr)
    {
        qDebug()<<"Don't set paper for detail!!!!";
        rect = this->scene()->sceneRect();
    }
    else
    {
        rect = paper->sceneBoundingRect();
    }
    QRectF myrect = sceneBoundingRect();
    if ( rect.contains( myrect )==true )
    {
        setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
        emit itemOut( numInOutList, false );
    }
    else
    {
        setPen(QPen(Qt::red, qApp->toPixel(qApp->widthMainLine())));
        emit itemOut( numInOutList, true );
    }
    QList<QGraphicsItem *> list = QGraphicsItem::collidingItems ();
    if ( list.size() - 2 > 0 )
    {
        list.append( this );
        setPen(QPen(Qt::red, qApp->toPixel(qApp->widthMainLine())));
        emit itemColliding( list, 1 );//Detail intersect with other details.
    }
    else
    {
        QList<QGraphicsItem *> itemList;
        itemList.append( this );
        emit itemColliding( itemList, 0 );//Detail doesn't intersect more with other details.
    }
    //qDebug()<<"list="<<list.size();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VItem::itemChange( GraphicsItemChange change, const QVariant &value )
{
    if ( change == QGraphicsItem::ItemPositionHasChanged && scene() )
    {
        checkItemChange();
        return QGraphicsPathItem::itemChange( change, value );
    }
    if ( change == QGraphicsItem::ItemParentHasChanged && scene() )
    {
        checkItemChange();
        return QGraphicsPathItem::itemChange( change, value );
    }
    return QGraphicsPathItem::itemChange( change, value );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthChanged handle signal change paper length.
 */
void VItem::LengthChanged()
{
    checkItemChange();
}

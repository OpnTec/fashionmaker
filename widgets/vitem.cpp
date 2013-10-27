/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vitem.h"

VItem::VItem (const QPainterPath & path, int numInList, QGraphicsItem * parent ):
    QGraphicsPathItem ( path, parent ), numInOutList(numInList){
}

void VItem::checkItemChange(){
    QRectF rect = parentItem()->sceneBoundingRect();
    QRectF myrect = sceneBoundingRect();
    if( rect.contains( myrect )==true ){
        qDebug()<<"Не виходить за рамки листа";
        setPen(QPen(Qt::black, widthMainLine));
        emit itemOut( numInOutList, false );
    } else {
        qDebug()<<"Виходить за рамки листа";
        setPen(QPen(Qt::red, widthMainLine));
        emit itemOut( numInOutList, true );
    }
    QList<QGraphicsItem *> list = QGraphicsItem::collidingItems ();
    if( list.size() - 2 > 0 ){
        list.append( this );
        setPen(QPen(Qt::red, widthMainLine));
        qDebug()<<"Деталь перетинається з іншими деталями "<<numInOutList;
        emit itemColliding( list, 1 );//Деталь перетинається з іншими деталями.
    } else {
        QList<QGraphicsItem *> itemList;
        itemList.append( this );
        qDebug()<<"Деталь більше не перетинається з іншими деталями "<<numInOutList;
        emit itemColliding( itemList, 0 );//Деталь більше не перетинається з іншими деталями.
    }
    //qDebug()<<"list="<<list.size();
}

QVariant VItem::itemChange( GraphicsItemChange change, const QVariant &value ){
    if ( change == QGraphicsItem::ItemPositionHasChanged && scene() ) {
        checkItemChange();
        return QGraphicsPathItem::itemChange( change, value );
    }
    if ( change == QGraphicsItem::ItemParentHasChanged && scene() ) {
        checkItemChange();
        return QGraphicsPathItem::itemChange( change, value );
    }
    return QGraphicsPathItem::itemChange( change, value );
}

void VItem::LengthChanged(){
    checkItemChange();
}

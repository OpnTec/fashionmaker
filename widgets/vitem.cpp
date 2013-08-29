#include "vitem.h"
#include <QGraphicsScene>
#include <QDebug>
#include "../options.h"

VItem::VItem():numInOutList(0){
}

VItem::VItem ( int numInList, QGraphicsItem * parent ):QGraphicsPathItem ( parent ),
    numInOutList(numInList){
}

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

void VItem::SetIndexInList( qint32 index ){
    numInOutList = index;
}

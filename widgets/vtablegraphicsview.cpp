#include "vtablegraphicsview.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScrollBar>

VTableGraphicsView::VTableGraphicsView(QGraphicsScene* pScene, QWidget *parent) :
    QGraphicsView(pScene, parent){
    QGraphicsView::setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    connect(pScene, &QGraphicsScene::selectionChanged, this, &VTableGraphicsView::selectionChanged);
}

void VTableGraphicsView::selectionChanged(){
    QList<QGraphicsItem *> listSelectedItems = scene()->selectedItems();
    if( listSelectedItems.isEmpty() == true ){
        qDebug() << "деталь не знайдено";
        emit itemChect(true);
    } else {
        qDebug() << "деталь знайдено";
        emit itemChect(false);
    }
}

void VTableGraphicsView::rotateItems(){
    rotateIt();
}

void VTableGraphicsView::MirrorItem(){
    QList<QGraphicsItem *> list = scene()->selectedItems();
    if(list.size()>0){
        for( qint32 i = 0; i < list.count(); ++i ){
            QRectF itemRectOld = list.at(i)->sceneBoundingRect();
            //Get the current transform
            QTransform transform(list.at(i)->transform());

            qreal m11 = transform.m11();    // Horizontal scaling
            qreal m12 = transform.m12();    // Vertical shearing
            qreal m13 = transform.m13();    // Horizontal Projection
            qreal m21 = transform.m21();    // Horizontal shearing
            qreal m22 = transform.m22();    // vertical scaling
            qreal m23 = transform.m23();    // Vertical Projection
            qreal m31 = transform.m31();    // Horizontal Position (DX)
            qreal m32 = transform.m32();    // Vertical Position (DY)
            qreal m33 = transform.m33();    // Addtional Projection Factor

            // Horizontal flip
            m11 = -m11;

            // Write back to the matrix
            transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

            // Set the items transformation
            list.at(i)->setTransform(transform);
            QRectF itemRectNew = list.at(i)->sceneBoundingRect();
            qreal dx, dy;
            dx = itemRectOld.center().x()-itemRectNew.center().x();
            dy = itemRectOld.center().y()-itemRectNew.center().y();
            list.at(i)->moveBy(dx, dy);
        }
    }
}

void VTableGraphicsView::ZoomIn(){
    scale(1.1,1.1);
}

void VTableGraphicsView::ZoomOut(){
    scale(1/1.1,1/1.1);
}

void VTableGraphicsView::wheelEvent(QWheelEvent *event){
    if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier){
        // Если нажата клавиша CTRL этот код выполнится
        if ((event->delta())>0){
            ZoomIn();
        } else if ((event->delta())<0){
            ZoomOut();
        }
    } else {
       verticalScrollBar()->setValue(verticalScrollBar()->value()-event->delta());
    }
}

void VTableGraphicsView::mousePressEvent(QMouseEvent *mousePress){
    if(mousePress->button() & Qt::LeftButton){
        switch(QGuiApplication::keyboardModifiers()){
        case Qt::ControlModifier:
            QGraphicsView::setDragMode(QGraphicsView::ScrollHandDrag);
            QGraphicsView::mousePressEvent(mousePress);
            break;
        default:
            QGraphicsView::mousePressEvent(mousePress);
            break;
        }
    }
}

void VTableGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent ( event );
    QGraphicsView::setDragMode( QGraphicsView::RubberBandDrag );
}

void VTableGraphicsView::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Space:
        rotateIt();
        break;
    case Qt::Key_Left:
        MoveItem(VTableGraphicsView::Left);
        break;
    case Qt::Key_Right:
        MoveItem(VTableGraphicsView::Right);
        break;
    case Qt::Key_Up:
        MoveItem(VTableGraphicsView::Up);
        break;
    case Qt::Key_Down:
        MoveItem(VTableGraphicsView::Down);
        break;
    }
    QGraphicsView::keyPressEvent ( event );
}

void VTableGraphicsView::rotateIt(){
    QList<QGraphicsItem *> list = scene()->selectedItems();
    if(list.size()>0){
        for( qint32 i = 0; i < list.count(); ++i ){
            list.at(i)->setTransformOriginPoint(list.at(i)->boundingRect().center());
            list.at(i)->setRotation(list.at(i)->rotation() + 180);
        }
    }
}

void VTableGraphicsView::MoveItem(VTableGraphicsView::typeMove_e move){
    qreal dx = 0, dy = 0;
    switch(move){
    case VTableGraphicsView::Left:
        dx = -3;
        dy = 0;
        break;
    case VTableGraphicsView::Right:
        dx = 3;
        dy = 0;
        break;
    case VTableGraphicsView::Up:
        dx = 0;
        dy = -3;
        break;
    case VTableGraphicsView::Down:
        dx = 0;
        dy = 3;
        break;
    }
    QList<QGraphicsItem *> listSelectedItems = scene()->selectedItems();
    if(listSelectedItems.size()>0){
        for( qint32 i = 0; i < listSelectedItems.count(); ++i ){
            listSelectedItems.at(i)->moveBy(dx, dy);
        }
    }
}

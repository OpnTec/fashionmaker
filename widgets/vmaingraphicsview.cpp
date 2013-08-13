#include "vmaingraphicsview.h"
#include <QApplication>
#include <QWheelEvent>
#include <QScrollBar>

VMainGraphicsView::VMainGraphicsView(QWidget *parent) :
    QGraphicsView(parent){
    QGraphicsView::setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void VMainGraphicsView::wheelEvent(QWheelEvent *event){
    if (QApplication::keyboardModifiers() == Qt::ControlModifier){
        // Если нажата клавиша CTRL этот код выполнится
        if ((event->delta())>0){
            ZoomIn();
        } else if ((event->delta())<0){
            ZoomOut();
        }
    } else {
       verticalScrollBar()->setValue(verticalScrollBar()->value() - event->delta());
    }
}

void VMainGraphicsView::ZoomIn(){
    scale(1.1,1.1);
}

void VMainGraphicsView::ZoomOut(){
    scale(1/1.1,1/1.1);
}

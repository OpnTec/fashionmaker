#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vmaingraphicsview.h"
#include <QApplication>
#include <QWheelEvent>
#include <QScrollBar>
#pragma GCC diagnostic pop

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

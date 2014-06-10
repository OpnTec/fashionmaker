/************************************************************************
 **
 **  @file   vtablegraphicsview.cpp
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

#include "vtablegraphicsview.h"

#include <QDebug>
#include <QWheelEvent>
#include "../options.h"

//---------------------------------------------------------------------------------------------------------------------
VTableGraphicsView::VTableGraphicsView(QGraphicsScene* pScene, QWidget *parent)
    :QGraphicsView(pScene, parent)
{
    QGraphicsView::setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    connect(pScene, &QGraphicsScene::selectionChanged, this, &VTableGraphicsView::selectionChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::selectionChanged()
{
    QList<QGraphicsItem *> listSelectedItems = scene()->selectedItems();
    if ( listSelectedItems.isEmpty() == true )
    {
        qDebug() << tr("can't find detail");
        emit itemChect(true);
    }
    else
    {
        qDebug() << tr("detail found");
        emit itemChect(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::MirrorItem()
{
    QList<QGraphicsItem *> list = scene()->selectedItems();
    if (list.size()>0)
    {
        for ( qint32 i = 0; i < list.count(); ++i )
        {
            QGraphicsItem *item = list.at(i);
            SCASSERT(item != nullptr);
            QRectF itemRectOld = item->sceneBoundingRect();
            //Get the current transform
            QTransform transform(item->transform());

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
            item->setTransform(transform);
            QRectF itemRectNew = item->sceneBoundingRect();
            qreal dx, dy;
            dx = itemRectOld.center().x()-itemRectNew.center().x();
            dy = itemRectOld.center().y()-itemRectNew.center().y();
            item->moveBy(dx, dy);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        // If was pressed button CTRL this code will execute
        if ((event->delta())>0)
        {
            ZoomIn();
        }
        else if ((event->delta())<0)
        {
            ZoomOut();
        }
    }
    else
    {
       verticalScrollBar()->setValue(verticalScrollBar()->value()-event->delta());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::mousePressEvent(QMouseEvent *mousePress)
{
    if (mousePress->button() & Qt::LeftButton)
    {
        switch (QGuiApplication::keyboardModifiers())
        {
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

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent ( event );
    QGraphicsView::setDragMode( QGraphicsView::RubberBandDrag );
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
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
        default:
            break;
    }
    QGraphicsView::keyPressEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::rotateIt()
{
    QList<QGraphicsItem *> list = scene()->selectedItems();
    if (list.size()>0)
    {
        for ( qint32 i = 0; i < list.count(); ++i )
        {
            QGraphicsItem *item = list.at(i);
            SCASSERT(item != nullptr);
            item->setTransformOriginPoint(item->boundingRect().center());
            item->setRotation(item->rotation() + 90);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableGraphicsView::MoveItem(VTableGraphicsView::typeMove_e move)
{
    qreal dx = 0, dy = 0;
    switch (move)
    {
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
        default:
            break;
    }
    QList<QGraphicsItem *> listSelectedItems = scene()->selectedItems();
    if (listSelectedItems.size()>0)
    {
        for ( qint32 i = 0; i < listSelectedItems.count(); ++i )
        {
            listSelectedItems.at(i)->moveBy(dx, dy);
        }
    }
}

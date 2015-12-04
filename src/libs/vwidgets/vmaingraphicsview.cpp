/************************************************************************
 **
 **  @file   vmaingraphicsview.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vmaingraphicsview.h"

#include <QTimeLine>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>
#include "vsimplepoint.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <qmath.h>

//---------------------------------------------------------------------------------------------------------------------
GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
  : QObject(view), _view(view), _modifiers(Qt::ControlModifier), _zoom_factor_base(1.0015),
    target_scene_pos(QPointF()), target_viewport_pos(QPointF()), anim(nullptr), _numScheduledScalings(0)
{
  _view->viewport()->installEventFilter(this);
  _view->setMouseTracking(true);
  anim = new QTimeLine(300, this);
  anim->setUpdateInterval(20);
  connect(anim, &QTimeLine::valueChanged, this, &GraphicsViewZoom::scrollingTime, Qt::UniqueConnection);
  connect(anim, &QTimeLine::finished, this, &GraphicsViewZoom::animFinished, Qt::UniqueConnection);
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::gentle_zoom(double factor)
{
  _view->scale(factor, factor);
  if (factor < 1)
  {
      // Because QGraphicsView centers the picture when it's smaller than the view. And QGraphicsView's scrolls
      // boundaries don't allow to put any picture point at any viewport position we will provide fictive scene size.
      // Temporary and bigger than view, scene size will help position an image under cursor.
      FictiveSceneRect(_view->scene(), _view);
  }
  _view->centerOn(target_scene_pos);
  QPointF delta_viewport_pos = target_viewport_pos - QPointF(_view->viewport()->width() / 2.0,
                                                             _view->viewport()->height() / 2.0);
  QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
  _view->centerOn(_view->mapToScene(viewport_center.toPoint()));
  // In the end we just set correct scene size
  VMainGraphicsView::NewSceneRect(_view->scene(), _view);
  emit zoomed();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void GraphicsViewZoom::set_modifiers(Qt::KeyboardModifiers modifiers)
{
  _modifiers = modifiers;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void GraphicsViewZoom::set_zoom_factor_base(double value)
{
    _zoom_factor_base = value;
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::scrollingTime(qreal x)
{
    Q_UNUSED(x);
    qreal factor = 1.0;
    if (_numScheduledScalings < 0)
    {
        factor = factor*13.8;
    }
    else
    {
        factor = factor*-13.8;
    }
    _view->verticalScrollBar()->setValue(qRound(_view->verticalScrollBar()->value() + factor));
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::animFinished()
{
    if (_numScheduledScalings > 0)
    {
        _numScheduledScalings--;
    }
    else
    {
        _numScheduledScalings++;
    }
    anim->stop();

    /*
     * In moust cases cursor position on view doesn't change, but for scene after scrolling position will be different.
     * We are goint to check changes and save new value.
     * If don't do that we will zoom using old value cursor position on scene. It is not what we expect.
     * Almoust the same we do in method GraphicsViewZoom::eventFilter.
     */
    const QPoint pos = _view->mapFromGlobal(QCursor::pos());
    const QPointF delta = target_scene_pos - _view->mapToScene(pos);
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
    {
        target_viewport_pos = pos;
        target_scene_pos = _view->mapToScene(pos);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    /*
     * Here we are saving cursor position on view and scene.
     * This data need for gentle_zoom().
     * Almoust the same we do in method GraphicsViewZoom::animFinished.
     */
    QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
    QPointF delta = target_viewport_pos - mouse_event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
    {
      target_viewport_pos = mouse_event->pos();
      target_scene_pos = _view->mapToScene(mouse_event->pos());
    }
  }
  else if (event->type() == QEvent::Wheel)
  {
    QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == _modifiers)
    {
      if (wheel_event->orientation() == Qt::Vertical)
      {
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(_zoom_factor_base, angle);
        gentle_zoom(factor);
        return true;
      }
    }
    else
    {
        int numSteps = wheel_event->delta() / 8 / 15;  // see QWheelEvent documentation

        _numScheduledScalings += numSteps;
        if (_numScheduledScalings * numSteps < 0)
        {  // if user moved the wheel in another direction, we reset
            _numScheduledScalings = numSteps;       // previously scheduled scalings
        }

        if (anim->state() != QTimeLine::Running)
        {
            anim->start();
        }
        return true;
    }
  }
  Q_UNUSED(object)
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::FictiveSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    SCASSERT(sc != nullptr);
    SCASSERT(view != nullptr);

    //Calculate view rect
    //to receive the currently visible area, map the widgets bounds to the scene
    const QPointF a = view->mapToScene(0, 0 );
    const QPointF b = view->mapToScene(view->viewport()->width(), view->viewport()->height());
    QRectF viewRect = QRectF( a, b );

    //Scale view
    QLineF topLeftRay(viewRect.center(), viewRect.topLeft());
    topLeftRay.setLength(topLeftRay.length()*2);

    QLineF bottomRightRay(viewRect.center(), viewRect.bottomRight());
    bottomRightRay.setLength(bottomRightRay.length()*2);

    viewRect = QRectF(topLeftRay.p2(), bottomRightRay.p2());

    //Calculate scene rect
    const QRectF sceneRect = sc->sceneRect();

    //Unite two rects
    const QRectF newRect = sceneRect.united(viewRect);

    sc->setSceneRect(newRect);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsView constructor.
 * @param parent parent object.
 */
VMainGraphicsView::VMainGraphicsView(QWidget *parent)
    :QGraphicsView(parent), zoom(nullptr), showToolOptions(true)
{
    zoom = new GraphicsViewZoom(this);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setInteractive(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomIn()
{
    scale(1.1, 1.1);
    VMainGraphicsView::NewSceneRect(this->scene(), this);
    emit NewFactor(1.1);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomOut()
{
    scale(1.0/1.1, 1.0/1.1);
    VMainGraphicsView::NewSceneRect(this->scene(), this);
    emit NewFactor(1.0/1.1);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomOriginal()
{
    QTransform trans = this->transform();
    trans.setMatrix(1.0, trans.m12(), trans.m13(), trans.m21(), 1.0, trans.m23(), trans.m31(), trans.m32(),
                    trans.m33());
    this->setTransform(trans);
    VMainGraphicsView::NewSceneRect(this->scene(), this);
    emit NewFactor(1.0);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomFitBest()
{
    QRectF rect = this->scene()->itemsBoundingRect();

    if (rect.isEmpty())
    {
        return;
    }

    this->fitInView(rect, Qt::KeepAspectRatio);
    VMainGraphicsView::NewSceneRect(this->scene(), this);
    emit NewFactor(this->transform().m11());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mousePressEvent handle mouse press events.
 * @param mousePress mouse press event.
 */
void VMainGraphicsView::mousePressEvent(QMouseEvent *mousePress)
{
    if (mousePress->button() & Qt::LeftButton)
    {
        switch (QGuiApplication::keyboardModifiers())
        {
            case Qt::ControlModifier:
                QGraphicsView::setDragMode(QGraphicsView::ScrollHandDrag);
                break;
            case Qt::NoModifier:
                if (showToolOptions)
                {
                    QList<QGraphicsItem *> list = items(mousePress->pos());
                    if (list.size() == 0)
                    {
                        emit itemClicked(nullptr);
                        break;
                    }
                    for (int i = 0; i < list.size(); ++i)
                    {
                        if (this->scene()->items().contains(list.at(i)))
                        {
                            if (list.at(i)->type() <= VSimplePoint::Type &&
                                list.at(i)->type() > QGraphicsItem::UserType)
                            {
                                emit itemClicked(list.at(i));
                                break;
                            }
                            else
                            {
                                emit itemClicked(nullptr);
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    QGraphicsView::mousePressEvent(mousePress);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VMainGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent ( event );
    QGraphicsView::setDragMode( QGraphicsView::RubberBandDrag );
    if (event->button() == Qt::LeftButton)
    {
        emit MouseRelease();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::setShowToolOptions(bool value)
{
    showToolOptions = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewSceneRect calculate scene rect what contains all items and doesn't less that size of scene view.
 * @param sc scene.
 * @param view view.
 */
void VMainGraphicsView::NewSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    SCASSERT(sc != nullptr);
    SCASSERT(view != nullptr);

    //Calculate view rect
    //to receive the currently visible area, map the widgets bounds to the scene
    const QPointF a = view->mapToScene(0, 0 );
    const QPointF b = view->mapToScene(view->width(), view->height());
    const QRectF viewRect = QRectF( a, b );

    //Calculate scene rect
    const QRectF itemsRect = sc->itemsBoundingRect();

    //Unite two rects
    const QRectF newRect = itemsRect.united(viewRect);

    if (newRect != itemsRect)
    {//Update if only need
        sc->setSceneRect(newRect);
    }
}

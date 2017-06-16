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

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QFlags>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QList>
#include <QMessageLogger>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QScrollBar>
#include <QTimeLine>
#include <QTransform>
#include <QWheelEvent>
#include <QWidget>
#include <QDesktopWidget>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "vmaingraphicsscene.h"
#include "vsimplecurve.h"

const int GraphicsViewZoom::duration = 300;
const int GraphicsViewZoom::updateInterval = 40;

const qreal maxSceneSize = ((20.0 * 1000.0) / 25.4) * PrintDPI; // 20 meters in pixels

//---------------------------------------------------------------------------------------------------------------------
GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
  : QObject(view),
    _view(view),
    _modifiers(Qt::ControlModifier),
    _zoom_factor_base(1.0015),
    target_scene_pos(QPointF()),
    target_viewport_pos(QPointF()),
    verticalScrollAnim(new QTimeLine(duration, this)),
    _numScheduledVerticalScrollings(0),
    horizontalScrollAnim(new QTimeLine(duration, this)),
    _numScheduledHorizontalScrollings(0)
{
  _view->viewport()->installEventFilter(this);
  _view->setMouseTracking(true);

  verticalScrollAnim->setUpdateInterval(updateInterval);
  connect(verticalScrollAnim, &QTimeLine::valueChanged, this, &GraphicsViewZoom::VerticalScrollingTime);
  connect(verticalScrollAnim, &QTimeLine::finished, this, &GraphicsViewZoom::animFinished);

  horizontalScrollAnim->setUpdateInterval(updateInterval);
  connect(horizontalScrollAnim, &QTimeLine::valueChanged, this, &GraphicsViewZoom::HorizontalScrollingTime);
  connect(horizontalScrollAnim, &QTimeLine::finished, this, &GraphicsViewZoom::animFinished);
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::gentle_zoom(double factor)
{
    // We need to check current scale factor because in Windows we have an error when we zoom in or zoom out to much.
    // See issue #532: Unexpected error occurs when zoom out image.
    // factor > 1 for zoomIn and factor < 1 for zoomOut.
    const qreal m11 = _view->transform().m11();

    if ((factor > 1 && m11 <= VMainGraphicsView::MaxScale()) || (factor < 1 && m11 >= VMainGraphicsView::MinScale()))
    {
        _view->scale(factor, factor);
        if (factor < 1)
        {
            // Because QGraphicsView centers the picture when it's smaller than the view. And QGraphicsView's scrolls
            // boundaries don't allow to put any picture point at any viewport position we will provide fictive scene
            // size. Temporary and bigger than view, scene size will help position an image under cursor.
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
void GraphicsViewZoom::VerticalScrollingTime(qreal x)
{
    Q_UNUSED(x)
    // Try to adapt scrolling to speed of rotating mouse wheel and scale factor
    // Value of _numScheduledScrollings is too short, so we scale the value

    qreal scroll = static_cast<qreal>(qAbs(_numScheduledVerticalScrollings))*(10. + 10./_view->transform().m22())
            /(static_cast<qreal>(duration)/static_cast<qreal>(updateInterval));

    if (qAbs(scroll) < 1)
    {
        scroll = 1;
    }

    if (_numScheduledVerticalScrollings > 0)
    {
        scroll = scroll * -1;
    }
    _view->verticalScrollBar()->setValue(qRound(_view->verticalScrollBar()->value() + scroll));
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::HorizontalScrollingTime(qreal x)
{
    Q_UNUSED(x)
    // Try to adapt scrolling to speed of rotating mouse wheel and scale factor
    // Value of _numScheduledScrollings is too short, so we scale the value

    qreal scroll = static_cast<qreal>(qAbs(_numScheduledHorizontalScrollings))*(10. + 10./_view->transform().m11())
            /(static_cast<qreal>(duration)/static_cast<qreal>(updateInterval));

    if (qAbs(scroll) < 1)
    {
        scroll = 1;
    }

    if (_numScheduledHorizontalScrollings > 0)
    {
        scroll = scroll * -1;
    }
    _view->horizontalScrollBar()->setValue(qRound(_view->horizontalScrollBar()->value() + scroll));
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::animFinished()
{
    _numScheduledVerticalScrollings = 0;
    verticalScrollAnim->stop();

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
        return false;
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        SCASSERT(wheel_event != nullptr)
        if (QApplication::keyboardModifiers() == _modifiers)
        {
            if (wheel_event->orientation() == Qt::Vertical)
            {
                const double angle = wheel_event->angleDelta().y();
                const double factor = qPow(_zoom_factor_base, angle);
                gentle_zoom(factor);
                return true;
            }
        }
        else
        {
            if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
            {
                return StartHorizontalScrollings(wheel_event);
            }
            else
            {
                return StartVerticalScrollings(wheel_event);
            }
        }
    }

    return QObject::eventFilter(object, event);
}

//---------------------------------------------------------------------------------------------------------------------
void GraphicsViewZoom::FictiveSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    SCASSERT(sc != nullptr)
    SCASSERT(view != nullptr)

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
bool GraphicsViewZoom::StartVerticalScrollings(QWheelEvent *wheel_event)
{
    SCASSERT(wheel_event != nullptr)

    const QPoint numPixels = wheel_event->pixelDelta();
    const QPoint numDegrees = wheel_event->angleDelta() / 8;
    int numSteps;

    if (not numPixels.isNull())
    {
        numSteps = numPixels.y();
    }
    else if (not numDegrees.isNull())
    {
        numSteps = numDegrees.y() / 15;
    }
    else
    {
        return true;//Just ignore
    }

    _numScheduledVerticalScrollings += numSteps;
    if (_numScheduledVerticalScrollings * numSteps < 0)
    {  // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledVerticalScrollings = numSteps;
    }

    if (verticalScrollAnim->state() != QTimeLine::Running)
    {
        verticalScrollAnim->start();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool GraphicsViewZoom::StartHorizontalScrollings(QWheelEvent *wheel_event)
{
    SCASSERT(wheel_event != nullptr)

    const QPoint numPixels = wheel_event->pixelDelta();
    const QPoint numDegrees = wheel_event->angleDelta() / 8;
    int numSteps;

    if (not numPixels.isNull())
    {
        numSteps = numPixels.y();
    }
    else if (not numDegrees.isNull())
    {
        numSteps = numDegrees.y() / 15;
    }
    else
    {
        return true;//Just ignore
    }

    _numScheduledHorizontalScrollings += numSteps;
    if (_numScheduledHorizontalScrollings * numSteps < 0)
    {  // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledHorizontalScrollings = numSteps;
    }

    if (horizontalScrollAnim->state() != QTimeLine::Running)
    {
        horizontalScrollAnim->start();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsView constructor.
 * @param parent parent object.
 */
VMainGraphicsView::VMainGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      zoom(new GraphicsViewZoom(this)),
      showToolOptions(true),
      isAllowRubberBand(true),
      m_ptStartPos()
{
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setInteractive(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomIn()
{
    // We need to check current scale factor because in Windows we have an error when we zoom in or zoom out to much.
    // See issue #532: Unexpected error occurs when zoom out image.
    if (this->transform().m11() <= MaxScale())
    {
        scale(1.1, 1.1);
        VMainGraphicsView::NewSceneRect(this->scene(), this);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomOut()
{
    // We need to check current scale factor because in Windows we have an error when we zoom in or zoom out to much.
    // See issue #532: Unexpected error occurs when zoom out image.
    if (this->transform().m11() >= MinScale())
    {
        scale(1.0/1.1, 1.0/1.1);
        VMainGraphicsView::NewSceneRect(this->scene(), this);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomOriginal()
{
    QTransform trans = this->transform();
    trans.setMatrix(1.0, trans.m12(), trans.m13(), trans.m21(), 1.0, trans.m23(), trans.m31(), trans.m32(),
                    trans.m33());
    this->setTransform(trans);
    VMainGraphicsView::NewSceneRect(this->scene(), this);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::ZoomFitBest()
{
    VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
    SCASSERT(currentScene)
    currentScene->SetOriginsVisible(false);
    const QRectF rect = currentScene->VisibleItemsBoundingRect();
    currentScene->SetOriginsVisible(true);
    if (rect.isEmpty())
    {
        return;
    }

    this->fitInView(rect, Qt::KeepAspectRatio);
    QTransform transform = this->transform();

    qreal factor = transform.m11();
    factor = qMax(factor, MinScale());
    factor = qMin(factor, MaxScale());

    transform.setMatrix(factor, transform.m12(), transform.m13(), transform.m21(), factor, transform.m23(),
                        transform.m31(), transform.m32(), transform.m33());
    this->setTransform(transform);

    VMainGraphicsView::NewSceneRect(scene(), this);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mousePressEvent handle mouse press events.
 * @param event mouse press event.
 */
void VMainGraphicsView::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            if (isAllowRubberBand)
            {
                QGraphicsView::setDragMode(QGraphicsView::RubberBandDrag);
            }
            if (showToolOptions)
            {
                QList<QGraphicsItem *> list = items(event->pos());
                if (list.size() == 0)
                {
                    emit itemClicked(nullptr);
                    break;
                }
                for (int i = 0; i < list.size(); ++i)
                {
                    if (this->scene()->items().contains(list.at(i)))
                    {
                        if (list.at(i)->type() <= VSimpleCurve::Type &&
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
        }
        case Qt::MiddleButton:
        {
            const QList<QGraphicsItem *> list = items(event->pos());
            if (list.size() == 0)
            {// Only when the user clicks on the scene background
                m_ptStartPos = event->pos();
                QGraphicsView::setDragMode(QGraphicsView::ScrollHandDrag);
                event->accept();
                viewport()->setCursor(Qt::ClosedHandCursor);
            }
            break;
        }
        default:
            break;
    }
    QGraphicsView::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (dragMode() == QGraphicsView::ScrollHandDrag)
    {
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        const QPoint delta = event->pos() - m_ptStartPos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());
        m_ptStartPos = event->pos();
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VMainGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent ( event ); // First because need to hide a rubber band
    QGraphicsView::setDragMode( QGraphicsView::NoDrag );
    if (event->button() == Qt::LeftButton)
    {
        emit MouseRelease();
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMainGraphicsView::MinScale()
{
    const QRect screenRect = QApplication::desktop()->availableGeometry();
    const qreal screenSize = qMin(screenRect.width(), screenRect.height());

    return screenSize / maxSceneSize;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMainGraphicsView::MaxScale()
{
    const QRect screenRect = QApplication::desktop()->availableGeometry();
    const qreal screenSize = qMin(screenRect.width(), screenRect.height());

    return maxSceneSize / screenSize;
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::setShowToolOptions(bool value)
{
    showToolOptions = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsView::AllowRubberBand(bool value)
{
    isAllowRubberBand = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewSceneRect calculate scene rect what contains all items and doesn't less that size of scene view.
 * @param sc scene.
 * @param view view.
 */
void VMainGraphicsView::NewSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    SCASSERT(sc != nullptr)
    SCASSERT(view != nullptr)

    //Calculate view rect
    const QRectF viewRect = SceneVisibleArea(view);

    //Calculate scene rect
    VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(sc);
    SCASSERT(currentScene)
    const QRectF itemsRect = currentScene->VisibleItemsBoundingRect();

    //Unite two rects
    sc->setSceneRect(itemsRect.united(viewRect));
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VMainGraphicsView::SceneVisibleArea(QGraphicsView *view)
{
    SCASSERT(view != nullptr)
    //to receive the currently visible area, map the widgets bounds to the scene
    return QRectF(view->mapToScene(0, 0), view->mapToScene(view->width(), view->height()));
}

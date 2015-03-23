/************************************************************************
 **
 **  @file   vmaingraphicsview.h
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

#ifndef VMAINGRAPHICSVIEW_H
#define VMAINGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

/*!
 * This class adds ability to zoom QGraphicsView using mouse wheel. The point under cursor
 * remains motionless while it's possible.
 *
 * Note that it becomes not possible when the scene's
 * size is not large enough comparing to the viewport size. QGraphicsView centers the picture
 * when it's smaller than the view. And QGraphicsView's scrolls boundaries don't allow to
 * put any picture point at any viewport position.
 *
 * When the user starts scrolling, this class remembers original scene position and
 * keeps it until scrolling is completed. It's better than getting original scene position at
 * each scrolling step because that approach leads to position errors due to before-mentioned
 * positioning restrictions.
 *
 * When zommed using scroll, this class emits zoomed() signal.
 *
 * Usage:
 *
 *   new Graphics_view_zoom(view);
 *
 * The object will be deleted automatically when the view is deleted.
 *
 * You can set keyboard modifiers used for zooming using set_modified(). Zooming will be
 * performed only on exact match of modifiers combination. The default modifier is Ctrl.
 *
 * You can change zoom velocity by calling set_zoom_factor_base().
 * Zoom coefficient is calculated as zoom_factor_base^angle_delta
 * (see QWheelEvent::angleDelta).
 * The default zoom factor base is 1.0015.
 */

class QTimeLine;

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    GraphicsViewZoom(QGraphicsView* view);
    void gentle_zoom(double factor);
    void set_modifiers(Qt::KeyboardModifiers modifiers);
    void set_zoom_factor_base(double value);
signals:
    void zoomed();
public slots:
    void scrollingTime(qreal x);
    void animFinished();
private:
    QGraphicsView*        _view;
    Qt::KeyboardModifiers _modifiers;
    double                _zoom_factor_base;
    QPointF               target_scene_pos;
    QPointF               target_viewport_pos;
    QTimeLine            *anim;

    bool eventFilter(QObject* object, QEvent* event);
private:
    Q_DISABLE_COPY(GraphicsViewZoom)
    /** @brief _numScheduledScalings keep number scheduled scalings. */
    qint32   _numScheduledScalings;
};

/**
 * @brief The VMainGraphicsView class main scene view.
 */
class VMainGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:

    explicit VMainGraphicsView(QWidget *parent = nullptr);
    void setShowToolOptions(bool value);

signals:
    /**
     * @brief NewFactor send new scale factor.
     * @param factor scene scale factor.
     */
    void     NewFactor(qreal factor);
    /**
     * @brief MouseRelease help catch mouse release event.
     *
     * Usefull when you need show dialog after working with tool visualization.
     */
    void     MouseRelease();
    void     itemClicked(QGraphicsItem *item);
public slots:
    void     ZoomIn();
    void     ZoomOut();
    void     ZoomOriginal();
    void     ZoomFitBest();
protected:
    void     mousePressEvent(QMouseEvent *mousePress);
    void     mouseReleaseEvent(QMouseEvent *event);
private:
    Q_DISABLE_COPY(VMainGraphicsView)
    GraphicsViewZoom* zoom;
    bool     showToolOptions;
};

#endif // VMAINGRAPHICSVIEW_H

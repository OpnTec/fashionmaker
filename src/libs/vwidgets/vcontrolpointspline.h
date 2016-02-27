/************************************************************************
 **
 **  @file   vcontrolpointspline.h
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

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/def.h"

/**
 * @brief The VControlPointSpline class control spline point.
 */
class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position, Unit patternUnit,
                        QGraphicsItem * parent = nullptr);
    VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position, const QPointF &controlPoint,
                        const QPointF &splinePoint, Unit patternUnit, QGraphicsItem * parent = nullptr);
    virtual ~VControlPointSpline() Q_DECL_OVERRIDE;
    virtual void      paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                            QWidget * widget = 0) Q_DECL_OVERRIDE;
    virtual int       type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ControlPointSpline)};
signals:
    /**
     * @brief ControlPointChangePosition emit when control point change position.
     * @param indexSpline index spline in list.
     * @param position position point in spline.
     * @param pos new posotion.
     */
    void              ControlPointChangePosition(const qint32 &indexSpline, SplinePointPosition position,
                                                 const QPointF &pos);
    /**
     * @brief ShowContextMenu emit when need show tool's context menu.
     * @param event context menu event.
     */
    void              ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
public slots:
    void              RefreshCtrlPoint(const qint32 &indexSpline, SplinePointPosition pos, const QPointF &controlPoint,
                                       const QPointF &splinePoint);
    void              setEnabledPoint(bool enable);
protected:
    /** @brief radius radius circle. */
    const qreal radius;

    /** @brief controlLine pointer to line control point. */
    QGraphicsLineItem *controlLine;

    virtual void      hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    QVariant          itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void      mousePressEvent( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void      mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent *event ) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VControlPointSpline)
    /** @brief indexSpline index spline in list.. */
    qint32            indexSpline;

    /** @brief position position point in spline. */
    SplinePointPosition position;

    Unit              patternUnit;

    inline qreal CircleRadius() const;
    void  Init();
    void  SetCtrlLine(const QPointF &controlPoint, const QPointF &splinePoint);
};

//---------------------------------------------------------------------------------------------------------------------
qreal VControlPointSpline::CircleRadius() const
{
    return (1.5/*mm*/ / 25.4) * PrintDPI;
}

#endif // VCONTROLPOINTSPLINE_H

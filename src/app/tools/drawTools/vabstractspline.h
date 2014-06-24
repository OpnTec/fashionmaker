/************************************************************************
 **
 **  @file   vabstractspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 3, 2014
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

#ifndef VABSTRACTSPLINE_H
#define VABSTRACTSPLINE_H

#include "vdrawtool.h"
#include <QGraphicsPathItem>
#include "../../widgets/vcontrolpointspline.h"

class VAbstractSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VAbstractSpline(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    static const QString TagName;
public slots:
    virtual void     FullUpdateFromFile ();
    void             Disable(bool disable);
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void             RefreshLine(const qint32 &indexSpline, SplinePointPosition pos,
                                 const QPointF &controlPoint, const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void             setEnabledPoint(bool enable);
protected:
    /**
     * @brief controlPoints list pointers of control points.
     */
    QVector<VControlPointSpline *>   controlPoints;
    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    virtual void     RefreshGeometry ()=0;
    virtual void     ChangedActivDraw ( const QString &newName );
    virtual void     ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    virtual void     SetFactor(qreal factor);
    virtual void     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void     keyReleaseEvent(QKeyEvent * event);
};

#endif // VABSTRACTSPLINE_H

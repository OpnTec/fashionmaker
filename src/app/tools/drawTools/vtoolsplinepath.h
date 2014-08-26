/************************************************************************
 **
 **  @file   vtoolsplinepath.h
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

#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include "vabstractspline.h"

/**
 * @brief The VToolSplinePath class tool for creation spline path.
 */
class VToolSplinePath:public VAbstractSpline
{
    Q_OBJECT
public:
    VToolSplinePath(VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                    QGraphicsItem * parent = nullptr);
     virtual void setDialog();
     static void  Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
     static void  Create(const quint32 _id, VSplinePath *path, VMainGraphicsScene  *scene, VPattern *doc,
                         VContainer *data, const Document &parse, const Source &typeCreation);
     static const QString ToolType;
     static void  UpdatePathPoint(VPattern *doc, QDomNode& node, const VSplinePath &path);
     virtual int  type() const {return Type;}
     enum { Type = UserType + static_cast<int>(Tool::SplinePath)};
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void          RefreshLine(const qint32 &indexSpline, SplinePointPosition pos, const QPointF &controlPoint,
                              const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void          setEnabledPoint(bool enable);
public slots:

    void          ControlPointChangePosition(const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
protected:
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void  AddToFile();
    virtual void  RefreshDataInFile();
    virtual void  RemoveReferens();
    virtual void  SaveDialog(QDomElement &domElement);
private:
    void          RefreshGeometry();
    void          AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint);
    void          UpdateControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline) const;
    void          RefreshSplinePath(VSplinePath &splPath);
};

#endif // VTOOLSPLINEPATH_H

/************************************************************************
 **
 **  @file   vtoolpointofintersectioncircles.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VTOOLPOINTOFINTERSECTIONCIRCLES_H
#define VTOOLPOINTOFINTERSECTIONCIRCLES_H

#include "vtoolsinglepoint.h"

class VFormula;

class VToolPointOfIntersectionCircles : public VToolSinglePoint
{
    Q_OBJECT
public:
    VToolPointOfIntersectionCircles(VAbstractPattern *doc, VContainer *data, const quint32 &id, const
                                    quint32 firstCircleCenterId, quint32 secondCircleCenterId,
                                    const QString &firstCircleRadius, const QString &secondCircleRadius,
                                    CrossCirclesPoint crossPoint, const Source &typeCreation,
                                    QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolPointOfIntersectionCircles *Create(DialogTool *dialog, VMainGraphicsScene  *scene,
                                                   VAbstractPattern *doc,
                                                   VContainer *data);
    static VToolPointOfIntersectionCircles *Create(const quint32 _id, const QString &pointName,
                                                   quint32 firstCircleCenterId, quint32 secondCircleCenterId,
                                                   QString &firstCircleRadius, QString &secondCircleRadius,
                                                   CrossCirclesPoint crossPoint,
                                                   const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                   VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                   const Source &typeCreation);
    static QPointF FindPoint(const QPointF &c1Point, const QPointF &c2Point, qreal c1Radius, qreal c2Radius,
                             const CrossCirclesPoint crossPoint);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionCircles) };

    quint32 GetFirstCircleCenterId() const;
    void    SetFirstCircleCenterId(const quint32 &value);

    quint32 GetSecondCircleCenterId() const;
    void    SetSecondCircleCenterId(const quint32 &value);

    VFormula GetFirstCircleRadius() const;
    void     SetFirstCircleRadius(const VFormula &value);

    VFormula GetSecondCircleRadius() const;
    void     SetSecondCircleRadius(const VFormula &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(CrossCirclesPoint &value);

    virtual void ShowVisualization(bool show);
protected:
    virtual void RemoveReferens();
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void ReadToolAttributes(const QDomElement &domElement);
    virtual void SetVisualization();
private:
    Q_DISABLE_COPY(VToolPointOfIntersectionCircles)

    quint32 firstCircleCenterId;
    quint32 secondCircleCenterId;

    QString firstCircleRadius;
    QString secondCircleRadius;

    CrossCirclesPoint crossPoint;
};

#endif // VTOOLPOINTOFINTERSECTIONCIRCLES_H

/************************************************************************
 **
 **  @file   vtoolpointfromcircleandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 6, 2015
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

#ifndef VTOOLPOINTFROMCIRCLEANDTANGENT_H
#define VTOOLPOINTFROMCIRCLEANDTANGENT_H

#include "vtoolpoint.h"

class VFormula;

class VToolPointFromCircleAndTangent : public VToolPoint
{
    Q_OBJECT
public:
    VToolPointFromCircleAndTangent(VAbstractPattern *doc, VContainer *data, const quint32 &id, quint32 circleCenterId,
                                   const QString &circleRadius, quint32 tangentPointId, CrossCirclesPoint crossPoint,
                                   const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolPointFromCircleAndTangent *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                                  VContainer *data);
    static VToolPointFromCircleAndTangent *Create(const quint32 _id, const QString &pointName,
                                                  quint32 circleCenterId, QString &circleRadius, quint32 tangentPointId,
                                                  CrossCirclesPoint crossPoint, const qreal &mx, const qreal &my,
                                                  VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                                  const Document &parse, const Source &typeCreation);
    static QPointF FindPoint(const QPointF &p, const QPointF &center, qreal radius, const CrossCirclesPoint crossPoint);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointFromCircleAndTangent) };

    quint32 GetTangentPointId() const;
    void    SetTangentPointId(const quint32 &value);

    quint32 GetCircleCenterId() const;
    void    SetCircleCenterId(const quint32 &value);

    VFormula GetCircleRadius() const;
    void     SetCircleRadius(const VFormula &value);

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
    Q_DISABLE_COPY(VToolPointFromCircleAndTangent)

    quint32 circleCenterId;
    quint32 tangentPointId;
    QString circleRadius;
    CrossCirclesPoint crossPoint;
};

#endif // VTOOLPOINTFROMCIRCLEANDTANGENT_H

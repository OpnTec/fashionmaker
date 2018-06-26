/************************************************************************
 **
 **  @file   vtoolpointfromcircleandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

class VFormula;
template <class T> class QSharedPointer;

struct VToolPointFromCircleAndTangentInitData : VToolSinglePointInitData
{
    VToolPointFromCircleAndTangentInitData()
        : VToolSinglePointInitData(),
          circleCenterId(NULL_ID),
          circleRadius('0'),
          tangentPointId(NULL_ID),
          crossPoint(CrossCirclesPoint::FirstPoint)
    {}

    quint32 circleCenterId;
    QString circleRadius;
    quint32 tangentPointId;
    CrossCirclesPoint crossPoint;
};

class VToolPointFromCircleAndTangent : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void setDialog() override;
    static VToolPointFromCircleAndTangent *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                                  VAbstractPattern *doc, VContainer *data);
    static VToolPointFromCircleAndTangent *Create(VToolPointFromCircleAndTangentInitData &initData);
    static bool FindPoint(const QPointF &p, const QPointF &center, qreal radius, const CrossCirclesPoint crossPoint,
                          QPointF *intersectionPoint);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointFromCircleAndTangent) };

    QString TangentPointName() const;
    QString CircleCenterPointName() const;

    VFormula GetCircleRadius() const;
    void     SetCircleRadius(const VFormula &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(const CrossCirclesPoint &value);

    virtual void ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void RemoveReferens() override;
    virtual void SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                            QList<quint32> &newDependencies) override;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void ReadToolAttributes(const QDomElement &domElement) override;
    virtual void SetVisualization() override;
private:
    Q_DISABLE_COPY(VToolPointFromCircleAndTangent)

    quint32 circleCenterId;
    quint32 tangentPointId;
    QString circleRadius;
    CrossCirclesPoint crossPoint;

    VToolPointFromCircleAndTangent(const VToolPointFromCircleAndTangentInitData &initData,
                                   QGraphicsItem *parent = nullptr);
};

#endif // VTOOLPOINTFROMCIRCLEANDTANGENT_H

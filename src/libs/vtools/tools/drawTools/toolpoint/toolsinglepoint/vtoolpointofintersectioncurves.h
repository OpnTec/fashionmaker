/************************************************************************
 **
 **  @file   vtoolpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VTOOLPOINTOFINTERSECTIONCURVES_H
#define VTOOLPOINTOFINTERSECTIONCURVES_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

struct VToolPointOfIntersectionCurvesInitData : VToolSinglePointInitData
{
    VToolPointOfIntersectionCurvesInitData()
        : VToolSinglePointInitData(),
          firstCurveId(NULL_ID),
          secondCurveId(NULL_ID),
          vCrossPoint(VCrossCurvesPoint::HighestPoint),
          hCrossPoint(HCrossCurvesPoint::LeftmostPoint)
    {}

    quint32 firstCurveId;
    quint32 secondCurveId;
    VCrossCurvesPoint vCrossPoint;
    HCrossCurvesPoint hCrossPoint;
};

class VToolPointOfIntersectionCurves : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void setDialog() override;
    static VToolPointOfIntersectionCurves *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                                  VAbstractPattern *doc, VContainer *data);
    static VToolPointOfIntersectionCurves *Create(VToolPointOfIntersectionCurvesInitData initData);
    static bool FindPoint(const QVector<QPointF> &curve1Points, const QVector<QPointF> &curve2Points,
                             VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint, QPointF *intersectionPoint);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionCurves) };

    QString FirstCurveName() const;
    QString SecondCurveName() const;

    VCrossCurvesPoint GetVCrossPoint() const;
    void              SetVCrossPoint(const VCrossCurvesPoint &value);

    HCrossCurvesPoint GetHCrossPoint() const;
    void              SetHCrossPoint(const HCrossCurvesPoint &value);

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
    Q_DISABLE_COPY(VToolPointOfIntersectionCurves)

    quint32 firstCurveId;
    quint32 secondCurveId;

    VCrossCurvesPoint vCrossPoint;
    HCrossCurvesPoint hCrossPoint;

    explicit VToolPointOfIntersectionCurves(const VToolPointOfIntersectionCurvesInitData &initData,
                                            QGraphicsItem *parent = nullptr);
};

#endif // VTOOLPOINTOFINTERSECTIONCURVES_H

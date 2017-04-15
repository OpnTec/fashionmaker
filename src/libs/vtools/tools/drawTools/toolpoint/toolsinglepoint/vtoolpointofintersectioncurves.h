/************************************************************************
 **
 **  @file   vtoolpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

class VToolPointOfIntersectionCurves : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolPointOfIntersectionCurves *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                                  VAbstractPattern *doc, VContainer *data);
    static VToolPointOfIntersectionCurves *Create(const quint32 _id, const QString &pointName,
                                                  quint32 firstCurveId, quint32 secondCurveId,
                                                  VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint,
                                                  const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                  VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                  const Source &typeCreation);
    static QPointF FindPoint(const QVector<QPointF> &curve1Points, const QVector<QPointF> &curve2Points,
                             VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint);
    static const QString ToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionCurves) };

    QString FirstCurveName() const;
    QString SecondCurveName() const;

    quint32 GetFirstCurveId() const;
    void    SetFirstCurveId(const quint32 &value);

    quint32 GetSecondCurveId() const;
    void    SetSecondCurveId(const quint32 &value);

    VCrossCurvesPoint GetVCrossPoint() const;
    void              SetVCrossPoint(const VCrossCurvesPoint &value);

    HCrossCurvesPoint GetHCrossPoint() const;
    void              SetHCrossPoint(const HCrossCurvesPoint &value);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void RemoveReferens() Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolPointOfIntersectionCurves)

    quint32 firstCurveId;
    quint32 secondCurveId;

    VCrossCurvesPoint vCrossPoint;
    HCrossCurvesPoint hCrossPoint;

    explicit VToolPointOfIntersectionCurves(VAbstractPattern *doc, VContainer *data, const quint32 &id, const
                                            quint32 firstCurveId, quint32 secondCurveId,
                                            VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint,
                                            const Source &typeCreation, QGraphicsItem * parent = nullptr);
};

#endif // VTOOLPOINTOFINTERSECTIONCURVES_H

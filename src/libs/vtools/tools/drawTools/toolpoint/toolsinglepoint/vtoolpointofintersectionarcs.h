/************************************************************************
 **
 **  @file   vtoolpointofintersectionarcs.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VTOOLPOINTOFINTERSECTIONARCS_H
#define VTOOLPOINTOFINTERSECTIONARCS_H

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

template <class T> class QSharedPointer;

struct VToolPointOfIntersectionArcsInitData : VToolSinglePointInitData
{
    VToolPointOfIntersectionArcsInitData()
        : VToolSinglePointInitData(),
          firstArcId(NULL_ID),
          secondArcId(NULL_ID),
          pType(CrossCirclesPoint::FirstPoint)
    {}

    quint32 firstArcId;
    quint32 secondArcId;
    CrossCirclesPoint pType;
};

class VToolPointOfIntersectionArcs : public VToolSinglePoint
{
    Q_OBJECT

public:
    virtual void setDialog() override;
    static VToolPointOfIntersectionArcs *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                                VAbstractPattern *doc, VContainer *data);
    static VToolPointOfIntersectionArcs *Create(VToolPointOfIntersectionArcsInitData initData);
    static bool FindPoint(const VArc *arc1, const VArc *arc2, const CrossCirclesPoint pType,
                          QPointF *intersectionPoint);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersectionArcs) };

    QString FirstArcName() const;
    QString SecondArcName() const;

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
    Q_DISABLE_COPY(VToolPointOfIntersectionArcs)

    /** @brief firstArcId id first arc. */
    quint32       firstArcId;

    /** @brief secondArcId id second arc. */
    quint32       secondArcId;

    CrossCirclesPoint crossPoint;

    VToolPointOfIntersectionArcs(const VToolPointOfIntersectionArcsInitData &initData,
                                 QGraphicsItem * parent = nullptr);
};

#endif // VTOOLPOINTOFINTERSECTIONARCS_H

/************************************************************************
 **
 **  @file   vtoolpointofcontact.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

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

struct VToolPointOfContactInitData : VToolSinglePointInitData
{
    VToolPointOfContactInitData()
        : VToolSinglePointInitData(),
          radius('0'),
          center(NULL_ID),
          firstPointId(NULL_ID),
          secondPointId(NULL_ID)
    {}

    QString radius;
    quint32 center;
    quint32 firstPointId;
    quint32 secondPointId;
};

/**
 * @brief The VToolPointOfContact class tool for creation point intersection line and arc.
 */
class VToolPointOfContact : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() override;
    static bool FindPoint(qreal radius, const QPointF &center, const QPointF &firstPoint,
                          const QPointF &secondPoint, QPointF *intersectionPoint);
    static VToolPointOfContact* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                       VAbstractPattern *doc, VContainer *data);
    static VToolPointOfContact* Create(VToolPointOfContactInitData &initData);
    static const QString ToolType;
    virtual int    type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfContact) };

    QString ArcCenterPointName() const;
    QString FirstPointName() const;
    QString SecondPointName() const;

    VFormula getArcRadius() const;
    void     setArcRadius(const VFormula &value);

    virtual void   ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    RemoveReferens() override;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SetVisualization() override;
    virtual QString MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolPointOfContact)

    /** @brief radius string with formula radius arc. */
    QString        arcRadius;

    /** @brief center id center arc point. */
    quint32        center;

    /** @brief firstPointId id first line point. */
    quint32        firstPointId;

    /** @brief secondPointId id second line point. */
    quint32        secondPointId;

    VToolPointOfContact(const VToolPointOfContactInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLPOINTOFCONTACT_H

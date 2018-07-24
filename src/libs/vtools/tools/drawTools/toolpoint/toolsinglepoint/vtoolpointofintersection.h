/************************************************************************
 **
 **  @file   vtoolpointofintersection.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VTOOLPOINTOFINTERSECTION_H
#define VTOOLPOINTOFINTERSECTION_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

struct VToolPointOfIntersectionInitData : VToolSinglePointInitData
{
    VToolPointOfIntersectionInitData()
        : VToolSinglePointInitData(),
          firstPointId(NULL_ID),
          secondPointId(NULL_ID)
    {}

    quint32 firstPointId;
    quint32 secondPointId;
};

/**
 * @brief The VToolPointOfIntersection class tool for creation point intersection two lines.
 */
class VToolPointOfIntersection : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void setDialog() override;
    static VToolPointOfIntersection *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                            VAbstractPattern *doc, VContainer *data);
    static VToolPointOfIntersection *Create(VToolPointOfIntersectionInitData initData);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PointOfIntersection) };

    QString FirstPointName() const;
    QString SecondPointName() const;

    virtual void ShowVisualization(bool show) override;
protected:
    virtual void RemoveReferens() override;
    virtual void SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                            QList<quint32> &newDependencies) override;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void ReadToolAttributes(const QDomElement &domElement) override;
    virtual void SetVisualization() override;
private slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
private:
    Q_DISABLE_COPY(VToolPointOfIntersection)

    /** @brief firstPointId id first line point. */
    quint32       firstPointId;

    /** @brief secondPointId id second line point. */
    quint32       secondPointId;

    VToolPointOfIntersection(const VToolPointOfIntersectionInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLPOINTOFINTERSECTION_H

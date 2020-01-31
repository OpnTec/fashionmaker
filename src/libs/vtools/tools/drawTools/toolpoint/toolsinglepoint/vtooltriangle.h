/************************************************************************
 **
 **  @file   vtooltriangle.h
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

#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

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

struct VToolTriangleInitData : VToolSinglePointInitData
{
    VToolTriangleInitData()
        : VToolSinglePointInitData(),
          axisP1Id(NULL_ID),
          axisP2Id(NULL_ID),
          firstPointId(NULL_ID),
          secondPointId(NULL_ID)
    {}

    quint32 axisP1Id;
    quint32 axisP2Id;
    quint32 firstPointId;
    quint32 secondPointId;
};

/**
 * @brief The VToolTriangle class for tool that find point intersection two foots right triangle
 * (triangle with 90 degree).
 */
class VToolTriangle : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() override;
    static VToolTriangle *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                 VContainer *data);
    static VToolTriangle *Create(VToolTriangleInitData initData);
    static bool FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                          const QPointF &secondPoint, QPointF *intersectionPoint);
    static const QString ToolType;
    virtual int    type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Triangle)};

    QString AxisP1Name() const;
    QString AxisP2Name() const;
    QString FirstPointName() const;
    QString SecondPointName() const;

    quint32 GetAxisP1Id() const;
    void    SetAxisP1Id(const quint32 &value);

    quint32 GetAxisP2Id() const;
    void    SetAxisP2Id(const quint32 &value);

    quint32 GetFirstPointId() const;
    void    SetFirstPointId(const quint32 &value);

    quint32 GetSecondPointId() const;
    void    SetSecondPointId(const quint32 &value);

    virtual void   ShowVisualization(bool show) override;
protected:
    virtual void   RemoveReferens() override;
    virtual void   SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                              QList<quint32> &newDependencies) override;
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void   ReadToolAttributes(const QDomElement &domElement) override;
    virtual void   SetVisualization() override;
private slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
private:
    Q_DISABLE_COPY(VToolTriangle)
    /** @brief axisP1Id id first axis point. */
    quint32        axisP1Id;

    /** @brief axisP2Id id second axis point. */
    quint32        axisP2Id;

    /** @brief firstPointId id first triangle point, what lies on the hypotenuse. */
    quint32        firstPointId;

    /** @brief secondPointId id second triangle point, what lies on the hypotenuse. */
    quint32        secondPointId;

    VToolTriangle(const VToolTriangleInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLTRIANGLE_H

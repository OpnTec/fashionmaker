/************************************************************************
 **
 **  @file   vtoollineintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
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

#ifndef VTOOLLINEINTERSECTAXIS_H
#define VTOOLLINEINTERSECTAXIS_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/vformula.h"
#include "../vmisc/def.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

struct VToolLineIntersectAxisInitData : VToolLinePointInitData
{
    VToolLineIntersectAxisInitData()
        : VToolLinePointInitData(),
          formulaAngle('0'),
          basePointId(NULL_ID),
          firstPointId(NULL_ID),
          secondPointId(NULL_ID)
    {}

    QString formulaAngle;
    quint32 basePointId;
    quint32 firstPointId;
    quint32 secondPointId;
};

class VToolLineIntersectAxis : public VToolLinePoint
{
    Q_OBJECT
public:
    virtual ~VToolLineIntersectAxis() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() override;

    static VToolLineIntersectAxis *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                          VAbstractPattern *doc, VContainer *data);
    static VToolLineIntersectAxis *Create(VToolLineIntersectAxisInitData &initData);

    static bool FindPoint(const QLineF &axis, const QLineF &line, QPointF *intersectionPoint);

    static const QString ToolType;
    virtual int       type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LineIntersectAxis)};

    QString FirstLinePoint() const;
    QString SecondLinePoint() const;

    VFormula     GetFormulaAngle() const;
    void         SetFormulaAngle(const VFormula &value);

    virtual void ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SetVisualization() override;
    virtual QString MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolLineIntersectAxis)

    QString formulaAngle;
    quint32 firstPointId;
    quint32 secondPointId;

    VToolLineIntersectAxis(const VToolLineIntersectAxisInitData &initData, QGraphicsItem * parent = nullptr);
};

#endif // VTOOLLINEINTERSECTAXIS_H

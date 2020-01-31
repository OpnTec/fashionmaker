/************************************************************************
 **
 **  @file   vtoollineintersect.h
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

#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

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

struct VToolLineIntersectInitData : VToolSinglePointInitData
{
    VToolLineIntersectInitData()
        : VToolSinglePointInitData(),
          p1Line1Id(NULL_ID),
          p2Line1Id(NULL_ID),
          p1Line2Id(NULL_ID),
          p2Line2Id(NULL_ID)
    {}

    quint32 p1Line1Id;
    quint32 p2Line1Id;
    quint32 p1Line2Id;
    quint32 p2Line2Id;
};

/**
 * @brief The VToolLineIntersect class help find point intersection lines.
 */
class VToolLineIntersect:public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual void setDialog() override;
    static VToolLineIntersect *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static VToolLineIntersect *Create(VToolLineIntersectInitData initData);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LineIntersect)};

    QString Line1P1Name() const;
    QString Line1P2Name() const;
    QString Line2P1Name() const;
    QString Line2P2Name() const;

    virtual void ShowVisualization(bool show) override;
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
    Q_DISABLE_COPY(VToolLineIntersect)

    /** @brief p1Line1 id first point first line. */
    quint32       p1Line1;

    /** @brief p2Line1 id second point first line. */
    quint32       p2Line1;

    /** @brief p1Line2 id first point second line. */
    quint32       p1Line2;

    /** @brief p2Line2 id second point second line.*/
    quint32       p2Line2;

    VToolLineIntersect(const VToolLineIntersectInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLLINEINTERSECT_H

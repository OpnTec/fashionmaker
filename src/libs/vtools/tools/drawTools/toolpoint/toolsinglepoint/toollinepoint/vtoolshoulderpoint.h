/************************************************************************
 **
 **  @file   vtoolshoulderpoint.h
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

#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

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
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

struct VToolShoulderPointInitData : VToolLinePointInitData
{
    VToolShoulderPointInitData()
        : VToolLinePointInitData(),
          formula(),
          p1Line(NULL_ID),
          p2Line(NULL_ID),
          pShoulder(NULL_ID)
    {}

    QString formula;
    quint32 p1Line;
    quint32 p2Line;
    quint32 pShoulder;
};

/**
 * @brief The VToolShoulderPoint class tool for creation point on shoulder. This tool for special situation, when you
 * want find point along line, but have only length from another point (shoulder).
 */
class VToolShoulderPoint : public VToolLinePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() override;
    static QPointF FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                             const qreal &length);
    static VToolShoulderPoint* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static VToolShoulderPoint* Create(VToolShoulderPointInitData &initData);
    static const QString ToolType;
    virtual int    type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::ShoulderPoint) };

    QString SecondPointName() const;
    QString ShoulderPointName() const;

    quint32 GetP2Line() const;
    void    SetP2Line(const quint32 &value);

    quint32 getPShoulder() const;
    void    setPShoulder(const quint32 &value);

    virtual void   ShowVisualization(bool show) override;
protected:
    virtual void    RemoveReferens() override;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SetVisualization() override;
    virtual QString MakeToolTip() const override;
private slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
private:
    Q_DISABLE_COPY(VToolShoulderPoint)

    /** @brief p2Line id second line point. */
    quint32         p2Line;

    /** @brief pShoulder id shoulder line point. */
    quint32         pShoulder;

    VToolShoulderPoint(const VToolShoulderPointInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLSHOULDERPOINT_H

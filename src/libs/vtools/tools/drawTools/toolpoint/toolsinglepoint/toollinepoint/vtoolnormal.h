/************************************************************************
 **
 **  @file   vtoolnormal.h
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

#ifndef VTOOLNORMAL_H
#define VTOOLNORMAL_H

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

struct VToolNormalInitData : VToolLinePointInitData
{
    VToolNormalInitData()
        : VToolLinePointInitData(),
          formula(),
          firstPointId(NULL_ID),
          secondPointId(NULL_ID),
          angle(0)
    {}

    QString formula;
    quint32 firstPointId;
    quint32 secondPointId;
    qreal angle;
};

/**
 * @brief The VToolNormal class tool for creation point on normal. Normal begin from first point of line.
 */
class VToolNormal : public VToolLinePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() override;
    static VToolNormal* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                               VContainer *data);
    static VToolNormal* Create(VToolNormalInitData initData);
    static QPointF FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, qreal length,
                             qreal angle = 0);
    static const QString ToolType;
    virtual int    type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Normal)};

    QString SecondPointName() const;

    virtual void   ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void   RemoveReferens() override;
    virtual void   SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                              QList<quint32> &newDependencies) override;
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void   ReadToolAttributes(const QDomElement &domElement) override;
    virtual void   SetVisualization() override;
private:
    Q_DISABLE_COPY(VToolNormal)

    /** @brief secondPointId id second line point. */
    quint32        secondPointId;

    VToolNormal(const VToolNormalInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLNORMAL_H

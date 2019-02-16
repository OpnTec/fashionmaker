/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
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

#ifndef VTOOLBASEPOINT_H
#define VTOOLBASEPOINT_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVariant>
#include <Qt>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

struct VToolBasePointInitData : VToolSinglePointInitData
{
    VToolBasePointInitData()
        : VToolSinglePointInitData(),
          nameActivPP(),
          x(10),
          y(10)
    {}

    QString nameActivPP;
    qreal x;
    qreal y;
};

/**
 * @brief The VToolBasePoint class tool for creation pattern base point. Only base point can move. All object
 * pattern peace depend on base point.
 */
class VToolBasePoint : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual ~VToolBasePoint() =default;
    virtual void setDialog() override;
    static VToolBasePoint *Create(VToolBasePointInitData initData);
    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::BasePoint)};
    virtual void ShowVisualization(bool show) override;

    QPointF GetBasePointPos() const;
    void    SetBasePointPos(const QPointF &pos);
public slots:
    virtual void FullUpdateFromFile() override;
    virtual void EnableToolMove(bool move) override;
signals:
    /**
     * @brief FullUpdateTree handle if need update pattern file.
     */
    void         LiteUpdateTree();
protected:
    virtual void AddToFile() override;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) override;
    virtual void DeleteToolWithConfirm(bool ask = true) override;
    virtual void SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                            QList<quint32> &newDependencies) override;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void ReadToolAttributes(const QDomElement &domElement) override;
    virtual void SetVisualization() override {}
    virtual QString MakeToolTip() const override;
private slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
private:
    Q_DISABLE_COPY(VToolBasePoint)

    QString namePP;

    VToolBasePoint (const VToolBasePointInitData &initData, QGraphicsItem * parent = nullptr );
};

#endif // VTOOLBASEPOINT_H

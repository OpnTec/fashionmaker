/************************************************************************
 **
 **  @file   vnodepoint.h
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

#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include <qcompilerdetection.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstractnode.h"
#include "../vwidgets/vscenepoint.h"

/**
 * @brief The VNodePoint class point detail node.
 */
class VNodePoint: public VAbstractNode, public VScenePoint
{
    Q_OBJECT
public:
    static void Create(const VAbstractNodeInitData &initData);

    static const QString ToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::NodePoint)};
    virtual QString getTagName() const override;

    virtual void ChangeLabelPosition(quint32 id, const QPointF &pos) override;
    virtual void SetLabelVisible(quint32 id, bool visible) override;
signals:
    void ShowOptions();
    void ToggleInLayout(bool checked);
    void ToggleForbidFlipping(bool checked);
    void ToggleForceFlipping(bool checked);
    void Delete();
    void ToggleExcludeState(quint32 id);
    void ToggleSeamAllowanceAngleType(quint32 id, PieceNodeAngle type);
    void TogglePassmark(quint32 id, bool toggle);
    void TogglePassmarkAngleType(quint32 id, PassmarkAngleType type);
    void TogglePassmarkLineType(quint32 id, PassmarkLineType type);
public slots:
    virtual void FullUpdateFromFile() override;
    void         NameChangePosition(const QPointF &pos);
    void         PointChoosed();
    void         EnableToolMove(bool move);
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
    void         AllowLabelHover(bool enabled);
    void         AllowLabelSelecting(bool enabled);
protected:
    virtual void AddToFile() override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void ShowNode() override;
    virtual void HideNode() override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
private:
    Q_DISABLE_COPY(VNodePoint)

    VNodePoint(const VAbstractNodeInitData &initData, QObject *qoParent = nullptr, QGraphicsItem *parent = nullptr);

    QHash<int, QAction *> InitContextMenu(QMenu *menu, vidtype pieceId, quint32 referens);
    void InitPassmarkMenu(QMenu *menu, vidtype pieceId, QHash<int, QAction *> &contextMenu);
    void InitAngleTypeMenu(QMenu *menu, vidtype pieceId, QHash<int, QAction *> &contextMenu);
    void InitPassmarkAngleTypeMenu(QMenu *menu, vidtype pieceId, QHash<int, QAction *> &contextMenu);
    void InitPassmarkLineTypeMenu(QMenu *menu, vidtype pieceId, QHash<int, QAction *> &contextMenu);
};

#endif // VNODEPOINT_H

/************************************************************************
 **
 **  @file   vnodepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include "vabstractnode.h"
#include <QGraphicsEllipseItem>

class VPointF;
class VGraphicsSimpleTextItem;
class QGraphicsLineItem;
class QGraphicsItem;

/**
 * @brief The VNodePoint class point detail node.
 */
class VNodePoint: public VAbstractNode, public QGraphicsEllipseItem
{
    Q_OBJECT
public:

    VNodePoint(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idPoint,  const Source &typeCreation,
               const quint32 &idTool = 0,  QObject *qoParent = nullptr, QGraphicsItem * parent = nullptr );

    static void  Create(VAbstractPattern *doc, VContainer *data, VMainGraphicsScene *scene,
                        quint32 id, quint32 idPoint, const Document &parse,
                        const Source &typeCreation, const quint32 &idTool = 0);
    static const QString TagName;
    static const QString ToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::NodePoint)};
    virtual QString getTagName() const Q_DECL_OVERRIDE;

    virtual void incrementReferens() Q_DECL_OVERRIDE;
    virtual void decrementReferens() Q_DECL_OVERRIDE;
public slots:
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE;
    void         NameChangePosition(const QPointF &pos);
    void         PointChoosed();
    void         EnableToolMove(bool move);
    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    void         AllowLabelHover(bool enabled);
    void         AllowLabelSelecting(bool enabled);
protected:
    /** @brief radius radius circle. */
    qreal        radius;

    /** @brief namePoint label name. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName pointer to label line. */
    QGraphicsLineItem       *lineName;

    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void UpdateNamePosition(qreal mx, qreal my);
    virtual void RefreshPointGeometry(const VPointF &point);
    void         RefreshLine();
    virtual void ShowNode() Q_DECL_OVERRIDE;
    virtual void HideNode() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VNodePoint)
};

#endif // VNODEPOINT_H

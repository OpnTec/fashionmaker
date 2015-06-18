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

    static void  Create(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idPoint, const Document &parse,
                        const Source &typeCreation, const quint32 &idTool = 0, QObject *parent = nullptr);
    static const QString TagName;
    static const QString ToolType;
    virtual void DeleteNode();
    virtual void RestoreNode();
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::NodePoint)};
    virtual QString getTagName() const;
public slots:
    virtual void FullUpdateFromFile();
    void         NameChangePosition(const QPointF &pos);
protected:
    /** @brief radius radius circle. */
    qreal        radius;

    /** @brief namePoint label name. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName pointer to label line. */
    QGraphicsLineItem       *lineName;

    virtual void AddToFile();
    virtual void RefreshDataInFile();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void UpdateNamePosition(qreal mx, qreal my);
    virtual void RefreshPointGeometry(const VPointF &point);
    void         RefreshLine();
private:
    Q_DISABLE_COPY(VNodePoint)
};

#endif // VNODEPOINT_H

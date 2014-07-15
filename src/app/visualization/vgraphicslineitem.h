/************************************************************************
 **
 **  @file   vgraphicslineitem.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VGRAPHICSLINEITEM_H
#define VGRAPHICSLINEITEM_H

#include <QGraphicsLineItem>
class VContainer;

class VGraphicsLineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VGraphicsLineItem(const VContainer *data, const quint32 &pointId, const QPointF &scenePos,
                      QGraphicsItem * parent = 0);
    VGraphicsLineItem(const VContainer *data, const quint32 &p1Id, const quint32 &p2Id, QGraphicsItem * parent = 0);
    virtual ~VGraphicsLineItem();
public slots:
    void SetFactor(qreal factor);
    void MousePos(const QPointF &scenePos);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
private:
    Q_DISABLE_COPY(VGraphicsLineItem)
    const VContainer *data;
    const quint32    point1Id;
    const quint32    point2Id;
    qreal            factor;
    QPointF          scenePos;
    const QColor     color;
    void         RefreshGeometry();
};

#endif // VGRAPHICSLINEITEM_H

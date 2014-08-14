/************************************************************************
 **
 **  @file   visline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
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

#ifndef VISLINE_H
#define VISLINE_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QPointF>

#include "../widgets/vapplication.h"

class VContainer;

class VisLine: public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VisLine(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisLine();

    void         setPoint1Id(const quint32 &value);
    void         setLineStyle(const Qt::PenStyle &value);
    virtual void RefreshGeometry()=0;
    void         setScenePos(const QPointF &value);
    void         VisualMode(const quint32 &pointId);
    void         setMainColor(const QColor &value);
signals:
    void         ToolTip(const QString &toolTip);
public slots:
    void         SetFactor(qreal factor);
    void         MousePos(const QPointF &scenePos);
protected:
    const VContainer *data;
    qreal            factor;
    QPointF          scenePos;
    QColor           mainColor;
    QColor           supportColor;
    Qt::PenStyle     lineStyle;
    quint32          point1Id;
    QString          toolTip;
    QRectF       PointRect();
    qreal        FindLength(const QString &expression);
    qreal        FindVal(const QString &expression);
    void         DrawLine(QGraphicsLineItem    *lineItem, const QLineF &line, const QColor &color,
                          Qt::PenStyle style = Qt::SolidLine);
    void         DrawPoint(QGraphicsEllipseItem *point, const QPointF &pos, const QColor &color);
    QLineF       Line(const QPointF &p1, const qreal& length, const qreal &angle);

    QGraphicsEllipseItem *InitPoint(const QColor &color);
    template <class Item>
    Item         *InitItem(const QColor &color)
    {
        Item *item = new Item(this);
        item->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
        item->setZValue(1);
        item->setFlags(QGraphicsItem::ItemStacksBehindParent);
        return item;
    }

    qreal        CorrectAngle(const qreal &angle) const;
    QPointF      Ray(const QPointF &firstPoint, const qreal &angle) const;
    QPointF      Ray(const QPointF &firstPoint) const;
    QLineF       Axis(const QPointF &p, const qreal &angle) const;
    QLineF       Axis(const QPointF &p1, const QPointF &p2) const;
private:
    Q_DISABLE_COPY(VisLine)
};

#endif // VISLINE_H

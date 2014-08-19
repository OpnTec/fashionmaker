/************************************************************************
 **
 **  @file   visualization.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QObject>
#include <QGraphicsItem>
#include "../widgets/vapplication.h"
#include "../widgets/vmaingraphicsscene.h"

class VContainer;

class Visualization : public QObject
{
    Q_OBJECT
public:
    Visualization(const VContainer *data);
    virtual ~Visualization();

    virtual void RefreshGeometry()=0;

    void         setPoint1Id(const quint32 &value);
    void         setLineStyle(const Qt::PenStyle &value);
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

    virtual void InitPen()=0;
    virtual void AddOnScene()=0;

    QGraphicsEllipseItem *InitPoint(const QColor &color, QGraphicsItem *parent) const;
    QRectF       PointRect(const qreal &radius) const;
    qreal        FindLength(const QString &expression);
    qreal        FindVal(const QString &expression);
    void         DrawPoint(QGraphicsEllipseItem *point, const QPointF &pos, const QColor &color,
                           Qt::PenStyle style = Qt::SolidLine);

    template <typename Item>
    void         AddItem(Item *item)
    {
        SCASSERT(item != nullptr);
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        SCASSERT(scene != nullptr);

        scene->addItem(item);
        connect(scene, &VMainGraphicsScene::NewFactor, item, &Visualization::SetFactor);
        connect(scene, &VMainGraphicsScene::mouseMove, item, &Visualization::MousePos);
    }

    template <class Item>
    Item         *InitItem(const QColor &color, QGraphicsItem *parent)
    {
        Item *item = new Item(parent);
        item->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
        item->setZValue(1);
        item->setFlags(QGraphicsItem::ItemStacksBehindParent);
        return item;
    }
private:
    Q_DISABLE_COPY(Visualization)
};

#endif // VISUALIZATION_H

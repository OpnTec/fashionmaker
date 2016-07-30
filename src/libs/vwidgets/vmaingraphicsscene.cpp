/************************************************************************
 **
 **  @file   vmaingraphicsscene.cpp
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

#include "vmaingraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QGraphicsItem>
#include <QApplication>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsScene default constructor.
 */
VMainGraphicsScene::VMainGraphicsScene()
    :QGraphicsScene(), horScrollBar(0), verScrollBar(0), scaleFactor(1), _transform(QTransform()), scenePos(QPointF()),
      origins()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMainGraphicsScene constructor.
 * @param sceneRect scene rect.
 * @param parent parent object.
 */
VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent)
    :QGraphicsScene ( sceneRect, parent ), horScrollBar(0), verScrollBar(0), scaleFactor(1), _transform(QTransform()),
      scenePos(QPointF()), origins()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMoveEvent handle mouse move events.
 * @param event mouse move event.
 */
void VMainGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    scenePos = event->scenePos();
    emit mouseMove(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mousePressEvent mouse press events.
 * @param event mouse press event
 */
void VMainGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        emit MouseLeftPressed();
    }

    QGraphicsScene::mousePressEvent(event);

    QTransform t;
    QGraphicsItem* pItem = itemAt(event->scenePos(), t);
    emit ItemClicked(pItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        emit MouseLeftReleased();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::InitOrigins()
{
    origins.clear();

    QPen originsPen(Qt::green, ToPixel(WidthHairLine(Unit::Mm), Unit::Mm), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush axisTextBrush(Qt::green);
    const qreal arrowAngle = 35.0;
    const qreal arrowLength = 12.0;

    {
        // X axis
        const QLineF lineX(QPointF(25, 0), QPointF(-5, 0));
        QGraphicsLineItem *xLine1 = new QGraphicsLineItem(lineX);
        xLine1->setPen(originsPen);
        xLine1->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        xLine1->setZValue(-1.0);
        addItem(xLine1);
        origins.append(xLine1);

        // Arrow left side
        QLineF arrowLeftLine = lineX;
        arrowLeftLine.setAngle(arrowLeftLine.angle()-arrowAngle);
        arrowLeftLine.setLength(arrowLength);
        QGraphicsLineItem *xLine2 = new QGraphicsLineItem(arrowLeftLine);
        xLine2->setPen(originsPen);
        xLine2->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        xLine2->setZValue(-1.0);
        addItem(xLine2);
        origins.append(xLine2);

        // Arrow right side
        QLineF arrowRightLine = lineX;
        arrowRightLine.setAngle(arrowRightLine.angle()+arrowAngle);
        arrowRightLine.setLength(arrowLength);
        QGraphicsLineItem *xLine3 = new QGraphicsLineItem(arrowRightLine);
        xLine3->setPen(originsPen);
        xLine3->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        xLine3->setZValue(-1.0);
        addItem(xLine3);
        origins.append(xLine3);

        // X axis text
        QGraphicsSimpleTextItem *xOrigin = new QGraphicsSimpleTextItem(QStringLiteral("X"), xLine1);
        xOrigin->setBrush(axisTextBrush);
        xOrigin->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        xOrigin->setZValue(-1.0);
        xOrigin->setPos(25, -(xOrigin->boundingRect().height()/2));
        origins.append(xOrigin);
    }

    {
        // Y axis
        const QLineF lineY(QPointF(0, 25), QPointF(0, -5));
        QGraphicsLineItem *yLine1 = new QGraphicsLineItem(lineY);
        yLine1->setPen(originsPen);
        yLine1->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        yLine1->setZValue(-1.0);
        addItem(yLine1);
        origins.append(yLine1);

        // Arrow left side
        QLineF arrowLeftLine = lineY;
        arrowLeftLine.setAngle(arrowLeftLine.angle()-arrowAngle);
        arrowLeftLine.setLength(arrowLength);
        QGraphicsLineItem *yLine2 = new QGraphicsLineItem(arrowLeftLine);
        yLine2->setPen(originsPen);
        yLine2->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        yLine2->setZValue(-1.0);
        addItem(yLine2);
        origins.append(yLine2);

        // Arrow right side
        QLineF arrowRightLine = lineY;
        arrowRightLine.setAngle(arrowRightLine.angle()+arrowAngle);
        arrowRightLine.setLength(arrowLength);
        QGraphicsLineItem *yLine3 = new QGraphicsLineItem(arrowRightLine);
        yLine3->setPen(originsPen);
        yLine3->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        yLine3->setZValue(-1.0);
        addItem(yLine3);
        origins.append(yLine3);

        // Y axis text
        QGraphicsSimpleTextItem *yOrigin = new QGraphicsSimpleTextItem(QStringLiteral("Y"), yLine1);
        yOrigin->setBrush(axisTextBrush);
        yOrigin->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        yOrigin->setZValue(-1.0);
        yOrigin->setPos(-(yOrigin->boundingRect().width()/2), 25);
        origins.append(yOrigin);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::SetOriginsVisible(bool visible)
{
    foreach (QGraphicsItem *item, origins)
    {
        item->setVisible(visible);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VMainGraphicsScene::getScenePos() const
{
    return scenePos;
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VMainGraphicsScene::VisibleItemsBoundingRect() const
{
    QRectF rect;
    foreach(QGraphicsItem *item, items())
    {
        if(not item->isVisible())
        {
            continue;
        }
        rect = rect.united(item->sceneBoundingRect());
    }
    return rect;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief transform return view transformation.
 * @return view transformation.
 */
QTransform VMainGraphicsScene::transform() const
{
    return _transform;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTransform set view transformation.
 * @param transform view transformation.
 */
void VMainGraphicsScene::setTransform(const QTransform &transform)
{
    _transform = transform;
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::SetDisableTools(bool disable, const QString &namePP)
{
    emit DisableItem(disable, namePP);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedItem emit ChoosedObject signal.
 * @param id object id.
 * @param type object scene type.
 */
void VMainGraphicsScene::ChoosedItem(quint32 id, const SceneObject &type)
{
    emit ChoosedObject(id, type);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::SelectedItem(bool selected, quint32 object, quint32 tool)
{
    emit SelectedObject(selected, object, tool);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor. scene scale factor.
 */
void VMainGraphicsScene::SetFactor(qreal factor)
{
    scaleFactor=scaleFactor*factor;
    emit NewFactor(scaleFactor);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::EnableItemMove(bool move)
{
    emit EnableToolMove(move);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::EnableDetailsMode(bool mode)
{
    emit CurveDetailsMode(mode);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ItemsSelection(const SelectionType &type)
{
    emit ItemSelection(type);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleLabelSelection(bool enabled)
{
    emit EnableLabelItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::TogglePointSelection(bool enabled)
{
    emit EnablePointItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleLineSelection(bool enabled)
{
    emit EnableLineItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleArcSelection(bool enabled)
{
    emit EnableArcItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleSplineSelection(bool enabled)
{
    emit EnableSplineItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleSplinePathSelection(bool enabled)
{
    emit EnableSplinePathItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleNodeLabelSelection(bool enabled)
{
    emit EnableNodeLabelItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleNodePointSelection(bool enabled)
{
    emit EnableNodePointItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleDetailSelection(bool enabled)
{
    emit EnableDetailItemSelection(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleLabelHover(bool enabled)
{
    emit EnableLabelItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::TogglePointHover(bool enabled)
{
    emit EnablePointItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleLineHover(bool enabled)
{
    emit EnableLineItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleArcHover(bool enabled)
{
    emit EnableArcItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleSplineHover(bool enabled)
{
    emit EnableSplineItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleSplinePathHover(bool enabled)
{
    emit EnableSplinePathItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleNodeLabelHover(bool enabled)
{
    emit EnableNodeLabelItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleNodePointHover(bool enabled)
{
    emit EnableNodePointItemHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VMainGraphicsScene::ToggleDetailHover(bool enabled)
{
    emit EnableDetailItemHover(enabled);
}

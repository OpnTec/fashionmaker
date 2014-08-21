/************************************************************************
 **
 **  @file   vabstractspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 3, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vabstractspline.h"
#include <QKeyEvent>

const QString VAbstractSpline::TagName = QStringLiteral("spline");

//---------------------------------------------------------------------------------------------------------------------
VAbstractSpline::VAbstractSpline(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), controlPoints(QVector<VControlPointSpline *>()),
      sceneType(SceneObject::Unknown), isHovered(false)
{
    ignoreFullUpdate = true;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsPathItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VAbstractSpline::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::Disable(bool disable)
{
    DisableItem(this, disable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VAbstractSpline::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    emit setEnabledPoint(selectable);
    VDrawTool::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool highlight tool.
 * @param id object id in container
 * @param color highlight color.
 * @param enable enable or disable highlight.
 */
void VAbstractSpline::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VAbstractSpline::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
// cppcheck-suppress unusedFunction
void VAbstractSpline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())/factor, Qt::SolidLine, Qt::RoundCap));
    this->setPath(ToolPath(PathDirection::Show));
    isHovered = true;
    QGraphicsPathItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
// cppcheck-suppress unusedFunction
void VAbstractSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setPath(ToolPath());
    isHovered = false;
    QGraphicsPathItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange hadle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VAbstractSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key release events.
 * @param event key release event.
 */
void VAbstractSpline::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent  handle mouse release events.
 * @param event mouse release event.
 */
void VAbstractSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, sceneType);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractSpline::ToolPath(PathDirection direction) const
{
    const QSharedPointer<VAbstractCurve> curve = VAbstractTool::data.GeometricObject<VAbstractCurve>(id);
    QPainterPath path;
    path.addPath(curve->GetPath(direction));
    path.setFillRule( Qt::WindingFill );
    return path;
}

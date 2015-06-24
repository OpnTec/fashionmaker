/************************************************************************
 **
 **  @file   vsimplecurve.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 12, 2013
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

#include "vsimplecurve.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QStyleOptionGraphicsItem>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSimpleSpline constructor.
 * @param id spline id.
 * @param factor scene scale factor.
 * @param currentColor current color.
 * @param parent parent object.
 */
VSimpleCurve::VSimpleCurve(quint32 id, const QColor &currentColor, SimpleCurvePoint pointPosition, Unit patternUnit,
                           qreal *factor, QObject *parent)
    :VAbstractSimple(id, currentColor, patternUnit, factor, parent), QGraphicsPathItem(), curvePosition(pointPosition)
{
    SetPen(this, currentColor, WidthHairLine(patternUnit));
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::ChangedActivDraw(const bool &flag)
{
    enabled = flag;
    setEnabled(enabled);
    SetPen(this, currentColor, WidthHairLine(patternUnit));
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VSimpleCurve::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit Choosed(id);
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover move event.
 */
void VSimpleCurve::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    SetPen(this, currentColor, WidthMainLine(patternUnit));
    emit HoverPath(id, curvePosition, PathDirection::Show);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VSimpleCurve::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    SetPen(this, currentColor, WidthHairLine(patternUnit));
    emit HoverPath(id, curvePosition, PathDirection::Hide);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QColor VSimpleCurve::GetCurrentColor() const
{
    return currentColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::SetCurrentColor(const QColor &value)
{
    currentColor = value;
    SetPen(this, CorrectColor(currentColor), pen().widthF());
}

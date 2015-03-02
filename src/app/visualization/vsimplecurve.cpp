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
#include "../core/vapplication.h"
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
VSimpleCurve::VSimpleCurve(quint32 id, QColor currentColor, SimpleCurvePoint pointPosition, qreal *factor,
                           QObject *parent)
    :QObject(parent), QGraphicsPathItem(), id (id), factor(factor), currentColor(currentColor),
      curvePosition(pointPosition), enabled(true)
{
    if (factor == nullptr)
    {
        setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())));
    }
    else
    {
        setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/ *factor));
    }
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::ChangedActivDraw(const bool &flag)
{
    enabled = flag;
    setEnabled(enabled);
    setPen(QPen(CorrectColor(currentColor), qApp->toPixel(qApp->widthHairLine())/ *factor));
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
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VSimpleCurve::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == nullptr)
    {
        this->setPen(QPen(CorrectColor(currentColor), qApp->toPixel(qApp->widthMainLine())));
    }
    else
    {
        this->setPen(QPen(CorrectColor(currentColor), qApp->toPixel(qApp->widthMainLine())/ *factor));
    }
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
    if (factor == nullptr)
    {
        this->setPen(QPen(CorrectColor(currentColor), qApp->toPixel(qApp->widthHairLine())));
    }
    else
    {
        this->setPen(QPen(CorrectColor(currentColor), qApp->toPixel(qApp->widthHairLine())/ *factor));
    }

    emit HoverPath(id, curvePosition, PathDirection::Hide);
}

//---------------------------------------------------------------------------------------------------------------------
QColor VSimpleCurve::GetCurrentColor() const
{
    return currentColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::SetCurrentColor(const QColor &value)
{
    currentColor = value;
    setPen(QPen(CorrectColor(currentColor), pen().widthF()));
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

//---------------------------------------------------------------------------------------------------------------------
QColor VSimpleCurve::CorrectColor(const QColor &color) const
{
    if (enabled)
    {
        return color;
    }
    else
    {
        return Qt::gray;
    }
}

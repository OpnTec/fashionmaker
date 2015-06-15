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

#include "vabstractspline.h"
#include <QKeyEvent>

const QString VAbstractSpline::TagName = QStringLiteral("spline");

//---------------------------------------------------------------------------------------------------------------------
VAbstractSpline::VAbstractSpline(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), controlPoints(QVector<VControlPointSpline *>()),
      sceneType(SceneObject::Unknown), isHovered(false), detailsMode(false)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractSpline::~VAbstractSpline()
{}

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
QString VAbstractSpline::getTagName() const
{
    return VAbstractSpline::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VAbstractSpline::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::Disable(bool disable, const QString &namePP)
{
    enabled = !CorrectDisable(disable, namePP);
    this->setEnabled(enabled);
    this->setPen(QPen(CorrectColor(lineColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor, Qt::SolidLine,
                      Qt::RoundCap));
    emit setEnabledPoint(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::DetailsMode(bool mode)
{
    detailsMode = mode;
    RefreshGeometry();
    ShowHandles(detailsMode);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool highlight tool.
 * @param id object id in container
 * @param enable enable or disable highlight.
 */
void VAbstractSpline::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
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
    this->setPen(QPen(CorrectColor(lineColor),
                      qApp->toPixel(WidthMainLine(*VAbstractTool::data.GetPatternUnit()))/factor, Qt::SolidLine,
                      Qt::RoundCap));
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
    this->setPen(QPen(CorrectColor(lineColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    if (detailsMode)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }
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
            DeleteTool();
            return; //Leave this method immediately after call!!!
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

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::ReadToolAttributes(const QDomElement &domElement)
{
    lineColor = doc->GetParametrString(domElement, AttrColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrColor, lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::ShowHandles(bool show)
{
    for (int i = 0; i < controlPoints.size(); ++i)
    {
        controlPoints.at(i)->setVisible(show);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSpline::setEnabled(bool enabled)
{
    QGraphicsPathItem::setEnabled(enabled);
    if (enabled)
    {
        setPen(QPen(QColor(lineColor),
                    qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
    else
    {
        setPen(QPen(Qt::gray,
                    qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
}

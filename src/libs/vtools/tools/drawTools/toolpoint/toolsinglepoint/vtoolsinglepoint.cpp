/************************************************************************
 **
 **  @file   vtoolsinglepoint.cpp
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

#include "vtoolsinglepoint.h"
#include "../vmisc/logging.h"
#include "../vgeometry/vpointf.h"
#include "../vwidgets/vgraphicssimpletextitem.h"
#include "../../../../undocommands/movelabel.h"

#include <QKeyEvent>

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-prototypes"
#elif defined(Q_CC_INTEL)
    #pragma warning( push )
    #pragma warning( disable: 1418 )
#endif

Q_LOGGING_CATEGORY(vToolSinglePoint, "v.toolSinglePoint")

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic pop
#elif defined(Q_CC_INTEL)
    #pragma warning( pop )
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolSinglePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param parent parent object.
 */
VToolSinglePoint::VToolSinglePoint(VAbstractPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent)
    :VAbstractPoint(doc, data, id), QGraphicsEllipseItem(parent), radius(ToPixel(DefPointRadius/*mm*/, Unit::Mm)),
      namePoint(nullptr), lineName(nullptr)
{
    namePoint = new VGraphicsSimpleTextItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::ShowContextMenu, this, &VToolSinglePoint::contextMenuEvent);
    connect(namePoint, &VGraphicsSimpleTextItem::DeleteTool, this, &VToolSinglePoint::DeleteFromLabel);
    connect(namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VToolSinglePoint::PointChoosed);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VToolSinglePoint::NameChangePosition);
    lineName = new QGraphicsLineItem(this);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
VToolSinglePoint::~VToolSinglePoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsEllipseItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolSinglePoint::name() const
{
    return PointName(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::setName(const QString &name)
{
    SetPointName(id, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::SetEnabled(bool enabled)
{
    SetToolEnabled(this, enabled);
    SetToolEnabled(lineName, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameChangePosition handle change posion point label.
 * @param pos new position.
 */
void VToolSinglePoint::NameChangePosition(const QPointF &pos)
{
    ChangePosition(this, id, pos);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateNamePosition save new position label to the pattern file.
 */
void VToolSinglePoint::UpdateNamePosition(quint32 id)
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    MoveLabel *moveLabel = new MoveLabel(doc, point->mx(), point->my(), id, this->scene());
    connect(moveLabel, &MoveLabel::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolSinglePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::Disable(bool disable, const QString &namePP)
{
    enabled = !CorrectDisable(disable, namePP);
    this->SetEnabled(enabled);
    namePoint->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::EnableToolMove(bool move)
{
    namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::PointChoosed()
{
    emit ChoosedTool(id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolSinglePoint::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mousePressEvent  handle mouse press events.
 * @param event mouse release event.
 */
void VToolSinglePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        PointChoosed();
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
void VToolSinglePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(CorrectColor(baseColor),
                      qApp->toPixel(WidthMainLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VToolSinglePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(CorrectColor(baseColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshPointGeometry refresh point on scene.
 * @param point point.
 */
void VToolSinglePoint::RefreshPointGeometry(const VPointF &point)
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    this->setPen(QPen(CorrectColor(baseColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point.toQPointF());
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    namePoint->blockSignals(true);
    QFont font = namePoint->font();
    font.setPointSize(static_cast<qint32>(namePoint->FontSize()/factor));
    namePoint->setFont(font);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    namePoint->blockSignals(false);
    RefreshLine(id);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshLine refresh line to label on scene.
 */
void VToolSinglePoint::RefreshLine(quint32 id)
{
    Q_UNUSED(id)
    VAbstractTool::RefreshLine(this, namePoint, lineName, radius);
    lineName->setPen(QPen(CorrectColor(Qt::black), qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange hadle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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

    return QGraphicsEllipseItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key release events.
 * @param event key release event.
 */
void VToolSinglePoint::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            try
            {
                DeleteTool();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e);
                return;//Leave this method immediately!!!
            }
            break;
        default:
            break;
    }
    QGraphicsEllipseItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));
}

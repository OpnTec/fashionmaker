/************************************************************************
 **
 **  @file   vtoolpoint.cpp
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

#include "vtoolpoint.h"
#include "../vmisc/logging.h"
#include "../../libs/vgeometry/vpointf.h"
#include "../../libs/vwidgets/vgraphicssimpletextitem.h"
#include "../../undocommands/movelabel.h"

#include <QKeyEvent>

Q_LOGGING_CATEGORY(vToolPoint, "v.toolPoint")

const QString VToolPoint::TagName = QStringLiteral("point");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolPoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param parent parent object.
 */
VToolPoint::VToolPoint(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsEllipseItem(parent), radius(DefPointRadius), namePoint(nullptr),
      lineName(nullptr)
{
    radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm);
    namePoint = new VGraphicsSimpleTextItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::ShowContextMenu, this, &VToolPoint::contextMenuEvent);
    connect(namePoint, &VGraphicsSimpleTextItem::DeleteTool, this, &VToolPoint::DeleteFromLabel);
    connect(namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VToolPoint::PointChoosed);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VToolPoint::NameChangePosition);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
VToolPoint::~VToolPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
QString VToolPoint::name() const
{
    try
    {
        return VAbstractTool::data.GeometricObject<VPointF>(id)->name();
    }
    catch (const VExceptionBadId &e)
    {
        qCDebug(vToolPoint, "Error! Couldn't get point name. %s %s", e.ErrorMessage().toUtf8().constData(),
                e.DetailedInformation().toUtf8().constData());
        return QString("");// Return empty string for property browser
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::setName(const QString &name)
{
    // Don't know if need check name here.
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    obj->setName(name);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPoint::getTagName() const
{
    return VToolPoint::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameChangePosition handle change posion point label.
 * @param pos new position.
 */
void VToolPoint::NameChangePosition(const QPointF &pos)
{
    VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<VPointF>(id));
    QPointF p = pos - this->pos();
    point->setMx(p.x());
    point->setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point->mx(), point->my());
    VAbstractTool::data.UpdateGObject(id, point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateNamePosition save new position label to the pattern file.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 */
void VToolPoint::UpdateNamePosition(qreal mx, qreal my)
{
    MoveLabel *moveLabel = new MoveLabel(doc, mx, my, id, this->scene());
    connect(moveLabel, &MoveLabel::NeedLiteParsing, doc, &VPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.
 * @param id object id in container.
 * @param enable enable or disable highlight.
 */
void VToolPoint::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolPoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::Disable(bool disable, const QString &namePP)
{
    enabled = !CorrectDisable(disable, namePP);
    this->setEnabled(enabled);
    namePoint->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::DeleteFromLabel()
{
    DeleteTool(); //Leave this method immediately after call!!!
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::EnableToolMove(bool move)
{
    namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::PointChoosed()
{
    emit ChoosedTool(id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolPoint::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent  handle mouse release events.
 * @param event mouse release event.
 */
void VToolPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        PointChoosed();
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
void VToolPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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
void VToolPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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
void VToolPoint::RefreshPointGeometry(const VPointF &point)
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
    RefreshLine();
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshLine refresh line to label on scene.
 */
void VToolPoint::RefreshLine()
{
    QRectF nRec = namePoint->sceneBoundingRect();
    nRec.translate(- scenePos());
    if (this->rect().intersects(nRec) == false)
    {
        QRectF nameRec = namePoint->sceneBoundingRect();
        QPointF p1, p2;
        VGObject::LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center() - scenePos()), p1, p2);
        QPointF pRec = VGObject::LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
        lineName->setLine(QLineF(p1, pRec - scenePos()));
        lineName->setPen(QPen(CorrectColor(Qt::black),
                              qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));

        if (QLineF(p1, pRec - scenePos()).length() <= ToPixel(4, Unit::Mm))
        {
            lineName->setVisible(false);
        }
        else
        {
            lineName->setVisible(true);
        }
    }
    else
    {
        lineName->setVisible(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange hadle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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
void VToolPoint::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool();
            return; //Leave this method immediately after call!!!
        default:
            break;
    }
    QGraphicsEllipseItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPoint::setEnabled(bool enabled)
{
    QGraphicsEllipseItem::setEnabled(enabled);
    if (enabled)
    {
        setPen(QPen(QColor(baseColor), qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
    else
    {
        setPen(QPen(Qt::gray, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
}

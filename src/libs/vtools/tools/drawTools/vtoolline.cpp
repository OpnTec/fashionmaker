/************************************************************************
 **
 **  @file   vtoolline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vtoolline.h"

#include <QKeyEvent>
#include <QLineF>
#include <QMessageLogger>
#include <QPen>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <QtDebug>
#include <new>

#include "../../dialogs/tools/dialogline.h"
#include "../../dialogs/tools/dialogtool.h"
#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoolline.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vabstracttool.h"
#include "vdrawtool.h"

template <class T> class QSharedPointer;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLine constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolLine::VToolLine(const VToolLineInitData &initData, QGraphicsItem *parent)
    :VDrawTool(initData.doc, initData.data, initData.id),
      VScaledLine(parent),
      firstPoint(initData.firstPoint),
      secondPoint(initData.secondPoint),
      lineColor(initData.lineColor),
      m_acceptHoverEvents(true)
{
    m_isBoldLine = false;
    this->m_lineType = initData.typeLine;
    //Line
    RefreshGeometry();
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus
    this->setAcceptHoverEvents(m_acceptHoverEvents);

    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolLine::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogLine> dialogTool = m_dialog.objectCast<DialogLine>();
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetFirstPoint(firstPoint);
    dialogTool->SetSecondPoint(secondPoint);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolLine *VToolLine::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                             VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogLine> dialogTool = dialog.objectCast<DialogLine>();
    SCASSERT(not dialogTool.isNull())

    VToolLineInitData initData;
    initData.firstPoint = dialogTool->GetFirstPoint();
    initData.secondPoint = dialogTool->GetSecondPoint();
    initData.typeLine = dialogTool->GetTypeLine();
    initData.lineColor = dialogTool->GetLineColor();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolLine *line = Create(initData);
    if (line != nullptr)
    {
        line->m_dialog = dialogTool;
    }
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 */
VToolLine * VToolLine::Create(VToolLineInitData initData)
{
    SCASSERT(initData.scene != nullptr)
    SCASSERT(initData.doc != nullptr)
    SCASSERT(initData.data != nullptr)
    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = VContainer::getNextId();
        initData.data->AddLine(initData.firstPoint, initData.secondPoint);
    }
    else
    {
        VContainer::UpdateId(initData.id);
        initData.data->AddLine(initData.firstPoint, initData.secondPoint);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::Line, initData.doc);
        VToolLine *line = new VToolLine(initData);
        initData.scene->addItem(line);
        InitDrawToolConnections(initData.scene, line);
        connect(initData.scene, &VMainGraphicsScene::EnableLineItemSelection, line, &VToolLine::AllowSelecting);
        connect(initData.scene, &VMainGraphicsScene::EnableLineItemHover, line, &VToolLine::AllowHover);
        VAbstractPattern::AddTool(initData.id, line);

        const QSharedPointer<VPointF> first = initData.data->GeometricObject<VPointF>(initData.firstPoint);
        const QSharedPointer<VPointF> second = initData.data->GeometricObject<VPointF>(initData.secondPoint);

        initData.doc->IncrementReferens(first->getIdTool());
        initData.doc->IncrementReferens(second->getIdTool());
        return line;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLine::getTagName() const
{
    return VDomDocument::TagLine;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Don't set pen width. Parent should take care of it.
    QPen lPen = pen();
    lPen.setColor(CorrectColor(this, lineColor));
    lPen.setStyle(LineStyleToPenStyle(m_lineType));
    setPen(lPen);

    PaintWithFixItemHighlightSelected<VScaledLine>(this, painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLine::FirstPointName() const
{
    return VAbstractTool::data.GetGObject(firstPoint)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLine::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(secondPoint)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolLine::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool highlight tool.
 * @param id object id in container
 * @param enable enable or disable highlight.
 */
void VToolLine::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::Disable(bool disable, const QString &namePP)
{
    const bool enabled = !CorrectDisable(disable, namePP);
    this->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::AllowHover(bool enabled)
{
    // Manually handle hover events. Need for setting cursor for not selectable paths.
    m_acceptHoverEvents = enabled;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::AllowSelecting(bool enabled)
{
    setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    Q_UNUSED(id)
    try
    {
        ContextMenu<DialogLine>(event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolLine::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = QSharedPointer<VGObject> ();
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
void VToolLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_acceptHoverEvents)
    {
        m_isBoldLine = true;
        setToolTip(MakeToolTip());
        VScaledLine::hoverEnterEvent(event);
    }
    else
    {
        setCursor(qApp->getSceneView()->viewport()->cursor());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_acceptHoverEvents && vis.isNull())
    {
        m_isBoldLine = false;
        VScaledLine::hoverLeaveEvent(event);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLine::RemoveReferens()
{
    const auto p1 = VAbstractTool::data.GetGObject(firstPoint);
    const auto p2 = VAbstractTool::data.GetGObject(secondPoint);

    doc->DecrementReferens(p1->getIdTool());
    doc->DecrementReferens(p2->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolLine::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        emit ChangedToolSelection(value.toBool(), m_id, m_id);
    }

    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key realse events.
 * @param event key realse event.
 */
void VToolLine::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            try
            {
                DeleteToolWithConfirm();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e)
                return;//Leave this method immediately!!!
            }
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolLine::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                           QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogLine> dialogTool = m_dialog.objectCast<DialogLine>();
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, firstPoint);
    AddDependence(oldDependencies, secondPoint);
    AddDependence(newDependencies, dialogTool->GetFirstPoint());
    AddDependence(newDependencies, dialogTool->GetSecondPoint());

    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPoint()));
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrFirstPoint, firstPoint);
    doc->SetAttribute(tag, AttrSecondPoint, secondPoint);
    doc->SetAttribute(tag, AttrTypeLine, m_lineType);
    doc->SetAttribute(tag, AttrLineColor, lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::ReadToolAttributes(const QDomElement &domElement)
{
    firstPoint = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPoint = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolLine *visual = qobject_cast<VisToolLine *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(firstPoint);
        visual->setPoint2Id(secondPoint);
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLine::MakeToolTip() const
{
    const QSharedPointer<VPointF> first = VAbstractTool::data.GeometricObject<VPointF>(firstPoint);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(secondPoint);

    const QLineF line(static_cast<QPointF>(*first), static_cast<QPointF>(*second));

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%4:</b> %5°</td> </tr>"
                                    "</table>")
            .arg(tr("Length"))
            .arg(qApp->fromPixel(line.length()))
            .arg(UnitsToStr(qApp->patternUnit(), true), tr("Angle"))
            .arg(line.angle());
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis.isNull())
        {
            AddVisualization<VisToolLine>();
            SetVisualization();
        }
        else
        {
            if (VisToolLine *visual = qobject_cast<VisToolLine *>(vis))
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        hoverLeaveEvent(nullptr);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetTypeLine(const QString &value)
{
    m_lineType = value;

    QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLine::GetLineColor() const
{
    return lineColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::SetLineColor(const QString &value)
{
    lineColor = value;

    QSharedPointer<VGObject> obj;//We don't have object for line in data container. Just will send empty object.
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLine::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object)
    setVisible(visible);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VToolLine::RefreshGeometry()
{
    const QSharedPointer<VPointF> first = VAbstractTool::data.GeometricObject<VPointF>(firstPoint);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(secondPoint);
    this->setLine(QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)));
}

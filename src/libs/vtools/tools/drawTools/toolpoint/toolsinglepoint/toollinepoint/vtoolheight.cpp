/************************************************************************
 **
 **  @file   vtoolheight.cpp
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

#include "vtoolheight.h"

#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialogheight.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolheight.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString VToolHeight::ToolType = QStringLiteral("height");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolHeight constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param basePointId id base point of projection.
 * @param p1LineId id first point of line.
 * @param p2LineId id second point of line.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolHeight::VToolHeight(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                         const QString &lineColor, const quint32 &basePointId, const quint32 &p1LineId,
                         const quint32 &p2LineId, const Source &typeCreation, QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, QString(), basePointId, 0, parent), p1LineId(p1LineId),
      p2LineId(p2LineId)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolHeight::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetBasePointId(basePointId);
    dialogTool->SetP1LineId(p1LineId);
    dialogTool->SetP2LineId(p2LineId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @return the created tool
 */
VToolHeight* VToolHeight::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    const quint32 basePointId = dialogTool->GetBasePointId();
    const quint32 p1LineId = dialogTool->GetP1LineId();
    const quint32 p2LineId = dialogTool->GetP2LineId();

    VToolHeight *point = Create(0, pointName, typeLine, lineColor, basePointId, p1LineId, p2LineId, 5, 10, scene, doc,
                                data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param typeLine line type.
 * @param basePointId id base point of projection.
 * @param p1LineId id first point of line.
 * @param p2LineId id second point of line.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolHeight* VToolHeight::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                 const QString &lineColor, const quint32 &basePointId, const quint32 &p1LineId,
                                 const quint32 &p2LineId, const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                 const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> p1Line = data->GeometricObject<VPointF>(p1LineId);
    const QSharedPointer<VPointF> p2Line = data->GeometricObject<VPointF>(p2LineId);

    QPointF pHeight = FindPoint(QLineF(*p1Line, *p2Line), *basePoint);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(pHeight, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(pHeight, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::Height, doc);
        VToolHeight *point = new VToolHeight(doc, data, id, typeLine, lineColor, basePointId, p1LineId, p2LineId,
                                             typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePoint->getIdTool());
        doc->IncrementReferens(p1Line->getIdTool());
        doc->IncrementReferens(p2Line->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find projection base point onto line.
 * @param line line
 * @param point base point.
 * @return point onto line.
 */
QPointF VToolHeight::FindPoint(const QLineF &line, const QPointF &point)
{
    return VGObject::ClosestPoint(line, point);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolHeight::FirstLinePointName() const
{
    return VAbstractTool::data.GetGObject(p1LineId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolHeight::SecondLinePointName() const
{
    return VAbstractTool::data.GetGObject(p2LineId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolHeight::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogHeight>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolHeight::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr)
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetP1LineId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetP2LineId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
    doc->SetAttribute(tag, AttrP1Line, p1LineId);
    doc->SetAttribute(tag, AttrP2Line, p2LineId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::ReadToolAttributes(const QDomElement &domElement)
{
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    p1LineId = doc->GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
    p2LineId = doc->GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolHeight *visual = qobject_cast<VisToolHeight *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(basePointId);
        visual->setLineP1Id(p1LineId);
        visual->setLineP2Id(p2LineId);
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolHeight::GetP2LineId() const
{
    return p2LineId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::SetP2LineId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p2LineId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolHeight>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolHeight::GetP1LineId() const
{
    return p1LineId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolHeight::SetP1LineId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p1LineId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

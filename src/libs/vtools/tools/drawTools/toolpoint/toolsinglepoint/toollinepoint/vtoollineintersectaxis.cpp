/************************************************************************
 **
 **  @file   vtoollineintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
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

#include "vtoollineintersectaxis.h"

#include <QLine>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialoglineintersectaxis.h"
#include "../../../../../visualization/line/vistoollineintersectaxis.h"
#include "../ifc/exception/vexception.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/../../../../../dialogs/support/../tools/dialogtool.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/../../../../../visualization/line/../visualization.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/../../../../vabstracttool.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/../../../vdrawtool.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollinepoint.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class VGObject;
template <class T> class QSharedPointer;

const QString VToolLineIntersectAxis::ToolType = QStringLiteral("lineIntersectAxis");

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis::VToolLineIntersectAxis(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                               const QString &typeLine, const QString &lineColor,
                                               const QString &formulaAngle, const quint32 &basePointId,
                                               const quint32 &firstPointId, const quint32 &secondPointId,
                                               const Source &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, QString(), basePointId, 0, parent), formulaAngle(formulaAngle),
      firstPointId(firstPointId), secondPointId(secondPointId)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis::~VToolLineIntersectAxis()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::setDialog()
{
    SCASSERT(dialog != nullptr);
    dialog->setModal(true);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetBasePointId(basePointId);
    dialogTool->SetFirstPointId(firstPointId);
    dialogTool->SetSecondPointId(secondPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis *VToolLineIntersectAxis::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                                                       VAbstractPattern *doc,
                                                       VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    QString formulaAngle = dialogTool->GetAngle();
    const quint32 basePointId = dialogTool->GetBasePointId();
    const quint32 firstPointId = dialogTool->GetFirstPointId();
    const quint32 secondPointId = dialogTool->GetSecondPointId();

    VToolLineIntersectAxis *point = Create(0, pointName, typeLine, lineColor, formulaAngle,
                                           basePointId, firstPointId, secondPointId, 5, 10,
                                           scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolLineIntersectAxis *VToolLineIntersectAxis::Create(const quint32 _id, const QString &pointName,
                                                       const QString &typeLine, const QString &lineColor,
                                                       QString &formulaAngle, const quint32 &basePointId,
                                                       const quint32 &firstPointId, const quint32 &secondPointId,
                                                       const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                       VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                       const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    QLineF axis = QLineF(*basePoint, QPointF(basePoint->x()+100, basePoint->y()));
    axis.setAngle(CheckFormula(_id, formulaAngle, data));

    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);
    QLineF line(*firstPoint, *secondPoint);

    QPointF fPoint = FindPoint(axis, line);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::LineIntersectAxis, doc);
    if (parse == Document::FullParse)
    {
        VToolLineIntersectAxis *point = new VToolLineIntersectAxis(doc, data, id, typeLine, lineColor, formulaAngle,
                                                                   basePointId, firstPointId, secondPointId,
                                                                   typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePoint->getIdTool());
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolLineIntersectAxis::FindPoint(const QLineF &axis, const QLineF &line)
{
    QPointF fPoint;
    QLineF::IntersectType intersect = axis.intersect(line, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        if(VFuzzyComparePossibleNulls(axis.angle(), line.angle())
           || VFuzzyComparePossibleNulls(qAbs(axis.angle() - line.angle()), 180))
        {
            return QPointF();
        }
        else
        {
            return fPoint;
        }
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolLineIntersectAxis::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersectAxis::GetFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetFirstPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersectAxis::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolLineIntersectAxis>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogLineIntersectAxis>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersectAxis *dialogTool = qobject_cast<DialogLineIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
    doc->SetAttribute(tag, AttrP1Line, firstPointId);
    doc->SetAttribute(tag, AttrP2Line, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::ReadToolAttributes(const QDomElement &domElement)
{
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    firstPointId = doc->GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersectAxis::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolLineIntersectAxis *visual = qobject_cast<VisToolLineIntersectAxis *>(vis);
        SCASSERT(visual != nullptr);

        visual->setObject1Id(firstPointId);
        visual->setPoint2Id(secondPointId);
        visual->setAxisPointId(basePointId);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

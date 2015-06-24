/************************************************************************
 **
 **  @file   vtoolcurveintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
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

#include "vtoolcurveintersectaxis.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../../../../../dialogs/tools/dialogcurveintersectaxis.h"
#include "../../../../../dialogs/support/dialogeditwrongformula.h"
#include "../vgeometry/vpointf.h"
#include "../../../../../visualization/vistoolcurveintersectaxis.h"

const QString VToolCurveIntersectAxis::ToolType = QStringLiteral("curveIntersectAxis");

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis::VToolCurveIntersectAxis(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                                 const QString &typeLine, const QString &lineColor,
                                                 const QString &formulaAngle, const quint32 &basePointId,
                                                 const quint32 &curveId, const Source &typeCreation,
                                                 QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, QString(), basePointId, 0, parent), formulaAngle(formulaAngle),
      curveId(curveId)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis::~VToolCurveIntersectAxis()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::setDialog()
{
    SCASSERT(dialog != nullptr);
    dialog->setModal(true);
    DialogCurveIntersectAxis *dialogTool = qobject_cast<DialogCurveIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetBasePointId(basePointId);
    dialogTool->setCurveId(curveId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis *VToolCurveIntersectAxis::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                                                         VAbstractPattern *doc,
                                                         VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCurveIntersectAxis *dialogTool = qobject_cast<DialogCurveIntersectAxis*>(dialog);
    SCASSERT(dialogTool);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    QString formulaAngle = dialogTool->GetAngle();
    const quint32 basePointId = dialogTool->GetBasePointId();
    const quint32 curveId = dialogTool->getCurveId();

    VToolCurveIntersectAxis *point = nullptr;
    point=Create(0, pointName, typeLine, lineColor, formulaAngle, basePointId, curveId, 5, 10, scene, doc, data,
                 Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis *VToolCurveIntersectAxis::Create(const quint32 _id, const QString &pointName,
                                                         const QString &typeLine, const QString &lineColor,
                                                         QString &formulaAngle, const quint32 &basePointId,
                                                         const quint32 &curveId, const qreal &mx, const qreal &my,
                                                         VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                         VContainer *data,
                                                         const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    qreal angle = CheckFormula(_id, formulaAngle, data);
    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(curveId);

    QPointF fPoint = FindPoint(basePoint->toQPointF(), angle, curve);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(basePointId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::CurveIntersectAxis, doc);
    if (parse == Document::FullParse)
    {
        VToolCurveIntersectAxis *point = new VToolCurveIntersectAxis(doc, data, id, typeLine, lineColor, formulaAngle,
                                                                     basePointId, curveId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolSinglePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolCurveIntersectAxis::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolCurveIntersectAxis::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolCurveIntersectAxis::EnableToolMove);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(curveId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolCurveIntersectAxis::FindPoint(const QPointF &point, qreal angle,
                                           const QSharedPointer<VAbstractCurve> &curve)
{
    QRectF rec = QRectF(0, 0, INT_MAX, INT_MAX);
    rec.translate(static_cast<qreal>(-INT_MAX/2), static_cast<qreal>(-INT_MAX/2));

    const QLineF axis = VGObject::BuildAxis(point, angle, rec);
    QVector<QPointF> points = curve->IntersectLine(axis);

    if (points.size() > 0)
    {
        if (points.size() == 1)
        {
            return points.at(0);
        }

        QMap<qreal, int> lengths;

        for ( qint32 i = 0; i < points.size(); ++i )
        {
            lengths.insert(QLineF(points.at(i), point).length(), i);
        }

        QMap<qreal, int>::const_iterator i = lengths.constBegin();
        if (i != lengths.constEnd())
        {
            return points.at(i.value());
        }
    }

    return QPointF();
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolCurveIntersectAxis::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolCurveIntersectAxis::getCurveId() const
{
    return curveId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::setCurveId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        curveId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCurveIntersectAxis>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCurveIntersectAxis>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCurveIntersectAxis *dialogTool = qobject_cast<DialogCurveIntersectAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
    doc->SetAttribute(domElement, AttrCurve, QString().setNum(dialogTool->getCurveId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
    doc->SetAttribute(tag, AttrCurve, curveId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::ReadToolAttributes(const QDomElement &domElement)
{
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    curveId = doc->GetParametrUInt(domElement, AttrCurve, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolCurveIntersectAxis *visual = qobject_cast<VisToolCurveIntersectAxis *>(vis);
        SCASSERT(visual != nullptr);

        visual->setPoint1Id(curveId);
        visual->setAxisPointId(basePointId);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle));
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

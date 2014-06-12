/************************************************************************
 **
 **  @file   vtoolcutspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "vtoolcutspline.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutspline.h"
#include "../../geometry/vpointf.h"

#include <geometry/vspline.h>

const QString VToolCutSpline::ToolType = QStringLiteral("cutSpline");
const QString VToolCutSpline::AttrSpline = QStringLiteral("spline");

//---------------------------------------------------------------------------------------------------------------------
VToolCutSpline::VToolCutSpline(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                               const quint32 &splineId, const quint32 &spl1id, const quint32 &spl2id,
                               const Source &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), splineId(splineId), firstSpline(), secondSpline(),
      spl1id(spl1id), spl2id(spl2id)
{
    Q_ASSERT_X(splineId > 0, Q_FUNC_INFO, "splineId <= 0");
    Q_ASSERT_X(spl1id > 0, Q_FUNC_INFO, "spl1id <= 0");
    Q_ASSERT_X(spl2id > 0, Q_FUNC_INFO, "spl2id <= 0");

    firstSpline = new VSimpleSpline(spl1id, &currentColor, &factor);
    RefreshSpline(firstSpline, spl1id, SimpleSplinePoint::ForthPoint);
    firstSpline->setParentItem(this);
    connect(firstSpline, &VSimpleSpline::Choosed, this, &VToolCutSpline::SplineChoosed);

    secondSpline = new VSimpleSpline(spl2id, &currentColor, &factor);
    RefreshSpline(secondSpline, spl2id, SimpleSplinePoint::FirstPoint);
    secondSpline->setParentItem(this);
    connect(secondSpline, &VSimpleSpline::Choosed, this, &VToolCutSpline::SplineChoosed);

    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setFormula(formula);
    dialogTool->setSplineId(splineId, id);
    dialogTool->setPointName(point->name());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                            VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->getFormula();
    const quint32 splineId = dialogTool->getSplineId();
    Create(0, pointName, formula, splineId, 5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &splineId,
                            const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                            VContainer *data, const Document &parse, const Source &typeCreation)
{
    const VSpline *spl = data->GeometricObject<const VSpline *>(splineId);

    const qreal result = CheckFormula(formula, data);

    QPointF spl1p2, spl1p3, spl2p2, spl2p3;
    QPointF point = spl->CutSpline(qApp->toPixel(result), spl1p2, spl1p3, spl2p2, spl2p3);

    quint32 id = _id;
    quint32 spl1id = 0;
    quint32 spl2id = 0;
    if (typeCreation == Source::FromGui)
    {
        VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
        id = data->AddGObject(p);

        VSpline *spline1 = new VSpline(spl->GetP1(), spl1p2, spl1p3, *p, spl->GetKcurve());
        spl1id = data->AddGObject(spline1);
        data->AddLengthSpline(spline1->name(), qApp->fromPixel(spline1->GetLength()));

        VSpline *spline2 = new VSpline(*p, spl2p2, spl2p3, spl->GetP4(), spl->GetKcurve());
        spl2id = data->AddGObject(spline2);
        data->AddLengthSpline(spline2->name(), qApp->fromPixel(spline2->GetLength()));
    }
    else
    {
        VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
        data->UpdateGObject(id, p);

        spl1id = id + 1;
        spl2id = id + 2;

        VSpline *spline1 = new VSpline(spl->GetP1(), spl1p2, spl1p3, *p, spl->GetKcurve());
        data->UpdateGObject(spl1id, spline1);
        data->AddLengthSpline(spline1->name(), qApp->fromPixel(spline1->GetLength()));

        VSpline *spline2 = new VSpline(*p, spl2p2, spl2p3, spl->GetP4(), spl->GetKcurve());
        data->UpdateGObject(spl2id, spline2);
        data->AddLengthSpline(spline2->name(), qApp->fromPixel(spline2->GetLength()));

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::CutSplineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolCutSpline *point = new VToolCutSpline(doc, data, id, formula, splineId, spl1id, spl2id, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        doc->AddTool(id, point);
        doc->AddTool(spl1id, point);
        doc->AddTool(spl2id, point);
        doc->IncrementReferens(splineId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splineId = domElement.attribute(AttrSpline, "").toUInt();
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::SplineChoosed(quint32 id)
{
    emit ChoosedTool(id, SceneObject::Spline);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::ChangedActivDraw(const QString &newName)
{
    bool flag = true;
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
        flag = true;
    }
    else
    {
        currentColor = Qt::gray;
        flag = false;
    }
    firstSpline->ChangedActivDraw(flag);
    secondSpline->ChangedActivDraw(flag);
    VToolPoint::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSpline>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSpline>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrSpline, splineId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrSpline, splineId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::RefreshGeometry()
{
    RefreshSpline(firstSpline, spl1id, SimpleSplinePoint::ForthPoint);
    RefreshSpline(secondSpline, spl2id, SimpleSplinePoint::FirstPoint);
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::RemoveReferens()
{
    doc->DecrementReferens(splineId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutSpline *dialogTool = qobject_cast<DialogCutSpline*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrSpline, QString().setNum(dialogTool->getSplineId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSpline::RefreshSpline(VSimpleSpline *spline, quint32 splid, SimpleSplinePoint tr)
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(splid);
    QPainterPath path;
    path.addPath(spl->GetPath());
    path.setFillRule( Qt::WindingFill );
    if (tr == SimpleSplinePoint::FirstPoint)
    {
        path.translate(-spl->GetP1().toQPointF().x(), -spl->GetP1().toQPointF().y());
    }
    else
    {
        path.translate(-spl->GetP4().toQPointF().x(), -spl->GetP4().toQPointF().y());
    }
    spline->setPath(path);
}

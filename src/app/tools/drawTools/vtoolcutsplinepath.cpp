/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
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

#include "vtoolcutsplinepath.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutsplinepath.h"
#include "../../geometry/vpointf.h"

#include "../../geometry/vsplinepath.h"

const QString VToolCutSplinePath::ToolType = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first splinePath.
 * @param splinePathId id splinePath (we cut this splinePath) in data container.
 * @param splPath1id id first splinePath after cutting.
 * @param splPath2id id second splinePath after cutting.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutSplinePath::VToolCutSplinePath(VPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &formula, const quint32 &splinePathId,
                                       const quint32 &splPath1id, const quint32 &splPath2id,
                                       const Source &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), splinePathId(splinePathId), firstSpline(), secondSpline(),
      splPath1id (splPath1id), splPath2id(splPath2id)
{
    Q_ASSERT_X(splinePathId > 0, Q_FUNC_INFO, "splinePathId <= 0");
    Q_ASSERT_X(splPath1id > 0, Q_FUNC_INFO, "spl1id <= 0");
    Q_ASSERT_X(splPath2id > 0, Q_FUNC_INFO, "spl2id <= 0");

    firstSpline = new VSimpleSpline(splPath1id, &currentColor, &factor);
    RefreshSpline(firstSpline, splPath1id, SimpleSplinePoint::ForthPoint);
    firstSpline->setParentItem(this);
    connect(firstSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

    secondSpline = new VSimpleSpline(splPath2id, &currentColor, &factor);
    RefreshSpline(secondSpline, splPath2id, SimpleSplinePoint::FirstPoint);
    secondSpline->setParentItem(this);
    connect(secondSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

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
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setFormula(formula);
    dialogTool->setSplinePathId(splinePathId, id);
    dialogTool->setPointName(point->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
void VToolCutSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->getFormula();
    const quint32 splinePathId = dialogTool->getSplinePathId();
    Create(0, pointName, formula, splinePathId, 5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param formula string with formula length first splinePath.
 * @param splinePathId id of splinePath in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
void VToolCutSplinePath::Create(const quint32 _id, const QString &pointName, QString &formula,
                                const quint32 &splinePathId, const qreal &mx, const qreal &my,
                                VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                const Document &parse, const Source &typeCreation)
{
    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(splinePathId);
    SCASSERT(splPath != nullptr);

    const qreal result = CheckFormula(formula, data);

    quint32 id = _id;
    QPointF spl1p2, spl1p3, spl2p2, spl2p3;
    qint32 p1 = 0, p2 = 0;

    const QPointF point = splPath->CutSplinePath(qApp->toPixel(result), p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
    VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(p);
    }
    else
    {
        data->UpdateGObject(id, p);
    }

    quint32 splPath1id = id + 1;
    quint32 splPath2id = id + 2;

    VSplinePoint splP1 = splPath->at(p1);
    VSplinePoint splP2 = splPath->at(p2);
    const VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->getKCurve());
    const VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->getKCurve());

    VSplinePath *splPath1 = new VSplinePath();
    VSplinePath *splPath2 = new VSplinePath();

    for (qint32 i = 0; i < splPath->CountPoint(); i++)
    {
        if (i <= p1 && i < p2)
        {
            if (i == p1)
            {
                splPath1->append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1()+180, spl1.GetKasm1(),
                                              spl1.GetAngle1()));
                VSplinePoint cutPoint;
                if (typeCreation == Source::FromGui)
                {
                    cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl1.GetAngle2(), spl1.GetAngle2()+180,
                                            spl1.GetAngle2());
                }
                else
                {
                    cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl1.GetAngle2(), spl2.GetKasm1(),
                                            spl1.GetAngle2()+180);
                }
                splPath1->append(cutPoint);
                continue;
            }
            splPath1->append(splPath->at(i));
        }
        else
        {
            if (i == p2)
            {
                const VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl2.GetAngle1()+180,
                                                           spl2.GetKasm1(), spl2.GetAngle1());
                splPath2->append(cutPoint);
                splPath2->append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2(), splP2.KAsm2(),
                                              spl2.GetAngle2()+180));
                continue;
            }
            splPath2->append(splPath->at(i));
        }
    }

    splPath1->setMaxCountPoints(splPath->CountPoint());
    splPath2->setMaxCountPoints(splPath->CountPoint());

    if (typeCreation == Source::FromGui)
    {
        splPath1id = data->AddGObject(splPath1);
        data->AddLengthSpline(splPath1->name(), qApp->fromPixel(splPath1->GetLength()));

        splPath2id = data->AddGObject(splPath2);
        data->AddLengthSpline(splPath2->name(), qApp->fromPixel(splPath2->GetLength()));
    }
    else
    {
        data->UpdateGObject(splPath1id, splPath1);
        data->AddLengthSpline(splPath1->name(), qApp->fromPixel(splPath1->GetLength()));

        data->UpdateGObject(splPath2id, splPath2);
        data->AddLengthSpline(splPath2->name(), qApp->fromPixel(splPath2->GetLength()));

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::CutSplinePathTool, doc);
    if (parse == Document::FullParse)
    {
        VToolCutSplinePath *point = new VToolCutSplinePath(doc, data, id, formula, splinePathId, splPath1id,
                                                           splPath2id, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->AddTool(splPath1id, point);
        doc->AddTool(splPath2id, point);
        doc->IncrementReferens(splinePathId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolCutSplinePath::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splinePathId = domElement.attribute(AttrSplinePath, "").toUInt();
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SplineChoosed send signal about selection splinePath.
 * @param id object id in container.
 */
void VToolCutSplinePath::SplineChoosed(quint32 id)
{
    emit ChoosedTool(id, SceneObject::SplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolCutSplinePath::ChangedActivDraw(const QString &newName)
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
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolCutSplinePath::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSplinePath>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSplinePath>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolCutSplinePath::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrSplinePath, splinePathId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolCutSplinePath::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrSplinePath, splinePathId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolCutSplinePath::RefreshGeometry()
{
    RefreshSpline(firstSpline, splPath1id, SimpleSplinePoint::ForthPoint);
    RefreshSpline(secondSpline, splPath2id, SimpleSplinePoint::FirstPoint);
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement referens value for used objects.
 */
void VToolCutSplinePath::RemoveReferens()
{
    doc->DecrementReferens(splinePathId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutSplinePath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrSplinePath, QString().setNum(dialogTool->getSplinePathId()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshSpline refresh splinePath on scene.
 * @param spline splinePath.
 * @param splPathid splinePath id.
 * @param tr splineType type.
 */
void VToolCutSplinePath::RefreshSpline(VSimpleSpline *spline, quint32 splPathid, SimpleSplinePoint tr)
{
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(splPathid);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    if (tr == SimpleSplinePoint::FirstPoint)
    {
        VSpline spl = splPath->GetSpline(1);
        path.translate(-spl.GetP1().toQPointF().x(), -spl.GetP1().toQPointF().y());
    }
    else
    {
        VSpline spl = splPath->GetSpline(splPath->Count());
        path.translate(-spl.GetP4().toQPointF().x(), -spl.GetP4().toQPointF().y());
    }
    spline->setPath(path);
}

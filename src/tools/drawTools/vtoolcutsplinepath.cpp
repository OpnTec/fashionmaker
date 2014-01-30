/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

const QString VToolCutSplinePath::ToolType = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

VToolCutSplinePath::VToolCutSplinePath(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &formula, const qint64 &splinePathId,
                                       const qint64 &splPath1id, const qint64 &splPath2id,
                                       const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), splinePathId(splinePathId),
      dialogCutSplinePath(QSharedPointer<DialogCutSplinePath>()), firstSpline(), secondSpline(),
      splPath1id (splPath1id), splPath2id(splPath2id)
{
    Q_ASSERT_X(splinePathId > 0, Q_FUNC_INFO, "splinePathId <= 0");
    Q_ASSERT_X(splPath1id > 0, Q_FUNC_INFO, "spl1id <= 0");
    Q_ASSERT_X(splPath2id > 0, Q_FUNC_INFO, "spl2id <= 0");

    firstSpline = new VSimpleSpline(splPath1id, &currentColor, &factor);
    Q_CHECK_PTR(firstSpline);
    RefreshSpline(firstSpline, splPath1id, SimpleSpline::ForthPoint);
    firstSpline->setParentItem(this);
    connect(firstSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

    secondSpline = new VSimpleSpline(splPath2id, &currentColor, &factor);
    Q_CHECK_PTR(secondSpline);
    RefreshSpline(secondSpline, splPath2id, SimpleSpline::FirstPoint);
    secondSpline->setParentItem(this);
    connect(secondSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolCutSplinePath::setDialog()
{
    Q_ASSERT(dialogCutSplinePath.isNull() == false);
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogCutSplinePath->setFormula(formula);
    dialogCutSplinePath->setSplinePathId(splinePathId, id);
    dialogCutSplinePath->setPointName(point->name());
}

void VToolCutSplinePath::Create(QSharedPointer<DialogCutSplinePath> &dialog, VMainGraphicsScene *scene,
                            VDomDocument *doc, VContainer *data)
{
    QString pointName = dialog->getPointName();
    QString formula = dialog->getFormula();
    qint64 splinePathId = dialog->getSplinePathId();
    Create(0, pointName, formula, splinePathId, 5, 10, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolCutSplinePath::Create(const qint64 _id, const QString &pointName, const QString &formula,
                            const qint64 &splinePathId, const qreal &mx, const qreal &my,
                            VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                            const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(splinePathId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF spl1p2, spl1p3, spl2p2, spl2p3;
        qint32 p1 = 0, p2 = 0;
        QPointF point = splPath->CutSplinePath(toPixel(result), p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);

        qint64 id = _id;
        qint64 splPath1id = 0;
        qint64 splPath2id = 0;
        if (typeCreation == Tool::FromGui)
        {
            VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
            Q_CHECK_PTR(p);
            id = data->AddGObject(p);
            splPath1id = id + 1;
            splPath2id = id + 2;

            VSplinePoint splP1 = splPath->at(p1);
            VSplinePoint splP2 = splPath->at(p2);
            VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->getKCurve());
            VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->getKCurve());


            VSplinePath *splPath1 = new VSplinePath();
            Q_CHECK_PTR(splPath1);
            VSplinePath *splPath2 = new VSplinePath();
            Q_CHECK_PTR(splPath2);
            for (qint32 i = 0; i < splPath->CountPoint(); i++)
            {
                if (i <= p1 && i < p2)
                {
                    if (i == p1)
                    {
                        splPath1->append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1(), spl1.GetKasm1()));
                        VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl1.GetAngle2()+180,
                                                             spl2.GetKasm1());
                        splPath1->append(cutPoint);
                        continue;
                    }
                    splPath1->append(splPath->at(i));
                }
                else
                {
                    if (i == p2)
                    {
                        VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl2.GetAngle1(), spl2.GetKasm1());
                        splPath2->append(cutPoint);
                        splPath2->append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2()+180, splP2.KAsm2()));
                        continue;
                    }
                    splPath2->append(splPath->at(i));
                }
            }

            splPath1->setMaxCountPoints(splPath->CountPoint());
            splPath2->setMaxCountPoints(splPath->CountPoint());

            splPath1id = data->AddGObject(splPath1);
            data->AddLengthSpline(splPath1->name(), toMM(splPath1->GetLength()));

            splPath2id = data->AddGObject(splPath2);
            data->AddLengthSpline(splPath2->name(), toMM(splPath2->GetLength()));
        }
        else
        {
            VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
            Q_CHECK_PTR(p);
            data->UpdateGObject(id, p);

            splPath1id = id + 1;
            splPath2id = id + 2;

            VSplinePoint splP1 = splPath->at(p1);
            VSplinePoint splP2 = splPath->at(p2);
            VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->getKCurve());
            VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->getKCurve());

            VSplinePath *splPath1 = new VSplinePath();
            Q_CHECK_PTR(splPath1);
            VSplinePath *splPath2 = new VSplinePath();
            Q_CHECK_PTR(splPath2);
            for (qint32 i = 0; i < splPath->CountPoint(); i++)
            {
                if (i <= p1 && i < p2)
                {
                    if (i == p1)
                    {
                        splPath1->append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1(), spl1.GetKasm1()));
                        VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl1.GetAngle2()+180,
                                                             spl2.GetKasm1());
                        splPath1->append(cutPoint);
                        continue;
                    }
                    splPath1->append(splPath->at(i));
                }
                else
                {
                    if (i == p2)
                    {
                        VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl2.GetAngle1(), spl2.GetKasm1());
                        splPath2->append(cutPoint);
                        splPath2->append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2()+180, splP2.KAsm2()));
                        continue;
                    }
                    splPath2->append(splPath->at(i));
                }
            }

            splPath1->setMaxCountPoints(splPath->CountPoint());
            splPath2->setMaxCountPoints(splPath->CountPoint());

            data->UpdateGObject(splPath1id, splPath1);
            data->AddLengthSpline(splPath1->name(), toMM(splPath1->GetLength()));

            data->UpdateGObject(splPath2id, splPath2);
            data->AddLengthSpline(splPath2->name(), toMM(splPath2->GetLength()));

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
            doc->AddTool(id, point);
            doc->AddTool(splPath1id, point);
            doc->AddTool(splPath2id, point);
            doc->IncrementReferens(splinePathId);
        }
    }
}

void VToolCutSplinePath::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splinePathId = domElement.attribute(AttrSplinePath, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolCutSplinePath::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SetAttribute(domElement, AttrName, dialogCutSplinePath->getPointName());
            SetAttribute(domElement, AttrLength, dialogCutSplinePath->getFormula());
            SetAttribute(domElement, AttrSplinePath, QString().setNum(dialogCutSplinePath->getSplinePathId()));
            emit FullUpdateTree();
        }
    }
    dialogCutSplinePath.clear();
}

void VToolCutSplinePath::SplineChoosed(qint64 id)
{
    emit ChoosedTool(id, Scene::SplinePath);
}

void VToolCutSplinePath::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
        firstSpline->setFlag(QGraphicsItem::ItemIsSelectable, true);
        firstSpline->setAcceptHoverEvents(true);
        secondSpline->setFlag(QGraphicsItem::ItemIsSelectable, true);
        secondSpline->setAcceptHoverEvents(true);
    }
    else
    {
        currentColor = Qt::gray;
        firstSpline->setFlag(QGraphicsItem::ItemIsSelectable, false);
        firstSpline->setAcceptHoverEvents(false);
        secondSpline->setFlag(QGraphicsItem::ItemIsSelectable, false);
        secondSpline->setAcceptHoverEvents(false);
    }
    firstSpline->setPen(QPen(currentColor, widthHairLine/factor));
    secondSpline->setPen(QPen(currentColor, widthHairLine/factor));
    VToolPoint::ChangedActivDraw(newName);
}

void VToolCutSplinePath::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSplinePath, this, event);
}

void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSplinePath, this, event);
}

void VToolCutSplinePath::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    SetAttribute(domElement, AttrLength, formula);
    SetAttribute(domElement, AttrSplinePath, splinePathId);

    AddToCalculation(domElement);
}

void VToolCutSplinePath::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrLength, formula);
        SetAttribute(domElement, AttrSplinePath, splinePathId);
    }
}

void VToolCutSplinePath::RefreshGeometry()
{
    RefreshSpline(firstSpline, splPath1id, SimpleSpline::ForthPoint);
    RefreshSpline(secondSpline, splPath2id, SimpleSpline::FirstPoint);
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

void VToolCutSplinePath::RemoveReferens()
{
    doc->DecrementReferens(splinePathId);
}

void VToolCutSplinePath::RefreshSpline(VSimpleSpline *spline, qint64 splPathid, SimpleSpline::Translation tr)
{
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(splPathid);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    if (tr == SimpleSpline::FirstPoint)
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

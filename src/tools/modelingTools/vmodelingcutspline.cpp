/************************************************************************
 **
 **  @file   vmodelingcutspline.cpp
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

#include "vmodelingcutspline.h"
#include "../../container/calculator.h"

const QString VModelingCutSpline::ToolType = QStringLiteral("cutSpline");
const QString VModelingCutSpline::AttrSpline = QStringLiteral("spline");

VModelingCutSpline::VModelingCutSpline(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &formula, const qint64 &splineId, const qint64 &spl1id,
                                       const qint64 &spl2id, const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), formula(formula), splineId(splineId),
      dialogCutSpline(QSharedPointer<DialogCutSpline>()), firstSpline(), secondSpline(), spl1id(spl1id), spl2id(spl2id)
{
    Q_ASSERT_X(splineId > 0, Q_FUNC_INFO, "splineId <= 0");
    Q_ASSERT_X(spl1id > 0, Q_FUNC_INFO, "spl1id <= 0");
    Q_ASSERT_X(spl2id > 0, Q_FUNC_INFO, "spl2id <= 0");

    firstSpline = new VSimpleSpline(spl1id, &currentColor);
    Q_ASSERT(firstSpline != 0);
    RefreshSpline(firstSpline, spl1id, SimpleSpline::ForthPoint);
    firstSpline->setParentItem(this);
    connect(firstSpline, &VSimpleSpline::Choosed, this, &VModelingCutSpline::SplineChoosed);

    secondSpline = new VSimpleSpline(spl2id, &currentColor);
    Q_ASSERT(secondSpline != 0);
    RefreshSpline(secondSpline, spl2id, SimpleSpline::FirstPoint);
    secondSpline->setParentItem(this);
    connect(secondSpline, &VSimpleSpline::Choosed, this, &VModelingCutSpline::SplineChoosed);
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingCutSpline::setDialog()
{
    Q_ASSERT(dialogCutSpline.isNull() == false);
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    dialogCutSpline->setFormula(formula);
    dialogCutSpline->setSplineId(splineId, id);
    dialogCutSpline->setPointName(point.name());
}

VModelingCutSpline *VModelingCutSpline::Create(QSharedPointer<DialogCutSpline> &dialog,
                                               VDomDocument *doc, VContainer *data)
{
    QString pointName = dialog->getPointName();
    QString formula = dialog->getFormula();
    qint64 splineId = dialog->getSplineId();
    return Create(0, pointName, formula, splineId, 5, 10, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingCutSpline *VModelingCutSpline::Create(const qint64 _id, const QString &pointName, const QString &formula,
                                               const qint64 &splineId, const qreal &mx, const qreal &my,
                                               VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                               const Tool::Sources &typeCreation)
{
    VModelingCutSpline *cutPoint = 0;
    VSpline spl = data->GetSplineModeling(splineId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF spl1p2, spl1p3, spl2p2, spl2p3;
        QPointF point = spl.CutSpline(toPixel(result), spl1p2, spl1p3, spl2p2, spl2p3);

        qint64 splP1id, splP4id;
        Draw::Draws typeObject = spl.getMode();
        if (typeObject == Draw::Calculation)
        {
            splP1id = data->PointConvertToModeling(spl.GetP1());
            splP4id = data->PointConvertToModeling(spl.GetP4());
            if (parse == Document::FullParse)
            {
                doc->IncrementReferens(splP1id);
                doc->IncrementReferens(splP4id);
            }
        }
        else
        {
            splP1id = spl.GetP1();
            splP4id = spl.GetP4();
        }

        qint64 id = _id;
        qint64 spl1id = 0;
        qint64 spl2id = 0;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddPointModeling(VPointF(point.x(), point.y(), pointName, mx, my));

            VSpline spline1 = VSpline(data->DataPointsModeling(), splP1id, spl1p2, spl1p3, id, spl.GetKcurve());
            spl1id = data->AddSplineModeling(spline1);
            data->AddLengthSpline(spline1.name(), toMM(spline1.GetLength()));

            VSpline spline2 = VSpline(data->DataPointsModeling(), id, spl2p2, spl2p3, splP4id, spl.GetKcurve());
            spl2id = data->AddSplineModeling(spline2);
            data->AddLengthSpline(spline2.name(), toMM(spline2.GetLength()));
        }
        else
        {
            data->UpdatePointModeling(id, VPointF(point.x(), point.y(), pointName, mx, my));

            spl1id = id + 1;
            spl2id = id + 2;

            VSpline spline1 = VSpline(data->DataPointsModeling(), splP1id, spl1p2, spl1p3, id, spl.GetKcurve());
            data->UpdateSplineModeling(spl1id, spline1);
            data->AddLengthSpline(spline1.name(), toMM(spline1.GetLength()));

            VSpline spline2 = VSpline(data->DataPointsModeling(), id, spl2p2, spl2p3, splP4id, spl.GetKcurve());
            data->UpdateSplineModeling(spl2id, spline2);
            data->AddLengthSpline(spline2.name(), toMM(spline2.GetLength()));

            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        //VDrawTool::AddRecord(id, Tool::CutSplineTool, doc);
        if (parse == Document::FullParse)
        {
            cutPoint = new VModelingCutSpline(doc, data, id, formula, splineId, spl1id, spl2id, typeCreation);

            doc->AddTool(id, cutPoint);
            doc->AddTool(spl1id, cutPoint);
            doc->AddTool(spl2id, cutPoint);
            doc->IncrementReferens(splineId);
        }
    }
    return cutPoint;
}

void VModelingCutSpline::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splineId = domElement.attribute(AttrSpline, "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingCutSpline::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogCutSpline->getPointName());
            domElement.setAttribute(AttrLength, dialogCutSpline->getFormula());
            domElement.setAttribute(AttrSpline, QString().setNum(dialogCutSpline->getSplineId()));
            emit FullUpdateTree();
        }
    }
    dialogCutSpline.clear();
}

void VModelingCutSpline::SplineChoosed(qint64 id)
{
    emit ChoosedTool(id, Scene::Spline);
}

void VModelingCutSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSpline, this, event);
}

void VModelingCutSpline::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrSpline, splineId);

    AddToModeling(domElement);
}

void VModelingCutSpline::RefreshGeometry()
{
    RefreshSpline(firstSpline, spl1id, SimpleSpline::ForthPoint);
    RefreshSpline(secondSpline, spl2id, SimpleSpline::FirstPoint);
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPointModeling(id));
}


void VModelingCutSpline::RefreshSpline(VSimpleSpline *spline, qint64 splid, SimpleSpline::Translation tr)
{
    VSpline spl = VModelingTool::data.GetSplineModeling(splid);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    if(tr == SimpleSpline::FirstPoint)
    {
        path.translate(-spl.GetPointP1().toQPointF().x(), -spl.GetPointP1().toQPointF().y());
    }
    else
    {
        path.translate(-spl.GetPointP4().toQPointF().x(), -spl.GetPointP4().toQPointF().y());
    }
    spline->setPath(path);
}

/************************************************************************
 **
 **  @file   vtoolshoulderpoint.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#include "vtoolshoulderpoint.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogshoulderpoint.h"

const QString VToolShoulderPoint::ToolType = QStringLiteral("shoulder");

VToolShoulderPoint::VToolShoulderPoint(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                                       const QString &formula, const quint32 &p1Line, const quint32 &p2Line,
                                       const quint32 &pShoulder, const Valentina::Sources &typeCreation,
                                       QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line), pShoulder(pShoulder)
{
    if (typeCreation == Valentina::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolShoulderPoint::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setP1Line(basePointId, id);
    dialogTool->setP2Line(p2Line, id);
    dialogTool->setPShoulder(pShoulder, id);
    dialogTool->setPointName(p->name());
}

QPointF VToolShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                      const qreal &length)
{
    QLineF line = QLineF(p1Line, p2Line);
    qreal toolLength = length;
    qreal dist = line.length();
    if (dist>toolLength)
    {
        qWarning()<<"Correction of length in shoulder point tool. Parameter length too small.";
        toolLength = dist;
    }
    if (qFuzzyCompare(dist, toolLength))
    {
        return line.p2();
    }
    qreal step = 0.01;
    while (1)
    {
        line.setLength(line.length()+step);
        QLineF line2 = QLineF(pShoulder, line.p2());
        if (line2.length()>=toolLength)
        {
            return line.p2();
        }
    }
}

void VToolShoulderPoint::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString formula = dialogTool->getFormula();
    quint32 p1Line = dialogTool->getP1Line();
    quint32 p2Line = dialogTool->getP2Line();
    quint32 pShoulder = dialogTool->getPShoulder();
    QString typeLine = dialogTool->getTypeLine();
    QString pointName = dialogTool->getPointName();
    Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Valentina::FromGui);
}

void VToolShoulderPoint::Create(const quint32 _id, const QString &formula, const quint32 &p1Line,
                                const quint32 &p2Line, const quint32 &pShoulder, const QString &typeLine,
                                const QString &pointName, const qreal &mx, const qreal &my,
                                VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(p1Line);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(p2Line);
    const VPointF *shoulderPoint = data->GeometricObject<const VPointF *>(pShoulder);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF fPoint = VToolShoulderPoint::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                                       shoulderPoint->toQPointF(), qApp->toPixel(result));
        quint32 id =  _id;
        if (typeCreation == Valentina::FromGui)
        {
            id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line, id);
            data->AddLine(p2Line, id);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line, id);
            data->AddLine(p2Line, id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Valentina::ShoulderPointTool, doc);
        if (parse == Document::FullParse)
        {
            VToolShoulderPoint *point = new VToolShoulderPoint(doc, data, id, typeLine, formula,
                                                               p1Line, p2Line, pShoulder,
                                                               typeCreation);
            scene->addItem(point);
            connect(point, &VToolShoulderPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolShoulderPoint::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line);
            doc->IncrementReferens(p2Line);
            doc->IncrementReferens(pShoulder);
        }
    }
}

void VToolShoulderPoint::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrP1Line, "").toUInt();
        p2Line = domElement.attribute(AttrP2Line, "").toUInt();
        pShoulder = domElement.attribute(AttrPShoulder, "").toUInt();
    }
    RefreshGeometry();
}

void VToolShoulderPoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolShoulderPoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogShoulderPoint>(this, event);
}

void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogShoulderPoint>(this, event);
}

void VToolShoulderPoint::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrP1Line, basePointId);
    doc->SetAttribute(domElement, AttrP2Line, p2Line);
    doc->SetAttribute(domElement, AttrPShoulder, pShoulder);

    AddToCalculation(domElement);
}

void VToolShoulderPoint::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrName, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrName, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrP1Line, basePointId);
        doc->SetAttribute(domElement, AttrP2Line, p2Line);
        doc->SetAttribute(domElement, AttrPShoulder, pShoulder);
    }
}

void VToolShoulderPoint::RemoveReferens()
{
    doc->DecrementReferens(p2Line);
    doc->DecrementReferens(pShoulder);
    VToolLinePoint::RemoveReferens();
}

void VToolShoulderPoint::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogShoulderPoint *dialogTool = qobject_cast<DialogShoulderPoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->getP1Line()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->getP2Line()));
    doc->SetAttribute(domElement, AttrPShoulder, QString().setNum(dialogTool->getPShoulder()));
}

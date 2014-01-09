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

const QString VToolShoulderPoint::ToolType = QStringLiteral("shoulder");

VToolShoulderPoint::VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                                       const qint64 &p2Line, const qint64 &pShoulder, const Tool::Sources &typeCreation,
                                       QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line), pShoulder(pShoulder),
    dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>())
{
    if (typeCreation == Tool::FromGui)
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
    Q_ASSERT(dialogShoulderPoint.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogShoulderPoint->setTypeLine(typeLine);
    dialogShoulderPoint->setFormula(formula);
    dialogShoulderPoint->setP1Line(basePointId, id);
    dialogShoulderPoint->setP2Line(p2Line, id);
    dialogShoulderPoint->setPShoulder(pShoulder, id);
    dialogShoulderPoint->setPointName(p->name());
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

void VToolShoulderPoint::Create(QSharedPointer<DialogShoulderPoint> &dialog, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data)
{
    QString formula = dialog->getFormula();
    qint64 p1Line = dialog->getP1Line();
    qint64 p2Line = dialog->getP2Line();
    qint64 pShoulder = dialog->getPShoulder();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolShoulderPoint::Create(const qint64 _id, const QString &formula, const qint64 &p1Line,
                                const qint64 &p2Line, const qint64 &pShoulder, const QString &typeLine,
                                const QString &pointName, const qreal &mx, const qreal &my,
                                VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                const Document::Documents &parse, const Tool::Sources &typeCreation)
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
                                                       shoulderPoint->toQPointF(), toPixel(result));
        qint64 id =  _id;
        if (typeCreation == Tool::FromGui)
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
        VDrawTool::AddRecord(id, Tool::ShoulderPointTool, doc);
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
        basePointId = domElement.attribute(AttrP1Line, "").toLongLong();
        p2Line = domElement.attribute(AttrP2Line, "").toLongLong();
        pShoulder = domElement.attribute(AttrPShoulder, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolShoulderPoint::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogShoulderPoint->getPointName());
            domElement.setAttribute(AttrTypeLine, dialogShoulderPoint->getTypeLine());
            domElement.setAttribute(AttrLength, dialogShoulderPoint->getFormula());
            domElement.setAttribute(AttrP1Line, QString().setNum(dialogShoulderPoint->getP1Line()));
            domElement.setAttribute(AttrP2Line, QString().setNum(dialogShoulderPoint->getP2Line()));
            domElement.setAttribute(AttrPShoulder, QString().setNum(dialogShoulderPoint->getPShoulder()));
            emit FullUpdateTree();
        }
    }
    dialogShoulderPoint.clear();
}

void VToolShoulderPoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolShoulderPoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogShoulderPoint, this, event);
}

void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogShoulderPoint, this, event);
}

void VToolShoulderPoint::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point->name());
    AddAttribute(domElement, AttrMx, toMM(point->mx()));
    AddAttribute(domElement, AttrMy, toMM(point->my()));

    AddAttribute(domElement, AttrTypeLine, typeLine);
    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrP1Line, basePointId);
    AddAttribute(domElement, AttrP2Line, p2Line);
    AddAttribute(domElement, AttrPShoulder, pShoulder);

    AddToCalculation(domElement);
}

void VToolShoulderPoint::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrName, point->name());
        domElement.setAttribute(AttrName, toMM(point->mx()));
        domElement.setAttribute(AttrName, toMM(point->my()));
        domElement.setAttribute(AttrTypeLine, typeLine);
        domElement.setAttribute(AttrLength, formula);
        domElement.setAttribute(AttrP1Line, basePointId);
        domElement.setAttribute(AttrP2Line, p2Line);
        domElement.setAttribute(AttrPShoulder, pShoulder);
    }
}

void VToolShoulderPoint::RemoveReferens()
{
    doc->DecrementReferens(p2Line);
    doc->DecrementReferens(pShoulder);
    VToolLinePoint::RemoveReferens();
}

/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vmodelingshoulderpoint.h"
#include "../drawTools/vtoolshoulderpoint.h"
#include "../../container/calculator.h"

const QString VModelingShoulderPoint::ToolType = QStringLiteral("shoulder");

VModelingShoulderPoint::VModelingShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                                       const qint64 &p2Line, const qint64 &pShoulder, const Tool::Sources &typeCreation,
                                       QGraphicsItem * parent)
    :VModelingLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line),
    pShoulder(pShoulder), dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>())
{
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingShoulderPoint::setDialog()
{
    Q_ASSERT(dialogShoulderPoint.isNull() == false);
    VPointF p = VAbstractTool::data.GetModelingPoint(id);
    dialogShoulderPoint->setTypeLine(typeLine);
    dialogShoulderPoint->setFormula(formula);
    dialogShoulderPoint->setP1Line(basePointId, id);
    dialogShoulderPoint->setP2Line(p2Line, id);
    dialogShoulderPoint->setPShoulder(pShoulder, id);
    dialogShoulderPoint->setPointName(p.name());
}

VModelingShoulderPoint *VModelingShoulderPoint::Create(QSharedPointer<DialogShoulderPoint> &dialog,
                                                       VDomDocument *doc, VContainer *data)
{
    QString formula = dialog->getFormula();
    qint64 p1Line = dialog->getP1Line();
    qint64 p2Line = dialog->getP2Line();
    qint64 pShoulder = dialog->getPShoulder();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    return Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingShoulderPoint *VModelingShoulderPoint::Create(const qint64 _id, const QString &formula,
                                                       const qint64 &p1Line, const qint64 &p2Line,
                                                       const qint64 &pShoulder, const QString &typeLine,
                                                       const QString &pointName, const qreal &mx,
                                                       const qreal &my, VDomDocument *doc, VContainer *data,
                                                       const Document::Documents &parse,
                                                       const Tool::Sources &typeCreation)
{
    VModelingShoulderPoint *point = 0;
    VPointF firstPoint = data->GetModelingPoint(p1Line);
    VPointF secondPoint = data->GetModelingPoint(p2Line);
    VPointF shoulderPoint = data->GetModelingPoint(pShoulder);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF fPoint = VToolShoulderPoint::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                       shoulderPoint.toQPointF(), toPixel(result));
        qint64 id =  _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddModelingPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        }
        else
        {
            data->UpdateModelingPoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLine(p1Line, id, Draw::Modeling);
        data->AddLine(p2Line, id, Draw::Modeling);
        if (parse == Document::FullParse)
        {
            point = new VModelingShoulderPoint(doc, data, id, typeLine, formula, p1Line, p2Line, pShoulder,
                                               typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line);
            doc->IncrementReferens(p2Line);
            doc->IncrementReferens(pShoulder);
        }
    }
    return point;
}

void VModelingShoulderPoint::FullUpdateFromFile()
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

void VModelingShoulderPoint::FullUpdateFromGui(int result)
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

void VModelingShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogShoulderPoint, this, event);
}

void VModelingShoulderPoint::AddToFile()
{
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrTypeLine, typeLine);
    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrP1Line, basePointId);
    AddAttribute(domElement, AttrP2Line, p2Line);
    AddAttribute(domElement, AttrPShoulder, pShoulder);

    AddToModeling(domElement);
}

void VModelingShoulderPoint::RemoveReferens()
{
    doc->DecrementReferens(p2Line);
    doc->DecrementReferens(pShoulder);
    VModelingLinePoint::RemoveReferens();
}

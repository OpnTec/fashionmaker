/************************************************************************
 **
 **  @file   vmodelingheight.cpp
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

#include "vmodelingheight.h"
#include "../drawTools/vtoolheight.h"

const QString VModelingHeight::ToolType = QStringLiteral("height");

VModelingHeight::VModelingHeight(VDomDocument *doc, VContainer *data, const qint64 &id,
                                 const QString &typeLine, const qint64 &basePointId, const qint64 &p1LineId,
                                 const qint64 &p2LineId, const Tool::Sources &typeCreation,
                                 QGraphicsItem * parent)
    :VModelingLinePoint(doc, data, id, typeLine, QString(), basePointId, 0, parent),
      dialogHeight(QSharedPointer<DialogHeight>()), p1LineId(p1LineId), p2LineId(p2LineId)
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingHeight::setDialog()
{
    Q_ASSERT(dialogHeight.isNull() == false);
    VPointF p = VAbstractTool::data.GetPointModeling(id);
    dialogHeight->setTypeLine(typeLine);
    dialogHeight->setBasePointId(basePointId, id);
    dialogHeight->setP1LineId(p1LineId, id);
    dialogHeight->setP2LineId(p2LineId, id);
    dialogHeight->setPointName(p.name());
}

VModelingHeight *VModelingHeight::Create(QSharedPointer<DialogHeight> &dialog, VDomDocument *doc, VContainer *data)
{
    disconnect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &DialogHeight::UpdateList);
    QString pointName = dialog->getPointName();
    QString typeLine = dialog->getTypeLine();
    qint64 basePointId = dialog->getBasePointId();
    qint64 p1LineId = dialog->getP1LineId();
    qint64 p2LineId = dialog->getP2LineId();
    return Create(0, pointName, typeLine, basePointId, p1LineId, p2LineId, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingHeight *VModelingHeight::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                         const qint64 &basePointId, const qint64 &p1LineId,
                                         const qint64 &p2LineId, const qreal &mx, const qreal &my,
                                         VDomDocument *doc, VContainer *data,
                                         const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VModelingHeight *point = 0;
    VPointF basePoint = data->GetPointModeling(basePointId);
    VPointF p1Line = data->GetPointModeling(p1LineId);
    VPointF p2Line = data->GetPointModeling(p2LineId);

    QPointF pHeight = VToolHeight::FindPoint(QLineF(p1Line.toQPointF(), p2Line.toQPointF()),
                                             basePoint.toQPointF());
    QLineF line = QLineF(basePoint.toQPointF(), pHeight);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddPointModeling(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
    }
    else
    {
        data->UpdatePointModeling(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLine(basePointId, id, Draw::Modeling);
    if (parse == Document::FullParse)
    {
        point = new VModelingHeight(doc, data, id, typeLine, basePointId, p1LineId, p2LineId, typeCreation);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(p1LineId);
        doc->IncrementReferens(p2LineId);
    }
    return point;
}

void VModelingHeight::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toLongLong();
        p1LineId = domElement.attribute(AttrP1Line, "").toLongLong();
        p2LineId = domElement.attribute(AttrP2Line, "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingHeight::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogHeight->getPointName());
            domElement.setAttribute(AttrTypeLine, dialogHeight->getTypeLine());
            domElement.setAttribute(AttrBasePoint, QString().setNum(dialogHeight->getBasePointId()));
            domElement.setAttribute(AttrP1Line, QString().setNum(dialogHeight->getP1LineId()));
            domElement.setAttribute(AttrP2Line, QString().setNum(dialogHeight->getP2LineId()));
            emit FullUpdateTree();
        }
    }
    dialogHeight.clear();
}

void VModelingHeight::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogHeight, this, event);
}

void VModelingHeight::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrTypeLine, typeLine);
    AddAttribute(domElement, AttrBasePoint, basePointId);
    AddAttribute(domElement, AttrP1Line, p1LineId);
    AddAttribute(domElement, AttrP2Line, p2LineId);

    AddToModeling(domElement);
}

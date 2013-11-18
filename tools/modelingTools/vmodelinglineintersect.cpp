/************************************************************************
 **
 **  @file   vmodelinglineintersect.cpp
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

#include "vmodelinglineintersect.h"

const QString VModelingLineIntersect::ToolType = QStringLiteral("lineIntersect");

VModelingLineIntersect::VModelingLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const qint64 &p1Line1, const qint64 &p2Line1, const qint64 &p1Line2,
                                       const qint64 &p2Line2, const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2), dialogLineIntersect(QSharedPointer<DialogLineIntersect>())
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingLineIntersect::setDialog()
{
    Q_ASSERT(dialogLineIntersect.isNull() == false);
    VPointF p = VAbstractTool::data.GetPointModeling(id);
    dialogLineIntersect->setP1Line1(p1Line1);
    dialogLineIntersect->setP2Line1(p2Line1);
    dialogLineIntersect->setP1Line2(p1Line2);
    dialogLineIntersect->setP2Line2(p2Line2);
    dialogLineIntersect->setPointName(p.name());
}

VModelingLineIntersect *VModelingLineIntersect::Create(QSharedPointer<DialogLineIntersect> &dialog,
                                                       VDomDocument *doc, VContainer *data)
{
    qint64 p1Line1Id = dialog->getP1Line1();
    qint64 p2Line1Id = dialog->getP2Line1();
    qint64 p1Line2Id = dialog->getP1Line2();
    qint64 p2Line2Id = dialog->getP2Line2();
    QString pointName = dialog->getPointName();
    return Create(0, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, pointName, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingLineIntersect *VModelingLineIntersect::Create(const qint64 _id, const qint64 &p1Line1Id,
                                                       const qint64 &p2Line1Id, const qint64 &p1Line2Id,
                                                       const qint64 &p2Line2Id, const QString &pointName,
                                                       const qreal &mx, const qreal &my, VDomDocument *doc,
                                                       VContainer *data, const Document::Documents &parse,
                                                       const Tool::Sources &typeCreation)
{
    VModelingLineIntersect *point = 0;
    VPointF p1Line1 = data->GetPointModeling(p1Line1Id);
    VPointF p2Line1 = data->GetPointModeling(p2Line1Id);
    VPointF p1Line2 = data->GetPointModeling(p1Line2Id);
    VPointF p2Line2 = data->GetPointModeling(p2Line2Id);

    QLineF line1(p1Line1.toQPointF(), p2Line1.toQPointF());
    QLineF line2(p1Line2.toQPointF(), p2Line2.toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        qint64 id = _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddPointModeling(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        }
        else
        {
            data->UpdatePointModeling(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLine(p1Line1Id, id, Draw::Modeling);
        data->AddLine(id, p2Line1Id, Draw::Modeling);
        data->AddLine(p1Line2Id, id, Draw::Modeling);
        data->AddLine(id, p2Line2Id, Draw::Modeling);
        if (parse == Document::FullParse)
        {
            point = new VModelingLineIntersect(doc, data, id, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id,
                                               typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line1Id);
            doc->IncrementReferens(p2Line1Id);
            doc->IncrementReferens(p1Line2Id);
            doc->IncrementReferens(p2Line2Id);
        }
    }
    return point;
}

void VModelingLineIntersect::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        p1Line1 = domElement.attribute(AttrP1Line1, "").toLongLong();
        p2Line1 = domElement.attribute(AttrP2Line1, "").toLongLong();
        p1Line2 = domElement.attribute(AttrP1Line2, "").toLongLong();
        p2Line2 = domElement.attribute(AttrP2Line2, "").toLongLong();
    }
    RefreshPointGeometry(VAbstractTool::data.GetPointModeling(id));
}

void VModelingLineIntersect::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogLineIntersect->getPointName());
            domElement.setAttribute(AttrP1Line1, QString().setNum(dialogLineIntersect->getP1Line1()));
            domElement.setAttribute(AttrP2Line1, QString().setNum(dialogLineIntersect->getP2Line1()));
            domElement.setAttribute(AttrP1Line2, QString().setNum(dialogLineIntersect->getP1Line2()));
            domElement.setAttribute(AttrP2Line2, QString().setNum(dialogLineIntersect->getP2Line2()));
            emit FullUpdateTree();
        }
    }
    dialogLineIntersect.clear();
}

void VModelingLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogLineIntersect, this, event);
}

void VModelingLineIntersect::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrP1Line1, p1Line1);
    AddAttribute(domElement, AttrP2Line1, p2Line1);
    AddAttribute(domElement, AttrP1Line2, p1Line2);
    AddAttribute(domElement, AttrP2Line2, p2Line2);

    AddToModeling(domElement);
}

void VModelingLineIntersect::RemoveReferens()
{
    doc->DecrementReferens(p1Line1);
    doc->DecrementReferens(p2Line1);
    doc->DecrementReferens(p1Line2);
    doc->DecrementReferens(p2Line2);
}

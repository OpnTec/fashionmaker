/************************************************************************
 **
 **  @file   vmodelingarc.cpp
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

#include "vmodelingarc.h"
#include "../../container/calculator.h"

const QString VModelingArc::TagName = QStringLiteral("arc");
const QString VModelingArc::ToolType = QStringLiteral("simple");

VModelingArc::VModelingArc(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                           QGraphicsItem *parent)
    :VModelingTool(doc, data, id), QGraphicsPathItem(parent), dialogArc(QSharedPointer<DialogArc>())
{
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshGeometry();

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingArc::setDialog()
{
    Q_ASSERT(dialogArc.isNull() == false);
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    dialogArc->SetCenter(arc.GetCenter());
    dialogArc->SetRadius(arc.GetFormulaRadius());
    dialogArc->SetF1(arc.GetFormulaF1());
    dialogArc->SetF2(arc.GetFormulaF2());
}

VModelingArc* VModelingArc::Create(QSharedPointer<DialogArc> &dialog, VDomDocument *doc, VContainer *data)
{
    qint64 center = dialog->GetCenter();
    QString radius = dialog->GetRadius();
    QString f1 = dialog->GetF1();
    QString f2 = dialog->GetF2();
    return Create(0, center, radius, f1, f2, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingArc* VModelingArc::Create(const qint64 _id, const qint64 &center, const QString &radius, const QString &f1,
                                   const QString &f2, VDomDocument *doc, VContainer *data,
                                   const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VModelingArc *toolArc = 0;
    qreal calcRadius = 0, calcF1 = 0, calcF2 = 0;

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcRadius = toPixel(result);
    }

    errorMsg.clear();
    result = cal.eval(f1, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcF1 = result;
    }

    errorMsg.clear();
    result = cal.eval(f2, &errorMsg);
    if (errorMsg.isEmpty())
    {
        calcF2 = result;
    }

    VArc arc = VArc(data->DataModelingPoints(), center, calcRadius, radius, calcF1, f1, calcF2, f2 );
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddModelingArc(arc);
    }
    else
    {
        data->UpdateModelingArc(id, arc);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLengthArc(arc.name(), toMM(arc.GetLength()));
    if (parse == Document::FullParse)
    {
        toolArc = new VModelingArc(doc, data, id, typeCreation);
        doc->AddTool(id, toolArc);
        doc->IncrementReferens(center);
    }
    return toolArc;
}

void VModelingArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VModelingArc::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrCenter, QString().setNum(dialogArc->GetCenter()));
            domElement.setAttribute(AttrRadius, dialogArc->GetRadius());
            domElement.setAttribute(AttrAngle1, dialogArc->GetF1());
            domElement.setAttribute(AttrAngle2, dialogArc->GetF2());
            emit FullUpdateTree();
        }
    }
    dialogArc.clear();
}

void VModelingArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogArc, this, event);
}

void VModelingArc::AddToFile()
{
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrCenter, arc.GetCenter());
    AddAttribute(domElement, AttrRadius, arc.GetFormulaRadius());
    AddAttribute(domElement, AttrAngle1, arc.GetFormulaF1());
    AddAttribute(domElement, AttrAngle2, arc.GetFormulaF2());

    AddToModeling(domElement);
}

void VModelingArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingArc::RemoveReferens()
{
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    doc->DecrementReferens(arc.GetCenter());
}

void VModelingArc::RefreshGeometry()
{
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}

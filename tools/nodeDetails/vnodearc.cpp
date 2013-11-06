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

#include "vnodearc.h"

const QString VNodeArc::TagName = QStringLiteral("arc");
const QString VNodeArc::ToolType = QStringLiteral("modeling");

VNodeArc::VNodeArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Draws typeobject,
                   const Tool::Sources &typeCreation, QGraphicsItem * parent)
    :VAbstractNode(doc, data, id, idArc, typeobject), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VNodeArc::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Draws typeobject,
                      const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    if (parse == Document::FullParse)
    {
        VNodeArc *arc = new VNodeArc(doc, data, id, idArc, typeobject, typeCreation);
        Q_ASSERT(arc != 0);
        doc->AddTool(id, arc);
        doc->IncrementReferens(idArc);
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

void VNodeArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VNodeArc::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrIdObject, idNode);
    if (typeobject == Draw::Calculation)
    {
        AddAttribute(domElement, AttrTypeObject, TypeObjectCalculation);
    }
    else
    {
        AddAttribute(domElement, AttrTypeObject, ToolType );
    }

    AddToModeling(domElement);
}

void VNodeArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeArc::RefreshGeometry()
{
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}

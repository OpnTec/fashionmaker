/************************************************************************
 **
 **  @file   vnodesplinepath.cpp
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

#include "vnodesplinepath.h"

#include <QtWidgets>

const QString VNodeSplinePath::TagName = QStringLiteral("spline");
const QString VNodeSplinePath::ToolType = QStringLiteral("modelingPath");

VNodeSplinePath::VNodeSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                 const Tool::Sources &typeCreation, QGraphicsItem * parent)
    :VAbstractNode(doc, data, id, idSpline), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VNodeSplinePath::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                             const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    if (parse == Document::FullParse)
    {
        VNodeSplinePath *splPath = new VNodeSplinePath(doc, data, id, idSpline, typeCreation);
        Q_ASSERT(splPath != 0);
        doc->AddTool(id, splPath);
        VSplinePath path = data->GetSplinePath(id);
        const QVector<VSplinePoint> *points = path.GetPoint();
        for (qint32 i = 0; i<points->size(); ++i)
        {
            doc->IncrementReferens(points->at(i).P());
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

void VNodeSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VNodeSplinePath::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrIdObject, idNode);

    AddToModeling(domElement);
}

void VNodeSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeSplinePath::RefreshGeometry()
{
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}

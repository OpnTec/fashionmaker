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
                                 const Tool::Sources &typeCreation, const qint64 &idTool, QObject *qoParent,
                                 QGraphicsItem * parent)
    :VAbstractNode(doc, data, id, idSpline, idTool, qoParent), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VNodeSplinePath::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                             const Document::Documents &parse, const Tool::Sources &typeCreation, const qint64 &idTool,
                             QObject *parent)
{
    VAbstractTool::AddRecord(id, Tool::NodeSplinePath, doc);
    if (parse == Document::FullParse)
    {
        VNodeSplinePath *splPath = new VNodeSplinePath(doc, data, id, idSpline, typeCreation, idTool, parent);
        Q_CHECK_PTR(splPath);
        doc->AddTool(id, splPath);
        const VSplinePath *path = data->GeometricObject<const VSplinePath *>(id);
        const QVector<VSplinePoint> *points = path->GetPoint();
        for (qint32 i = 0; i<points->size(); ++i)
        {
            if (idTool != 0)
            {
                doc->IncrementReferens(idTool);
                //Some nodes we don't show on scene. Tool that create this nodes must free memory.
                VDataTool *tool = doc->getTool(idTool);
                Q_CHECK_PTR(tool);
                splPath->setParent(tool);
            }
            else
            {
                doc->IncrementReferens(points->at(i).P().id());
            }
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

void VNodeSplinePath::DeleteNode()
{
    VAbstractNode::DeleteNode();
    this->setVisible(false);
}

void VNodeSplinePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VNodeSplinePath::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != 0)
    {
        SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

void VNodeSplinePath::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrIdObject, QString().setNum(idNode));
        if (idTool != 0)
        {
            SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
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
    const VSplinePath *splPath = VAbstractTool::data.GeometricObject<const VSplinePath *>(id);
    QPainterPath path;
    path.addPath(splPath->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}

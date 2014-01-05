/************************************************************************
 **
 **  @file   vtooldetail.cpp
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

#include "vtooldetail.h"
#include "nodeDetails/nodedetails.h"

const QString VToolDetail::TagName          = QStringLiteral("detail");
const QString VToolDetail::TagNode          = QStringLiteral("node");
const QString VToolDetail::AttrSupplement   = QStringLiteral("supplement");
const QString VToolDetail::AttrClosed       = QStringLiteral("closed");
const QString VToolDetail::AttrWidth        = QStringLiteral("width");
const QString VToolDetail::AttrIdObject     = QStringLiteral("idObject");
const QString VToolDetail::AttrNodeType     = QStringLiteral("nodeType");
const QString VToolDetail::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolDetail::NodeTypeModeling = QStringLiteral("Modeling");

VToolDetail::VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, const Tool::Sources &typeCreation,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(parent), dialogDetail(QSharedPointer<DialogDetail>()),
      sceneDetails(scene)
{
    VDetail detail = data->GetDetail(id);
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail[i].getTypeTool())
        {
            case (Tool::NodePoint):
                InitTool<VNodePoint>(scene, detail[i]);
                break;
            case (Tool::NodeArc):
                InitTool<VNodeArc>(scene, detail[i]);
                break;
            case (Tool::NodeSpline):
                InitTool<VNodeSpline>(scene, detail[i]);
                break;
            case (Tool::NodeSplinePath):
                InitTool<VNodeSplinePath>(scene, detail[i]);
                break;
            default:
                qWarning()<<"Get wrong tool type. Ignore.";
                break;
        }
        doc->IncrementReferens(detail[i].getId());
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();
    this->setPos(detail.getMx(), detail.getMy());
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    if (typeCreation == Tool::FromGui || typeCreation == Tool::FromTool)
    {
       AddToFile();
    }
}

void VToolDetail::setDialog()
{
    Q_ASSERT(dialogDetail.isNull() == false);
    VDetail detail = VAbstractTool::data.GetDetail(id);
    dialogDetail->setDetails(detail);
}

void VToolDetail::Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data)
{
    VDetail detail = dialog->getDetails();
    VDetail det;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        qint64 id = 0;
        switch (detail[i].getTypeTool())
        {
            case (Tool::NodePoint):
            {
                id = CreateNode<VPointF>(data, detail[i].getId());
                VNodePoint::Create(doc, data, id, detail[i].getId(), Document::FullParse, Tool::FromGui);
            }
            break;
            case (Tool::NodeArc):
            {
                id = CreateNode<VArc>(data, detail[i].getId());
                VNodeArc::Create(doc, data, id, detail[i].getId(), Document::FullParse, Tool::FromGui);
            }
            break;
            case (Tool::NodeSpline):
            {
                id = CreateNode<VSpline>(data, detail[i].getId());
                VNodeSpline::Create(doc, data, id, detail[i].getId(), Document::FullParse, Tool::FromGui);
            }
            break;
            case (Tool::NodeSplinePath):
            {
                id = CreateNode<VSplinePath>(data, detail[i].getId());
                VNodeSplinePath::Create(doc, data, id, detail[i].getId(), Document::FullParse, Tool::FromGui);
            }
            break;
            default:
                qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        VNodeDetail node(id, detail[i].getTypeTool(), NodeDetail::Contour);
        det.append(node);
    }
    det.setName(detail.getName());
    Create(0, det, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolDetail::Create(const qint64 &_id, const VDetail &newDetail, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    qint64 id = _id;
    if (typeCreation == Tool::FromGui || typeCreation == Tool::FromTool)
    {
        id = data->AddDetail(newDetail);
    }
    else
    {
        data->UpdateDetail(id, newDetail);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::Detail, doc);
    if (parse == Document::FullParse)
    {
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        QHash<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, detail);
    }
}

void VToolDetail::Remove()
{
    DeleteTool(this);
}

void VToolDetail::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolDetail::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            VDetail det = dialogDetail->getDetails();
            domElement.setAttribute(AttrName, det.getName());
            domElement.setAttribute(AttrSupplement, QString().setNum(det.getSupplement()));
            domElement.setAttribute(AttrClosed, QString().setNum(det.getClosed()));
            domElement.setAttribute(AttrWidth, QString().setNum(det.getWidth()));
            RemoveAllChild(domElement);
            for (ptrdiff_t i = 0; i < det.CountNode(); ++i)
            {
               AddNode(domElement, det[i]);
            }
            emit FullUpdateTree();
        }
    }
    dialogDetail.clear();
}

void VToolDetail::AddToFile()
{
    VDetail detail = VAbstractTool::data.GetDetail(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrName, detail.getName());
    AddAttribute(domElement, AttrMx, toMM(detail.getMx()));
    AddAttribute(domElement, AttrMy, toMM(detail.getMy()));
    AddAttribute(domElement, AttrSupplement, detail.getSupplement());
    AddAttribute(domElement, AttrClosed, detail.getClosed());
    AddAttribute(domElement, AttrWidth, detail.getWidth());

    for (ptrdiff_t i = 0; i < detail.CountNode(); ++i)
    {
       AddNode(domElement, detail.at(i));
    }

    QDomElement element;
    bool ok = doc->GetActivDetailsElement(element);
    if (ok)
    {
        element.appendChild(domElement);
    }
}

void VToolDetail::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        VDetail det = VAbstractTool::data.GetDetail(id);
        domElement.setAttribute(AttrName, det.getName());
        domElement.setAttribute(AttrSupplement, QString().setNum(det.getSupplement()));
        domElement.setAttribute(AttrClosed, QString().setNum(det.getClosed()));
        domElement.setAttribute(AttrWidth, QString().setNum(det.getWidth()));
        RemoveAllChild(domElement);
        for (ptrdiff_t i = 0; i < det.CountNode(); ++i)
        {
           AddNode(domElement, det.at(i));
        }
    }
}

QVariant VToolDetail::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        // value - this is new position.
        QPointF newPos = value.toPointF();
        //qDebug()<<newPos;
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrMx, QString().setNum(toMM(newPos.x())));
            domElement.setAttribute(AttrMy, QString().setNum(toMM(newPos.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }

    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void VToolDetail::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

void VToolDetail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Detail);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolDetail::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *actionOption = menu.addAction(tr("Options"));
    QAction *actionRemove = menu.addAction(tr("Delete"));
    if (_referens > 1)
    {
        actionRemove->setEnabled(false);
    }
    else
    {
        actionRemove->setEnabled(true);
    }
    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        dialogDetail = QSharedPointer<DialogDetail>(new DialogDetail(getData()));
        connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                dialogDetail.data(), &DialogDetail::ChoosedObject);
        connect(dialogDetail.data(), &DialogDetail::DialogClosed, this, &VToolDetail::FullUpdateFromGui);
        setDialog();
        dialogDetail->show();
    }
    if (selectedAction == actionRemove)
    {
        DeleteTool(this);
    }
}

void VToolDetail::RemoveReferens()
{
    VDetail detail = VAbstractTool::data.GetDetail(id);
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        doc->DecrementReferens(detail.at(i).getId());
    }
}

void VToolDetail::AddNode(QDomElement &domElement, const VNodeDetail &node)
{
    QDomElement nod = doc->createElement(TagNode);

    AddAttribute(nod, AttrIdObject, node.getId());
    AddAttribute(nod, AttrMx, toMM(node.getMx()));
    AddAttribute(nod, AttrMy, toMM(node.getMy()));
    if (node.getTypeNode() == NodeDetail::Contour)
    {
        AddAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        AddAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
    {
        case (Tool::NodeArc):
            AddAttribute(nod, AttrType, QStringLiteral("NodeArc"));
            break;
        case (Tool::NodePoint):
            AddAttribute(nod, AttrType, QStringLiteral("NodePoint"));
            break;
        case (Tool::NodeSpline):
            AddAttribute(nod, AttrType, QStringLiteral("NodeSpline"));
            break;
        case (Tool::NodeSplinePath):
            AddAttribute(nod, AttrType, QStringLiteral("NodeSplinePath"));
            break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

void VToolDetail::RefreshGeometry()
{
    QPainterPath path = VAbstractTool::data.ContourPath(id);
    this->setPath(path);
}

template <typename Tool>
void VToolDetail::InitTool(VMainGraphicsScene *scene, const VNodeDetail &node)
{
    QHash<qint64, VDataTool*>* tools = doc->getTools();
    Q_ASSERT(tools != 0);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.getId()));
    Q_ASSERT(tool != 0);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
}

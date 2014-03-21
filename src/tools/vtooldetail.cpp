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
#include "../geometry/vequidistant.h"

const QString VToolDetail::TagName          = QStringLiteral("detail");
const QString VToolDetail::TagNode          = QStringLiteral("node");

const QString VToolDetail::AttrSupplement   = QStringLiteral("supplement");
const QString VToolDetail::AttrClosed       = QStringLiteral("closed");
const QString VToolDetail::AttrWidth        = QStringLiteral("width");
const QString VToolDetail::AttrIdObject     = QStringLiteral("idObject");
const QString VToolDetail::AttrNodeType     = QStringLiteral("nodeType");

const QString VToolDetail::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolDetail::NodeTypeModeling = QStringLiteral("Modeling");

const QString VToolDetail::NodeArc        = QStringLiteral("NodeArc");
const QString VToolDetail::NodePoint      = QStringLiteral("NodePoint");
const QString VToolDetail::NodeSpline     = QStringLiteral("NodeSpline");
const QString VToolDetail::NodeSplinePath = QStringLiteral("NodeSplinePath");

VToolDetail::VToolDetail(VPattern *doc, VContainer *data, const quint32 &id, const Valentina::Sources &typeCreation,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(parent), dialog(nullptr), sceneDetails(scene)
{
    VDetail detail = data->GetDetail(id);
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail[i].getTypeTool())
        {
            case (Valentina::NodePoint):
                InitTool<VNodePoint>(scene, detail[i]);
                break;
            case (Valentina::NodeArc):
                InitTool<VNodeArc>(scene, detail[i]);
                break;
            case (Valentina::NodeSpline):
                InitTool<VNodeSpline>(scene, detail[i]);
                break;
            case (Valentina::NodeSplinePath):
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
    if (typeCreation == Valentina::FromGui || typeCreation == Valentina::FromTool)
    {
       AddToFile();
    }
}

VToolDetail::~VToolDetail()
{
    delete dialog;
}

void VToolDetail::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    Q_CHECK_PTR(dialogTool);
    VDetail detail = VAbstractTool::data.GetDetail(id);
    dialogTool->setDetails(detail);
}

void VToolDetail::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    Q_CHECK_PTR(dialogTool);
    VDetail detail = dialogTool->getDetails();
    VDetail det;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        quint32 id = 0;
        switch (detail[i].getTypeTool())
        {
            case (Valentina::NodePoint):
            {
                id = CreateNode<VPointF>(data, detail[i].getId());
                VNodePoint::Create(doc, data, id, detail[i].getId(), Document::FullParse, Valentina::FromGui);
            }
            break;
            case (Valentina::NodeArc):
            {
                id = CreateNode<VArc>(data, detail[i].getId());
                VNodeArc::Create(doc, data, id, detail[i].getId(), Document::FullParse, Valentina::FromGui);
            }
            break;
            case (Valentina::NodeSpline):
            {
                id = CreateNode<VSpline>(data, detail[i].getId());
                VNodeSpline::Create(doc, data, id, detail[i].getId(), Document::FullParse, Valentina::FromGui);
            }
            break;
            case (Valentina::NodeSplinePath):
            {
                id = CreateNode<VSplinePath>(data, detail[i].getId());
                VNodeSplinePath::Create(doc, data, id, detail[i].getId(), Document::FullParse, Valentina::FromGui);
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
    Create(0, det, scene, doc, data, Document::FullParse, Valentina::FromGui);
}

void VToolDetail::Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data, const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Valentina::FromGui || typeCreation == Valentina::FromTool)
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
    VAbstractTool::AddRecord(id, Valentina::DetailTool, doc);
    if (parse == Document::FullParse)
    {
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        QHash<quint32, VDataTool*>* tools = doc->getTools();
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
            Q_CHECK_PTR(dialog);
            DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
            Q_CHECK_PTR(dialogTool);
            VDetail det = dialogTool->getDetails();
            doc->SetAttribute(domElement, AttrName, det.getName());
            doc->SetAttribute(domElement, AttrSupplement, QString().setNum(det.getSeamAllowance()));
            doc->SetAttribute(domElement, AttrClosed, QString().setNum(det.getClosed()));
            doc->SetAttribute(domElement, AttrWidth, QString().setNum(det.getWidth()));
            RemoveAllChild(domElement);
            for (ptrdiff_t i = 0; i < det.CountNode(); ++i)
            {
               AddNode(domElement, det[i]);
            }
            VDetail detail = VAbstractTool::data.GetDetail(id);
            QList<quint32> list = detail.Missing(det);
            QHash<quint32, VDataTool*>* tools = doc->getTools();
            if (list.size()>0)
            {
                for (qint32 i = 0; i < list.size(); ++i)
                {
                    VAbstractNode *node = qobject_cast<VAbstractNode *>(tools->value(list[i]));
                    node->DeleteNode();
                }
            }
            emit FullUpdateTree();
            emit toolhaveChange();
        }
    }
    delete dialog;
    dialog = 0;
}

void VToolDetail::AddToFile()
{
    VDetail detail = VAbstractTool::data.GetDetail(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrName, detail.getName());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(detail.getMx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(detail.getMy()));
    doc->SetAttribute(domElement, AttrSupplement, detail.getSeamAllowance());
    doc->SetAttribute(domElement, AttrClosed, detail.getClosed());
    doc->SetAttribute(domElement, AttrWidth, detail.getWidth());

    for (ptrdiff_t i = 0; i < detail.CountNode(); ++i)
    {
       AddNode(domElement, detail.at(i));
    }

    QDomElement element;
    bool ok = doc->GetActivNodeElement(VPattern::TagDetails, element);
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
        doc->SetAttribute(domElement, AttrName, det.getName());
        doc->SetAttribute(domElement, AttrSupplement, QString().setNum(det.getSeamAllowance()));
        doc->SetAttribute(domElement, AttrClosed, QString().setNum(det.getClosed()));
        doc->SetAttribute(domElement, AttrWidth, QString().setNum(det.getWidth()));
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
            doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(newPos.x())));
            doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(newPos.y())));

            QList<QGraphicsView*> list = this->scene()->views();
            VAbstractTool::NewSceneRect(this->scene(), list[0]);

            doc->haveLiteChange();
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
        emit ChoosedTool(id, Valentina::Detail);
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
        QGraphicsScene *scene = this->scene();
        QList<QGraphicsView *> list =  scene->views();
        dialog = new DialogDetail(getData(), list.first());
        connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                dialog, &DialogTool::ChoosedObject);
        connect(dialog, &DialogTool::DialogClosed, this, &VToolDetail::FullUpdateFromGui);
        setDialog();
        dialog->show();
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

    doc->SetAttribute(nod, AttrIdObject, node.getId());
    doc->SetAttribute(nod, AttrMx, qApp->fromPixel(node.getMx()));
    doc->SetAttribute(nod, AttrMy, qApp->fromPixel(node.getMy()));
    if (node.getTypeNode() == NodeDetail::Contour)
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
    {
        case (Valentina::NodeArc):
            doc->SetAttribute(nod, AttrType, NodeArc);
            break;
        case (Valentina::NodePoint):
            doc->SetAttribute(nod, AttrType, NodePoint);
            break;
        case (Valentina::NodeSpline):
            doc->SetAttribute(nod, AttrType, NodeSpline);
            break;
        case (Valentina::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, NodeSplinePath);
            break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

void VToolDetail::RefreshGeometry()
{
    QPainterPath path = VEquidistant().ContourPath(id, this->getData());
    this->setPath(path);
}

template <typename Tool>
//cppcheck-suppress unusedFunction
void VToolDetail::InitTool(VMainGraphicsScene *scene, const VNodeDetail &node)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    Q_CHECK_PTR(tools);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.getId()));
    Q_CHECK_PTR(tool);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
}

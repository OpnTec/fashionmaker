/************************************************************************
 **
 **  @file   vtooldetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../geometry/varc.h"
#include "../geometry/vsplinepath.h"
#include "../geometry/vequidistant.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../dialogs/tools/dialogtool.h"
#include "../dialogs/tools/dialogdetail.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsView>
#include "../undocommands/savedetailoptions.h"
#include "../undocommands/movedetail.h"

const QString VToolDetail::TagName          = QStringLiteral("detail");
const QString VToolDetail::TagNode          = QStringLiteral("node");

const QString VToolDetail::AttrSupplement   = QStringLiteral("supplement");
const QString VToolDetail::AttrClosed       = QStringLiteral("closed");
const QString VToolDetail::AttrWidth        = QStringLiteral("width");
const QString VToolDetail::AttrIdObject     = QStringLiteral("idObject");
const QString VToolDetail::AttrNodeType     = QStringLiteral("nodeType");

const QString VToolDetail::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolDetail::NodeTypeModeling = QStringLiteral("Modeling");

const QString VToolDetail::NodeArc          = QStringLiteral("NodeArc");
const QString VToolDetail::NodePoint        = QStringLiteral("NodePoint");
const QString VToolDetail::NodeSpline       = QStringLiteral("NodeSpline");
const QString VToolDetail::NodeSplinePath   = QStringLiteral("NodeSplinePath");

//---------------------------------------------------------------------------------------------------------------------
VToolDetail::VToolDetail(VPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(parent), dialog(nullptr), sceneDetails(scene)
{
    VDetail detail = data->GetDetail(id);
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
                InitTool<VNodePoint>(scene, detail.at(i));
                break;
            case (Tool::NodeArc):
                InitTool<VNodeArc>(scene, detail.at(i));
                break;
            case (Tool::NodeSpline):
                InitTool<VNodeSpline>(scene, detail.at(i));
                break;
            case (Tool::NodeSplinePath):
                InitTool<VNodeSplinePath>(scene, detail.at(i));
                break;
            case (Tool::ArrowTool):
            case (Tool::SinglePointTool):
            case (Tool::EndLineTool):
            case (Tool::LineTool):
            case (Tool::AlongLineTool):
            case (Tool::ShoulderPointTool):
            case (Tool::NormalTool):
            case (Tool::BisectorTool):
            case (Tool::LineIntersectTool):
            case (Tool::SplineTool):
            case (Tool::CutSplineTool):
            case (Tool::CutArcTool):
            case (Tool::ArcTool):
            case (Tool::SplinePathTool):
            case (Tool::CutSplinePathTool):
            case (Tool::PointOfContact):
            case (Tool::DetailTool):
            case (Tool::Height):
            case (Tool::Triangle):
            case (Tool::PointOfIntersection):
            case (Tool::UnionDetails):
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
        doc->IncrementReferens(detail.at(i).getId());
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
       AddToFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolDetail::~VToolDetail()
{
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail detail = VAbstractTool::data.GetDetail(id);
    dialogTool->setDetails(detail);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail detail = dialogTool->getDetails();
    VDetail det;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        quint32 id = 0;
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                id = CreateNode<VPointF>(data, detail.at(i).getId());
                VNodePoint::Create(doc, data, id, detail.at(i).getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeArc):
            {
                id = CreateNode<VArc>(data, detail.at(i).getId());
                VNodeArc::Create(doc, data, id, detail.at(i).getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSpline):
            {
                id = CreateNode<VSpline>(data, detail.at(i).getId());
                VNodeSpline::Create(doc, data, id, detail.at(i).getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSplinePath):
            {
                id = CreateNode<VSplinePath>(data, detail.at(i).getId());
                VNodeSplinePath::Create(doc, data, id, detail.at(i).getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::ArrowTool):
            case (Tool::SinglePointTool):
            case (Tool::EndLineTool):
            case (Tool::LineTool):
            case (Tool::AlongLineTool):
            case (Tool::ShoulderPointTool):
            case (Tool::NormalTool):
            case (Tool::BisectorTool):
            case (Tool::LineIntersectTool):
            case (Tool::SplineTool):
            case (Tool::CutSplineTool):
            case (Tool::CutArcTool):
            case (Tool::ArcTool):
            case (Tool::SplinePathTool):
            case (Tool::CutSplinePathTool):
            case (Tool::PointOfContact):
            case (Tool::DetailTool):
            case (Tool::Height):
            case (Tool::Triangle):
            case (Tool::PointOfIntersection):
            case (Tool::UnionDetails):
            default:
                qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        VNodeDetail node(id, detail.at(i).getTypeTool(), NodeDetail::Contour);
        det.append(node);
    }
    det.setName(detail.getName());
    Create(0, det, scene, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data, const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
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
    VAbstractTool::AddRecord(id, Tool::DetailTool, doc);
    if (parse == Document::FullParse)
    {
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        tools->insert(id, detail);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::Remove()
{
    DeleteTool(this);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(dialog != nullptr);
        DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
        SCASSERT(dialogTool != nullptr);
        VDetail newDet = dialogTool->getDetails();
        VDetail oldDet = VAbstractTool::data.GetDetail(id);

        SaveDetailOptions *saveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
        connect(saveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveCommand);
    }
    delete dialog;
    dialog = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
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
       AddNode(doc, domElement, detail.at(i));
    }

    QDomElement element;
    bool ok = doc->GetActivNodeElement(VPattern::TagDetails, element);
    if (ok)
    {
        element.appendChild(domElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
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
        doc->RemoveAllChild(domElement);
        for (ptrdiff_t i = 0; i < det.CountNode(); ++i)
        {
           AddNode(doc, domElement, det.at(i));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VToolDetail::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        // value - this is new position.
        QPointF newPos = value.toPointF();

        MoveDetail *moveDet = new MoveDetail(doc, newPos.x(), newPos.y(), id, this->scene());
        connect(moveDet, &MoveDetail::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(moveDet);
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

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::Detail);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
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
        connect(dialog, &DialogTool::DialogClosed, this, &VToolDetail::FullUpdateFromGuiOk);
        setDialog();
        dialog->show();
    }
    if (selectedAction == actionRemove)
    {
        DeleteTool(this);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::RemoveReferens()
{
    VDetail detail = VAbstractTool::data.GetDetail(id);
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        doc->DecrementReferens(detail.at(i).getId());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::AddNode(VPattern *doc, QDomElement &domElement, const VNodeDetail &node)
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
        case (Tool::NodeArc):
            doc->SetAttribute(nod, AttrType, NodeArc);
            break;
        case (Tool::NodePoint):
            doc->SetAttribute(nod, AttrType, NodePoint);
            break;
        case (Tool::NodeSpline):
            doc->SetAttribute(nod, AttrType, NodeSpline);
            break;
        case (Tool::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, NodeSplinePath);
            break;
        case (Tool::ArrowTool):
        case (Tool::SinglePointTool):
        case (Tool::EndLineTool):
        case (Tool::LineTool):
        case (Tool::AlongLineTool):
        case (Tool::ShoulderPointTool):
        case (Tool::NormalTool):
        case (Tool::BisectorTool):
        case (Tool::LineIntersectTool):
        case (Tool::SplineTool):
        case (Tool::CutSplineTool):
        case (Tool::CutArcTool):
        case (Tool::ArcTool):
        case (Tool::SplinePathTool):
        case (Tool::CutSplinePathTool):
        case (Tool::PointOfContact):
        case (Tool::DetailTool):
        case (Tool::Height):
        case (Tool::Triangle):
        case (Tool::PointOfIntersection):
        case (Tool::UnionDetails):
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::RefreshGeometry()
{
    QPainterPath path = VEquidistant().ContourPath(id, this->getData());
    this->setPath(path);

    VDetail detail = VAbstractTool::data.GetDetail(id);
    this->setPos(detail.getMx(), detail.getMy());
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Tool>
//cppcheck-suppress unusedFunction
void VToolDetail::InitTool(VMainGraphicsScene *scene, const VNodeDetail &node)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.getId()));
    SCASSERT(tool != nullptr);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
}

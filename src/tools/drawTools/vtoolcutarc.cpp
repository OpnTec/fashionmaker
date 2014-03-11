/************************************************************************
 **
 **  @file   vtoolcutarc.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   7 1, 2014
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

#include "vtoolcutarc.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutarc.h"

const QString VToolCutArc::ToolType = QStringLiteral("cutArc");
const QString VToolCutArc::AttrArc = QStringLiteral("arc");

VToolCutArc::VToolCutArc(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                         const quint32 &arcId, const quint32 &arc1id, const quint32 &arc2id,
                         const Valentina::Sources &typeCreation, QGraphicsItem * parent)
    :VToolPoint(doc, data, id, parent), formula(formula), arcId(arcId), firstArc(), secondArc(), arc1id(arc1id),
      arc2id(arc2id)
{
    Q_ASSERT_X(arcId > 0, Q_FUNC_INFO, "arcId <= 0");
    Q_ASSERT_X(arc1id > 0, Q_FUNC_INFO, "arc1id <= 0");
    Q_ASSERT_X(arc2id > 0, Q_FUNC_INFO, "arc2id <= 0");

    firstArc = new VSimpleArc(arc1id, &currentColor, &factor);
    RefreshArc(firstArc, arc1id, SimpleArc::ForthPoint);
    firstArc->setParentItem(this);
    connect(firstArc, &VSimpleArc::Choosed, this, &VToolCutArc::ArcChoosed);

    secondArc = new VSimpleArc(arc2id, &currentColor, &factor);
    RefreshArc(secondArc, arc2id, SimpleArc::FirstPoint);
    secondArc->setParentItem(this);
    connect(secondArc, &VSimpleArc::Choosed, this, &VToolCutArc::ArcChoosed);

    if (typeCreation == Valentina::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolCutArc::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setFormula(formula);
    dialogTool->setArcId(arcId, id);
    dialogTool->setPointName(point->name());
}

void VToolCutArc::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->getFormula();
    quint32 arcId = dialogTool->getArcId();
    Create(0, pointName, formula, arcId, 5, 10, scene, doc, data, Document::FullParse, Valentina::FromGui);
}

void VToolCutArc::Create(const quint32 _id, const QString &pointName, const QString &formula, const quint32 &arcId,
                         const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data, const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VArc *arc = data->GeometricObject<const VArc *>(arcId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        VArc arc1;
        VArc arc2;
        QPointF point = arc->CutArc(toPixel(result), arc1, arc2);

        quint32 id = _id;
        quint32 arc1id = 0;
        quint32 arc2id = 0;
        if (typeCreation == Valentina::FromGui)
        {
            VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
            id = data->AddGObject(p);

            VArc * ar1 = new VArc(arc1);
            arc1id = data->AddGObject(ar1);

            VArc * ar2 = new VArc(arc2);
            arc2id = data->AddGObject(ar2);
        }
        else
        {
            VPointF *p = new VPointF(point.x(), point.y(), pointName, mx, my);
            data->UpdateGObject(id, p);

            arc1id = id + 1;
            arc2id = id + 2;

            VArc * ar1 = new VArc(arc1);
            data->UpdateGObject(arc1id, ar1);

            VArc * ar2 = new VArc(arc2);
            data->UpdateGObject(arc2id, ar2);

            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLengthArc(arc1id);
        data->AddLengthArc(arc2id);

        VDrawTool::AddRecord(id, Valentina::CutArcTool, doc);
        if (parse == Document::FullParse)
        {
            VToolCutArc *point = new VToolCutArc(doc, data, id, formula, arcId, arc1id, arc2id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
            doc->AddTool(id, point);
            doc->AddTool(arc1id, point);
            doc->AddTool(arc2id, point);
            doc->IncrementReferens(arcId);
        }
    }
}

void VToolCutArc::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        arcId = domElement.attribute(AttrArc, "").toUInt();
    }
    RefreshGeometry();
}

void VToolCutArc::ArcChoosed(quint32 id)
{
    emit ChoosedTool(id, Valentina::Arc);
}

void VToolCutArc::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
        firstArc->setFlag(QGraphicsItem::ItemIsSelectable, true);
        firstArc->setAcceptHoverEvents(true);
        secondArc->setFlag(QGraphicsItem::ItemIsSelectable, true);
        secondArc->setAcceptHoverEvents(true);
    }
    else
    {
        currentColor = Qt::gray;
        firstArc->setFlag(QGraphicsItem::ItemIsSelectable, false);
        firstArc->setAcceptHoverEvents(false);
        secondArc->setFlag(QGraphicsItem::ItemIsSelectable, false);
        secondArc->setAcceptHoverEvents(false);
    }
    firstArc->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    secondArc->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    VToolPoint::ChangedActivDraw(newName);
}

void VToolCutArc::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutArc>(this, event);
}

void VToolCutArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutArc>(this, event);
}

void VToolCutArc::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
    doc->SetAttribute(domElement, AttrMy, toMM(point->my()));

    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrArc, arcId);

    AddToCalculation(domElement);
}

void VToolCutArc::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
        doc->SetAttribute(domElement, AttrMy, toMM(point->my()));
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrArc, arcId);
    }
}

void VToolCutArc::RefreshGeometry()
{
    RefreshArc(firstArc, arc1id, SimpleArc::ForthPoint);
    RefreshArc(secondArc, arc2id, SimpleArc::FirstPoint);
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

void VToolCutArc::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrArc, QString().setNum(dialogTool->getArcId()));
}

void VToolCutArc::RefreshArc(VSimpleArc *sArc, quint32 arcid, SimpleArc::Translation tr)
{
    const VArc *arc = VAbstractTool::data.GeometricObject<const VArc *>(arcid);
    QPainterPath path;
    path.addPath(arc->GetPath());
    path.setFillRule( Qt::WindingFill );
    if (tr == SimpleArc::FirstPoint)
    {
        path.translate(-arc->GetP1().x(), -arc->GetP1().y());
    }
    else
    {
        path.translate(-arc->GetP2().x(), -arc->GetP2().y());
    }
    sArc->setPath(path);
}

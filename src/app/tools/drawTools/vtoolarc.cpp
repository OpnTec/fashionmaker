/************************************************************************
 **
 **  @file   vtoolarc.cpp
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

#include "vtoolarc.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogarc.h"
#include <QKeyEvent>
#include "../../geometry/varc.h"

const QString VToolArc::TagName = QStringLiteral("arc");
const QString VToolArc::ToolType = QStringLiteral("simple");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolArc constuctor.
 * @param doc dom document container
 * @param data container with variables
 * @param id object id in container
 * @param typeCreation way we create this tool.
 * @param parent parent object
 */
VToolArc::VToolArc(VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation, QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::Arc;

    this->setPath(ToolPath());
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolArc::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    dialogTool->SetCenter(arc->GetCenter().id());
    dialogTool->SetF1(arc->GetFormulaF1());
    dialogTool->SetF2(arc->GetFormulaF2());
    dialogTool->SetRadius(arc->GetFormulaRadius());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool
 * @param dialog dialog options.
 * @param scene pointer to scene.
 * @param doc dom document container
 * @param data container with variables
 */
VToolArc* VToolArc::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 center = dialogTool->GetCenter();
    QString radius = dialogTool->GetRadius();
    QString f1 = dialogTool->GetF1();
    QString f2 = dialogTool->GetF2();
    VToolArc* point = nullptr;
    point=Create(0, center, radius, f1, f2, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param center id arc center point.
 * @param radius arc radius.
 * @param f1 start angle of arc.
 * @param f2 end angle of arc.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolArc* VToolArc::Create(const quint32 _id, const quint32 &center, QString &radius, QString &f1, QString &f2,
                      VMainGraphicsScene *scene, VPattern *doc, VContainer *data, const Document &parse,
                      const Source &typeCreation)
{
    qreal calcRadius = 0, calcF1 = 0, calcF2 = 0;

    calcRadius = qApp->toPixel(CheckFormula(_id, radius, data));

    calcF1 = CheckFormula(_id, f1, data);
    calcF2 = CheckFormula(_id, f2, data);

    VPointF c = *data->GeometricObject<VPointF>(center);
    VArc *arc = new VArc(c, calcRadius, radius, calcF1, f1, calcF2, f2 );
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(arc);
        data->AddCurveLength<VArcLength>(id);
    }
    else
    {
        data->UpdateGObject(id, arc);
        data->AddCurveLength<VArcLength>(id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::ArcTool, doc);
    if (parse == Document::FullParse)
    {
        VToolArc *toolArc = new VToolArc(doc, data, id, typeCreation);
        scene->addItem(toolArc);
        connect(toolArc, &VToolArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, toolArc, &VToolArc::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, toolArc, &VToolArc::Disable);
        doc->AddTool(id, toolArc);
        doc->IncrementReferens(center);
        return toolArc;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogArc>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolArc::AddToFile()
{
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrCenter, arc->GetCenter().id());
    doc->SetAttribute(domElement, AttrRadius, arc->GetFormulaRadius());
    doc->SetAttribute(domElement, AttrAngle1, arc->GetFormulaF1());
    doc->SetAttribute(domElement, AttrAngle2, arc->GetFormulaF2());

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolArc::RefreshDataInFile()
{
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrCenter, arc->GetCenter().id());
        doc->SetAttribute(domElement, AttrRadius, arc->GetFormulaRadius());
        doc->SetAttribute(domElement, AttrAngle1, arc->GetFormulaF1());
        doc->SetAttribute(domElement, AttrAngle2, arc->GetFormulaF2());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolArc::RemoveReferens()
{
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    doc->DecrementReferens(arc->GetCenter().id());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolArc::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogArc *dialogTool = qobject_cast<DialogArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetCenter()));
    doc->SetAttribute(domElement, AttrRadius, dialogTool->GetRadius());
    doc->SetAttribute(domElement, AttrAngle1, dialogTool->GetF1());
    doc->SetAttribute(domElement, AttrAngle2, dialogTool->GetF2());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolArc::RefreshGeometry()
{
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setPath(ToolPath());
}

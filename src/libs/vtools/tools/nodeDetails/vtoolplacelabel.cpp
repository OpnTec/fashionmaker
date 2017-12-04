/************************************************************************
 **
 **  @file   vtoolplacelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "vtoolplacelabel.h"
#include "../../dialogs/tools/piece/dialogplacelabel.h"
#include "../../undocommands/savepieceoptions.h"
#include "../vtoolseamallowance.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vplacelabelitem.h"

const QString VToolPlaceLabel::ToolType = QStringLiteral("placeLabel");

//---------------------------------------------------------------------------------------------------------------------
VToolPlaceLabel *VToolPlaceLabel::Create(QSharedPointer<DialogTool> dialog, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull());
    QSharedPointer<DialogPlaceLabel> dialogTool = dialog.objectCast<DialogPlaceLabel>();
    SCASSERT(not dialogTool.isNull())

    VToolPlaceLabelInitData initData;
    initData.width = dialogTool->GetWidth();
    initData.height = dialogTool->GetHeight();
    initData.angle = dialogTool->GetAngle();
    initData.type = dialogTool->GetLabelType();
    initData.centerPoint = dialogTool->GetCenterPoint();
    initData.idObject = dialogTool->GetPieceId();
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    return Create(initData);
}

//---------------------------------------------------------------------------------------------------------------------
VToolPlaceLabel *VToolPlaceLabel::Create(VToolPlaceLabelInitData &initData)
{
    const qreal w = qAbs(qApp->toPixel(CheckFormula(initData.id, initData.width, initData.data)));
    const qreal h = qAbs(qApp->toPixel(CheckFormula(initData.id, initData.height, initData.data)));
    const qreal a = CheckFormula(initData.id, initData.angle, initData.data);

    QSharedPointer<VPlaceLabelItem> node(new VPlaceLabelItem());
    node->SetWidth(w, initData.width);
    node->SetHeight(h, initData.height);
    node->SetAngle(a, initData.angle);
    node->SetLabelType(initData.type);
    node->SetCenterPoint(initData.centerPoint);

    if (initData.typeCreation == Source::FromGui)
    {
        //We can't use exist object. Need create new.
        auto point = initData.data->GeometricObject<VPointF>(initData.centerPoint);

        node->setName(point->name());
        node->setX(point->x());
        node->setY(point->y());
        node->setMx(point->mx());
        node->setMy(point->my());

        initData.id = initData.data->AddGObject(node);
    }
    else
    {
        QSharedPointer<VPointF> point;
        try
        {
            point = initData.data->GeometricObject<VPointF>(initData.centerPoint);
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return nullptr;// Just ignore
        }
        node->setName(point->name());
        node->setX(point->x());
        node->setY(point->y());
        node->setMx(point->mx());
        node->setMy(point->my());

        if (initData.idTool != NULL_ID)
        {
            QSharedPointer<VPlaceLabelItem> label = qSharedPointerDynamicCast<VPlaceLabelItem>(point);
            SCASSERT(label.isNull() == false)

            node->SetCorrectionAngle(label->GetCorrectionAngle());
        }

        initData.data->UpdateGObject(initData.id, node);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }
    VAbstractTool::AddRecord(initData.id, Tool::PlaceLabel, initData.doc);
    VToolPlaceLabel *point = nullptr;
    if (initData.parse == Document::FullParse)
    {
        point = new VToolPlaceLabel(initData);

        VAbstractPattern::AddTool(initData.id, point);
        if (initData.idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that creates this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(initData.idTool);
            SCASSERT(tool != nullptr)
            point->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            initData.doc->AddToolOnRemove(point);
        }
    }
    else
    {
        initData.doc->UpdateToolData(initData.id, initData.data);
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPlaceLabel::getTagName() const
{
    return VAbstractPattern::TagPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPlaceLabel::AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id,
                                    const VPlaceLabelItem &label)
{
    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, label.GetCenterPoint());
    doc->SetAttribute(domElement, AttrWidth, label.GetWidthFormula());
    doc->SetAttribute(domElement, AttrHeight, label.GetHeightFormula());
    doc->SetAttribute(domElement, AttrAngle, label.GetAngleFormula());
    doc->SetAttribute(domElement, AttrPlaceLabelType, static_cast<int>(label.GetLabelType()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPlaceLabel::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPlaceLabel::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPlaceLabel::AddToFile()
{
    auto label = VAbstractTool::data.GeometricObject<VPlaceLabelItem>(m_id);

    QDomElement domElement = doc->createElement(getTagName());

    AddAttributes(doc, domElement, m_id, *label);
    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);

    if (m_pieceId > NULL_ID)
    {
        const VPiece oldDet = VAbstractTool::data.GetPiece(m_pieceId);
        VPiece newDet = oldDet;

        newDet.GetPlaceLabels().append(m_id);
        incrementReferens(); // Manually increment reference since in this case a piece tool will not do this for us
        qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, m_pieceId));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolPlaceLabel::VToolPlaceLabel(const VToolPlaceLabelInitData &initData, QObject *qoParent)
    : VAbstractNode(initData.doc, initData.data, initData.id, initData.centerPoint, initData.drawName,
                    initData.idTool, qoParent),
      m_pieceId(initData.idObject)
{
    ToolCreation(initData.typeCreation);
}

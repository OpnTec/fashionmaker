/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
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

#include "vtoolpin.h"
#include "../../dialogs/tools/piece/dialogpin.h"
#include "../../undocommands/savepieceoptions.h"
#include "../vtoolseamallowance.h"
#include "../vgeometry/vpointf.h"

const QString VToolPin::ToolType = QStringLiteral("pin");

//---------------------------------------------------------------------------------------------------------------------
VToolPin *VToolPin::Create(QSharedPointer<DialogTool> dialog, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull());
    QSharedPointer<DialogPin> dialogTool = dialog.objectCast<DialogPin>();
    SCASSERT(not dialogTool.isNull())

    VToolPinInitData initData;
    initData.pointId = dialogTool->GetPointId();
    initData.idObject = dialogTool->GetPieceId();
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    return Create(initData);
}

//---------------------------------------------------------------------------------------------------------------------
VToolPin *VToolPin::Create(VToolPinInitData initData)
{
    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = CreateNode<VPointF>(initData.data, initData.pointId);
    }
    else
    {
        QSharedPointer<VPointF> point;
        try
        {
            point = initData.data->GeometricObject<VPointF>(initData.pointId);
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return nullptr;// Just ignore
        }
        VPointF *pinPoint = new VPointF(*point);
        pinPoint->setMode(Draw::Modeling);
        initData.data->UpdateGObject(initData.id, pinPoint);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }
    VAbstractTool::AddRecord(initData.id, Tool::Pin, initData.doc);
    VToolPin *point = nullptr;
    if (initData.parse == Document::FullParse)
    {
        point = new VToolPin(initData);

        VAbstractPattern::AddTool(initData.id, point);
        if (initData.idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
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
QString VToolPin::getTagName() const
{
    return VAbstractPattern::TagPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPin::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPin::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPin::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, m_id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);

    if (m_pieceId > NULL_ID)
    {
        const VPiece oldDet = VAbstractTool::data.GetPiece(m_pieceId);
        VPiece newDet = oldDet;

        newDet.GetPins().append(m_id);

        qApp->getUndoStack()->push(new SavePieceOptions(oldDet, newDet, doc, m_pieceId));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolPin::VToolPin(const VToolPinInitData &initData, QObject *qoParent)
    : VAbstractNode(initData.doc, initData.data, initData.id, initData.pointId, initData.drawName, initData.idTool,
                    qoParent),
      m_pieceId(initData.idObject)
{
    ToolCreation(initData.typeCreation);
}

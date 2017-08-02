/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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
#include "../../dialogs/tools/dialogpin.h"
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
    const quint32 pointId = dialogTool->GetPointId();
    const quint32 pieceId = dialogTool->GetPieceId();

    return Create(0, pointId, pieceId, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
VToolPin *VToolPin::Create(quint32 _id, quint32 pointId, quint32 pieceId, VAbstractPattern *doc, VContainer *data,
                           const Document &parse, const Source &typeCreation, const QString &drawName,
                           const quint32 &idTool)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = CreateNode<VPointF>(data, pointId);
    }
    else
    {
        QSharedPointer<VPointF> point;
        try
        {
            point = data->GeometricObject<VPointF>(pointId);
        }
        catch (const VExceptionBadId &e)
        { // Possible case. Parent was deleted, but the node object is still here.
            Q_UNUSED(e)
            return nullptr;// Just ignore
        }
        VPointF *pinPoint = new VPointF(*point);
        pinPoint->setMode(Draw::Modeling);
        data->UpdateGObject(id, pinPoint);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::Pin, doc);
    VToolPin *point = nullptr;
    if (parse == Document::FullParse)
    {
        point = new VToolPin(doc, data, id, pointId, pieceId, typeCreation, drawName, idTool, doc);

        VAbstractPattern::AddTool(id, point);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(idTool);
            SCASSERT(tool != nullptr)
            point->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            doc->AddToolOnRemove(point);
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
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

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
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

        newDet.GetPins().append(id);

        SavePieceOptions *saveCommand = new SavePieceOptions(oldDet, newDet, doc, m_pieceId);
        qApp->getUndoStack()->push(saveCommand);// First push then make a connect
        VAbstractTool::data.UpdatePiece(m_pieceId, newDet);// Update piece because first save will not call lite update
        connect(saveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolPin::VToolPin(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 pointId, quint32 pieceId,
                   const Source &typeCreation, const QString &drawName, const quint32 &idTool, QObject *qoParent)
    : VAbstractNode(doc, data, id, pointId, drawName, idTool, qoParent),
      m_pieceId(pieceId)
{
    ToolCreation(typeCreation);
}

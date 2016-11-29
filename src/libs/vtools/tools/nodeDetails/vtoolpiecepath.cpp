/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vtoolpiecepath.h"
#include "../../dialogs/tools/dialogpiecepath.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/vpiecenode.h"
#include "../../undocommands/savepieceoptions.h"

//---------------------------------------------------------------------------------------------------------------------
VToolPiecePath *VToolPiecePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                       VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogPiecePath *dialogTool = qobject_cast<DialogPiecePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPiecePath path = dialogTool->GetPiecePath();
    const quint32 pieceId = dialogTool->GetPieceId();

    VToolPiecePath *pathTool = Create(0, path, pieceId, scene, doc, data, Document::FullParse, Source::FromGui);
    return pathTool;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPiecePath *VToolPiecePath::Create(quint32 _id, const VPiecePath &path, quint32 pieceId, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data, const Document &parse,
                                       const Source &typeCreation, const QString &drawName, const quint32 &idTool)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddPiecePath(path);
    }
    else
    {
        data->UpdatePiecePath(id, path);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VAbstractTool::AddRecord(id, Tool::PiecePath, doc);
    if (parse == Document::FullParse)
    {
        //TODO Need create garbage collector and remove all nodes, that we don't use.
        //Better check garbage before each saving file. Check only modeling tags.
        VToolPiecePath *pathTool = new VToolPiecePath(doc, data, id, pieceId, typeCreation, drawName, idTool, doc);

        doc->AddTool(id, pathTool);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            SCASSERT(tool != nullptr);
            pathTool->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Try to prevent memory leak
            scene->addItem(pathTool);// First adopted by scene
            pathTool->hide();// If no one will use node, it will stay hidden
            pathTool->SetParentType(ParentType::Scene);
        }
        return pathTool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPiecePath::getTagName() const
{
    return VAbstractPattern::TagPath;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VPieceNode &node)
{
    domElement.appendChild(AddSANode(doc, VAbstractPattern::TagNode, node));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiecePath &path)
{
    if (path.CountNodes() > 0)
    {
        QDomElement nodesElement = doc->createElement(VAbstractPattern::TagNodes);
        for (int i = 0; i < path.CountNodes(); ++i)
        {
            AddNode(doc, nodesElement, path.at(i));
        }
        domElement.appendChild(nodesElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiecePath &path)
{
    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrName, path.GetName());
    doc->SetAttribute(domElement, AttrType, static_cast<int>(path.GetType()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    const VPiecePath path = VAbstractTool::data.GetPiecePath(id);

    AddAttributes(doc, domElement, id, path);

    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddNodes(doc, domElement, path);

    qApp->getUndoStack()->beginMacro(tr("new path"));

    AddToModeling(domElement);

    VPiece oldDet = VAbstractTool::data.GetPiece(m_pieceId);
    VPiece newDet = oldDet;

    if (path.GetType() == PiecePathType::InternalPath)
    {

    }
    else if (path.GetType() == PiecePathType::CustomSeamAllowance)
    {
        CustomSARecord record;
        record.path = id;

        QVector<CustomSARecord> records = newDet.GetCustomSARecords();
        records.append(record);
        newDet.SetCustomSARecords(records);
    }

    SavePieceOptions *saveCommand = new SavePieceOptions(oldDet, newDet, doc, m_pieceId);
    qApp->getUndoStack()->push(saveCommand);// First push then make a connect
    VAbstractTool::data.UpdatePiece(m_pieceId, newDet);// Update piece because first save will not call lite update
    connect(saveCommand, &SavePieceOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);

    qApp->getUndoStack()->endMacro();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        if (idTool != NULL_ID)
        {
            doc->SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::ShowNode()
{
    if (parentType != ParentType::Scene)
    {
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::HideNode()
{
    hide();
}

//---------------------------------------------------------------------------------------------------------------------
VToolPiecePath::VToolPiecePath(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 pieceId,
                               const Source &typeCreation, const QString &drawName, const quint32 &idTool,
                               QObject *qoParent, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, 0, drawName, idTool, qoParent),
      QGraphicsPathItem(parent),
      m_pieceId(pieceId)
{
    RefreshGeometry();
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPiecePath::RefreshGeometry()
{
    const VPiecePath path = VAbstractTool::data.GetPiecePath(id);
    if (path.GetType() == PiecePathType::InternalPath)
    {
        QPainterPath p = path.PainterPath(this->getData());
        p.setFillRule(Qt::OddEvenFill);
        this->setPath(p);
    }
}

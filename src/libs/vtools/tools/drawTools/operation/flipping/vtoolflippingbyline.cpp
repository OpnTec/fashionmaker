/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#include "vtoolflippingbyline.h"

#include <limits.h>
#include <qiterator.h>
#include <QColor>
#include <QDomNode>
#include <QDomNodeList>
#include <QMapIterator>
#include <QPoint>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QUndoStack>
#include <new>

#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../dialogs/tools/dialogflippingbyline.h"
#include "../../../../visualization/line/operation/vistoolflippingbyline.h"
#include "../../../../visualization/visualization.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/logging.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../ifc/ifcdef.h"
#include "../ifc/exception/vexception.h"
#include "../vwidgets/vabstractsimple.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../../vdatatool.h"
#include "../../vdrawtool.h"

template <class T> class QSharedPointer;

const QString VToolFlippingByLine::ToolType = QStringLiteral("flippingByLine");

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine::~VToolFlippingByLine()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr)
    dialogTool->SetFirstLinePointId(m_firstLinePointId);
    dialogTool->SetSecondLinePointId(m_secondLinePointId);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                 VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const quint32 firstLinePointId = dialogTool->GetFirstLinePointId();
    const quint32 secondLinePointId = dialogTool->GetSecondLinePointId();
    const QString suffix = dialogTool->GetSuffix();
    const QVector<quint32> source = dialogTool->GetObjects();
    VToolFlippingByLine* operation = Create(0, firstLinePointId, secondLinePointId, suffix, source,
                                            QVector<DestinationItem>(), scene, doc, data, Document::FullParse,
                                            Source::FromGui);
    if (operation != nullptr)
    {
        operation->dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(const quint32 _id, quint32 firstLinePointId, quint32 secondLinePointId,
                                                 const QString &suffix, const QVector<quint32> &source,
                                                 const QVector<DestinationItem> &destination, VMainGraphicsScene *scene,
                                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                 const Source &typeCreation)
{
    const auto firstPoint = *data->GeometricObject<VPointF>(firstLinePointId);
    const QPointF fPoint = static_cast<QPointF>(firstPoint);

    const auto secondPoint = *data->GeometricObject<VPointF>(secondLinePointId);
    const QPointF sPoint = static_cast<QPointF>(secondPoint);

    QVector<DestinationItem> dest = destination;

    quint32 id = _id;
    CreateDestination(typeCreation, id, dest, source, fPoint, sPoint, suffix, doc, data, parse);

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::FlippingByLine, doc);
        VToolFlippingByLine *tool = new VToolFlippingByLine(doc, data, id, firstLinePointId, secondLinePointId, suffix,
                                                            source, dest, typeCreation);
        scene->addItem(tool);
        InitOperationToolConnections(scene, tool);
        VAbstractPattern::AddTool(id, tool);
        doc->IncrementReferens(firstPoint.getIdTool());
        doc->IncrementReferens(secondPoint.getIdTool());
        for (int i = 0; i < source.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(source.at(i))->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolFlippingByLine::FirstLinePointName() const
{
    return VAbstractTool::data.GetGObject(m_firstLinePointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolFlippingByLine::SecondLinePointName() const
{
    return VAbstractTool::data.GetGObject(m_secondLinePointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolFlippingByLine>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolFlippingByLine *visual = qobject_cast<VisToolFlippingByLine *>(vis);
        SCASSERT(visual != nullptr)

        visual->SetObjects(source);
        visual->SetFirstLinePointId(m_firstLinePointId);
        visual->SetSecondLinePointId(m_secondLinePointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr)

    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetFirstLinePointId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetSecondLinePointId()));
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::ReadToolAttributes(const QDomElement &domElement)
{
    m_firstLinePointId = doc->GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
    m_secondLinePointId = doc->GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrP1Line, QString().setNum(m_firstLinePointId));
    doc->SetAttribute(tag, AttrP2Line, QString().setNum(m_secondLinePointId));
    doc->SetAttribute(tag, AttrSuffix, suffix);

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogFlippingByLine>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine::VToolFlippingByLine(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 firstLinePointId,
                                         quint32 secondLinePointId, const QString &suffix,
                                         const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                         const Source &typeCreation, QGraphicsItem *parent)
    : VAbstractFlipping(doc, data, id, suffix, source, destination, parent),
      m_firstLinePointId(firstLinePointId),
      m_secondLinePointId(secondLinePointId)
{
    InitOperatedObjects();
    ToolCreation(typeCreation);
}

/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
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

#include "vtoolflippingbyaxis.h"

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
#include "../../../../dialogs/tools/dialogflippingbyaxis.h"
#include "../../../../visualization/line/operation/vistoolflippingbyaxis.h"
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

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
template <class T> class QSharedPointer;

const QString VToolFlippingByAxis::ToolType = QStringLiteral("flippingByAxis");

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByAxis::~VToolFlippingByAxis()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByAxis *dialogTool = qobject_cast<DialogFlippingByAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->SetOriginPointId(m_originPointId);
    dialogTool->SetAxisType(m_axisType);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByAxis *VToolFlippingByAxis::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                 VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByAxis *dialogTool = qobject_cast<DialogFlippingByAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 originPointId = dialogTool->GetOriginPointId();
    const AxisType axisType = dialogTool->GetAxisType();
    const QString suffix = dialogTool->GetSuffix();
    const QVector<quint32> source = dialogTool->GetObjects();
    VToolFlippingByAxis* operation = Create(0, originPointId, axisType, suffix, source, QVector<DestinationItem>(),
                                            scene, doc, data, Document::FullParse, Source::FromGui);
    if (operation != nullptr)
    {
        operation->dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByAxis *VToolFlippingByAxis::Create(const quint32 _id, quint32 originPointId, AxisType axisType,
                                                 const QString &suffix, const QVector<quint32> &source,
                                                 const QVector<DestinationItem> &destination, VMainGraphicsScene *scene,
                                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                 const Source &typeCreation)
{
    const auto originPoint = *data->GeometricObject<VPointF>(originPointId);
    const QPointF fPoint = originPoint;

    QPointF sPoint;
    if (axisType == AxisType::VerticalAxis)
    {
        sPoint = QPointF(fPoint.x(), fPoint.y() + 100);
    }
    else
    {
        sPoint = QPointF(fPoint.x() + 100, fPoint.y());
    }

    QVector<DestinationItem> dest = destination;

    quint32 id = _id;
    CreateDestination(typeCreation, id, dest, source, fPoint, sPoint, suffix, doc, data, parse);

    VDrawTool::AddRecord(id, Tool::FlippingByAxis, doc);
    if (parse == Document::FullParse)
    {
        VToolFlippingByAxis *tool = new VToolFlippingByAxis(doc, data, id, originPointId, axisType, suffix, source,
                                                            dest, typeCreation);
        scene->addItem(tool);
        InitOperationToolConnections(scene, tool);
        doc->AddTool(id, tool);
        doc->IncrementReferens(originPoint.getIdTool());
        for (int i = 0; i < source.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(source.at(i))->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
AxisType VToolFlippingByAxis::GetAxisType() const
{
    return m_axisType;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::SetAxisType(AxisType value)
{
    m_axisType = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolFlippingByAxis>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolFlippingByAxis *visual = qobject_cast<VisToolFlippingByAxis *>(vis);
        SCASSERT(visual != nullptr);

        visual->SetObjects(source);
        visual->SetOriginPointId(m_originPointId);
        visual->SetAxisType(m_axisType);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByAxis *dialogTool = qobject_cast<DialogFlippingByAxis*>(dialog);
    SCASSERT(dialogTool != nullptr);

    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetOriginPointId()));
    doc->SetAttribute(domElement, AttrAxisType, QString().setNum(static_cast<int>(dialogTool->GetAxisType())));
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::ReadToolAttributes(const QDomElement &domElement)
{
    m_originPointId = doc->GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
    m_axisType = static_cast<AxisType>(doc->GetParametrUInt(domElement, AttrAxisType, "1"));
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrCenter, QString().setNum(m_originPointId));
    doc->SetAttribute(tag, AttrAxisType, QString().setNum(static_cast<int>(m_axisType)));
    doc->SetAttribute(tag, AttrSuffix, suffix);

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByAxis::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogFlippingByAxis>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByAxis::VToolFlippingByAxis(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 originPointId,
                                         AxisType axisType, const QString &suffix,
                                         const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                         const Source &typeCreation, QGraphicsItem *parent)
    : VAbstractFlipping(doc, data, id, suffix, source, destination, parent),
      m_originPointId(originPointId),
      m_axisType(axisType)
{
    InitOperatedObjects();
    ToolCreation(typeCreation);
}


/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
void VToolFlippingByLine::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogFlippingByLine> dialogTool = qobject_cast<DialogFlippingByLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetFirstLinePointId(m_firstLinePointId);
    dialogTool->SetSecondLinePointId(m_secondLinePointId);
    dialogTool->SetSuffix(suffix);

    SetDialogVisibilityGroupData(dialogTool);
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                                 VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogFlippingByLine> dialogTool = qobject_cast<DialogFlippingByLine *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolFlippingByLineInitData initData;
    initData.firstLinePointId = dialogTool->GetFirstLinePointId();
    initData.secondLinePointId = dialogTool->GetSecondLinePointId();
    initData.suffix = dialogTool->GetSuffix();
    initData.source = dialogTool->GetObjects();
    initData.hasLinkedVisibilityGroup = dialogTool->HasLinkedVisibilityGroup();
    initData.visibilityGroupName = dialogTool->GetVisibilityGroupName();
    initData.visibilityGroupTags = dialogTool->GetVisibilityGroupTags();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolFlippingByLine* operation = Create(initData);
    if (operation != nullptr)
    {
        operation->m_dialog = dialog;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(VToolFlippingByLineInitData initData)
{
    const auto firstPoint = *initData.data->GeometricObject<VPointF>(initData.firstLinePointId);
    const QPointF fPoint = static_cast<QPointF>(firstPoint);

    const auto secondPoint = *initData.data->GeometricObject<VPointF>(initData.secondLinePointId);
    const QPointF sPoint = static_cast<QPointF>(secondPoint);

    CreateDestination(initData, fPoint, sPoint);

    if (initData.parse == Document::FullParse)
    {
        if (initData.typeCreation == Source::FromGui && initData.hasLinkedVisibilityGroup)
        {
            qApp->getUndoStack()->beginMacro(tr("flipping by line"));
        }

        VAbstractTool::AddRecord(initData.id, Tool::FlippingByLine, initData.doc);
        VToolFlippingByLine *tool = new VToolFlippingByLine(initData);
        initData.scene->addItem(tool);
        InitOperationToolConnections(initData.scene, tool);
        VAbstractPattern::AddTool(initData.id, tool);
        initData.doc->IncrementReferens(firstPoint.getIdTool());
        initData.doc->IncrementReferens(secondPoint.getIdTool());
        for (auto idObject : qAsConst(initData.source))
        {
            initData.doc->IncrementReferens(initData.data->GetGObject(idObject)->getIdTool());
        }

        if (initData.typeCreation == Source::FromGui && initData.hasLinkedVisibilityGroup)
        {
            VAbstractOperation::CreateVisibilityGroup(initData);
            qApp->getUndoStack()->endMacro();
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
void VToolFlippingByLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogFlippingByLine>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
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
void VToolFlippingByLine::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                     QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogFlippingByLine> dialogTool = qobject_cast<DialogFlippingByLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, m_firstLinePointId);
    AddDependence(oldDependencies, m_secondLinePointId);
    AddDependence(newDependencies, dialogTool->GetFirstLinePointId());
    AddDependence(newDependencies, dialogTool->GetSecondLinePointId());

    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetFirstLinePointId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetSecondLinePointId()));
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());

    // Save for later use.
    hasLinkedGroup = dialogTool->HasLinkedVisibilityGroup();
    groupName = dialogTool->GetVisibilityGroupName();
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
QString VToolFlippingByLine::MakeToolTip() const
{
    return QStringLiteral("<tr> <td><b>%1:</b> %2</td> </tr>"
                          "<tr> <td><b>%3:</b> %4</td> </tr>"
                          "%5")
            .arg(tr("First line point"), FirstLinePointName(),
             tr("Second line point"), SecondLinePointName()) // 1, 2, 3, 4
            .arg(VisibilityGroupToolTip()); // 5
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine::VToolFlippingByLine(const VToolFlippingByLineInitData &initData, QGraphicsItem *parent)
    : VAbstractFlipping(initData.doc, initData.data, initData.id, initData.suffix, initData.source,
                        initData.destination, parent),
      m_firstLinePointId(initData.firstLinePointId),
      m_secondLinePointId(initData.secondLinePointId)
{
    InitOperatedObjects();
    ToolCreation(initData.typeCreation);
}

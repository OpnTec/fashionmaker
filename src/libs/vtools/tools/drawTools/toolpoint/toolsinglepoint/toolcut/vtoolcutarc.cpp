/************************************************************************
 **
 **  @file   vtoolcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include <QPointF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialogcutarc.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/path/vistoolcutarc.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoolcut.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class VGObject;
template <class T> class QSharedPointer;

const QString VToolCutArc::ToolType = QStringLiteral("cutArc");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutArc constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first arc.
 * @param arcId id arc in data container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutArc::VToolCutArc(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                         const quint32 &arcId, const Source &typeCreation, QGraphicsItem * parent)
    :VToolCut(doc, data, id, formula, arcId, parent)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutArc::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFormula(formula);
    dialogTool->setArcId(curveCutId);
    dialogTool->SetPointName(point->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolCutArc* VToolCutArc::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->GetFormula();
    const quint32 arcId = dialogTool->getArcId();
    VToolCutArc* point = Create(0, pointName, formula, arcId, 5, 10, scene, doc, data, Document::FullParse,
                                Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param formula string with formula length first arc.
 * @param arcId id arc in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolCutArc* VToolCutArc::Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &arcId,
                                 const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(arcId);

    const qreal result = CheckFormula(_id, formula, data);

    VArc arc1;
    VArc arc2;
    QPointF point = arc->CutArc(qApp->toPixel(result), arc1, arc2);

    quint32 id = _id;
    VPointF *p = new VPointF(point, pointName, mx, my);
    auto a1 = QSharedPointer<VArc>(new VArc(arc1));
    auto a2 = QSharedPointer<VArc>(new VArc(arc2));
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(p);
        a1->setId(data->getNextId());
        a2->setId(data->getNextId());
        data->AddArc(a1, a1->id(), id);
        data->AddArc(a2, a2->id(), id);
    }
    else
    {
        data->UpdateGObject(id, p);
        a1->setId(id + 1);
        a2->setId(id + 2);
        data->AddArc(a1, a1->id(), id);
        data->AddArc(a2, a2->id(), id);

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::CutArc, doc);
    if (parse == Document::FullParse)
    {
        VToolCutArc *point = new VToolCutArc(doc, data, id, formula, arcId, typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(arc->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCutArc>(show);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogCutArc>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutArc::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutArc *dialogTool = qobject_cast<DialogCutArc*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrArc, QString().setNum(dialogTool->getArcId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolCut::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formula);
    doc->SetAttribute(tag, AttrArc, curveCutId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::ReadToolAttributes(const QDomElement &domElement)
{
    formula = doc->GetParametrString(domElement, AttrLength, "");
    curveCutId = doc->GetParametrUInt(domElement, AttrArc, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutArc::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolCutArc *visual = qobject_cast<VisToolCutArc *>(vis);
        SCASSERT(visual != nullptr);

        visual->setObject1Id(curveCutId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

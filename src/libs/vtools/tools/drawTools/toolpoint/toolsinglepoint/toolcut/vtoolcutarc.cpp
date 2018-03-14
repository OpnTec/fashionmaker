/************************************************************************
 **
 **  @file   vtoolcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

template <class T> class QSharedPointer;

const QString VToolCutArc::ToolType = QStringLiteral("cutArc");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutArc constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolCutArc::VToolCutArc(const VToolCutArcInitData &initData, QGraphicsItem * parent)
    :VToolCut(initData.doc, initData.data, initData.id, initData.formula, initData.arcId, parent)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutArc::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogCutArc> dialogTool = m_dialog.objectCast<DialogCutArc>();
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(m_id);
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
VToolCutArc* VToolCutArc::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogCutArc> dialogTool = dialog.objectCast<DialogCutArc>();
    SCASSERT(not dialogTool.isNull())

    VToolCutArcInitData initData;
    initData.formula = dialogTool->GetFormula();
    initData.arcId = dialogTool->getArcId();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolCutArc* point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 */
VToolCutArc* VToolCutArc::Create(VToolCutArcInitData &initData)
{
    const QSharedPointer<VArc> arc = initData.data->GeometricObject<VArc>(initData.arcId);

    const qreal result = CheckFormula(initData.id, initData.formula, initData.data);

    VArc arc1;
    VArc arc2;
    QPointF point = arc->CutArc(qApp->toPixel(result), arc1, arc2);

    VPointF *p = new VPointF(point, initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    auto a1 = QSharedPointer<VArc>(new VArc(arc1));
    auto a2 = QSharedPointer<VArc>(new VArc(arc2));
    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        a1->setId(VContainer::getNextId());
        a2->setId(VContainer::getNextId());
        initData.data->AddArc(a1, a1->id(), initData.id);
        initData.data->AddArc(a2, a2->id(), initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        a1->setId(initData.id + 1);
        a2->setId(initData.id + 2);
        initData.data->AddArc(a1, a1->id(), initData.id);
        initData.data->AddArc(a2, a2->id(), initData.id);

        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::CutArc, initData.doc);
        VToolCutArc *point = new VToolCutArc(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(arc->getIdTool());
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
void VToolCutArc::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogCutArc>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutArc::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                             QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogCutArc> dialogTool = m_dialog.objectCast<DialogCutArc>();
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, curveCutId);
    AddDependence(newDependencies, dialogTool->getArcId());

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
        SCASSERT(visual != nullptr)

        visual->setObject1Id(curveCutId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator()));

        const QSharedPointer<VAbstractCurve> curve = VAbstractTool::data.GeometricObject<VAbstractCurve>(curveCutId);
        visual->setLineStyle(LineStyleToPenStyle(curve->GetPenStyle()));

        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCutArc::MakeToolTip() const
{
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(curveCutId);

    const QString expression = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    const qreal length = Visualization::FindValFromUser(expression, VAbstractTool::data.DataVariables());

    const QString arcStr = tr("Arc");
    const QString lengthStr = tr("length");
    const QString startAngleStr = tr("start angle");
    const QString endAngleStr = tr("end angle");
    const QString radiusStr = tr("radius");

    VArc ar1;
    VArc ar2;
    arc->CutArc(qApp->toPixel(length), ar1, ar2);

    auto ArcToolTip = [arcStr, lengthStr, startAngleStr, endAngleStr, radiusStr](QString toolTip, const VArc &arc,
            const QString &arcNumber)
    {
        toolTip += QString("<tr> <td><b>%10:</b> %11</td> </tr>"
                           "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                           "<tr> <td><b>%4:</b> %5 %3</td> </tr>"
                           "<tr> <td><b>%6:</b> %7°</td> </tr>"
                           "<tr> <td><b>%8:</b> %9°</td> </tr>")
                    .arg(arcStr + arcNumber + QLatin1String(" ") + lengthStr)
                    .arg(qApp->fromPixel(arc.GetLength()))
                    .arg(UnitsToStr(qApp->patternUnit(), true), arcStr + arcNumber + QLatin1String(" ") + radiusStr)
                    .arg(qApp->fromPixel(arc.GetRadius()))
                    .arg(arcStr + arcNumber + QLatin1String(" ") + startAngleStr)
                    .arg(qApp->fromPixel(arc.GetStartAngle()))
                    .arg(arcStr + arcNumber + QLatin1String(" ") + endAngleStr)
                    .arg(qApp->fromPixel(arc.GetEndAngle()))
                    .arg(arcStr + arcNumber + QLatin1String(" ") + tr("label"), arc.name());
        return toolTip;
    };

    return ArcToolTip(ArcToolTip("<table>", ar1, "1"), ar2, "2") + QLatin1String("</table>");
}

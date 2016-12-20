/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "vtoolcutsplinepath.h"

#include <QPointF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVector>
#include <new>

#include "../../../../../dialogs/tools/dialogcutsplinepath.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/path/vistoolcutsplinepath.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vsplinepoint.h"
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

const QString VToolCutSplinePath::ToolType       = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first splinePath.
 * @param splinePathId id splinePath (we cut this splinePath) in data container.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutSplinePath::VToolCutSplinePath(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &formula, const quint32 &splinePathId,
                                       const Source &typeCreation, QGraphicsItem *parent)
    :VToolCut(doc, data, id, formula, splinePathId, parent)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFormula(formula);
    dialogTool->setSplinePathId(curveCutId);
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
VToolCutSplinePath* VToolCutSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->GetFormula();
    const quint32 splinePathId = dialogTool->getSplinePathId();
    VToolCutSplinePath* point = Create(0, pointName, formula, splinePathId, 5, 10, scene, doc, data,
                                       Document::FullParse, Source::FromGui);
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
 * @param formula string with formula length first splinePath.
 * @param splinePathId id of splinePath in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolCutSplinePath* VToolCutSplinePath::Create(const quint32 _id, const QString &pointName, QString &formula,
                                               const quint32 &splinePathId, const qreal &mx, const qreal &my,
                                               VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data, const Document &parse, const Source &typeCreation)
{
    const auto splPath = data->GeometricObject<VAbstractCubicBezierPath>(splinePathId);
    SCASSERT(splPath != nullptr)

    const qreal result = CheckFormula(_id, formula, data);

    quint32 id = _id;
    VSplinePath *splPath1 = nullptr;
    VSplinePath *splPath2 = nullptr;
    VPointF *p = VToolCutSplinePath::CutSplinePath(qApp->toPixel(result), splPath, pointName, &splPath1, &splPath2);

    SCASSERT(splPath1 != nullptr)
    SCASSERT(splPath2 != nullptr)
    SCASSERT(p != nullptr)

    p->setMx(mx);
    p->setMy(my);

    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(p);

        data->AddSpline(QSharedPointer<VAbstractBezier>(splPath1), NULL_ID, id);
        data->AddSpline(QSharedPointer<VAbstractBezier>(splPath2), NULL_ID, id);
    }
    else
    {
        data->UpdateGObject(id, p);

        data->AddSpline(QSharedPointer<VAbstractBezier>(splPath1), NULL_ID, id);
        data->AddSpline(QSharedPointer<VAbstractBezier>(splPath2), NULL_ID, id);

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::CutSplinePath, doc);
    if (parse == Document::FullParse)
    {
        VToolCutSplinePath *point = new VToolCutSplinePath(doc, data, id, formula, splinePathId, typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(splPath->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCutSplinePath>(show);
}

//---------------------------------------------------------------------------------------------------------------------
VPointF *VToolCutSplinePath::CutSplinePath(qreal length, const QSharedPointer<VAbstractCubicBezierPath> &splPath,
                                           const QString &pName, VSplinePath **splPath1, VSplinePath **splPath2)
{
    SCASSERT(splPath != nullptr)

    QPointF spl1p2, spl1p3, spl2p2, spl2p3;
    qint32 p1 = 0, p2 = 0;

    const QPointF point = splPath->CutSplinePath(length, p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
    VPointF *p = new VPointF(point);
    p->setName(pName);

    const QVector<VSplinePoint> points = splPath->GetSplinePath();

    const VSplinePoint splP1 = points.at(p1);
    const VSplinePoint splP2 = points.at(p2);
    const VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p);
    const VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P());

    *splPath1 = new VSplinePath();
    *splPath2 = new VSplinePath();

    for (qint32 i = 0; i < points.size(); i++)
    {
        if (i <= p1 && i < p2)
        {
            if (i == p1)
            {
                const qreal angle1 = spl1.GetStartAngle()+180;
                const QString angle1F = QString().number(angle1);

                (*splPath1)->append(VSplinePoint(splP1.P(), angle1, angle1F, spl1.GetStartAngle(),
                                                 spl1.GetStartAngleFormula(), splP1.Length1(), splP1.Length1Formula(),
                                                 spl1.GetC1Length(), spl1.GetC1LengthFormula()));

                const qreal angle2 = spl1.GetEndAngle()+180;
                const QString angle2F = QString().number(angle2);

                const auto cutPoint = VSplinePoint(*p, spl1.GetEndAngle(), spl1.GetEndAngleFormula(), angle2, angle2F,
                                                   spl1.GetC2Length(), spl1.GetC2LengthFormula(), spl2.GetC1Length(),
                                                   spl2.GetC1LengthFormula());
                (*splPath1)->append(cutPoint);
                continue;
            }
            (*splPath1)->append(points.at(i));
        }
        else
        {
            if (i == p2)
            {
                const qreal angle1 = spl2.GetStartAngle()+180;
                const QString angle1F = QString().number(angle1);

                const auto cutPoint = VSplinePoint(*p, angle1, angle1F, spl2.GetStartAngle(),
                                                   spl2.GetStartAngleFormula(), spl1.GetC2Length(),
                                                   spl1.GetC2LengthFormula(), spl2.GetC1Length(),
                                                   spl2.GetC1LengthFormula());

                (*splPath2)->append(cutPoint);

                const qreal angle2 = spl2.GetEndAngle()+180;
                const QString angle2F = QString().number(angle2);

                (*splPath2)->append(VSplinePoint(splP2.P(), spl2.GetEndAngle(), spl2.GetEndAngleFormula(), angle2,
                                                 angle2F, spl2.GetC2Length(), spl2.GetC2LengthFormula(),
                                                 splP2.Length2(), splP2.Length2Formula()));
                continue;
            }
            (*splPath2)->append(points.at(i));
        }
    }

    return p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogCutSplinePath>(this, event);
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
void VToolCutSplinePath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr)
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrSplinePath, QString().setNum(dialogTool->getSplinePathId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolCut::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formula);
    doc->SetAttribute(tag, AttrSplinePath, curveCutId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::ReadToolAttributes(const QDomElement &domElement)
{
    formula = doc->GetParametrString(domElement, AttrLength, "");
    curveCutId = doc->GetParametrUInt(domElement, AttrSplinePath, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolCutSplinePath *visual = qobject_cast<VisToolCutSplinePath *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(curveCutId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

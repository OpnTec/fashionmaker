/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

template <class T> class QSharedPointer;

const QString VToolCutSplinePath::ToolType       = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutSplinePath constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolCutSplinePath::VToolCutSplinePath(const VToolCutSplinePathInitData &initData, QGraphicsItem *parent)
    :VToolCut(initData.doc, initData.data, initData.id, initData.formula, initData.splinePathId, parent)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutSplinePath::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogCutSplinePath> dialogTool = m_dialog.objectCast<DialogCutSplinePath>();
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(m_id);
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
VToolCutSplinePath* VToolCutSplinePath::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogCutSplinePath> dialogTool = dialog.objectCast<DialogCutSplinePath>();
    SCASSERT(not dialogTool.isNull())

    VToolCutSplinePathInitData initData;
    initData.formula = dialogTool->GetFormula();
    initData.splinePathId = dialogTool->getSplinePathId();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolCutSplinePath* point = Create(initData);
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
VToolCutSplinePath* VToolCutSplinePath::Create(VToolCutSplinePathInitData &initData)
{
    const auto splPath = initData.data->GeometricObject<VAbstractCubicBezierPath>(initData.splinePathId);
    SCASSERT(splPath != nullptr)

    const qreal result = CheckFormula(initData.id, initData.formula, initData.data);

    VSplinePath *splPath1 = nullptr;
    VSplinePath *splPath2 = nullptr;
    VPointF *p = VToolCutSplinePath::CutSplinePath(qApp->toPixel(result), splPath, initData.name, &splPath1, &splPath2);

    SCASSERT(splPath1 != nullptr)
    SCASSERT(splPath2 != nullptr)
    SCASSERT(p != nullptr)

    p->setMx(initData.mx);
    p->setMy(initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);

        initData.data->AddSpline(QSharedPointer<VAbstractBezier>(splPath1), NULL_ID, initData.id);
        initData.data->AddSpline(QSharedPointer<VAbstractBezier>(splPath2), NULL_ID, initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);

        initData.data->AddSpline(QSharedPointer<VAbstractBezier>(splPath1), NULL_ID, initData.id);
        initData.data->AddSpline(QSharedPointer<VAbstractBezier>(splPath2), NULL_ID, initData.id);

        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::CutSplinePath, initData.doc);
        VToolCutSplinePath *point = new VToolCutSplinePath(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(splPath->getIdTool());
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

    VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p);
    spl1.SetApproximationScale(splPath->GetApproximationScale());

    VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P());
    spl2.SetApproximationScale(splPath->GetApproximationScale());

    *splPath1 = new VSplinePath();
    (*splPath1)->SetApproximationScale(splPath->GetApproximationScale());
    *splPath2 = new VSplinePath();
    (*splPath2)->SetApproximationScale(splPath->GetApproximationScale());

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
void VToolCutSplinePath::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogCutSplinePath>(event, id);
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
void VToolCutSplinePath::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                    QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogCutSplinePath> dialogTool = m_dialog.objectCast<DialogCutSplinePath>();
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, curveCutId);
    AddDependence(newDependencies, dialogTool->getSplinePathId());

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
    formula = doc->GetParametrString(domElement, AttrLength, QString());
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

        const QSharedPointer<VAbstractCurve> curve = VAbstractTool::data.GeometricObject<VAbstractCurve>(curveCutId);
        visual->setLineStyle(LineStyleToPenStyle(curve->GetPenStyle()));

        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCutSplinePath::MakeToolTip() const
{
    const auto splPath = VAbstractTool::data.GeometricObject<VAbstractCubicBezierPath>(curveCutId);

    const QString expression = qApp->TrVars()->FormulaToUser(formula, qApp->Settings()->GetOsSeparator());
    const qreal length = Visualization::FindValFromUser(expression, VAbstractTool::data.DataVariables());

    VSplinePath *splPath1 = nullptr;
    VSplinePath *splPath2 = nullptr;
    VPointF *p = VToolCutSplinePath::CutSplinePath(qApp->toPixel(length), splPath, "X", &splPath1, &splPath2);
    delete p; // Don't need this point

    const QString curveStr = tr("Curve");
    const QString lengthStr = tr("length");

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%6:</b> %7</td> </tr>"
                                    "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%8:</b> %9</td> </tr>"
                                    "<tr> <td><b>%4:</b> %5 %3</td> </tr>"
                                    "</table>")
            .arg(curveStr + QLatin1String("1 ") + lengthStr)
            .arg(qApp->fromPixel(splPath1->GetLength()))
            .arg(UnitsToStr(qApp->patternUnit(), true), curveStr + QLatin1String("2 ") + lengthStr)
            .arg(qApp->fromPixel(splPath2->GetLength()))
            .arg(curveStr + QLatin1String(" 1") + tr("label"), splPath1->name(),
                 curveStr + QLatin1String(" 2") + tr("label"), splPath2->name());

    delete splPath1;
    delete splPath2;

    return toolTip;
}

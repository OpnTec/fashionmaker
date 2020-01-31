/************************************************************************
 **
 **  @file   vtoolcurveintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vtoolcurveintersectaxis.h"

#include <limits.h>
#include <QLineF>
#include <QMap>
#include <QRectF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVector>
#include <new>

#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../dialogs/tools/dialogcurveintersectaxis.h"
#include "../ifc/ifcdef.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../qmuparser/qmudef.h"
#include "../toolcut/vtoolcutsplinepath.h"
#include "../vgeometry/vabstractcubicbezier.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vtools/visualization/visualization.h"
#include "../vtools/visualization/line/vistoolcurveintersectaxis.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString VToolCurveIntersectAxis::ToolType = QStringLiteral("curveIntersectAxis");

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis::VToolCurveIntersectAxis(const VToolCurveIntersectAxisInitData &initData,
                                                 QGraphicsItem *parent)
    :VToolLinePoint(initData.doc, initData.data, initData.id, initData.typeLine, initData.lineColor, QString(),
                    initData.basePointId, 0, parent),
      formulaAngle(initData.formulaAngle),
      curveId(initData.curveId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    m_dialog->setModal(true);
    const QPointer<DialogCurveIntersectAxis> dialogTool = qobject_cast<DialogCurveIntersectAxis *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetBasePointId(basePointId);
    dialogTool->setCurveId(curveId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis *VToolCurveIntersectAxis::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                                         VAbstractPattern *doc,
                                                         VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogCurveIntersectAxis> dialogTool = qobject_cast<DialogCurveIntersectAxis *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolCurveIntersectAxisInitData initData;
    initData.formulaAngle = dialogTool->GetAngle();
    initData.basePointId = dialogTool->GetBasePointId();
    initData.curveId = dialogTool->getCurveId();
    initData.typeLine = dialogTool->GetTypeLine();
    initData.lineColor = dialogTool->GetLineColor();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolCurveIntersectAxis *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolCurveIntersectAxis *VToolCurveIntersectAxis::Create(VToolCurveIntersectAxisInitData &initData)
{
    const QSharedPointer<VPointF> basePoint = initData.data->GeometricObject<VPointF>(initData.basePointId);
    const qreal angle = CheckFormula(initData.id, initData.formulaAngle, initData.data);
    const QSharedPointer<VAbstractCurve> curve = initData.data->GeometricObject<VAbstractCurve>(initData.curveId);

    QPointF fPoint;
    const bool success = FindPoint(static_cast<QPointF>(*basePoint), angle, curve->GetPoints(), &fPoint);

    if (not success)
    {
        const QString errorMsg = tr("Error calculating point '%1'. There is no intersection with curve '%2' and axis"
                                    " through point '%3' with angle %4°")
                .arg(initData.name, curve->name(), basePoint->name()).arg(angle);
        qApp->IsPedantic() ? throw VExceptionObjectError(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
    }

    const qreal segLength = curve->GetLengthByPoint(fPoint);

    VPointF *p = new VPointF(fPoint, initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        initData.data->AddLine(initData.basePointId, initData.id);

        initData.data->getNextId();
        initData.data->getNextId();
        InitSegments(curve->getType(), segLength, p, initData.curveId, initData.data);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        initData.data->AddLine(initData.basePointId, initData.id);

        InitSegments(curve->getType(), segLength, p, initData.curveId, initData.data);

        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::CurveIntersectAxis, initData.doc);
        VToolCurveIntersectAxis *point = new VToolCurveIntersectAxis(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(basePoint->getIdTool());
        initData.doc->IncrementReferens(curve->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolCurveIntersectAxis::FindPoint(const QPointF &point, qreal angle, const QVector<QPointF> &curvePoints,
                                        QPointF *intersectionPoint)
{
    return VAbstractCurve::CurveIntersectAxis(point, angle, curvePoints, intersectionPoint);
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolCurveIntersectAxis::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(m_id);
    fAngle.setPostfix(degreeSymbol);
    fAngle.Eval();
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCurveIntersectAxis::CurveName() const
{
    return VAbstractTool::data.GetGObject(curveId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCurveIntersectAxis>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogCurveIntersectAxis>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                         QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogCurveIntersectAxis> dialogTool = qobject_cast<DialogCurveIntersectAxis *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, basePointId);
    AddDependence(oldDependencies, curveId);
    AddDependence(newDependencies, dialogTool->GetBasePointId());
    AddDependence(newDependencies, dialogTool->getCurveId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
    doc->SetAttribute(domElement, AttrCurve, QString().setNum(dialogTool->getCurveId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
    doc->SetAttribute(tag, AttrCurve, curveId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    curveId = doc->GetParametrUInt(domElement, AttrCurve, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, QString());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCurveIntersectAxis::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolCurveIntersectAxis *visual = qobject_cast<VisToolCurveIntersectAxis *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(curveId);
        visual->setAxisPointId(basePointId);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolCurveIntersectAxis::InitArc(VContainer *data, qreal segLength, const VPointF *p, quint32 curveId)
{
    QSharedPointer<Item> a1;
    QSharedPointer<Item> a2;

    const QSharedPointer<Item> arc = data->GeometricObject<Item>(curveId);
    Item arc1;
    Item arc2;

    if (not VFuzzyComparePossibleNulls(segLength, -1))
    {
        arc->CutArc(segLength, arc1, arc2);
    }
    else
    {
        arc->CutArc(0, arc1, arc2);
    }

    // Arc highly depend on id. Need for creating the name.
    arc1.setId(p->id() + 1);
    arc2.setId(p->id() + 2);

    if (not VFuzzyComparePossibleNulls(segLength, -1))
    {
        a1 = QSharedPointer<Item>(new Item(arc1));
        a2 = QSharedPointer<Item>(new Item(arc2));
    }
    else
    {
        a1 = QSharedPointer<Item>(new Item());
        a2 = QSharedPointer<Item>(new Item());

        // Take names for empty arcs from donors.
        a1->setName(arc1.name());
        a2->setName(arc2.name());
    }

    data->AddArc(a1, arc1.id(), p->id());
    data->AddArc(a2, arc2.id(), p->id());
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
void VToolCurveIntersectAxis::InitSegments(GOType curveType, qreal segLength, const VPointF *p, quint32 curveId,
                                           VContainer *data)
{
    switch(curveType)
    {
        case GOType::EllipticalArc:
            InitArc<VEllipticalArc>(data, segLength, p, curveId);
            break;
        case GOType::Arc:
            InitArc<VArc>(data, segLength, p, curveId);
            break;
        case GOType::CubicBezier:
        case GOType::Spline:
        {
            QSharedPointer<VAbstractBezier> spline1;
            QSharedPointer<VAbstractBezier> spline2;

            const auto spl = data->GeometricObject<VAbstractCubicBezier>(curveId);
            QPointF spl1p2, spl1p3, spl2p2, spl2p3;
            if (not VFuzzyComparePossibleNulls(segLength, -1))
            {
                spl->CutSpline(segLength, spl1p2, spl1p3, spl2p2, spl2p3);
            }
            else
            {
                spl->CutSpline(0, spl1p2, spl1p3, spl2p2, spl2p3);
            }

            VSpline *spl1 = new VSpline(spl->GetP1(), spl1p2, spl1p3, *p);
            VSpline *spl2 = new VSpline(*p, spl2p2, spl2p3, spl->GetP4());

            if (not VFuzzyComparePossibleNulls(segLength, -1))
            {
                spline1 = QSharedPointer<VAbstractBezier>(spl1);
                spline2 = QSharedPointer<VAbstractBezier>(spl2);
            }
            else
            {
                spline1 = QSharedPointer<VAbstractBezier>(new VSpline());
                spline2 = QSharedPointer<VAbstractBezier>(new VSpline());

                // Take names for empty splines from donors.
                spline1->setName(spl1->name());
                spline2->setName(spl2->name());

                delete spl1;
                delete spl2;
            }

            data->AddSpline(spline1, NULL_ID, p->id());
            data->AddSpline(spline2, NULL_ID, p->id());
            break;
        }
        case GOType::CubicBezierPath:
        case GOType::SplinePath:
        {
            QSharedPointer<VAbstractBezier> splP1;
            QSharedPointer<VAbstractBezier> splP2;

            const auto splPath = data->GeometricObject<VAbstractCubicBezierPath>(curveId);
            VSplinePath *splPath1 = nullptr;
            VSplinePath *splPath2 = nullptr;
            if (not VFuzzyComparePossibleNulls(segLength, -1))
            {
                VPointF *pC = VToolCutSplinePath::CutSplinePath(segLength, splPath, p->name(), &splPath1, &splPath2);
                delete pC;
            }
            else
            {
                VPointF *pC = VToolCutSplinePath::CutSplinePath(0, splPath, p->name(), &splPath1, &splPath2);
                delete pC;
            }

            SCASSERT(splPath1 != nullptr)
            SCASSERT(splPath2 != nullptr)

            if (not VFuzzyComparePossibleNulls(segLength, -1))
            {
                splP1 = QSharedPointer<VAbstractBezier>(splPath1);
                splP2 = QSharedPointer<VAbstractBezier>(splPath2);
            }
            else
            {
                splP1 = QSharedPointer<VAbstractBezier>(new VSplinePath());
                splP2 = QSharedPointer<VAbstractBezier>(new VSplinePath());

                // Take names for empty spline paths from donors.
                splP1->setName(splPath1->name());
                splP2->setName(splPath2->name());

                delete splPath1;
                delete splPath2;
            }

            data->AddSpline(splP1, NULL_ID, p->id());
            data->AddSpline(splP2, NULL_ID, p->id());
            break;
        }
        case GOType::Point:
        case GOType::PlaceLabel:
        case GOType::Unknown:
            Q_UNREACHABLE();
            break;
    }
}

QT_WARNING_POP

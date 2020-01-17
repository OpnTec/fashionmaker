/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vtoolmove.h"

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

#include "../../../dialogs/tools/dialogtool.h"
#include "../../../dialogs/tools/dialogmove.h"
#include "../../../visualization/line/operation/vistoolmove.h"
#include "../../../visualization/visualization.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/compatibility.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../ifc/ifcdef.h"
#include "../ifc/exception/vexception.h"
#include "../vwidgets/vabstractsimple.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../vabstracttool.h"
#include "../../vdatatool.h"
#include "../vdrawtool.h"

template <class T> class QSharedPointer;

const QString VToolMove::ToolType = QStringLiteral("moving");

namespace
{
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
QPointF GetOriginPoint(const QVector<quint32> objects, const VContainer *data, qreal calcLength, qreal calcAngle)
{
    QPolygonF originObjects;

    for (auto id : objects)
    {
        const QSharedPointer<VGObject> obj = data->GetGObject(id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects were handled.");

        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
                originObjects.append(data->GeometricObject<VPointF>(id)->toQPointF());
                break;
            case GOType::Arc:
            case GOType::EllipticalArc:
            case GOType::Spline:
            case GOType::SplinePath:
            case GOType::CubicBezier:
            case GOType::CubicBezierPath:
                AppendTo(originObjects, data->GeometricObject<VAbstractCurve>(id)->GetPoints());
                break;
            case GOType::Unknown:
            case GOType::PlaceLabel:
                Q_UNREACHABLE();
                break;
        }
    }

    QPointF rotationOrigin = originObjects.boundingRect().center();
    QLineF move(rotationOrigin, QPointF(rotationOrigin.x()+calcLength, rotationOrigin.y()));
    move.setAngle(calcAngle);
    return move.p2();
}
QT_WARNING_POP
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogMove> dialogTool = qobject_cast<DialogMove *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetRotationAngle(formulaRotationAngle);
    dialogTool->SetLength(formulaLength);
    dialogTool->SetSuffix(suffix);
    dialogTool->SetRotationOrigPointId(origPointId);
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove *VToolMove::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                             VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogMove> dialogTool = qobject_cast<DialogMove *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolMoveInitData initData;
    initData.formulaAngle = dialogTool->GetAngle();
    initData.formulaRotationAngle = dialogTool->GetRotationAngle();
    initData.formulaLength = dialogTool->GetLength();
    initData.rotationOrigin = dialogTool->GetRotationOrigPointId();
    initData.suffix = dialogTool->GetSuffix();
    initData.source = dialogTool->GetObjects();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolMove* operation = Create(initData);
    if (operation != nullptr)
    {
        operation->m_dialog = dialog;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove *VToolMove::Create(VToolMoveInitData &initData)
{
    qreal calcAngle = 0;
    qreal calcRotationAngle = 0;
    qreal calcLength = 0;

    calcAngle = CheckFormula(initData.id, initData.formulaAngle, initData.data);
    calcRotationAngle = CheckFormula(initData.id, initData.formulaRotationAngle, initData.data);
    calcLength = qApp->toPixel(CheckFormula(initData.id, initData.formulaLength, initData.data));

    QPointF rotationOrigin;
    QSharedPointer<VPointF> originPoint;

    if (initData.rotationOrigin == NULL_ID)
    {
        rotationOrigin = GetOriginPoint(initData.source, initData.data, calcLength, calcAngle);
    }
    else
    {
        originPoint = initData.data->GeometricObject<VPointF>(initData.rotationOrigin);
        rotationOrigin = static_cast<QPointF>(*originPoint);
    }

    if (initData.typeCreation == Source::FromGui)
    {
        initData.destination.clear();// Try to avoid mistake, value must be empty

        initData.id = initData.data->getNextId();//Just reserve id for tool

        for (auto idObject : qAsConst(initData.source))
        {
            const QSharedPointer<VGObject> obj = initData.data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    initData.destination.append(CreatePoint(initData.id, idObject, calcAngle, calcLength,
                                                            calcRotationAngle, rotationOrigin, initData.suffix,
                                                            initData.data));
                    break;
                case GOType::Arc:
                    initData.destination.append(CreateArc<VArc>(initData.id, idObject, calcAngle, calcLength,
                                                                calcRotationAngle, rotationOrigin, initData.suffix,
                                                                initData.data));
                    break;
                case GOType::EllipticalArc:
                    initData.destination.append(CreateArc<VEllipticalArc>(initData.id, idObject, calcAngle, calcLength,
                                                                          calcRotationAngle, rotationOrigin,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::Spline:
                    initData.destination.append(CreateCurve<VSpline>(initData.id, idObject, calcAngle, calcLength,
                                                                     calcRotationAngle, rotationOrigin, initData.suffix,
                                                                     initData.data));
                    break;
                case GOType::SplinePath:
                    initData.destination.append(CreateCurveWithSegments<VSplinePath>(initData.id, idObject, calcAngle,
                                                                                     calcLength, calcRotationAngle,
                                                                                     rotationOrigin, initData.suffix,
                                                                                     initData.data));
                    break;
                case GOType::CubicBezier:
                    initData.destination.append(CreateCurve<VCubicBezier>(initData.id, idObject, calcAngle, calcLength,
                                                                          calcRotationAngle, rotationOrigin,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::CubicBezierPath:
                    initData.destination.append(CreateCurveWithSegments<VCubicBezierPath>(initData.id, idObject,
                                                                                          calcAngle, calcLength,
                                                                                          calcRotationAngle,
                                                                                          rotationOrigin,
                                                                                          initData.suffix,
                                                                                          initData.data));
                    break;
                case GOType::Unknown:
                case GOType::PlaceLabel:
                    Q_UNREACHABLE();
                    break;
            }
QT_WARNING_POP
        }
    }
    else
    {
        for (int i = 0; i < initData.source.size(); ++i)
        {
            const quint32 idObject = initData.source.at(i);
            const QSharedPointer<VGObject> obj = initData.data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    UpdatePoint(initData.id, idObject, calcAngle, calcLength, calcRotationAngle, rotationOrigin,
                                initData.suffix, initData.data, initData.destination.at(i));
                    break;
                case GOType::Arc:
                    UpdateArc<VArc>(initData.id, idObject, calcAngle, calcLength, calcRotationAngle, rotationOrigin,
                                    initData.suffix, initData.data, initData.destination.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(initData.id, idObject, calcAngle, calcLength, calcRotationAngle,
                                              rotationOrigin, initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(initData.id, idObject, calcAngle, calcLength, calcRotationAngle,
                                         rotationOrigin, initData.suffix, initData.data, initData.destination.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(initData.id, idObject, calcAngle, calcLength,
                                                         calcRotationAngle, rotationOrigin,initData.suffix,
                                                         initData.data, initData.destination.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(initData.id, idObject, calcAngle, calcLength, calcRotationAngle,
                                              rotationOrigin,initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(initData.id, idObject, calcAngle, calcLength,
                                                              calcRotationAngle, rotationOrigin,
                                                              initData.suffix, initData.data,
                                                              initData.destination.at(i).id);
                    break;
                case GOType::Unknown:
                case GOType::PlaceLabel:
                    Q_UNREACHABLE();
                    break;
            }
QT_WARNING_POP
        }
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::Move, initData.doc);
        VToolMove *tool = new VToolMove(initData);
        initData.scene->addItem(tool);
        InitOperationToolConnections(initData.scene, tool);
        VAbstractPattern::AddTool(initData.id, tool);

        if (not originPoint.isNull())
        {
            initData.doc->IncrementReferens(originPoint->getIdTool());
        }

        for (auto idObject : qAsConst(initData.source))
        {
            initData.doc->IncrementReferens(initData.data->GetGObject(idObject)->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolMove::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(m_id);
    fAngle.setPostfix(degreeSymbol);
    fAngle.Eval();
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolMove::GetFormulaRotationAngle() const
{
    VFormula fAngle(formulaRotationAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(m_id);
    fAngle.setPostfix(degreeSymbol);
    fAngle.Eval();
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetFormulaRotationAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaRotationAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolMove::GetFormulaLength() const
{
    VFormula fLength(formulaLength, getData());
    fLength.setCheckZero(true);
    fLength.setToolId(m_id);
    fLength.setPostfix(UnitsToStr(qApp->patternUnit()));
    fLength.Eval();
    return fLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetFormulaLength(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaLength = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolMove::OriginPointName() const
{
    try
    {
        return VAbstractTool::data.GetGObject(origPointId)->name();
    }
    catch (const VExceptionBadId &)
    {
        return tr("Center point");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolMove>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogMove>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolMove *visual = qobject_cast<VisToolMove *>(vis);
        SCASSERT(visual != nullptr)

        visual->SetObjects(source);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->SetRotationAngle(qApp->TrVars()->FormulaToUser(formulaRotationAngle,
                                                               qApp->Settings()->GetOsSeparator()));
        visual->SetLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->SetRotationOriginPointId(origPointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies, QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogMove> dialogTool = qobject_cast<DialogMove *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, origPointId);
    AddDependence(newDependencies, dialogTool->GetRotationOrigPointId());

    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetLength());
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetRotationOrigPointId()));
    doc->SetAttribute(domElement, AttrRotationAngle, dialogTool->GetRotationAngle());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::ReadToolAttributes(const QDomElement &domElement)
{
    origPointId = doc->GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, QChar('0'));
    formulaRotationAngle = doc->GetParametrString(domElement, AttrRotationAngle, QChar('0'));
    formulaLength = doc->GetParametrString(domElement, AttrLength, QChar('0'));
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrRotationAngle, formulaRotationAngle);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrSuffix, suffix);
    doc->SetAttribute(tag, AttrCenter, QString().setNum(origPointId));

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolMove::MakeToolTip() const
{
    return QStringLiteral("<tr> <td><b>%1:</b> %2°</td> </tr>"
                          "<tr> <td><b>%3:</b> %4 %5</td> </tr>"
                          "<tr> <td><b>%6:</b> %7°</td> </tr>"
                          "<tr> <td><b>%8:</b> %9</td> </tr>")
            .arg(tr("Angle"))                                // 1
            .arg(GetFormulaAngle().getDoubleValue())         // 2
            .arg(tr("Length"))                               // 3
            .arg(GetFormulaLength().getDoubleValue())        // 4
            .arg(UnitsToStr(qApp->patternUnit(), true),      // 5
                 tr("Rotation angle"))                       // 6
            .arg(GetFormulaRotationAngle().getDoubleValue()) // 7
            .arg(tr("Rotation origin point"),                // 8
                 OriginPointName());                         // 9
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove::VToolMove(const VToolMoveInitData &initData, QGraphicsItem *parent)
    : VAbstractOperation(initData.doc, initData.data, initData.id, initData.suffix, initData.source,
                         initData.destination, parent),
      formulaAngle(initData.formulaAngle),
      formulaRotationAngle(initData.formulaRotationAngle),
      formulaLength(initData.formulaLength),
      origPointId(initData.rotationOrigin)
{
    InitOperatedObjects();
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VToolMove::CreatePoint(quint32 idTool, quint32 idItem, qreal angle,
                                         qreal length, qreal rotationAngle, const QPointF &rotationOrigin,
                                       const QString &suffix, VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF moved = point->Move(length, angle, suffix).Rotate(rotationOrigin, rotationAngle);
    moved.setIdObject(idTool);

    DestinationItem item;
    item.mx = moved.mx();
    item.my = moved.my();
    item.showLabel = moved.IsShowLabel();
    item.id = data->AddGObject(new VPointF(moved));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                     const QPointF &rotationOrigin, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix,
                                                  data);
    data->AddArc(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::UpdatePoint(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                            const QPointF &rotationOrigin, const QString &suffix, VContainer *data,
                            const DestinationItem &item)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF moved = point->Move(length, angle, suffix).Rotate(rotationOrigin, rotationAngle);
    moved.setIdObject(idTool);
    moved.setMx(item.mx);
    moved.setMy(item.my);
    moved.SetShowLabel(item.showLabel);
    data->UpdateGObject(item.id, new VPointF(moved));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                          const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix, data, id);
    data->AddArc(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateItem(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                      const QPointF &rotationOrigin, const QString &suffix, VContainer *data)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item moved = i->Move(length, angle, suffix).Rotate(rotationOrigin, rotationAngle);
    moved.setIdObject(idTool);

    DestinationItem item;
    item.mx = INT_MAX;
    item.my = INT_MAX;
    item.id = data->AddGObject(new Item(moved));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                       const QPointF &rotationOrigin, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix,
                                                  data);
    data->AddSpline(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle,
                                                   qreal length, qreal rotationAngle,
                                                   const QPointF &rotationOrigin, const QString &suffix,
                                                   VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix,
                                                  data);
    data->AddCurveWithSegments(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateItem(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                           const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item moved = i->Move(length, angle, suffix).Rotate(rotationOrigin, rotationAngle);
    moved.setIdObject(idTool);
    data->UpdateGObject(id, new Item(moved));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                            const QPointF &rotationOrigin, const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix, data, id);
    data->AddSpline(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle, qreal length, qreal rotationAngle,
                                        const QPointF &rotationOrigin, const QString &suffix, VContainer *data,
                                        quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, rotationAngle, rotationOrigin, suffix, data, id);
    data->AddCurveWithSegments(data->GeometricObject<Item>(id), id);
}

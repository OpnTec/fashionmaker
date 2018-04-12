/************************************************************************
 **
 **  @file   vtoolrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2016
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

#include "vtoolrotation.h"

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
#include "../../../dialogs/tools/dialogrotation.h"
#include "../../../visualization/line/operation/vistoolrotation.h"
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
#include "../vmisc/logging.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../ifc/ifcdef.h"
#include "../ifc/exception/vexception.h"
#include "../vwidgets/vabstractsimple.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../vabstracttool.h"
#include "../../vdatatool.h"
#include "../vdrawtool.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
template <class T> class QSharedPointer;

const QString VToolRotation::ToolType = QStringLiteral("rotation");

//---------------------------------------------------------------------------------------------------------------------
VToolRotation::VToolRotation(const VToolRotationInitData &initData, QGraphicsItem *parent)
    : VAbstractOperation(initData.doc, initData.data, initData.id, initData.suffix, initData.source,
                         initData.destination, parent),
      origPointId(initData.origin),
      formulaAngle(initData.angle)
{
    InitOperatedObjects();
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogRotation> dialogTool = m_dialog.objectCast<DialogRotation>();
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetOrigPointId(origPointId);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation *VToolRotation::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                     VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogRotation> dialogTool = dialog.objectCast<DialogRotation>();
    SCASSERT(not dialogTool.isNull())

    VToolRotationInitData initData;
    initData.origin = dialogTool->GetOrigPointId();
    initData.angle = dialogTool->GetAngle();
    initData.suffix = dialogTool->GetSuffix();
    initData.source = dialogTool->GetObjects();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolRotation* operation = Create(initData);
    if (operation != nullptr)
    {
        operation->m_dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation *VToolRotation::Create(VToolRotationInitData &initData)
{
    qreal calcAngle = 0;

    calcAngle = CheckFormula(initData.id, initData.angle, initData.data);

    const auto originPoint = *initData.data->GeometricObject<VPointF>(initData.origin);
    const QPointF oPoint = static_cast<QPointF>(originPoint);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.destination.clear();// Try to avoid mistake, value must be empty

        initData.id = VContainer::getNextId();//Just reserve id for tool

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
                    initData.destination.append(CreatePoint(initData.id, idObject, oPoint, calcAngle, initData.suffix,
                                                            initData.data));
                    break;
                case GOType::Arc:
                    initData.destination.append(CreateArc<VArc>(initData.id, idObject, oPoint, calcAngle,
                                                                initData.suffix, initData.data));
                    break;
                case GOType::EllipticalArc:
                    initData.destination.append(CreateArc<VEllipticalArc>(initData.id, idObject, oPoint, calcAngle,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::Spline:
                    initData.destination.append(CreateCurve<VSpline>(initData.id, idObject, oPoint, calcAngle,
                                                                     initData.suffix, initData.data));
                    break;
                case GOType::SplinePath:
                    initData.destination.append(CreateCurveWithSegments<VSplinePath>(initData.id, idObject, oPoint,
                                                                                     calcAngle, initData.suffix,
                                                                                     initData.data));
                    break;
                case GOType::CubicBezier:
                    initData.destination.append(CreateCurve<VCubicBezier>(initData.id, idObject, oPoint, calcAngle,
                                                                          initData.suffix, initData.data));
                    break;
                case GOType::CubicBezierPath:
                    initData.destination.append(CreateCurveWithSegments<VCubicBezierPath>(initData.id, idObject, oPoint,
                                                                                          calcAngle, initData.suffix,
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
                {
                    UpdatePoint(initData.id, idObject, oPoint, calcAngle, initData.suffix, initData.data,
                                initData.destination.at(i));
                    break;
                }
                case GOType::Arc:
                    UpdateArc<VArc>(initData.id, idObject, oPoint, calcAngle, initData.suffix, initData.data,
                                    initData.destination.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(initData.id, idObject, oPoint, calcAngle, initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(initData.id, idObject, oPoint, calcAngle, initData.suffix, initData.data,
                                         initData.destination.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(initData.id, idObject, oPoint, calcAngle, initData.suffix,
                                                         initData.data, initData.destination.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(initData.id, idObject, oPoint, calcAngle, initData.suffix, initData.data,
                                              initData.destination.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(initData.id, idObject, oPoint, calcAngle, initData.suffix,
                                                              initData.data, initData.destination.at(i).id);
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
        VAbstractTool::AddRecord(initData.id, Tool::Rotation, initData.doc);
        VToolRotation *tool = new VToolRotation(initData);
        initData.scene->addItem(tool);
        InitOperationToolConnections(initData.scene, tool);
        VAbstractPattern::AddTool(initData.id, tool);
        initData.doc->IncrementReferens(originPoint.getIdTool());
        for (auto idObject : qAsConst(initData.source))
        {
            initData.doc->IncrementReferens(initData.data->GetGObject(idObject)->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolRotation::OriginPointName() const
{
    return VAbstractTool::data.GetGObject(origPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolRotation::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(m_id);
    fAngle.setPostfix(degreeSymbol);
    fAngle.Eval();
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolRotation>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogRotation>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolRotation *visual = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(visual != nullptr)

        visual->SetObjects(source);
        visual->SetOriginPointId(origPointId);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogRotation> dialogTool = m_dialog.objectCast<DialogRotation>();
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, origPointId);
    AddDependence(newDependencies, dialogTool->GetOrigPointId());

    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetOrigPointId()));
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ReadToolAttributes(const QDomElement &domElement)
{
    origPointId = doc->GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, QChar('0'));
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrCenter, QString().setNum(origPointId));
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrSuffix, suffix);

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolRotation::MakeToolTip() const
{
    const QString toolTip = QString("<tr> <td><b>%1:</b> %2</td> </tr>"
                                    "<tr> <td><b>%3:</b> %4°</td> </tr>")
            .arg(tr("Origin point"), OriginPointName(), tr("Rotation angle"))
            .arg(GetFormulaAngle().getDoubleValue());
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VToolRotation::CreatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                          const QString &suffix, VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Rotate(origin, angle, suffix);
    rotated.setIdObject(idTool);

    DestinationItem item;
    item.mx = rotated.mx();
    item.my = rotated.my();
    item.showLabel = rotated.IsShowLabel();
    item.id = data->AddGObject(new VPointF(rotated));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolRotation::CreateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                           const QString &suffix, VContainer *data)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item rotated = i->Rotate(origin, angle, suffix);
    rotated.setIdObject(idTool);

    DestinationItem item;
    item.mx = INT_MAX;
    item.my = INT_MAX;
    item.id = data->AddGObject(new Item(rotated));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolRotation::CreateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                          const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, origin, angle, suffix, data);
    data->AddArc(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolRotation::CreateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                           const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, origin, angle, suffix, data);
    data->AddSpline(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolRotation::CreateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin,
                                                       qreal angle, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, origin, angle, suffix, data);
    data->AddCurveWithSegments(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::UpdatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                const QString &suffix, VContainer *data, const DestinationItem &item)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Rotate(origin, angle, suffix);
    rotated.setIdObject(idTool);
    rotated.setMx(item.mx);
    rotated.setMy(item.my);
    rotated.SetShowLabel(item.showLabel);
    data->UpdateGObject(item.id, new VPointF(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                               const QString &suffix, VContainer *data, quint32 id)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item rotated = i->Rotate(origin, angle, suffix);
    rotated.setIdObject(idTool);
    data->UpdateGObject(id, new Item(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                              const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddArc(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddSpline(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                            const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddCurveWithSegments(data->GeometricObject<Item>(id), id);
}

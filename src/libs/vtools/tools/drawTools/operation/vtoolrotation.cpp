/************************************************************************
 **
 **  @file   vtoolrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2016
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
VToolRotation::VToolRotation(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 origPointId,
                             const QString &angle, const QString &suffix, const QVector<quint32> &source,
                             const QVector<DestinationItem> &destination, const Source &typeCreation,
                             QGraphicsItem *parent)
    : VAbstractOperation(doc, data, id, suffix, source, destination, parent),
      origPointId(origPointId),
      formulaAngle(angle)
{
    InitOperatedObjects();
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation::~VToolRotation()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr)
    dialogTool->SetOrigPointId(origPointId);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation *VToolRotation::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                     VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const quint32 originPointId = dialogTool->GetOrigPointId();
    QString angle = dialogTool->GetAngle();
    const QString suffix = dialogTool->GetSuffix();
    const QVector<quint32> source = dialogTool->GetObjects();
    VToolRotation* operation = Create(0, originPointId, angle, suffix, source, QVector<DestinationItem>(),
                                      scene, doc, data, Document::FullParse, Source::FromGui);
    if (operation != nullptr)
    {
        operation->dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation *VToolRotation::Create(const quint32 _id, const quint32 &origin, QString &angle, const QString &suffix,
                                     const QVector<quint32> &source,
                                     const QVector<DestinationItem> &destination,
                                     VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                     const Document &parse, const Source &typeCreation)
{
    qreal calcAngle = 0;

    calcAngle = CheckFormula(_id, angle, data);

    const auto originPoint = *data->GeometricObject<VPointF>(origin);
    const QPointF oPoint = originPoint;

    QVector<DestinationItem> dest = destination;

    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        dest.clear();// Try to avoid mistake, value must be empty

        id = data->getNextId();//Just reserve id for tool

        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    dest.append(CreatePoint(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::Arc:
                    dest.append(CreateArc<VArc>(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::EllipticalArc:
                    dest.append(CreateArc<VEllipticalArc>(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::Spline:
                    dest.append(CreateCurve<VSpline>(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::SplinePath:
                    dest.append(CreateCurveWithSegments<VSplinePath>(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::CubicBezier:
                    dest.append(CreateCurve<VCubicBezier>(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::CubicBezierPath:
                    dest.append(CreateCurveWithSegments<VCubicBezierPath>(id, idObject, oPoint, calcAngle, suffix,
                                                                          data));
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
    }
    else
    {
        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    UpdatePoint(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id, dest.at(i).mx,
                                dest.at(i).my);
                    break;
                case GOType::Arc:
                    UpdateArc<VArc>(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(id, idObject, oPoint, calcAngle, suffix, data,
                                                              dest.at(i).id);
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::Rotation, doc);
        VToolRotation *tool = new VToolRotation(doc, data, id, origin, angle, suffix, source, dest, typeCreation);
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
QString VToolRotation::OriginPointName() const
{
    return VAbstractTool::data.GetGObject(origPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolRotation::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolRotation>(show);
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
void VToolRotation::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr)

    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetOrigPointId()));
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ReadToolAttributes(const QDomElement &domElement)
{
    origPointId = doc->GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, "0");
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
void VToolRotation::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogRotation>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
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
                                const QString &suffix, VContainer *data, quint32 id, qreal mx, qreal my)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Rotate(origin, angle, suffix);
    rotated.setIdObject(idTool);
    rotated.setMx(mx);
    rotated.setMy(my);
    data->UpdateGObject(id, new VPointF(rotated));
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

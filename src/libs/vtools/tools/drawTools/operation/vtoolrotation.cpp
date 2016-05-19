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
#include "../../../dialogs/tools/dialogrotation.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../../../visualization/line/vistoolrotation.h"
#include "../vwidgets/vsimplepoint.h"
#include "../vwidgets/vsimplecurve.h"
#include "../../../undocommands/label/rotationmovelabel.h"
#include "../vpatterndb/vformula.h"

const QString VToolRotation::ToolType       = QStringLiteral("rotation");
const QString VToolRotation::TagItem        = QStringLiteral("item");
const QString VToolRotation::TagSource      = QStringLiteral("source");
const QString VToolRotation::TagDestination = QStringLiteral("destination");

//---------------------------------------------------------------------------------------------------------------------
VToolRotation::VToolRotation(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 origPointId,
                             const QString &angle, const QString &suffix, const QVector<quint32> &source,
                             const QVector<DestinationItem> &destination, const Source &typeCreation,
                             QGraphicsItem *parent)
    : VDrawTool(doc, data, id),
      QGraphicsItem(parent),
      origPointId(origPointId),
      formulaAngle(angle),
      suffix(suffix),
      source(source),
      destination(destination),
      rObjects()
{
    for (int i = 0; i < destination.size(); ++i)
    {
        const DestinationItem object = destination.at(i);
        const QSharedPointer<VGObject> obj = data->GetGObject(object.id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

#if defined(Q_CC_GNU)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-default"
#endif
        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                VSimplePoint *point = new VSimplePoint(object.id, QColor(baseColor), *data->GetPatternUnit(), &factor);
                point->setParentItem(this);
                point->SetType(GOType::Point);
                connect(point, &VSimplePoint::Choosed, [this](quint32 id)
                {
                    emit ChoosedTool(id, SceneObject::Point);
                });
                connect(point, &VSimplePoint::Selected, this, &VToolRotation::ObjectSelected);
                connect(point, &VSimplePoint::ShowContextMenu, this, &VToolRotation::contextMenuEvent);
                connect(point, &VSimplePoint::Delete, this, &VToolRotation::DeleteFromLabel);
                connect(point, &VSimplePoint::NameChangedPosition, this, &VToolRotation::LabelChangePosition);
                point->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(object.id));
                rObjects.insert(object.id, point);
                break;
            }
            case GOType::Arc:
            case GOType::EllipticalArc:
            {
                VSimpleCurve *curve = InitCurve(object.id, data, GOType::Arc);
                connect(curve, &VSimpleCurve::Choosed, [this](quint32 id)
                {
                    emit ChoosedTool(id, SceneObject::Arc);
                });
                break;
            }
            case GOType::Spline:
            case GOType::CubicBezier:
            {
                VSimpleCurve *curve = InitCurve(object.id, data, GOType::Spline);
                connect(curve, &VSimpleCurve::Choosed, [this](quint32 id)
                {
                    emit ChoosedTool(id, SceneObject::Spline);
                });
                break;
            }
            case GOType::SplinePath:
            case GOType::CubicBezierPath:
            {
                VSimpleCurve *curve = InitCurve(object.id, data, GOType::SplinePath);
                connect(curve, &VSimpleCurve::Choosed, [this](quint32 id)
                {
                    emit ChoosedTool(id, SceneObject::SplinePath);
                });
                break;
            }
            case GOType::Unknown:
                break;
        }
#if defined(Q_CC_GNU)
    #pragma GCC diagnostic pop
#endif
    }

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation::~VToolRotation()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->SetOrigPointId(origPointId);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolRotation *VToolRotation::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                     VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr);
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

#if defined(Q_CC_GNU)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-default"
#endif
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    dest.append(CreatePoint(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::Arc:
                    dest.append(CreateArc(id, idObject, oPoint, calcAngle, suffix, data));
                    break;
                case GOType::EllipticalArc:
                    //dest.append(CreateItem<VEllipticalArc>(id, idObject, oPoint, angle, suffix));
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
#if defined(Q_CC_GNU)
    #pragma GCC diagnostic pop
#endif
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

#if defined(Q_CC_GNU)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-default"
#endif
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    UpdatePoint(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id, dest.at(i).mx,
                                dest.at(i).my);
                    break;
                case GOType::Arc:
                    UpdateArc(id, idObject, oPoint, calcAngle, suffix, data, dest.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    //dest.append(UpdateItem<VEllipticalArc>(id, idObject, oPoint, angle, suffix, data));
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
#if defined(Q_CC_GNU)
    #pragma GCC diagnostic pop
#endif
        }
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::Rotation, doc);
    if (parse == Document::FullParse)
    {
        VToolRotation *tool = new VToolRotation(doc, data, id, origin, angle, suffix, source, dest, typeCreation);
        scene->addItem(tool);
        InitRotationToolConnections(scene, tool);
        doc->AddTool(id, tool);
        doc->IncrementReferens(originPoint.getIdTool());
        for (int i = 0; i < source.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(source.at(i))->getIdTool());
        }

        for (int i = 0; i < dest.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(dest.at(i).id)->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ExtractData(VAbstractPattern *doc, const QDomElement &domElement, QVector<quint32> &source,
                                QVector<DestinationItem> &destination)
{
    SCASSERT(doc != nullptr)
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement dataElement = nodeList.at(i).toElement();
        if (not dataElement.isNull() && dataElement.tagName() == TagSource)
        {
            source.clear();
            const QDomNodeList srcList = dataElement.childNodes();
            for (qint32 j = 0; j < srcList.size(); ++j)
            {
                const QDomElement element = srcList.at(j).toElement();
                if (not element.isNull())
                {
                    source.append(doc->GetParametrUInt(element, AttrIdObject, NULL_ID_STR));
                }
            }
        }

        if (not dataElement.isNull() && dataElement.tagName() == TagDestination)
        {
            destination.clear();
            const QDomNodeList srcList = dataElement.childNodes();
            for (qint32 j = 0; j < srcList.size(); ++j)
            {
                const QDomElement element = srcList.at(j).toElement();
                if (not element.isNull())
                {
                    DestinationItem d;
                    d.id = doc->GetParametrUInt(element, AttrIdObject, NULL_ID_STR);
                    d.mx = qApp->toPixel(doc->GetParametrDouble(element, AttrMx, "0.0"));
                    d.my = qApp->toPixel(doc->GetParametrDouble(element, AttrMy, "0.0"));
                    destination.append(d);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolRotation::getTagName() const
{
    return VAbstractPattern::TagOperation;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetEnabled(bool enabled)
{
    this->setEnabled(enabled);
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
QString VToolRotation::Suffix() const
{
    return suffix;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetSuffix(const QString &suffix)
{
    // Don't know if need check name here.
    this->suffix = suffix;
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::GroupVisibility(quint32 object, bool visible)
{
    if (rObjects.contains(object))
    {
        VAbstractSimple *obj = rObjects.value(object);
        if (obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr);
            item->setVisible(visible);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(obj);
            SCASSERT(item != nullptr);
            item->setVisible(visible);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolRotation>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::FullUpdateFromFile()
{
    ReadAttributes();
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(i.key()));
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            item->RefreshGeometry(VAbstractTool::data.GeometricObject<VAbstractCurve>(i.key()));
        }
    }
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(i.key()));
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            item->RefreshGeometry(VAbstractTool::data.GeometricObject<VAbstractCurve>(i.key()));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->setAcceptHoverEvents(enabled);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            item->setAcceptHoverEvents(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowPointHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->setAcceptHoverEvents(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowPointSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowPointLabelHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->AllowLabelHover(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowPointLabelSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr);
            item->AllowLabelSelecting(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowSplineHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::Spline);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowSplineSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::Spline);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowSplinePathHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::SplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowSplinePathSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::SplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowArcHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::Arc);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowArcSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::Arc);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
        SaveOptions(domElement, obj);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolRotation *visual = qobject_cast<VisToolRotation *>(vis);
        SCASSERT(visual != nullptr);

        visual->SetObjects(source);
        visual->SetOriginPointId(origPointId);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VToolRotation::boundingRect() const
{
    QRectF recTool;
    const QList<QGraphicsItem *> items = childItems();
    foreach (QGraphicsItem *item, items)
    {
        recTool = recTool.united(item->boundingRect());
        recTool = recTool.united(item->childrenBoundingRect());
    }

    return recTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::Disable(bool disable, const QString &namePP)
{
    enabled = !CorrectDisable(disable, namePP);
    SetEnabled(enabled);

    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        i.value()->SetEnabled(enabled);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ObjectSelected(bool selected, quint32 objId)
{
    emit ChangedToolSelection(selected, objId, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::DeleteFromLabel()
{
    try
    {
        DeleteTool();
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::LabelChangePosition(const QPointF &pos, quint32 labelId)
{
    if (rObjects.contains(labelId))
    {
        VAbstractSimple *obj = rObjects.value(labelId);
        if (obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr);
            ChangePosition(item, labelId, pos);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(obj);
            SCASSERT(item != nullptr);
            ChangePosition(item, labelId, pos);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogRotation *dialogTool = qobject_cast<DialogRotation*>(dialog);
    SCASSERT(dialogTool != nullptr);

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

    doc->RemoveAllChildren(tag);

    QDomElement tagObjects = doc->createElement(TagSource);
    for (int i = 0; i < source.size(); ++i)
    {
        QDomElement item = doc->createElement(TagItem);
        doc->SetAttribute(item, AttrIdObject, source.at(i));
        tagObjects.appendChild(item);
    }
    tag.appendChild(tagObjects);

    tagObjects = doc->createElement(TagDestination);
    for (int i = 0; i < destination.size(); ++i)
    {
        QDomElement item = doc->createElement(TagItem);
        doc->SetAttribute(item, AttrIdObject, destination.at(i).id);
        doc->SetAttribute(item, AttrMx, qApp->fromPixel(destination.at(i).mx));
        doc->SetAttribute(item, AttrMy, qApp->fromPixel(destination.at(i).my));
        tagObjects.appendChild(item);
    }
    tag.appendChild(tagObjects);
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
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::DoChangePosition(quint32 id, qreal mx, qreal my)
{
    if (rObjects.contains(id))
    {
        VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<VPointF>(id));
        point->setMx(mx);
        point->setMy(my);
        VAbstractTool::data.UpdateGObject(id, point);

        VSimplePoint *item = qobject_cast<VSimplePoint *>(rObjects.value(id));
        SCASSERT(item != nullptr);

        item->blockSignals(true);
        item->setPos(QPointF(mx, my));
        item->blockSignals(false);
        item->RefreshLine();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::UpdateNamePosition(quint32 id)
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    auto moveLabel = new RotationMoveLabel(this->id, doc, point->mx(), point->my(), id);
    connect(moveLabel, &RotationMoveLabel::ChangePosition, this, &VToolRotation::DoChangePosition);
    qApp->getUndoStack()->push(moveLabel);
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
DestinationItem VToolRotation::CreateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                          const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<VArc>(idTool, idItem, origin, angle, suffix, data);
    data->AddArc(data->GeometricObject<VArc>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolRotation::CreateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                           const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, origin, angle, suffix, data);
    data->AddCurve(data->GeometricObject<Item>(item.id), item.id);
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
void VToolRotation::UpdateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                               const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<VArc>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddArc(data->GeometricObject<VArc>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                               const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddCurve(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolRotation::UpdateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                               const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, origin, angle, suffix, data, id);
    data->AddCurveWithSegments(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VToolRotation::ShowToolVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            AddVisualization<T>();
            SetVisualization();
        }
        else
        {
            if (T *visual = qobject_cast<T *>(vis))
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        vis = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::ChangePosition(QGraphicsItem *item, quint32 id, const QPointF &pos)
{
    const QPointF p = pos - item->pos();
    DoChangePosition(id, p.x(), p.y());
    UpdateNamePosition(id);
}

//---------------------------------------------------------------------------------------------------------------------
VSimpleCurve *VToolRotation::InitCurve(quint32 id, VContainer *data, GOType curveType)
{
    VSimpleCurve *curve = new VSimpleCurve(id, QColor(baseColor), *data->GetPatternUnit(), &factor);
    curve->setParentItem(this);
    curve->SetType(curveType);
    connect(curve, &VSimpleCurve::Selected, this, &VToolRotation::ObjectSelected);
    connect(curve, &VSimpleCurve::ShowContextMenu, this, &VToolRotation::contextMenuEvent);
    curve->RefreshGeometry(VAbstractTool::data.GeometricObject<VAbstractCurve>(id));
    rObjects.insert(id, curve);
    return curve;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowCurveHover(bool enabled, GOType type)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() != GOType::Point)
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            if (item->GetType() == type)
            {
                item->setAcceptHoverEvents(enabled);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AllowCurveSelecting(bool enabled, GOType type)
{
    QMapIterator<quint32, VAbstractSimple *> i(rObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() != GOType::Point)
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr);
            if (item->GetType() == type)
            {
                item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolRotation::AddSourceObject(VAbstractPattern *doc, QDomElement &domElement, quint32 objId)
{
    QDomElement obj = doc->createElement(TagItem);
    doc->SetAttribute(obj, AttrIdObject, objId);
    domElement.appendChild(obj);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VToolRotation::InitRotationToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

    InitDrawToolConnections(scene, tool);

    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemHover, tool, &T::AllowPointHover);
    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemSelection, tool, &T::AllowPointSelecting);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemHover, tool, &T::AllowPointLabelHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemSelection, tool, &T::AllowPointLabelSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemHover, tool, &T::AllowSplineHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemSelection, tool, &T::AllowSplineSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemHover, tool, &T::AllowSplinePathHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemSelection, tool, &T::AllowSplinePathSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemHover, tool, &T::AllowArcHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemSelection, tool, &T::AllowArcSelecting);
}

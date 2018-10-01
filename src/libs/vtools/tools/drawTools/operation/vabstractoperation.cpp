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

#include "vabstractoperation.h"
#include "../../../undocommands/label/operationmovelabel.h"
#include "../../../undocommands/label/operationshowlabel.h"
#include "../vgeometry/vpointf.h"

const QString VAbstractOperation::TagItem        = QStringLiteral("item");
const QString VAbstractOperation::TagSource      = QStringLiteral("source");
const QString VAbstractOperation::TagDestination = QStringLiteral("destination");

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractOperation::getTagName() const
{
    return VAbstractPattern::TagOperation;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractOperation::Suffix() const
{
    return suffix;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::SetSuffix(const QString &suffix)
{
    // Don't know if need check name here.
    this->suffix = suffix;
    QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::GroupVisibility(quint32 object, bool visible)
{
    if (operatedObjects.contains(object))
    {
        VAbstractSimple *obj = operatedObjects.value(object);
        if (obj && obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr)
            item->setVisible(visible);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(obj);
            SCASSERT(item != nullptr)
            item->setVisible(visible);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::ChangeLabelPosition(quint32 id, const QPointF &pos)
{
    if (operatedObjects.contains(id))
    {
        VAbstractSimple *obj = operatedObjects.value(id);
        if (obj && obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr)
            QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
            point->setMx(pos.x());
            point->setMy(pos.y());
            item->RefreshPointGeometry(*(point.data()));

            if (QGraphicsScene *sc = item->scene())
            {
                VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), item);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractOperation::IsLabelVisible(quint32 id) const
{
    if (operatedObjects.contains(id))
    {
        VAbstractSimple *obj = operatedObjects.value(id);
        if (obj && obj->GetType() == GOType::Point)
        {
            return VAbstractTool::data.GeometricObject<VPointF>(id)->IsShowLabel();
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::SetLabelVisible(quint32 id, bool visible)
{
    if (operatedObjects.contains(id))
    {
        VAbstractSimple *obj = operatedObjects.value(id);
        if (obj && obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr)
            const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
            point->SetShowLabel(visible);
            item->RefreshPointGeometry(*point);
            if (QGraphicsScene *sc = item->scene())
            {
                VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), item);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::ExtractData(const QDomElement &domElement, VAbstractOperationInitData &initData)
{
    const QDomNodeList nodeList = domElement.childNodes();
    for (qint32 i = 0; i < nodeList.size(); ++i)
    {
        const QDomElement dataElement = nodeList.at(i).toElement();
        if (not dataElement.isNull() && dataElement.tagName() == TagSource)
        {
            initData.source.clear();
            const QDomNodeList srcList = dataElement.childNodes();
            for (qint32 j = 0; j < srcList.size(); ++j)
            {
                const QDomElement element = srcList.at(j).toElement();
                if (not element.isNull())
                {
                    initData.source.append(VDomDocument::GetParametrUInt(element, AttrIdObject, NULL_ID_STR));
                }
            }
        }

        if (not dataElement.isNull() && dataElement.tagName() == TagDestination)
        {
            initData.destination.clear();
            const QDomNodeList srcList = dataElement.childNodes();
            for (qint32 j = 0; j < srcList.size(); ++j)
            {
                const QDomElement element = srcList.at(j).toElement();
                if (not element.isNull())
                {
                    DestinationItem d;
                    d.id = VDomDocument::GetParametrUInt(element, AttrIdObject, NULL_ID_STR);
                    d.mx = qApp->toPixel(VDomDocument::GetParametrDouble(element, AttrMx, QString::number(INT_MAX)));
                    d.my = qApp->toPixel(VDomDocument::GetParametrDouble(element, AttrMy, QString::number(INT_MAX)));
                    d.showLabel = VDomDocument::GetParametrBool(element, AttrShowLabel, trueStr);
                    initData.destination.append(d);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::FullUpdateFromFile()
{
    ReadAttributes();
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->setToolTip(ComplexPointToolTip(i.key()));
            item->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(i.key()));
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            item->setToolTip(ComplexCurveToolTip(i.key()));
            item->RefreshGeometry(VAbstractTool::data.GeometricObject<VAbstractCurve>(i.key()));
        }
    }
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->setAcceptHoverEvents(enabled);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            item->setAcceptHoverEvents(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::EnableToolMove(bool move)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->EnableToolMove(move);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowPointHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->setAcceptHoverEvents(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowPointSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowPointLabelHover(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->AllowLabelHover(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowPointLabelSelecting(bool enabled)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->AllowLabelSelecting(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowSplineHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::Spline);
    AllowCurveHover(enabled, GOType::CubicBezier);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowSplineSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::Spline);
    AllowCurveSelecting(enabled, GOType::CubicBezier);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowSplinePathHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::SplinePath);
    AllowCurveHover(enabled, GOType::CubicBezierPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowSplinePathSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::SplinePath);
    AllowCurveSelecting(enabled, GOType::CubicBezierPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowArcHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::Arc);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowArcSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::Arc);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowElArcHover(bool enabled)
{
    AllowCurveHover(enabled, GOType::EllipticalArc);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowElArcSelecting(bool enabled)
{
    AllowCurveSelecting(enabled, GOType::EllipticalArc);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::ToolSelectionType(const SelectionType &type)
{
    VAbstractTool::ToolSelectionType(type);
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->ToolSelectionType(selectionType);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::Disable(bool disable, const QString &namePP)
{
    const bool enabled = !CorrectDisable(disable, namePP);
    setEnabled(enabled);

    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(i.value());
            SCASSERT(item != nullptr)
            item->SetEnabled(enabled);
        }
        else
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            item->setEnabled(enabled);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::ObjectSelected(bool selected, quint32 objId)
{
    emit ChangedToolSelection(selected, objId, m_id);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::DeleteFromLabel()
{
    try
    {
        DeleteToolWithConfirm();
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::LabelChangePosition(const QPointF &pos, quint32 labelId)
{
    if (operatedObjects.contains(labelId))
    {
        VAbstractSimple *obj = operatedObjects.value(labelId);
        if (obj && obj->GetType() == GOType::Point)
        {
            VSimplePoint *item = qobject_cast<VSimplePoint *>(obj);
            SCASSERT(item != nullptr)
            UpdateNamePosition(labelId, pos - item->pos());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractOperation::VAbstractOperation(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &suffix,
                                       const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                       QGraphicsItem *parent)
    : VDrawTool(doc, data, id),
      QGraphicsLineItem(parent),
      suffix(suffix),
      source(source),
      destination(destination),
      operatedObjects()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::ChangeLabelVisibility(quint32 id, bool visible)
{
    if (operatedObjects.contains(id))
    {
        VAbstractSimple *obj = operatedObjects.value(id);
        if (obj && obj->GetType() == GOType::Point)
        {
            qApp->getUndoStack()->push(new OperationShowLabel(doc, m_id, id, visible));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::UpdateNamePosition(quint32 id, const QPointF &pos)
{
    if (operatedObjects.contains(id))
    {
        VAbstractSimple *obj = operatedObjects.value(id);
        if (obj && obj->GetType() == GOType::Point)
        {
            qApp->getUndoStack()->push(new OperationMoveLabel(m_id, doc, pos, id));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::SaveSourceDestination(QDomElement &tag)
{
    doc->RemoveAllChildren(tag);

    QDomElement tagObjects = doc->createElement(TagSource);
    for (auto id : qAsConst(source))
    {
        QDomElement item = doc->createElement(TagItem);
        doc->SetAttribute(item, AttrIdObject, id);
        tagObjects.appendChild(item);
    }
    tag.appendChild(tagObjects);

    tagObjects = doc->createElement(TagDestination);
    for (auto dItem : qAsConst(destination))
    {
        QDomElement item = doc->createElement(TagItem);
        doc->SetAttribute(item, AttrIdObject, dItem.id);

        if (not VFuzzyComparePossibleNulls(dItem.mx, INT_MAX) &&
            not VFuzzyComparePossibleNulls(dItem.my, INT_MAX))
        {
            doc->SetAttribute(item, AttrMx, qApp->fromPixel(dItem.mx));
            doc->SetAttribute(item, AttrMy, qApp->fromPixel(dItem.my));
            doc->SetAttribute<bool>(item, AttrShowLabel, dItem.showLabel);
        }

        tagObjects.appendChild(item);
    }
    tag.appendChild(tagObjects);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::InitCurve(quint32 id, VContainer *data, GOType curveType, SceneObject sceneType)
{
    const QSharedPointer<VAbstractCurve> initCurve = data->GeometricObject<VAbstractCurve>(id);
    VSimpleCurve *curve = new VSimpleCurve(id, initCurve);
    curve->setParentItem(this);
    curve->SetType(curveType);
    curve->setToolTip(ComplexCurveToolTip(id));
    connect(curve, &VSimpleCurve::Selected, this, &VAbstractOperation::ObjectSelected);
    connect(curve, &VSimpleCurve::ShowContextMenu, this, [this](QGraphicsSceneContextMenuEvent * event, quint32 id)
    {
        ShowContextMenu(event, id);
    });
    connect(curve, &VSimpleCurve::Choosed, this, [this, sceneType](quint32 id)
    {
        emit ChoosedTool(id, sceneType);
    });
    connect(curve, &VSimpleCurve::Delete, this, &VAbstractOperation::DeleteFromLabel);
    curve->RefreshGeometry(VAbstractTool::data.GeometricObject<VAbstractCurve>(id));
    operatedObjects.insert(id, curve);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowCurveHover(bool enabled, GOType type)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() != GOType::Point)
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            if (item->GetType() == type)
            {
                item->setAcceptHoverEvents(enabled);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::AllowCurveSelecting(bool enabled, GOType type)
{
    QMapIterator<quint32, VAbstractSimple *> i(operatedObjects);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->GetType() != GOType::Point)
        {
            VSimpleCurve *item = qobject_cast<VSimpleCurve *>(i.value());
            SCASSERT(item != nullptr)
            if (item->GetType() == type)
            {
                item->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractOperation::InitOperatedObjects()
{
    for (auto object : qAsConst(destination))
    {
        const QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(object.id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                VSimplePoint *point = new VSimplePoint(object.id, QColor(Qt::black));
                point->setParentItem(this);
                point->SetType(GOType::Point);
                point->setToolTip(ComplexPointToolTip(object.id));
                connect(point, &VSimplePoint::Choosed, this, [this](quint32 id)
                {
                    emit ChoosedTool(id, SceneObject::Point);
                });
                connect(point, &VSimplePoint::Selected, this, &VAbstractOperation::ObjectSelected);
                connect(point, &VSimplePoint::ShowContextMenu,
                        this, [this](QGraphicsSceneContextMenuEvent * event, quint32 id)
                {
                    ShowContextMenu(event, id);
                });
                connect(point, &VSimplePoint::Delete, this, &VAbstractOperation::DeleteFromLabel);
                connect(point, &VSimplePoint::NameChangedPosition, this, &VAbstractOperation::LabelChangePosition);
                point->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(object.id));
                operatedObjects.insert(object.id, point);
                break;
            }
            case GOType::Arc:
                InitCurve(object.id, &(VAbstractTool::data), obj->getType(), SceneObject::Arc);
                break;
            case GOType::EllipticalArc:
                InitCurve(object.id, &(VAbstractTool::data), obj->getType(), SceneObject::ElArc);
                break;
            case GOType::Spline:
            case GOType::CubicBezier:
                InitCurve(object.id, &(VAbstractTool::data), obj->getType(), SceneObject::Spline);
                break;
            case GOType::SplinePath:
            case GOType::CubicBezierPath:
                InitCurve(object.id, &(VAbstractTool::data), obj->getType(), SceneObject::SplinePath);
                break;
            case GOType::Unknown:
            case GOType::PlaceLabel:
                Q_UNREACHABLE();
                break;
        }
QT_WARNING_POP
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractOperation::ComplexPointToolTip(quint32 itemId) const
{
    return QStringLiteral("<table>"
                          "<tr> <td><b>%1:</b> %2</td> </tr>"
                          "%3"
                          "</table>")
            .arg(tr("Label"), VAbstractTool::data.GetGObject(itemId)->name(), MakeToolTip());
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractOperation::ComplexCurveToolTip(quint32 itemId) const
{
    const QSharedPointer<VAbstractCurve> curve = VAbstractTool::data.GeometricObject<VAbstractCurve>(itemId);

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%1:</b> %2</td> </tr>"
                                    "<tr> <td><b>%3:</b> %4 %5</td> </tr>"
                                    "%6"
                                    "</table>")
            .arg(tr("Label"), curve->name(), tr("Length"))
            .arg(qApp->fromPixel(curve->GetLength()))
            .arg(UnitsToStr(qApp->patternUnit(), true), MakeToolTip());
    return toolTip;
}

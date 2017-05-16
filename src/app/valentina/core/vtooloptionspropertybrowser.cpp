/************************************************************************
 **
 **  @file   vtooloptionspropertybrowser.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 8, 2014
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

#include "vtooloptionspropertybrowser.h"
#include "../vtools/tools/drawTools/drawtools.h"
#include "../core/vapplication.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vwidgets/vgraphicssimpletextitem.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "../vwidgets/vsimplepoint.h"
#include "../vwidgets/vsimplecurve.h"
#include "../vpropertyexplorer/vproperties.h"
#include "vformulaproperty.h"
#include "../vpatterndb/vformula.h"

#include <QDockWidget>
#include <QHBoxLayout>
#include <QDebug>
#include <QRegularExpression>

//---------------------------------------------------------------------------------------------------------------------
VToolOptionsPropertyBrowser::VToolOptionsPropertyBrowser(QDockWidget *parent)
    :QObject(parent), PropertyModel(nullptr), formView(nullptr), currentItem(nullptr),
      propertyToId(QMap<VPE::VProperty *, QString>()),
      idToProperty(QMap<QString, VPE::VProperty *>())
{
    PropertyModel = new VPE::VPropertyModel(this);
    formView = new VPE::VPropertyFormView(PropertyModel, parent);
    formView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QScrollArea *scroll = new QScrollArea(parent);
    scroll->setWidgetResizable(true);
    scroll->setWidget(formView);

    parent->setWidget(scroll);

    connect(PropertyModel, &VPE::VPropertyModel::onDataChangedByEditor, this,
            &VToolOptionsPropertyBrowser::userChangedData);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ClearPropertyBrowser()
{
    PropertyModel->clear();
    propertyToId.clear();
    idToProperty.clear();
    currentItem = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowItemOptions(QGraphicsItem *item)
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53, "Not all tools were used in switch.");

    switch (item->type())
    {
        case VToolBasePoint::Type:
            ShowOptionsToolSinglePoint(item);
            break;
        case VToolEndLine::Type:
            ShowOptionsToolEndLine(item);
            break;
        case VToolAlongLine::Type:
            ShowOptionsToolAlongLine(item);
            break;
        case VToolArc::Type:
            ShowOptionsToolArc(item);
            break;
        case VToolArcWithLength::Type:
            ShowOptionsToolArcWithLength(item);
            break;
        case VToolBisector::Type:
            ShowOptionsToolBisector(item);
            break;
        case VToolCutArc::Type:
            ShowOptionsToolCutArc(item);
            break;
        case VToolCutSpline::Type:
            ShowOptionsToolCutSpline(item);
            break;
        case VToolCutSplinePath::Type:
            ShowOptionsToolCutSplinePath(item);
            break;
        case VToolHeight::Type:
            ShowOptionsToolHeight(item);
            break;
        case VToolLine::Type:
            ShowOptionsToolLine(item);
            break;
        case VToolLineIntersect::Type:
            ShowOptionsToolLineIntersect(item);
            break;
        case VToolNormal::Type:
            ShowOptionsToolNormal(item);
            break;
        case VToolPointOfContact::Type:
            ShowOptionsToolPointOfContact(item);
            break;
        case VToolPointOfIntersection::Type:
            ShowOptionsToolPointOfIntersection(item);
            break;
        case VToolPointOfIntersectionArcs::Type:
            ShowOptionsToolPointOfIntersectionArcs(item);
            break;
        case VToolPointOfIntersectionCircles::Type:
            ShowOptionsToolPointOfIntersectionCircles(item);
            break;
        case VToolPointOfIntersectionCurves::Type:
            ShowOptionsToolPointOfIntersectionCurves(item);
            break;
        case VToolShoulderPoint::Type:
            ShowOptionsToolShoulderPoint(item);
            break;
        case VToolSpline::Type:
            ShowOptionsToolSpline(item);
            break;
        case VToolCubicBezier::Type:
            ShowOptionsToolCubicBezier(item);
            break;
        case VToolSplinePath::Type:
            ShowOptionsToolSplinePath(item);
            break;
        case VToolCubicBezierPath::Type:
            ShowOptionsToolCubicBezierPath(item);
            break;
        case VToolTriangle::Type:
            ShowOptionsToolTriangle(item);
            break;
        case VGraphicsSimpleTextItem::Type:
        case VControlPointSpline::Type:
        case VSimplePoint::Type:
        case VSimpleCurve::Type:
            currentItem = item->parentItem();
            ShowItemOptions(currentItem);
            break;
        case VToolLineIntersectAxis::Type:
            ShowOptionsToolLineIntersectAxis(item);
            break;
        case VToolCurveIntersectAxis::Type:
            ShowOptionsToolCurveIntersectAxis(item);
            break;
        case VToolPointFromCircleAndTangent::Type:
            ShowOptionsToolPointFromCircleAndTangent(item);
            break;
        case VToolPointFromArcAndTangent::Type:
            ShowOptionsToolPointFromArcAndTangent(item);
            break;
        case VToolTrueDarts::Type:
            ShowOptionsToolTrueDarts(item);
            break;
        case VToolRotation::Type:
            ShowOptionsToolRotation(item);
            break;
        case VToolFlippingByLine::Type:
            ShowOptionsToolFlippingByLine(item);
            break;
        case VToolFlippingByAxis::Type:
            ShowOptionsToolFlippingByAxis(item);
            break;
        case VToolMove::Type:
            ShowOptionsToolMove(item);
            break;
        case VToolEllipticalArc::Type:
            ShowOptionsToolEllipticalArc(item);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptions()
{
    if (currentItem == nullptr)
    {
        return;
    }

    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53, "Not all tools were used in switch.");

    switch (currentItem->type())
    {
        case VToolBasePoint::Type:
            UpdateOptionsToolSinglePoint();
            break;
        case VToolEndLine::Type:
            UpdateOptionsToolEndLine();
            break;
        case VToolAlongLine::Type:
            UpdateOptionsToolAlongLine();
            break;
        case VToolArc::Type:
            UpdateOptionsToolArc();
            break;
        case VToolArcWithLength::Type:
            UpdateOptionsToolArcWithLength();
            break;
        case VToolBisector::Type:
            UpdateOptionsToolBisector();
            break;
        case VToolCutArc::Type:
            UpdateOptionsToolCutArc();
            break;
        case VToolCutSpline::Type:
            UpdateOptionsToolCutSpline();
            break;
        case VToolCutSplinePath::Type:
            UpdateOptionsToolCutSplinePath();
            break;
        case VToolHeight::Type:
            UpdateOptionsToolHeight();
            break;
        case VToolLine::Type:
            UpdateOptionsToolLine();
            break;
        case VToolLineIntersect::Type:
            UpdateOptionsToolLineIntersect();
            break;
        case VToolNormal::Type:
            UpdateOptionsToolNormal();
            break;
        case VToolPointOfContact::Type:
            UpdateOptionsToolPointOfContact();
            break;
        case VToolPointOfIntersection::Type:
            UpdateOptionsToolPointOfIntersection();
            break;
        case VToolPointOfIntersectionArcs::Type:
            UpdateOptionsToolPointOfIntersectionArcs();
            break;
        case VToolPointOfIntersectionCircles::Type:
            UpdateOptionsToolPointOfIntersectionCircles();
            break;
        case VToolPointOfIntersectionCurves::Type:
            UpdateOptionsToolPointOfIntersectionCurves();
            break;
        case VToolShoulderPoint::Type:
            UpdateOptionsToolShoulderPoint();
            break;
        case VToolSpline::Type:
            UpdateOptionsToolSpline();
            break;
        case VToolCubicBezier::Type:
            UpdateOptionsToolCubicBezier();
            break;
        case VToolSplinePath::Type:
            UpdateOptionsToolSplinePath();
            break;
        case VToolCubicBezierPath::Type:
            UpdateOptionsToolCubicBezierPath();
            break;
        case VToolTriangle::Type:
            UpdateOptionsToolTriangle();
            break;
        case VGraphicsSimpleTextItem::Type:
        case VControlPointSpline::Type:
            ShowItemOptions(currentItem->parentItem());
            break;
        case VToolLineIntersectAxis::Type:
            UpdateOptionsToolLineIntersectAxis();
            break;
        case VToolCurveIntersectAxis::Type:
            UpdateOptionsToolCurveIntersectAxis();
            break;
        case VToolPointFromCircleAndTangent::Type:
            UpdateOptionsToolPointFromCircleAndTangent();
            break;
        case VToolPointFromArcAndTangent::Type:
            UpdateOptionsToolPointFromArcAndTangent();
            break;
        case VToolTrueDarts::Type:
            UpdateOptionsToolTrueDarts();
            break;
        case VToolRotation::Type:
            UpdateOptionsToolRotation();
            break;
        case VToolFlippingByLine::Type:
            UpdateOptionsToolFlippingByLine();
            break;
        case VToolFlippingByAxis::Type:
            UpdateOptionsToolFlippingByAxis();
            break;
        case VToolMove::Type:
            UpdateOptionsToolMove();
            break;
        case VToolEllipticalArc::Type:
            UpdateOptionsToolEllipticalArc();
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::RefreshOptions()
{
    QGraphicsItem *item = currentItem;
    itemClicked(nullptr);//close options
    itemClicked(item);//reopen options
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::userChangedData(VPE::VProperty *property)
{
    VPE::VProperty *prop = property;
    if (!propertyToId.contains(prop))
    {
        if (!propertyToId.contains(prop->getParent()))// Maybe we know parent
        {
            return;
        }
        else
        {
            prop = prop->getParent();
        }
    }

    if (!currentItem)
    {
        return;
    }

    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 53, "Not all tools were used in switch.");

    switch (currentItem->type())
    {
        case VToolBasePoint::Type:
            ChangeDataToolSinglePoint(prop);
            break;
        case VToolEndLine::Type:
            ChangeDataToolEndLine(prop);
            break;
        case VToolAlongLine::Type:
            ChangeDataToolAlongLine(prop);
            break;
        case VToolArc::Type:
            ChangeDataToolArc(prop);
            break;
        case VToolArcWithLength::Type:
            ChangeDataToolArcWithLength(prop);
            break;
        case VToolBisector::Type:
            ChangeDataToolBisector(prop);
            break;
        case VToolCutArc::Type:
            ChangeDataToolCutArc(prop);
            break;
        case VToolCutSpline::Type:
            ChangeDataToolCutSpline(prop);
            break;
        case VToolCutSplinePath::Type:
            ChangeDataToolCutSplinePath(prop);
            break;
        case VToolHeight::Type:
            ChangeDataToolHeight(prop);
            break;
        case VToolLine::Type:
            ChangeDataToolLine(prop);
            break;
        case VToolLineIntersect::Type:
            ChangeDataToolLineIntersect(prop);
            break;
        case VToolNormal::Type:
            ChangeDataToolNormal(prop);
            break;
        case VToolPointOfContact::Type:
            ChangeDataToolPointOfContact(prop);
            break;
        case VToolPointOfIntersection::Type:
            ChangeDataToolPointOfIntersection(prop);
            break;
        case VToolPointOfIntersectionArcs::Type:
            ChangeDataToolPointOfIntersectionArcs(prop);
            break;
        case VToolPointOfIntersectionCircles::Type:
            ChangeDataToolPointOfIntersectionCircles(prop);
            break;
        case VToolPointOfIntersectionCurves::Type:
            ChangeDataToolPointOfIntersectionCurves(prop);
            break;
        case VToolShoulderPoint::Type:
            ChangeDataToolShoulderPoint(prop);
            break;
        case VToolSpline::Type:
            ChangeDataToolSpline(prop);
            break;
        case VToolCubicBezier::Type:
            ChangeDataToolCubicBezier(prop);
            break;
        case VToolSplinePath::Type:
            ChangeDataToolSplinePath(prop);
            break;
        case VToolCubicBezierPath::Type:
            ChangeDataToolCubicBezierPath(prop);
            break;
        case VToolTriangle::Type:
            ChangeDataToolTriangle(prop);
            break;
        case VToolLineIntersectAxis::Type:
            ChangeDataToolLineIntersectAxis(prop);
            break;
        case VToolCurveIntersectAxis::Type:
            ChangeDataToolCurveIntersectAxis(prop);
            break;
        case VToolPointFromCircleAndTangent::Type:
            ChangeDataToolPointFromCircleAndTangent(prop);
            break;
        case VToolPointFromArcAndTangent::Type:
            ChangeDataToolPointFromArcAndTangent(prop);
            break;
        case VToolTrueDarts::Type:
            ChangeDataToolTrueDarts(prop);
            break;
        case VToolRotation::Type:
            ChangeDataToolRotation(prop);
            break;
        case VToolFlippingByLine::Type:
            ChangeDataToolFlippingByLine(prop);
            break;
        case VToolFlippingByAxis::Type:
            ChangeDataToolFlippingByAxis(prop);
            break;
        case VToolMove::Type:
            ChangeDataToolMove(prop);
            break;
        case VToolEllipticalArc::Type:
            ChangeDataToolEllipticalArc(prop);
            break;
        default:
            break;
    }
    qApp->getSceneView()->update();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::itemClicked(QGraphicsItem *item)
{
    if (item != nullptr)
    {
        if (item->isEnabled()==false)
        {
            return;
        }
    }

    if (currentItem == item && item != nullptr)
    {
        UpdateOptions();
        return;
    }

    PropertyModel->clear();
    propertyToId.clear();
    idToProperty.clear();


    if (currentItem != nullptr)
    {
        VAbstractTool *previousTool = dynamic_cast<VAbstractTool *>(currentItem);
        if (previousTool != nullptr)
        {
            previousTool->ShowVisualization(false); // hide for previous tool
        }
    }

    currentItem = item;
    if (currentItem == nullptr)
    {
        formView->setTitle("");
        return;
    }

    ShowItemOptions(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::AddPropertyFormula(const QString &propertyName, const VFormula &formula,
                                                     const QString &attrName)
{
    VFormulaProperty* itemLength = new VFormulaProperty(propertyName);
    itemLength->SetFormula(formula);
    AddProperty(itemLength, attrName);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyObjectName(Tool *i, const QString &propertyName, bool readOnly)
{
    auto itemName = new VPE::VStringProperty(propertyName);
    itemName->setClearButtonEnable(true);
    itemName->setValue(qApp->TrVars()->VarToUser(i->name()));
    itemName->setReadOnly(readOnly);
    AddProperty(itemName, AttrName);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyPointName1(Tool *i, const QString &propertyName)
{
    VPE::VStringProperty *itemName = new VPE::VStringProperty(propertyName);
    itemName->setClearButtonEnable(true);
    itemName->setValue(i->nameP1());
    AddProperty(itemName, AttrName1);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyPointName2(Tool *i, const QString &propertyName)
{
    VPE::VStringProperty *itemName = new VPE::VStringProperty(propertyName);
    itemName->setClearButtonEnable(true);
    itemName->setValue(i->nameP2());
    AddProperty(itemName, AttrName2);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyOperationSuffix(Tool *i, const QString &propertyName, bool readOnly)
{
    auto itemSuffix = new VPE::VStringProperty(propertyName);
    itemSuffix->setClearButtonEnable(true);
    itemSuffix->setValue(i->Suffix());
    itemSuffix->setReadOnly(readOnly);
    AddProperty(itemSuffix, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::AddPropertyParentPointName(const QString &pointName, const QString &propertyName,
                                                             const QString &propertyAttribure)
{
    auto itemParentPoint = new VPE::VLabelProperty(propertyName);
    itemParentPoint->setValue(pointName);
    AddProperty(itemParentPoint, propertyAttribure);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyCrossPoint(Tool *i, const QString &propertyName)
{
    VPE::VEnumProperty* itemProperty = new VPE::VEnumProperty(propertyName);
    itemProperty->setLiterals(QStringList()<< tr("First point") << tr("Second point"));
    itemProperty->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);
    AddProperty(itemProperty, AttrCrossPoint);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyVCrossPoint(Tool *i, const QString &propertyName)
{
    auto itemProperty = new VPE::VEnumProperty(propertyName);
    itemProperty->setLiterals(QStringList()<< tr("Highest point") << tr("Lowest point"));
    itemProperty->setValue(static_cast<int>(i->GetVCrossPoint())-1);
    AddProperty(itemProperty, AttrVCrossPoint);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyHCrossPoint(Tool *i, const QString &propertyName)
{
    auto itemProperty = new VPE::VEnumProperty(propertyName);
    itemProperty->setLiterals(QStringList()<< tr("Leftmost point") << tr("Rightmost point"));
    itemProperty->setValue(static_cast<int>(i->GetHCrossPoint())-1);
    AddProperty(itemProperty, AttrHCrossPoint);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyAxisType(Tool *i, const QString &propertyName)
{
    auto itemProperty = new VPE::VEnumProperty(propertyName);
    itemProperty->setLiterals(QStringList()<< tr("Vertical axis") << tr("Horizontal axis"));
    itemProperty->setValue(static_cast<int>(i->GetAxisType())-1);
    AddProperty(itemProperty, AttrAxisType);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyLineType(Tool *i, const QString &propertyName,
                                                      const QMap<QString, QIcon> &styles)
{
    VPE::VLineTypeProperty *lineTypeProperty = new VPE::VLineTypeProperty(propertyName);
    lineTypeProperty->setStyles(styles);
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(styles, i->getLineType());
    if (index == -1)
    {
        qWarning()<<"Can't find line style" << i->getLineType()<<"in list";
    }
    lineTypeProperty->setValue(index);
    AddProperty(lineTypeProperty, AttrTypeLine);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyCurvePenStyle(Tool *i, const QString &propertyName,
                                                           const QMap<QString, QIcon> &styles)
{
    VPE::VLineTypeProperty *penStyleProperty = new VPE::VLineTypeProperty(propertyName);
    penStyleProperty->setStyles(styles);
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(styles, i->GetPenStyle());
    if (index == -1)
    {
        qWarning()<<"Can't find pen style" << i->getLineType()<<"in list";
    }
    penStyleProperty->setValue(index);
    AddProperty(penStyleProperty, AttrPenStyle);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyLineColor(Tool *i, const QString &propertyName,
                                                       const QMap<QString, QString> &colors, const QString &id)
{
    VPE::VLineColorProperty *lineColorProperty = new VPE::VLineColorProperty(propertyName);
    lineColorProperty->setColors(colors);
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(colors, i->GetLineColor());
    if (index == -1)
    {
        qWarning()<<"Can't find line style" << i->GetLineColor()<<"in list";
    }
    lineColorProperty->setValue(index);
    AddProperty(lineColorProperty, id);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetPointName(const QString &name)
{
    if (Tool *i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        if (name == i->name())
        {
            return;
        }

        QRegularExpression rx(NameRegExp());
        if (name.isEmpty() || VContainer::IsUnique(name) == false || rx.match(name).hasMatch() == false)
        {
            idToProperty[AttrName]->setValue(i->name());
        }
        else
        {
            i->setName(name);
        }
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetPointName1(const QString &name)
{
    if (Tool *i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        if (name == i->nameP1())
        {
            return;
        }

        QRegularExpression rx(NameRegExp());
        if (name.isEmpty() || VContainer::IsUnique(name) == false || rx.match(name).hasMatch() == false)
        {
            idToProperty[AttrName1]->setValue(i->nameP1());
        }
        else
        {
            i->setNameP1(name);
        }
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetPointName2(const QString &name)
{
    if (Tool *i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        if (name == i->nameP2())
        {
            return;
        }

        QRegularExpression rx(NameRegExp());
        if (name.isEmpty() || VContainer::IsUnique(name) == false || rx.match(name).hasMatch() == false)
        {
            idToProperty[AttrName2]->setValue(i->nameP2());
        }
        else
        {
            i->setNameP2(name);
        }
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetOperationSuffix(const QString &suffix)
{
    if (Tool *item = qgraphicsitem_cast<Tool *>(currentItem))
    {
        if (suffix == item->Suffix())
        {
            return;
        }

        if (suffix.isEmpty())
        {
            idToProperty[AttrSuffix]->setValue(item->Suffix());
            return;
        }

        QRegularExpression rx(NameRegExp());
        const QStringList uniqueNames = VContainer::AllUniqueNames();
        for (int i=0; i < uniqueNames.size(); ++i)
        {
            const QString name = uniqueNames.at(i) + suffix;
            if (not rx.match(name).hasMatch() || not VContainer::IsUnique(name))
            {
                idToProperty[AttrSuffix]->setValue(item->Suffix());
                return;
            }
        }

        item->SetSuffix(suffix);
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Type>
Type VToolOptionsPropertyBrowser::GetCrossPoint(const QVariant &value)
{
    bool ok = false;
    const int val = value.toInt(&ok);

    auto cross = static_cast<Type>(1);
    if (ok)
    {
        switch(val)
        {
            case 0:
            case 1:
                cross = static_cast<Type>(val+1);
                break;
            default:
                break;
        }
    }

    return cross;
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetCrossCirclesPoint(const QVariant &value)
{
    if (Tool *i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        i->SetCrossCirclesPoint(GetCrossPoint<CrossCirclesPoint>(value));
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetVCrossCurvesPoint(const QVariant &value)
{
    if (auto i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        i->SetVCrossPoint(GetCrossPoint<VCrossCurvesPoint>(value));
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetHCrossCurvesPoint(const QVariant &value)
{
    if (auto i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        i->SetHCrossPoint(GetCrossPoint<HCrossCurvesPoint>(value));
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::SetAxisType(const QVariant &value)
{
    if (auto i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        i->SetAxisType(GetCrossPoint<AxisType>(value));
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::AddProperty(VPE::VProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    PropertyModel->addProperty(property, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSinglePoint(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolBasePoint>(value.toString());
            break;
        case 1: // QLatin1String("position")
            currentItem->setPos(value.toPointF());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolEndLine(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolEndLine>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 5: // AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        case 2: // AttrBasePoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolAlongLine(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolAlongLine>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 2: // AttrBasePoint (read only)
        case 7: // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolArc(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolArc *i = qgraphicsitem_cast<VToolArc *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 8: // AttrRadius
            i->SetFormulaRadius(value.value<VFormula>());
            break;
        case 9: // AttrAngle1
            i->SetFormulaF1(value.value<VFormula>());
            break;
        case 10: // AttrAngle2
            i->SetFormulaF2(value.value<VFormula>());
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 11: // AttrCenter (read only)
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolArcWithLength(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolArcWithLength *i = qgraphicsitem_cast<VToolArcWithLength *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 8: // AttrRadius
            i->SetFormulaRadius(value.value<VFormula>());
            break;
        case 9: // AttrAngle1
            i->SetFormulaF1(value.value<VFormula>());
            break;
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 11: // AttrCenter (read only)
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolBisector(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolBisector>(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 6:  // AttrFirstPoint (read only)
        case 2:  // AttrBasePoint (read only)
        case 12: // AttrThirdPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolTrueDarts(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 32: // AttrName1
            SetPointName1<VToolTrueDarts>(value.toString());
            break;
        case 33: // AttrName2
            SetPointName2<VToolTrueDarts>(value.toString());
            break;
        case 6:  // AttrFirstPoint (read only)
        case 7:  // AttrSecondPoint (read only)
        case 43: // AttrDartP1 (read only)
        case 44: // AttrDartP2 (read only)
        case 45: // AttrDartP3 (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutArc(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolCutArc>(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 13: // AttrArc (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSpline(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolCutSpline>(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 46: // AttrCurve (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSplinePath(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolCutSplinePath>(value.toString());
            break;
        case 4: // AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 46: // AttrCurve (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolHeight(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolHeight>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 2:  // AttrBasePoint (read only)
        case 16: // AttrP1Line (read only)
        case 17: // AttrP2Line (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLine(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolLine *i = qgraphicsitem_cast<VToolLine *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 6: // AttrFirstPoint (read only)
        case 7: // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLineIntersect(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolLineIntersect>(value.toString());
            break;
        case 18: // AttrP1Line1 (read only)
        case 19: // AttrP2Line1 (read only)
        case 20: // AttrP1Line2 (read only)
        case 21: // AttrP2Line2 (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolNormal(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 0: // AttrName
            SetPointName<VToolNormal>(value.toString());
            break;
        case 5: // AttrAngle
            i->SetAngle(value.toDouble());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 2: // AttrBasePoint (read only)
        case 7: // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfContact(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 8: // AttrRadius
            i->setArcRadius(value.value<VFormula>());
            break;
        case 0: // AttrName
            SetPointName<VToolPointOfContact>(value.toString());
            break;
        case 11: // AttrCenter (read only)
        case 6:  // AttrFirstPoint (read only)
        case 7:  // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersection(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointOfIntersection>(value.toString());
            break;
        case 6:  // AttrFirstPoint (read only)
        case 7:  // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersectionArcs(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointOfIntersectionArcs>(value.toString());
            break;
        case 28: // AttrCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointOfIntersectionArcs>(value);
            break;
        }
        case 47: // AttrFirstArc (read only)
        case 48: // AttrSecondArc (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersectionCircles(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointOfIntersectionCircles>(value.toString());
            break;
        case 28: // AttrCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointOfIntersectionCircles>(value);
            break;
        }
        case 29: // AttrC1Radius
            i->SetFirstCircleRadius(value.value<VFormula>());
            break;
        case 30: // AttrC2Radius
            i->SetSecondCircleRadius(value.value<VFormula>());
            break;
        case 49: // AttrC1Center (read only)
        case 50: // AttrC2Center (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersectionCurves(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointOfIntersectionCurves>(value.toString());
            break;
        case 34: // AttrVCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetVCrossCurvesPoint<VToolPointOfIntersectionCurves>(value);
            break;
        }
        case 35: // AttrHCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetHCrossCurvesPoint<VToolPointOfIntersectionCurves>(value);
            break;
        }
        case 51: // AttrCurve1 (read only)
        case 52: // AttrCurve2 (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointFromCircleAndTangent(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointFromCircleAndTangent>(value.toString());
            break;
        case 31: // AttrCRadius
            i->SetCircleRadius(value.value<VFormula>());
            break;
        case 28: // AttrCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointFromCircleAndTangent>(value);
            break;
        }
        case 53: // AttrCCenter (read only)
        case 54: // AttrTangent (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointFromArcAndTangent(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolPointFromArcAndTangent>(value.toString());
            break;
        case 28: // AttrCrossPoint
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointFromArcAndTangent>(value);
            break;
        }
        case 54: // AttrTangent (read only)
        case 13: // AttrArc (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolShoulderPoint(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 0: // AttrName
            SetPointName<VToolShoulderPoint>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 6:  // AttrFirstPoint (read only)
        case 2:  // AttrBasePoint (read only)
        case 12: // AttrThirdPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSpline(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    auto i = qgraphicsitem_cast<VToolSpline *>(currentItem);
    SCASSERT(i != nullptr)

    VSpline spl = i->getSpline();
    const VFormula f = value.value<VFormula>();

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            Q_UNREACHABLE();//The attribute is read only
            break;
        case 9: // AttrAngle1
            if (not f.error())
            {
                spl.SetStartAngle(f.getDoubleValue(), f.GetFormula(FormulaType::FromUser));
                i->setSpline(spl);
            }
            break;
        case 10: // AttrAngle2
            if (not f.error())
            {
                spl.SetEndAngle(f.getDoubleValue(), f.GetFormula(FormulaType::FromUser));
                i->setSpline(spl);
            }
            break;
        case 36: // AttrLength1
            if (not f.error() && f.getDoubleValue() >= 0)
            {
                spl.SetC1Length(qApp->toPixel(f.getDoubleValue()), f.GetFormula(FormulaType::FromUser));
                i->setSpline(spl);
            }
            break;
        case 37: // AttrLength2
            if (not f.error() && f.getDoubleValue() >= 0)
            {
                spl.SetC2Length(qApp->toPixel(f.getDoubleValue()), f.GetFormula(FormulaType::FromUser));
                i->setSpline(spl);
            }
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCubicBezier(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    auto i = qgraphicsitem_cast<VToolCubicBezier *>(currentItem);
    SCASSERT(i != nullptr)

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            Q_UNREACHABLE();//The attribute is read only
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        case 55: // AttrPoint1 (read only)
        case 56: // AttrPoint2 (read only)
        case 57: // AttrPoint3 (read only)
        case 58: // AttrPoint4 (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSplinePath(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            Q_UNREACHABLE();//The attribute is read only
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCubicBezierPath(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCubicBezierPath *i = qgraphicsitem_cast<VToolCubicBezierPath *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            Q_UNREACHABLE();//The attribute is read only
            break;
        case 27: // AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolTriangle(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolTriangle>(value.toString());
            break;
        case 23: // AttrAxisP1 (read only)
        case 24: // AttrAxisP2 (read only)
        case 6:  // AttrFirstPoint (read only)
        case 7:  // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLineIntersectAxis(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolLineIntersectAxis>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 5: // AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        case 2: // AttrBasePoint (read only)
        case 6: // AttrFirstPoint (read only)
        case 7: // AttrSecondPoint (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCurveIntersectAxis(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 0: // AttrName
            SetPointName<VToolCurveIntersectAxis>(value.toString());
            break;
        case 3: // AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 5: // AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        case 2:  // AttrBasePoint (read only)
        case 46: // AttrCurve (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolRotation(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolRotation *i = qgraphicsitem_cast<VToolRotation *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 38: // AttrSuffix
            SetOperationSuffix<VToolRotation>(value.toString());
            break;
        case 5: // AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        case 11: // AttrCenter (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolMove(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolMove *i = qgraphicsitem_cast<VToolMove *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 38: // AttrSuffix
            SetOperationSuffix<VToolMove>(value.toString());
            break;
        case 5: // AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        case 4: // AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolFlippingByLine(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 38: // AttrSuffix
            SetOperationSuffix<VToolFlippingByLine>(value.toString());
            break;
        case 6: // AttrFirstPoint
        case 7: // AttrSecondPoint
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolFlippingByAxis(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 39: // AttrAxisType
        {
            const QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::EditRole);
            SetAxisType<VToolFlippingByAxis>(value);
            break;
        }
        case 38: // AttrSuffix
            SetOperationSuffix<VToolFlippingByAxis>(value.toString());
            break;
        case 11: // AttrCenter (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolEllipticalArc(VPE::VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VPE::VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolEllipticalArc *i = qgraphicsitem_cast<VToolEllipticalArc *>(currentItem);
    SCASSERT(i != nullptr)
    switch (PropertiesList().indexOf(id))
    {
        case 40://AttrRadius1
            i->SetFormulaRadius1(value.value<VFormula>());
            break;
        case 41://AttrRadius2
            i->SetFormulaRadius2(value.value<VFormula>());
            break;
        case 9://AttrAngle1
            i->SetFormulaF1(value.value<VFormula>());
            break;
        case 10://AttrAngle2
            i->SetFormulaF2(value.value<VFormula>());
            break;
        case 42://AttrRotationAngle
            i->SetFormulaRotationAngle(value.value<VFormula>());
            break;
        case 27://AttrColor
            i->SetLineColor(value.toString());
            break;
        case 59: // AttrPenStyle
            i->SetPenStyle(value.toString());
            break;
        case 11: // AttrCenter (read only)
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSinglePoint(QGraphicsItem *item)
{
    VToolBasePoint *i = qgraphicsitem_cast<VToolBasePoint *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Base point"));

    AddPropertyObjectName(i, tr("Point label:"));

    VPE::VPointFProperty* itemPosition = new VPE::VPointFProperty(tr("Position:"));
    itemPosition->setValue(i->pos());
    AddProperty(itemPosition, QLatin1String("position"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolEndLine(QGraphicsItem *item)
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at distance and angle"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("Base point:"), AttrBasePoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
    AddPropertyFormula(tr("Angle:"), i->GetFormulaAngle(), AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolAlongLine(QGraphicsItem *item)
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at distance along line"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("First point:"), AttrBasePoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrSecondPoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolArc(QGraphicsItem *item)
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Arc"));

    AddPropertyParentPointName(i->CenterPointName(), tr("Center point:"), AttrCenter);
    AddPropertyFormula(tr("Radius:"), i->GetFormulaRadius(), AttrRadius);
    AddPropertyFormula(tr("First angle:"), i->GetFormulaF1(), AttrAngle1);
    AddPropertyFormula(tr("Second angle:"), i->GetFormulaF2(), AttrAngle2);
    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolArcWithLength(QGraphicsItem *item)
{
    VToolArcWithLength *i = qgraphicsitem_cast<VToolArcWithLength *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Arc with given length"));

    AddPropertyParentPointName(i->CenterPointName(), tr("Center point:"), AttrCenter);
    AddPropertyFormula(tr("Radius:"), i->GetFormulaRadius(), AttrRadius);
    AddPropertyFormula(tr("First angle:"), i->GetFormulaF1(), AttrAngle1);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolBisector(QGraphicsItem *item)
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point along bisector"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->FirstPointName(), tr("First point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->BasePointName(), tr("Second point:"), AttrBasePoint);
    AddPropertyParentPointName(i->ThirdPointName(), tr("Third point:"), AttrThirdPoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolTrueDarts(QGraphicsItem *item)
{
    VToolTrueDarts *i = qgraphicsitem_cast<VToolTrueDarts *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("True darts"));

    AddPropertyPointName1(i, tr("Point 1 label:"));
    AddPropertyPointName2(i, tr("Point 2 label:"));
    AddPropertyParentPointName(i->BaseLineP1Name(), tr("First base point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->BaseLineP2Name(), tr("Second base point:"), AttrSecondPoint);
    AddPropertyParentPointName(i->DartP1Name(), tr("First dart point:"), AttrDartP1);
    AddPropertyParentPointName(i->DartP2Name(), tr("First dart point:"), AttrDartP2);
    AddPropertyParentPointName(i->DartP3Name(), tr("First dart point:"), AttrDartP3);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutArc(QGraphicsItem *item)
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Cut arc tool"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->CurveName(), tr("Arc:"), AttrArc);
    AddPropertyFormula(tr("Length:"), i->GetFormula(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSpline(QGraphicsItem *item)
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool for segmenting a curve"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->CurveName(), tr("Curve:"), AttrCurve);
    AddPropertyFormula(tr("Length:"), i->GetFormula(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSplinePath(QGraphicsItem *item)
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool segment a pathed curve"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->CurveName(), tr("Curve:"), AttrCurve);
    AddPropertyFormula(tr("Length:"), i->GetFormula(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolHeight(QGraphicsItem *item)
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Perpendicular point along line"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("Base point:"), AttrBasePoint);
    AddPropertyParentPointName(i->FirstLinePointName(), tr("First line point:"), AttrP1Line);
    AddPropertyParentPointName(i->SecondLinePointName(), tr("Second line point:"), AttrP2Line);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLine(QGraphicsItem *item)
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Line between points"));

    AddPropertyParentPointName(i->FirstPointName(), tr("First point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrSecondPoint);
    QMap<QString, QIcon> styles = LineStylesPics();
    styles.remove(TypeLineNone);
    AddPropertyLineType(i, tr("Line type:"), styles);
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLineIntersect(QGraphicsItem *item)
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at line intersection"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->Line1P1Name(), tr("First line (first point):"), AttrP1Line1);
    AddPropertyParentPointName(i->Line1P2Name(), tr("First line (second point):"), AttrP2Line1);
    AddPropertyParentPointName(i->Line2P1Name(), tr("Second line (first point):"), AttrP1Line2);
    AddPropertyParentPointName(i->Line2P2Name(), tr("Second line (second point):"), AttrP2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolNormal(QGraphicsItem *item)
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point along perpendicular"));

    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("First point:"), AttrBasePoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrSecondPoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);

    VPE::VDoubleProperty* itemAngle = new VPE::VDoubleProperty(tr("Additional angle degrees:"));
    itemAngle->setValue(i->GetAngle());
    itemAngle->setSetting("Min", -360);
    itemAngle->setSetting("Max", 360);
    itemAngle->setSetting("Precision", 3);
    AddProperty(itemAngle, AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfContact(QGraphicsItem *item)
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at intersection of arc and line"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->ArcCenterPointName(), tr("Center of arc:"), AttrCenter);
    AddPropertyParentPointName(i->FirstPointName(), tr("Top of the line:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("End of the line:"), AttrSecondPoint);
    AddPropertyFormula(tr("Radius:"), i->getArcRadius(), AttrRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersection(QGraphicsItem *item)
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from x & y of two other points"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->FirstPointName(), tr("X: vertical point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Y: horizontal point:"), AttrSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersectionArcs(QGraphicsItem *item)
{
    VToolPointOfIntersectionArcs *i = qgraphicsitem_cast<VToolPointOfIntersectionArcs *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from intersection two arcs"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->FirstArcName(), tr("First arc:"), AttrFirstArc);
    AddPropertyParentPointName(i->SecondArcName(), tr("Second arc:"), AttrSecondArc);
    AddPropertyCrossPoint(i, tr("Take:"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersectionCircles(QGraphicsItem *item)
{
    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from intersection two circles"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->FirstCircleCenterPointName(), tr("Center of the first circle:"), AttrC1Center);
    AddPropertyParentPointName(i->SecondCircleCenterPointName(), tr("Center of the second circle:"), AttrC2Center);
    AddPropertyFormula(tr("First circle radius:"), i->GetFirstCircleRadius(), AttrC1Radius);
    AddPropertyFormula(tr("Second circle radius:"), i->GetSecondCircleRadius(), AttrC2Radius);
    AddPropertyCrossPoint(i, tr("Take:"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersectionCurves(QGraphicsItem *item)
{
    auto i = qgraphicsitem_cast<VToolPointOfIntersectionCurves *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from intersection two curves"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->FirstCurveName(), tr("First curve:"), AttrCurve1);
    AddPropertyParentPointName(i->SecondCurveName(), tr("Second curve:"), AttrCurve2);
    AddPropertyVCrossPoint(i, tr("Vertical correction:"));
    AddPropertyHCrossPoint(i, tr("Horizontal correction:"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointFromCircleAndTangent(QGraphicsItem *item)
{
    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from circle and tangent"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->CircleCenterPointName(), tr("Center of the circle:"), AttrCCenter);
    AddPropertyParentPointName(i->TangentPointName(), tr("Tangent point:"), AttrTangent);
    AddPropertyFormula(tr("Circle radius:"), i->GetCircleRadius(), AttrCRadius);
    AddPropertyCrossPoint(i, tr("Take:"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointFromArcAndTangent(QGraphicsItem *item)
{
    VToolPointFromArcAndTangent *i = qgraphicsitem_cast<VToolPointFromArcAndTangent *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from arc and tangent"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->TangentPointName(), tr("Tangent point:"), AttrTangent);
    AddPropertyParentPointName(i->ArcName(), tr("Arc:"), AttrArc);
    AddPropertyCrossPoint(i, tr("Take:"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolShoulderPoint(QGraphicsItem *item)
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Special point on shoulder"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("First point:"), AttrBasePoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrSecondPoint);
    AddPropertyParentPointName(i->ShoulderPointName(), tr("Third point:"), AttrThirdPoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSpline(QGraphicsItem *item)
{
    auto i = qgraphicsitem_cast<VToolSpline *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Curve tool"));

    const auto spl = i->getSpline();

    AddPropertyObjectName(i, tr("Name:"), true);

    VFormula angle1(spl.GetStartAngleFormula(), i->getData());
    angle1.setCheckZero(false);
    angle1.setToolId(i->getId());
    angle1.setPostfix(degreeSymbol);
    AddPropertyFormula(tr("C1: angle:"), angle1, AttrAngle1);

    VFormula length1(spl.GetC1LengthFormula(), i->getData());
    length1.setCheckZero(false);
    length1.setToolId(i->getId());
    length1.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    AddPropertyFormula(tr("C1: length:"), length1, AttrLength1);

    VFormula angle2(spl.GetEndAngleFormula(), i->getData());
    angle2.setCheckZero(false);
    angle2.setToolId(i->getId());
    angle2.setPostfix(degreeSymbol);
    AddPropertyFormula(tr("C2: angle:"), angle2, AttrAngle2);

    VFormula length2(spl.GetC2LengthFormula(), i->getData());
    length2.setCheckZero(false);
    length2.setToolId(i->getId());
    length2.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    AddPropertyFormula(tr("C2: length:"), length2, AttrLength2);

    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCubicBezier(QGraphicsItem *item)
{
    auto i = qgraphicsitem_cast<VToolCubicBezier *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Cubic bezier curve"));

    AddPropertyObjectName(i, tr("Name:"), true);
    AddPropertyParentPointName(i->FirstPointName(), tr("First point:"), AttrPoint1);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrPoint2);
    AddPropertyParentPointName(i->ThirdPointName(), tr("Third point:"), AttrPoint3);
    AddPropertyParentPointName(i->ForthPointName(), tr("Fourth point:"), AttrPoint4);
    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSplinePath(QGraphicsItem *item)
{
    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool for path curve"));

    AddPropertyObjectName(i, tr("Name:"), true);
    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCubicBezierPath(QGraphicsItem *item)
{
    VToolCubicBezierPath *i = qgraphicsitem_cast<VToolCubicBezierPath *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool cubic bezier curve"));

    AddPropertyObjectName(i, tr("Name:"), true);
    AddPropertyCurvePenStyle(i, tr("Pen style:"), CurvePenStylesPics());
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolTriangle(QGraphicsItem *item)
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool triangle"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->AxisP1Name(), tr("First point of axis:"), AttrAxisP1);
    AddPropertyParentPointName(i->AxisP2Name(), tr("Second point of axis:"), AttrAxisP2);
    AddPropertyParentPointName(i->FirstPointName(), tr("First point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondPointName(), tr("Second point:"), AttrSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLineIntersectAxis(QGraphicsItem *item)
{
    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point intersection line and axis"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("Axis point:"), AttrBasePoint);
    AddPropertyParentPointName(i->FirstLinePoint(), tr("First line point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondLinePoint(), tr("Second line point:"), AttrSecondPoint);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Angle:"), i->GetFormulaAngle(), AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCurveIntersectAxis(QGraphicsItem *item)
{
    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point intersection curve and axis"));

    AddPropertyObjectName(i, tr("Point label:"));
    AddPropertyParentPointName(i->BasePointName(), tr("Axis point:"), AttrBasePoint);
    AddPropertyParentPointName(i->CurveName(), tr("Curve:"), AttrCurve);
    AddPropertyLineType(i, tr("Line type:"), LineStylesPics());
    AddPropertyLineColor(i, tr("Line color:"), VAbstractTool::ColorsList(), AttrLineColor);
    AddPropertyFormula(tr("Angle:"), i->GetFormulaAngle(), AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolRotation(QGraphicsItem *item)
{
    VToolRotation *i = qgraphicsitem_cast<VToolRotation *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool rotation"));

    AddPropertyOperationSuffix(i, tr("Suffix:"));
    AddPropertyParentPointName(i->OriginPointName(), tr("Origin point:"), AttrCenter);
    AddPropertyFormula(tr("Angle:"), i->GetFormulaAngle(), AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolMove(QGraphicsItem *item)
{
    VToolMove *i = qgraphicsitem_cast<VToolMove *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool move"));

    AddPropertyOperationSuffix(i, tr("Suffix:"));
    AddPropertyFormula(tr("Angle:"), i->GetFormulaAngle(), AttrAngle);
    AddPropertyFormula(tr("Length:"), i->GetFormulaLength(), AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolFlippingByLine(QGraphicsItem *item)
{
    VToolFlippingByLine *i = qgraphicsitem_cast<VToolFlippingByLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool flipping by line"));

    AddPropertyOperationSuffix(i, tr("Suffix:"));
    AddPropertyParentPointName(i->FirstLinePointName(), tr("First line point:"), AttrFirstPoint);
    AddPropertyParentPointName(i->SecondLinePointName(), tr("Second line point:"), AttrSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolFlippingByAxis(QGraphicsItem *item)
{
    VToolFlippingByAxis *i = qgraphicsitem_cast<VToolFlippingByAxis *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool flipping by axis"));

    AddPropertyAxisType(i, tr("Axis type:"));
    AddPropertyOperationSuffix(i, tr("Suffix:"));
    AddPropertyParentPointName(i->OriginPointName(), tr("Origin point:"), AttrCenter);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolEllipticalArc(QGraphicsItem *item)
{
    VToolEllipticalArc *i = qgraphicsitem_cast<VToolEllipticalArc *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Elliptical arc"));

    AddPropertyParentPointName(i->CenterPointName(), tr("Center point:"), AttrCenter);
    AddPropertyFormula(tr("Radius:"), i->GetFormulaRadius1(), AttrRadius1);
    AddPropertyFormula(tr("Radius:"), i->GetFormulaRadius2(), AttrRadius2);
    AddPropertyFormula(tr("First angle:"), i->GetFormulaF1(), AttrAngle1);
    AddPropertyFormula(tr("Second angle:"), i->GetFormulaF2(), AttrAngle2);
    AddPropertyFormula(tr("Rotation angle:"), i->GetFormulaRotationAngle(), AttrRotationAngle);
    AddPropertyLineColor(i, tr("Color:"), VAbstractTool::ColorsList(), AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSinglePoint()
{
    VToolBasePoint *i = qgraphicsitem_cast<VToolBasePoint *>(currentItem);
    idToProperty[AttrName]->setValue(i->name());
    idToProperty[QLatin1String("position")]->setValue(i->pos());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolEndLine()
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueFormula);

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[AttrAngle]->setValue(valueAngle);

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolAlongLine()
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueFormula);

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolArc()
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(currentItem);

    QVariant valueRadius;
    valueRadius.setValue(i->GetFormulaRadius());
    idToProperty[AttrRadius]->setValue(valueRadius);

    QVariant valueFirstAngle;
    valueFirstAngle.setValue(i->GetFormulaF1());
    idToProperty[AttrAngle1]->setValue(valueFirstAngle);

    QVariant valueSecondAngle;
    valueSecondAngle.setValue(i->GetFormulaF2());
    idToProperty[AttrAngle2]->setValue(valueSecondAngle);

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    {
        const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
        idToProperty[AttrColor]->setValue(index);
    }

    QVariant valueCenterPoint;
    valueCenterPoint.setValue(i->CenterPointName());
    idToProperty[AttrCenter]->setValue(valueCenterPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolArcWithLength()
{
    VToolArcWithLength *i = qgraphicsitem_cast<VToolArcWithLength *>(currentItem);

    QVariant valueRadius;
    valueRadius.setValue(i->GetFormulaRadius());
    idToProperty[AttrRadius]->setValue(valueRadius);

    QVariant valueFirstAngle;
    valueFirstAngle.setValue(i->GetFormulaF1());
    idToProperty[AttrAngle1]->setValue(valueFirstAngle);

    QVariant valueLength;
    valueLength.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueLength);

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    {
        const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
        idToProperty[AttrColor]->setValue(index);
    }

    QVariant valueCenterPoint;
    valueCenterPoint.setValue(i->CenterPointName());
    idToProperty[AttrCenter]->setValue(valueCenterPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolBisector()
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueFormula);

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueThirdPoint;
    valueThirdPoint.setValue(i->ThirdPointName());
    idToProperty[AttrThirdPoint]->setValue(valueThirdPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolTrueDarts()
{
    VToolTrueDarts *i = qgraphicsitem_cast<VToolTrueDarts *>(currentItem);

    idToProperty[AttrName1]->setValue(i->nameP1());
    idToProperty[AttrName2]->setValue(i->nameP2());

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->BaseLineP1Name());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->BaseLineP2Name());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);

    QVariant valueDrartFirstPoint;
    valueDrartFirstPoint.setValue(i->DartP1Name());
    idToProperty[AttrDartP1]->setValue(valueDrartFirstPoint);

    QVariant valueDartSecondPoint;
    valueDartSecondPoint.setValue(i->DartP2Name());
    idToProperty[AttrDartP2]->setValue(valueDartSecondPoint);

    QVariant valueDartThirdPoint;
    valueDartThirdPoint.setValue(i->DartP3Name());
    idToProperty[AttrDartP3]->setValue(valueDartThirdPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutArc()
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[AttrLength]->setValue(valueFormula);

    QVariant valueArc;
    valueArc.setValue(i->CurveName());
    idToProperty[AttrArc]->setValue(valueArc);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSpline()
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[AttrLength]->setValue(valueFormula);

    QVariant valueCurve;
    valueCurve.setValue(i->CurveName());
    idToProperty[AttrCurve]->setValue(valueCurve);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSplinePath()
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[AttrLength]->setValue(valueFormula);

    QVariant valueCurve;
    valueCurve.setValue(i->CurveName());
    idToProperty[AttrCurve]->setValue(valueCurve);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolHeight()
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueFirstLinePoint;
    valueFirstLinePoint.setValue(i->FirstLinePointName());
    idToProperty[AttrP1Line]->setValue(valueFirstLinePoint);

    QVariant valueSecondLinePoint;
    valueSecondLinePoint.setValue(i->SecondLinePointName());
    idToProperty[AttrP2Line]->setValue(valueSecondLinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLine()
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(currentItem);

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLineIntersect()
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueLine1P1;
    valueLine1P1.setValue(i->Line1P1Name());
    idToProperty[AttrP1Line1]->setValue(valueLine1P1);

    QVariant valueLine1P2;
    valueLine1P2.setValue(i->Line1P2Name());
    idToProperty[AttrP2Line1]->setValue(valueLine1P2);

    QVariant valueLine2P1;
    valueLine2P1.setValue(i->Line2P1Name());
    idToProperty[AttrP1Line2]->setValue(valueLine2P1);

    QVariant valueLine2P2;
    valueLine2P2.setValue(i->Line2P2Name());
    idToProperty[AttrP2Line2]->setValue(valueLine2P2);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolNormal()
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueFormula);

    idToProperty[AttrName]->setValue(i->name());

    idToProperty[AttrAngle]->setValue( i->GetAngle());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfContact()
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->getArcRadius());
    idToProperty[AttrRadius]->setValue(valueFormula);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueCenterPoint;
    valueCenterPoint.setValue(i->ArcCenterPointName());
    idToProperty[AttrCenter]->setValue(valueCenterPoint);

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersection()
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersectionArcs()
{
    VToolPointOfIntersectionArcs *i = qgraphicsitem_cast<VToolPointOfIntersectionArcs *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());
    idToProperty[AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant valueFirstArc;
    valueFirstArc.setValue(i->FirstArcName());
    idToProperty[AttrFirstArc]->setValue(valueFirstArc);

    QVariant valueSecondArc;
    valueSecondArc.setValue(i->SecondArcName());
    idToProperty[AttrSecondArc]->setValue(valueSecondArc);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersectionCircles()
{
    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());
    idToProperty[AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant c1Radius;
    c1Radius.setValue(i->GetFirstCircleRadius());
    idToProperty[AttrC1Radius]->setValue(c1Radius);

    QVariant c2Radius;
    c2Radius.setValue(i->GetSecondCircleRadius());
    idToProperty[AttrC2Radius]->setValue(c2Radius);

    QVariant valueC1Center;
    valueC1Center.setValue(i->FirstCircleCenterPointName());
    idToProperty[AttrC1Center]->setValue(valueC1Center);

    QVariant valueC2Center;
    valueC2Center.setValue(i->SecondCircleCenterPointName());
    idToProperty[AttrC2Center]->setValue(valueC2Center);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersectionCurves()
{
    auto i = qgraphicsitem_cast<VToolPointOfIntersectionCurves *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());
    idToProperty[AttrVCrossPoint]->setValue(static_cast<int>(i->GetVCrossPoint())-1);
    idToProperty[AttrHCrossPoint]->setValue(static_cast<int>(i->GetHCrossPoint())-1);

    QVariant valueCurve1;
    valueCurve1.setValue(i->FirstCurveName());
    idToProperty[AttrCurve1]->setValue(valueCurve1);

    QVariant valueCurve2;
    valueCurve2.setValue(i->SecondCurveName());
    idToProperty[AttrCurve2]->setValue(valueCurve2);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointFromCircleAndTangent()
{
    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());
    idToProperty[AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant cRadius;
    cRadius.setValue(i->GetCircleRadius());
    idToProperty[AttrCRadius]->setValue(cRadius);

    QVariant valueCenterPoint;
    valueCenterPoint.setValue(i->CircleCenterPointName());
    idToProperty[AttrCCenter]->setValue(valueCenterPoint);

    QVariant valueTangentPoint;
    valueTangentPoint.setValue(i->TangentPointName());
    idToProperty[AttrTangent]->setValue(valueTangentPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointFromArcAndTangent()
{
    VToolPointFromArcAndTangent *i = qgraphicsitem_cast<VToolPointFromArcAndTangent *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());
    idToProperty[AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant valueTangentPoint;
    valueTangentPoint.setValue(i->TangentPointName());
    idToProperty[AttrTangent]->setValue(valueTangentPoint);

    QVariant valueArc;
    valueArc.setValue(i->ArcName());
    idToProperty[AttrArc]->setValue(valueArc);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolShoulderPoint()
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueFormula);

    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);

    QVariant valueThirdPoint;
    valueThirdPoint.setValue(i->ShoulderPointName());
    idToProperty[AttrThirdPoint]->setValue(valueThirdPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSpline()
{
    auto i = qgraphicsitem_cast<VToolSpline *>(currentItem);
    const VSpline spl = i->getSpline();

    idToProperty[AttrName]->setValue(qApp->TrVars()->VarToUser(i->name()));

    VFormula angle1F(spl.GetStartAngleFormula(), i->getData());
    angle1F.setCheckZero(false);
    angle1F.setToolId(i->getId());
    angle1F.setPostfix(degreeSymbol);
    QVariant angle1;
    angle1.setValue(angle1F);
    idToProperty[AttrAngle1]->setValue(angle1);

    VFormula length1F(spl.GetC1LengthFormula(), i->getData());
    length1F.setCheckZero(false);
    length1F.setToolId(i->getId());
    length1F.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    QVariant length1;
    length1.setValue(length1F);
    idToProperty[AttrLength1]->setValue(length1);

    VFormula angle2F(spl.GetEndAngleFormula(), i->getData());
    angle2F.setCheckZero(false);
    angle2F.setToolId(i->getId());
    angle2F.setPostfix(degreeSymbol);
    QVariant angle2;
    angle2.setValue(angle2F);
    idToProperty[AttrAngle2]->setValue(angle2);

    VFormula length2F(spl.GetC2LengthFormula(), i->getData());
    length2F.setCheckZero(false);
    length2F.setToolId(i->getId());
    length2F.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    QVariant length2;
    length2.setValue(length2F);
    idToProperty[AttrLength2]->setValue(length2);

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    idToProperty[AttrColor]->setValue(VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(),
                                                                            i->GetLineColor()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCubicBezier()
{
    auto i = qgraphicsitem_cast<VToolCubicBezier *>(currentItem);

    idToProperty[AttrName]->setValue(qApp->TrVars()->VarToUser(i->name()));

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    idToProperty[AttrColor]->setValue(VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(),
                                                                       i->GetLineColor()));

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrPoint1]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrPoint2]->setValue(valueSecondPoint);

    QVariant valueThirdPoint;
    valueThirdPoint.setValue(i->ThirdPointName());
    idToProperty[AttrPoint3]->setValue(valueThirdPoint);

    QVariant valueForthPoint;
    valueForthPoint.setValue(i->ForthPointName());
    idToProperty[AttrPoint4]->setValue(valueForthPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSplinePath()
{
    auto i = qgraphicsitem_cast<VToolSplinePath *>(currentItem);

    idToProperty[AttrName]->setValue(qApp->TrVars()->VarToUser(i->name()));

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    idToProperty[AttrColor]->setValue(VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(),
                                                                            i->GetLineColor()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCubicBezierPath()
{
    auto i = qgraphicsitem_cast<VToolCubicBezierPath *>(currentItem);

    idToProperty[AttrName]->setValue(qApp->TrVars()->VarToUser(i->name()));

    {
        const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(CurvePenStylesPics(), i->GetPenStyle());
        idToProperty[AttrPenStyle]->setValue(index);
    }

    idToProperty[AttrColor]->setValue(VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(),
                                                                            i->GetLineColor()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolTriangle()
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(currentItem);

    idToProperty[AttrName]->setValue(i->name());

    QVariant valueAxisP1;
    valueAxisP1.setValue(i->AxisP1Name());
    idToProperty[AttrAxisP1]->setValue(valueAxisP1);

    QVariant valueAxisP2;
    valueAxisP2.setValue(i->AxisP2Name());
    idToProperty[AttrAxisP2]->setValue(valueAxisP2);

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstPointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondPointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLineIntersectAxis()
{
    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(currentItem);
    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[AttrAngle]->setValue(valueAngle);

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstLinePoint());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondLinePoint());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCurveIntersectAxis()
{
    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(currentItem);
    idToProperty[AttrName]->setValue(i->name());

    {
    const qint32 index = VPE::VLineTypeProperty::IndexOfStyle(LineStylesPics(), i->getLineType());
    idToProperty[AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrLineColor]->setValue(index);
    }

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[AttrAngle]->setValue(valueAngle);

    QVariant valueBasePoint;
    valueBasePoint.setValue(i->BasePointName());
    idToProperty[AttrBasePoint]->setValue(valueBasePoint);

    QVariant valueCurve;
    valueCurve.setValue(i->CurveName());
    idToProperty[AttrCurve]->setValue(valueCurve);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolRotation()
{
    VToolRotation *i = qgraphicsitem_cast<VToolRotation *>(currentItem);
    idToProperty[AttrSuffix]->setValue(i->Suffix());

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[AttrAngle]->setValue(valueAngle);

    QVariant valueOriginPoint;
    valueOriginPoint.setValue(i->OriginPointName());
    idToProperty[AttrCenter]->setValue(valueOriginPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolMove()
{
    VToolMove *i = qgraphicsitem_cast<VToolMove *>(currentItem);
    idToProperty[AttrSuffix]->setValue(i->Suffix());

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[AttrAngle]->setValue(valueAngle);

    QVariant valueLength;
    valueLength.setValue(i->GetFormulaLength());
    idToProperty[AttrLength]->setValue(valueLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolFlippingByLine()
{
    VToolFlippingByLine *i = qgraphicsitem_cast<VToolFlippingByLine *>(currentItem);
    idToProperty[AttrSuffix]->setValue(i->Suffix());

    QVariant valueFirstPoint;
    valueFirstPoint.setValue(i->FirstLinePointName());
    idToProperty[AttrFirstPoint]->setValue(valueFirstPoint);

    QVariant valueSecondPoint;
    valueSecondPoint.setValue(i->SecondLinePointName());
    idToProperty[AttrSecondPoint]->setValue(valueSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolFlippingByAxis()
{
    VToolFlippingByAxis *i = qgraphicsitem_cast<VToolFlippingByAxis *>(currentItem);
    idToProperty[AttrAxisType]->setValue(static_cast<int>(i->GetAxisType())-1);
    idToProperty[AttrSuffix]->setValue(i->Suffix());

    QVariant valueOriginPoint;
    valueOriginPoint.setValue(i->OriginPointName());
    idToProperty[AttrCenter]->setValue(valueOriginPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolEllipticalArc()
{
    VToolEllipticalArc *i = qgraphicsitem_cast<VToolEllipticalArc *>(currentItem);

    QVariant valueFormulaRadius1;
    valueFormulaRadius1.setValue(i->GetFormulaRadius1());
    idToProperty[AttrRadius1]->setValue(valueFormulaRadius1);

    QVariant valueFormulaRadius2;
    valueFormulaRadius2.setValue(i->GetFormulaRadius2());
    idToProperty[AttrRadius2]->setValue(valueFormulaRadius2);

    QVariant valueFormulaF1;
    valueFormulaF1.setValue(i->GetFormulaF1());
    idToProperty[AttrAngle1]->setValue(valueFormulaF1);

    QVariant valueFormulaF2;
    valueFormulaF2.setValue(i->GetFormulaF2());
    idToProperty[AttrAngle2]->setValue(valueFormulaF2);

    QVariant valueFormulaRotationAngle;
    valueFormulaRotationAngle.setValue(i->GetFormulaRotationAngle());
    idToProperty[AttrRotationAngle]->setValue(valueFormulaRotationAngle);

    const qint32 index = VPE::VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[AttrColor]->setValue(index);

    QVariant valueCenterPoint;
    valueCenterPoint.setValue(i->CenterPointName());
    idToProperty[AttrCenter]->setValue(valueCenterPoint);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VToolOptionsPropertyBrowser::PropertiesList() const
{
    static QStringList attr = QStringList() << AttrName                           /* 0 */
                                            << QLatin1String("position")          /* 1 */
                                            << AttrBasePoint                      /* 2 */
                                            << AttrTypeLine                       /* 3 */
                                            << AttrLength                         /* 4 */
                                            << AttrAngle                          /* 5 */
                                            << AttrFirstPoint                     /* 6 */
                                            << AttrSecondPoint                    /* 7 */
                                            << AttrRadius                         /* 8 */
                                            << AttrAngle1                         /* 9 */
                                            << AttrAngle2                         /* 10 */
                                            << AttrCenter                         /* 11 */
                                            << AttrThirdPoint                     /* 12 */
                                            << AttrArc                            /* 13 */
                                            << VToolCutSpline::AttrSpline         /* 14 */
                                            << VToolCutSplinePath::AttrSplinePath /* 15 */
                                            << AttrP1Line                         /* 16 */
                                            << AttrP2Line                         /* 17 */
                                            << AttrP1Line1                        /* 18 */
                                            << AttrP2Line1                        /* 19 */
                                            << AttrP1Line2                        /* 20 */
                                            << AttrP2Line2                        /* 21 */
                                            << AttrPShoulder                      /* 22 */
                                            << AttrAxisP1                         /* 23 */
                                            << AttrAxisP2                         /* 24 */
                                            << AttrKCurve /*Not used*/            /* 25 */
                                            << AttrLineColor                      /* 26 */
                                            << AttrColor                          /* 27 */
                                            << AttrCrossPoint                     /* 28 */
                                            << AttrC1Radius                       /* 29 */
                                            << AttrC2Radius                       /* 30 */
                                            << AttrCRadius                        /* 31 */
                                            << AttrName1                          /* 32 */
                                            << AttrName2                          /* 33 */
                                            << AttrVCrossPoint                    /* 34 */
                                            << AttrHCrossPoint                    /* 35 */
                                            << AttrLength1                        /* 36 */
                                            << AttrLength2                        /* 37 */
                                            << AttrSuffix                         /* 38 */
                                            << AttrAxisType                       /* 39 */
                                            << AttrRadius1                        /* 40 */
                                            << AttrRadius2                        /* 41 */
                                            << AttrRotationAngle                  /* 42 */
                                            << AttrDartP1                         /* 43 */
                                            << AttrDartP2                         /* 44 */
                                            << AttrDartP3                         /* 45 */
                                            << AttrCurve                          /* 46 */
                                            << AttrFirstArc                       /* 47 */
                                            << AttrSecondArc                      /* 48 */
                                            << AttrC1Center                       /* 49 */
                                            << AttrC2Center                       /* 50 */
                                            << AttrCurve1                         /* 51 */
                                            << AttrCurve2                         /* 52 */
                                            << AttrCCenter                        /* 53 */
                                            << AttrTangent                        /* 54 */
                                            << AttrPoint1                         /* 55 */
                                            << AttrPoint2                         /* 56 */
                                            << AttrPoint3                         /* 57 */
                                            << AttrPoint4                         /* 58 */
                                            << AttrPenStyle;                      /* 59 */
    return attr;
}

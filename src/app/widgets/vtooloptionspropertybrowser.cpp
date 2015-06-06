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
#include "../tools/drawTools/drawtools.h"
#include "../core/vapplication.h"
#include "vmaingraphicsview.h"
#include "../visualization/vgraphicssimpletextitem.h"
#include "../visualization/vcontrolpointspline.h"
#include "../../libs/vpropertyexplorer/vproperties.h"
#include "vformulaproperty.h"
#include "../container/vformula.h"

#include <QDockWidget>
#include <QHBoxLayout>

using namespace VPE;

//---------------------------------------------------------------------------------------------------------------------
VToolOptionsPropertyBrowser::VToolOptionsPropertyBrowser(QDockWidget *parent)
    :QObject(parent), PropertyModel(nullptr), formView(nullptr), currentItem(nullptr),
      propertyToId(QMap<VProperty *, QString>()),
      idToProperty(QMap<QString, VProperty *>())
{
    PropertyModel = new VPropertyModel(this);
    formView = new VPropertyFormView(PropertyModel, parent);
    formView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QScrollArea *scroll = new QScrollArea(parent);
    scroll->setWidgetResizable(true);
    scroll->setWidget(formView);

    parent->setWidget(scroll);

    connect(PropertyModel, &VPropertyModel::onDataChangedByEditor, this, &VToolOptionsPropertyBrowser::userChangedData);
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
    switch (item->type())
    {
        case VToolSinglePoint::Type:
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
        case VToolShoulderPoint::Type:
            ShowOptionsToolShoulderPoint(item);
            break;
        case VToolSpline::Type:
            ShowOptionsToolSpline(item);
            break;
        case VToolSplinePath::Type:
            ShowOptionsToolSplinePath(item);
            break;
        case VToolTriangle::Type:
            ShowOptionsToolTriangle(item);
            break;
        case VGraphicsSimpleTextItem::Type:
            currentItem = item->parentItem();
            ShowItemOptions(currentItem);
            break;
        case VControlPointSpline::Type:
            currentItem = item->parentItem();
            ShowItemOptions(currentItem);
            break;
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

    switch (currentItem->type())
    {
        case VToolSinglePoint::Type:
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
        case VToolShoulderPoint::Type:
            UpdateOptionsToolShoulderPoint();
            break;
        case VToolSpline::Type:
            UpdateOptionsToolSpline();
            break;
        case VToolSplinePath::Type:
            UpdateOptionsToolSplinePath();
            break;
        case VToolTriangle::Type:
            UpdateOptionsToolTriangle();
            break;
        case VGraphicsSimpleTextItem::Type:
            ShowItemOptions(currentItem->parentItem());
            break;
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
void VToolOptionsPropertyBrowser::userChangedData(VProperty *property)
{
    VProperty *prop = property;
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

    switch (currentItem->type())
    {
        case VToolSinglePoint::Type:
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
        case VToolShoulderPoint::Type:
            ChangeDataToolShoulderPoint(prop);
            break;
        case VToolSpline::Type:
            ChangeDataToolSpline(prop);
            break;
        case VToolSplinePath::Type:
            ChangeDataToolSplinePath(prop);
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
void VToolOptionsPropertyBrowser::AddPropertyPointName(Tool *i, const QString &propertyName)
{
    VProperty* itemName = new VProperty(propertyName);
    itemName->setValue(i->name());
    AddProperty(itemName, VAbstractTool::AttrName);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyCrossPoint(Tool *i, const QString &propertyName)
{
    VEnumProperty* itemProperty = new VEnumProperty(propertyName);
    itemProperty->setLiterals(QStringList()<< tr("First point") << tr("Second point"));
    itemProperty->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);
    AddProperty(itemProperty, VAbstractTool::AttrCrossPoint);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyLineType(Tool *i, const QString &propertyName,
                                                      const QMap<QString, QIcon> &styles)
{
    VLineTypeProperty *lineTypeProperty = new VLineTypeProperty(propertyName);
    lineTypeProperty->setStyles(styles);
    const qint32 index = VLineTypeProperty::IndexOfStyle(styles, i->getLineType());
    if (index == -1)
    {
        qWarning()<<"Can't find line style" << i->getLineType()<<"in list";
    }
    lineTypeProperty->setValue(index);
    AddProperty(lineTypeProperty, VAbstractTool::AttrTypeLine);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyLineColor(Tool *i, const QString &propertyName,
                                                       const QMap<QString, QString> &colors, const QString &id)
{
    VLineColorProperty *lineColorProperty = new VLineColorProperty(propertyName);
    lineColorProperty->setColors(colors);
    const qint32 index = VLineColorProperty::IndexOfColor(colors, i->GetLineColor());
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
            idToProperty[VAbstractTool::AttrName]->setValue(i->name());
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
void VToolOptionsPropertyBrowser::SetCrossCirclesPoint(const QVariant value)
{
    if (Tool *i = qgraphicsitem_cast<Tool *>(currentItem))
    {
        bool ok = false;
        const int val = value.toInt(&ok);

        CrossCirclesPoint cross = CrossCirclesPoint::FirstPoint;
        if (ok)
        {
            switch(val)
            {
                case 0:
                    cross = CrossCirclesPoint::FirstPoint;
                    break;
                case 1:
                    cross = CrossCirclesPoint::SecondPoint;
                    break;
                default:
                    cross = CrossCirclesPoint::FirstPoint;
                    break;
            }
        }
        i->SetCrossCirclesPoint(cross);
    }
    else
    {
        qWarning()<<"Can't cast item";
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::AddProperty(VProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    PropertyModel->addProperty(property, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSinglePoint(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolSinglePoint>(value.toString());
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
void VToolOptionsPropertyBrowser::ChangeDataToolEndLine(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolEndLine>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 5: // VAbstractTool::AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolAlongLine(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolAlongLine>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolArc(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolArc *i = qgraphicsitem_cast<VToolArc *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 8: // VAbstractTool::AttrRadius
            i->SetFormulaRadius(value.value<VFormula>());
            break;
        case 9: // VAbstractTool::AttrAngle1
            i->SetFormulaF1(value.value<VFormula>());
            break;
        case 10: // VAbstractTool::AttrAngle2
            i->SetFormulaF2(value.value<VFormula>());
            break;
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolBisector(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolBisector>(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutArc(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolCutArc>(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSpline(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolCutSpline>(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSplinePath(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolCutSplinePath>(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->SetFormula(value.value<VFormula>());
            break;
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolHeight(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolHeight>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLine(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolLine *i = qgraphicsitem_cast<VToolLine *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLineIntersect(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolLineIntersect>(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolNormal(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 4: // VAbstractTool::AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolNormal>(value.toString());
            break;
        case 5: // VAbstractTool::AttrAngle
            i->SetAngle(value.toDouble());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfContact(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 8: // VAbstractTool::AttrRadius
            i->setArcRadius(value.value<VFormula>());
            break;
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointOfContact>(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersection(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointOfIntersection>(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersectionArcs(VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointOfIntersectionArcs>(value.toString());
            break;
        case 28: // VAbstractTool::AttrCrossPoint
        {
            const QVariant value = property->data(VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointOfIntersectionArcs>(value);
            break;
        }
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersectionCircles(VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointOfIntersectionCircles>(value.toString());
            break;
        case 28: // VAbstractTool::AttrCrossPoint
        {
            const QVariant value = property->data(VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointOfIntersectionCircles>(value);
            break;
        }
        case 29: // VAbstractTool::AttrC1Radius
            i->SetFirstCircleRadius(value.value<VFormula>());
            break;
        case 30: // VAbstractTool::AttrC2Radius
            i->SetSecondCircleRadius(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointFromCircleAndTangent(VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointFromCircleAndTangent>(value.toString());
            break;
        case 31: // VAbstractTool::AttrCRadius
            i->SetCircleRadius(value.value<VFormula>());
            break;
        case 28: // VAbstractTool::AttrCrossPoint
        {
            const QVariant value = property->data(VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointFromCircleAndTangent>(value);
            break;
        }
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointFromArcAndTangent(VProperty *property)
{
    SCASSERT(property != nullptr)

    const QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolPointFromArcAndTangent>(value.toString());
            break;
        case 28: // VAbstractTool::AttrCrossPoint
        {
            const QVariant value = property->data(VProperty::DPC_Data, Qt::EditRole);
            SetCrossCirclesPoint<VToolPointFromArcAndTangent>(value);
            break;
        }
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolShoulderPoint(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 4: // VAbstractTool::AttrLength
            i->SetFormulaLength(value.value<VFormula>());
            break;
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolShoulderPoint>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSpline(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolSpline *i = qgraphicsitem_cast<VToolSpline *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 25: // VAbstractTool::AttrKCurve
        {
            VSpline spl = i->getSpline();
            spl.SetKcurve(value.toDouble());
            i->setSpline(spl);
            break;
        }
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSplinePath(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 25: // VAbstractTool::AttrKCurve
        {
            VSplinePath splPath = i->getSplinePath();
            splPath.SetKCurve(value.toDouble());
            i->setSplinePath(splPath);
            break;
        }
        case 27: // VAbstractTool::AttrTypeColor
            i->SetLineColor(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolTriangle(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolTriangle>(value.toString());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLineIntersectAxis(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolLineIntersectAxis>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 5: // VAbstractTool::AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCurveIntersectAxis(VProperty *property)
{
    SCASSERT(property != nullptr)

    QVariant value = property->data(VProperty::DPC_Data, Qt::DisplayRole);
    const QString id = propertyToId[property];

    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(currentItem);
    SCASSERT(i != nullptr);
    switch (PropertiesList().indexOf(id))
    {
        case 0: // VAbstractTool::AttrName
            SetPointName<VToolCurveIntersectAxis>(value.toString());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->SetTypeLine(value.toString());
            break;
        case 26: // VAbstractTool::AttrTypeLineColor
            i->SetLineColor(value.toString());
            break;
        case 5: // VAbstractTool::AttrAngle
            i->SetFormulaAngle(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSinglePoint(QGraphicsItem *item)
{
    VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Base point"));

    AddPropertyPointName(i, tr("Point label"));

    VPointFProperty* itemPosition = new VPointFProperty(tr("Position"));
    itemPosition->setValue(i->pos());
    AddProperty(itemPosition, QLatin1String("position"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolEndLine(QGraphicsItem *item)
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at distance and angle"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Length"), i->GetFormulaLength(), VAbstractTool::AttrLength);
    AddPropertyFormula(tr("Angle"), i->GetFormulaAngle(), VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolAlongLine(QGraphicsItem *item)
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at distance along line"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Length"), i->GetFormulaLength(), VAbstractTool::AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolArc(QGraphicsItem *item)
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Arc"));

    AddPropertyFormula(tr("Radius"), i->GetFormulaRadius(), VAbstractTool::AttrRadius);
    AddPropertyFormula(tr("First angle"), i->GetFormulaF1(), VAbstractTool::AttrAngle1);
    AddPropertyFormula(tr("Second angle"), i->GetFormulaF2(), VAbstractTool::AttrAngle2);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolBisector(QGraphicsItem *item)
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point along bisector"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Length"), i->GetFormulaLength(), VAbstractTool::AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutArc(QGraphicsItem *item)
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Cut arc tool"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("Length"), i->GetFormula(), VAbstractTool::AttrLength);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSpline(QGraphicsItem *item)
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool for segmenting a curve"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("Length"), i->GetFormula(), VAbstractTool::AttrLength);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSplinePath(QGraphicsItem *item)
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool segment a pathed curve"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("Length"), i->GetFormula(), VAbstractTool::AttrLength);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolHeight(QGraphicsItem *item)
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Perpendicular point along line"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLine(QGraphicsItem *item)
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Line between points"));

    QMap<QString, QIcon> styles = VAbstractTool::LineStylesPics();
    styles.remove(VAbstractTool::TypeLineNone);
    AddPropertyLineType(i, tr("Line type"), styles);
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLineIntersect(QGraphicsItem *item)
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at line intersection"));

    AddPropertyPointName(i, tr("Point label"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolNormal(QGraphicsItem *item)
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point along perpendicular"));

    AddPropertyFormula(tr("Length"), i->GetFormulaLength(), VAbstractTool::AttrLength);
    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);

    VDoubleProperty* itemAngle = new VDoubleProperty(tr("Additional angle degrees"));
    itemAngle->setValue(i->GetAngle());
    itemAngle->setSetting("Min", -360);
    itemAngle->setSetting("Max", 360);
    itemAngle->setSetting("Precision", 3);
    AddProperty(itemAngle, VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfContact(QGraphicsItem *item)
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point at intersection of arc and line"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("Radius"), i->getArcRadius(), VAbstractTool::AttrRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersection(QGraphicsItem *item)
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from x & y of two other points"));

    AddPropertyPointName(i, tr("Point label"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersectionArcs(QGraphicsItem *item)
{
    VToolPointOfIntersectionArcs *i = qgraphicsitem_cast<VToolPointOfIntersectionArcs *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from intersection two arcs"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyCrossPoint(i, tr("Take"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersectionCircles(QGraphicsItem *item)
{
    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from intersection two circles"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("First circle radius"), i->GetFirstCircleRadius(), VAbstractTool::AttrC1Radius);
    AddPropertyFormula(tr("Second circle radius"), i->GetSecondCircleRadius(), VAbstractTool::AttrC2Radius);
    AddPropertyCrossPoint(i, tr("Take"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointFromCircleAndTangent(QGraphicsItem *item)
{
    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from circle and tangent"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyFormula(tr("Circle radius"), i->GetCircleRadius(), VAbstractTool::AttrCRadius);
    AddPropertyCrossPoint(i, tr("Take"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointFromArcAndTangent(QGraphicsItem *item)
{
    VToolPointFromArcAndTangent *i = qgraphicsitem_cast<VToolPointFromArcAndTangent *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool to make point from arc and tangent"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyCrossPoint(i, tr("Take"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolShoulderPoint(QGraphicsItem *item)
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Special point on shoulder"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Length"), i->GetFormulaLength(), VAbstractTool::AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSpline(QGraphicsItem *item)
{
    VToolSpline *i = qgraphicsitem_cast<VToolSpline *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Curve tool"));

    VDoubleProperty* itemFactor = new VDoubleProperty(tr("Curve factor"));
    VSpline spl = i->getSpline();
    itemFactor->setSetting("Min", 0.1);
    itemFactor->setSetting("Max", 1000);
    itemFactor->setSetting("Step", 0.01);
    itemFactor->setSetting("Precision", 3);
    itemFactor->setValue(spl.GetKcurve());
    AddProperty(itemFactor, VAbstractTool::AttrKCurve);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSplinePath(QGraphicsItem *item)
{
    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool for path curve"));

    VDoubleProperty* itemFactor = new VDoubleProperty(tr("Curve factor"));
    VSplinePath splPath = i->getSplinePath();
    itemFactor->setSetting("Min", 0.1);
    itemFactor->setSetting("Max", 1000);
    itemFactor->setSetting("Step", 0.01);
    itemFactor->setSetting("Precision", 3);
    itemFactor->setValue(splPath.GetKCurve());
    AddProperty(itemFactor, VAbstractTool::AttrKCurve);
    AddPropertyLineColor(i, tr("Color"), VAbstractTool::ColorsList(), VAbstractTool::AttrColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolTriangle(QGraphicsItem *item)
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Tool triangle"));

    AddPropertyPointName(i, tr("Point label"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLineIntersectAxis(QGraphicsItem *item)
{
    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point intersection line and axis"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Angle"), i->GetFormulaAngle(), VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCurveIntersectAxis(QGraphicsItem *item)
{
    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(item);
    i->ShowVisualization(true);
    formView->setTitle(tr("Point intersection curve and axis"));

    AddPropertyPointName(i, tr("Point label"));
    AddPropertyLineType(i, tr("Line type"), VAbstractTool::LineStylesPics());
    AddPropertyLineColor(i, tr("Line color"), VAbstractTool::ColorsList(), VAbstractTool::AttrLineColor);
    AddPropertyFormula(tr("Angle"), i->GetFormulaAngle(), VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSinglePoint()
{
    VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(currentItem);
    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
    idToProperty[QLatin1String("position")]->setValue(i->pos());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolEndLine()
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[VAbstractTool::AttrAngle]->setValue(valueAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolAlongLine()
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolArc()
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(currentItem);


    QVariant valueRadius;
    valueRadius.setValue(i->GetFormulaRadius());
    idToProperty[VAbstractTool::AttrRadius]->setValue(valueRadius);

    QVariant valueFirstAngle;
    valueFirstAngle.setValue(i->GetFormulaF1());
    idToProperty[VAbstractTool::AttrAngle1]->setValue(valueFirstAngle);

    QVariant valueSecondAngle;
    valueSecondAngle.setValue(i->GetFormulaF2());
    idToProperty[VAbstractTool::AttrAngle2]->setValue(valueSecondAngle);

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolBisector()
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutArc()
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSpline()
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSplinePath()
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormula());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolHeight()
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLine()
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(currentItem);

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLineIntersect()
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolNormal()
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    idToProperty[VAbstractTool::AttrAngle]->setValue( i->GetAngle());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfContact()
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->getArcRadius());
    idToProperty[VAbstractTool::AttrRadius]->setValue(valueFormula);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersection()
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersectionArcs()
{
    VToolPointOfIntersectionArcs *i = qgraphicsitem_cast<VToolPointOfIntersectionArcs *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
    idToProperty[VAbstractTool::AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersectionCircles()
{
    VToolPointOfIntersectionCircles *i = qgraphicsitem_cast<VToolPointOfIntersectionCircles *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
    idToProperty[VAbstractTool::AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant c1Radius;
    c1Radius.setValue(i->GetFirstCircleRadius());
    idToProperty[VAbstractTool::AttrC1Radius]->setValue(c1Radius);

    QVariant c2Radius;
    c2Radius.setValue(i->GetSecondCircleRadius());
    idToProperty[VAbstractTool::AttrC2Radius]->setValue(c2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointFromCircleAndTangent()
{
    VToolPointFromCircleAndTangent *i = qgraphicsitem_cast<VToolPointFromCircleAndTangent *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
    idToProperty[VAbstractTool::AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);

    QVariant cRadius;
    cRadius.setValue(i->GetCircleRadius());
    idToProperty[VAbstractTool::AttrCRadius]->setValue(cRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointFromArcAndTangent()
{
    VToolPointFromArcAndTangent *i = qgraphicsitem_cast<VToolPointFromArcAndTangent *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
    idToProperty[VAbstractTool::AttrCrossPoint]->setValue(static_cast<int>(i->GetCrossCirclesPoint())-1);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolShoulderPoint()
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(currentItem);

    QVariant valueFormula;
    valueFormula.setValue(i->GetFormulaLength());
    idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSpline()
{
    VToolSpline *i = qgraphicsitem_cast<VToolSpline *>(currentItem);

    VSpline spl = i->getSpline();
    idToProperty[VAbstractTool::AttrKCurve]->setValue(spl.GetKcurve());

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSplinePath()
{
    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(currentItem);

    VSplinePath splPath = i->getSplinePath();
    idToProperty[VAbstractTool::AttrKCurve]->setValue(splPath.GetKCurve());

    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrColor]->setValue(index);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolTriangle()
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(currentItem);

    idToProperty[VAbstractTool::AttrName]->setValue(i->name());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLineIntersectAxis()
{
    VToolLineIntersectAxis *i = qgraphicsitem_cast<VToolLineIntersectAxis *>(currentItem);
    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[VAbstractTool::AttrAngle]->setValue(valueAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCurveIntersectAxis()
{
    VToolCurveIntersectAxis *i = qgraphicsitem_cast<VToolCurveIntersectAxis *>(currentItem);
    idToProperty[VAbstractTool::AttrName]->setValue(i->name());

    {
    const qint32 index = VLineTypeProperty::IndexOfStyle(VAbstractTool::LineStylesPics(), i->getLineType());
    idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);
    }

    {
    const qint32 index = VLineColorProperty::IndexOfColor(VAbstractTool::ColorsList(), i->GetLineColor());
    idToProperty[VAbstractTool::AttrLineColor]->setValue(index);
    }

    QVariant valueAngle;
    valueAngle.setValue(i->GetFormulaAngle());
    idToProperty[VAbstractTool::AttrAngle]->setValue(valueAngle);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VToolOptionsPropertyBrowser::PropertiesList() const
{
    QStringList attr = QStringList() << VAbstractTool::AttrName            /* 0 */
                                     << QLatin1String("position")          /* 1 */
                                     << VAbstractTool::AttrBasePoint       /* 2 */
                                     << VAbstractTool::AttrTypeLine        /* 3 */
                                     << VAbstractTool::AttrLength          /* 4 */
                                     << VAbstractTool::AttrAngle           /* 5 */
                                     << VAbstractTool::AttrFirstPoint      /* 6 */
                                     << VAbstractTool::AttrSecondPoint     /* 7 */
                                     << VAbstractTool::AttrRadius          /* 8 */
                                     << VAbstractTool::AttrAngle1          /* 9 */
                                     << VAbstractTool::AttrAngle2          /* 10 */
                                     << VAbstractTool::AttrCenter          /* 11 */
                                     << VAbstractTool::AttrThirdPoint      /* 12 */
                                     << VToolCutArc::AttrArc               /* 13 */
                                     << VToolCutSpline::AttrSpline         /* 14 */
                                     << VToolCutSplinePath::AttrSplinePath /* 15 */
                                     << VAbstractTool::AttrP1Line          /* 16 */
                                     << VAbstractTool::AttrP2Line          /* 17 */
                                     << VAbstractTool::AttrP1Line1         /* 18 */
                                     << VAbstractTool::AttrP2Line1         /* 19 */
                                     << VAbstractTool::AttrP1Line2         /* 20 */
                                     << VAbstractTool::AttrP2Line2         /* 21 */
                                     << VAbstractTool::AttrPShoulder       /* 22 */
                                     << VAbstractTool::AttrAxisP1          /* 23 */
                                     << VAbstractTool::AttrAxisP2          /* 24 */
                                     << VAbstractTool::AttrKCurve          /* 25 */
                                     << VAbstractTool::AttrLineColor       /* 26 */
                                     << VAbstractTool::AttrColor           /* 27 */
                                     << VAbstractTool::AttrCrossPoint      /* 28 */
                                     << VAbstractTool::AttrC1Radius        /* 29 */
                                     << VAbstractTool::AttrC2Radius        /* 30 */
                                     << VAbstractTool::AttrCRadius;        /* 31 */
    return attr;
}

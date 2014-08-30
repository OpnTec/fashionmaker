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
 **  Copyright (C) 2014 Valentina project
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
#include "tools/drawTools/drawtools.h"
#include "widgets/vapplication.h"
#include "widgets/vmaingraphicsview.h"
#include "visualization/vgraphicssimpletextitem.h"
#include "visualization/vcontrolpointspline.h"
#include "../libs/vpropertyexplorer/vproperties.h"
#include "vformulaproperty.h"
#include "../container/vformula.h"

#include <QDockWidget>
#include <QHBoxLayout>

using namespace VPE;

//---------------------------------------------------------------------------------------------------------------------
VToolOptionsPropertyBrowser::VToolOptionsPropertyBrowser(QDockWidget *parent)
    :QObject(parent),
      currentItem(nullptr),
      propertyToId(QMap<VProperty *, QString>()),
      idToProperty(QMap<QString, VProperty *>())
{
    PropertyModel = new VPropertyModel(this);
    TreeView = new VPropertyFormView(PropertyModel, parent);
    TreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QScrollArea *scroll = new QScrollArea(parent);
    scroll->setWidgetResizable(true);
    scroll->setWidget(TreeView);

    parent->setWidget(scroll);

    connect(PropertyModel, SIGNAL(onDataChangedByEditor(VProperty*)), this, SLOT(userChangedData(VProperty*)));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowItemOptions(QGraphicsItem *item)
{
    switch (item->type())
    {
        case VToolSinglePoint::Type:
        {
            VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(item);
            TreeView->setTitle(tr("Base point"));

            AddPropertyPointName(i, tr("Point name"));

            VPointFProperty* itemPosition = new VPointFProperty(tr("Position"));
            itemPosition->setValue(i->pos());
            AddProperty(itemPosition, QLatin1String("position"));
            break;
        }
        case VToolEndLine::Type:
        {
            VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(item);
            TreeView->setTitle(tr("Point at distance and angle"));

            AddPropertyPointName(i, tr("Point name"));
            AddPropertyPointsList(i, tr("Base point"), i->getBasePointId(), VAbstractTool::AttrBasePoint);
            AddPropertyLineType(i, tr("Line type"));
            AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
            AddPropertyFormula(tr("Angle"), i->getFormulaLength(), VAbstractTool::AttrAngle);
            break;
        }
        case VToolAlongLine::Type:
        {
            VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(item);
            TreeView->setTitle(tr("Point at distance along line"));

            AddPropertyPointName(i, tr("Point name"));
            AddPropertyPointsList(i, tr("First point"), i->getBasePointId(), VAbstractTool::AttrFirstPoint);
            AddPropertyPointsList(i, tr("Second point"), i->getSecondPointId(), VAbstractTool::AttrSecondPoint);
            AddPropertyLineType(i, tr("Line type"));
            AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
            break;
        }
        case VGraphicsSimpleTextItem::Type:
            currentItem = item->parentItem();
            ShowItemOptions(currentItem);
            break;
        case VControlPointSpline::Type:
            currentItem = item->parentItem();
            ShowItemOptions(currentItem);
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
        {
            VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(currentItem);
            idToProperty[VAbstractTool::AttrName]->setValue(i->name());
            idToProperty[QLatin1String("position")]->setValue(i->pos());
            break;
        }
        case VToolEndLine::Type:
        {
            VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
            idToProperty[VAbstractTool::AttrName]->setValue(i->name());
            idToProperty[VAbstractTool::AttrBasePoint]->setValue(i->getBasePointId());

            QStringList styles = VAbstractTool::Styles();
            qint32 index = styles.indexOf(i->getLineType());
            idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);

            QVariant valueFormula;
            valueFormula.setValue(i->getFormulaLength());
            idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

            QVariant valueAngle;
            valueAngle.setValue(i->getFormulaAngle());
            idToProperty[VAbstractTool::AttrAngle]->setValue(valueAngle);

            break;
        }
        case VToolAlongLine::Type:
        {
            VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
            idToProperty[VAbstractTool::AttrName]->setValue(i->name());
            idToProperty[VAbstractTool::AttrFirstPoint]->setValue(i->getBasePointId());
            idToProperty[VAbstractTool::AttrSecondPoint]->setValue(i->getSecondPointId());

            QStringList styles = VAbstractTool::Styles();
            qint32 index = styles.indexOf(i->getLineType());
            idToProperty[VAbstractTool::AttrTypeLine]->setValue(index);

            QVariant valueFormula;
            valueFormula.setValue(i->getFormulaLength());
            idToProperty[VAbstractTool::AttrLength]->setValue(valueFormula);

            break;
        }
        case VGraphicsSimpleTextItem::Type:
            ShowItemOptions(currentItem->parentItem());
            break;
        case VControlPointSpline::Type:
            ShowItemOptions(currentItem->parentItem());
            break;
        default:
            break;
    }
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
        default:
            break;
    }
    qApp->getSceneView()->update();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::itemClicked(QGraphicsItem *item)
{
    if (currentItem == item)
    {
        UpdateOptions();
        return;
    }

    PropertyModel->clear();
    propertyToId.clear();
    idToProperty.clear();

    currentItem = item;

    if (currentItem == nullptr)
    {
        TreeView->setTitle("");
        return;
    }

    ShowItemOptions(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::AddPropertyFormula(const QString &propertyName, const VFormula &formula,
                                                     const QString &attrName)
{
    VFormulaProperty* itemLength = new VFormulaProperty(propertyName);
    itemLength->setFormula(formula);
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
void VToolOptionsPropertyBrowser::AddPropertyPointsList(Tool *i, const QString &propertyName, const quint32 &value,
                                                        const QString &attrName)
{
    VObjectProperty *pointsProperty = new VObjectProperty(propertyName);
    QMap<QString, quint32> pointsList = i->PointsList();
    pointsProperty->setObjectsList(pointsList);
    pointsProperty->setValue(value);
    AddProperty(pointsProperty, attrName);
}

//---------------------------------------------------------------------------------------------------------------------
template<class Tool>
void VToolOptionsPropertyBrowser::AddPropertyLineType(Tool *i, const QString &propertyName)
{
    VEnumProperty *lineTypeProperty = new VEnumProperty(propertyName);
    lineTypeProperty->setLiterals(VAbstractTool::Styles());
    QStringList styles = VAbstractTool::Styles();
    qint32 index = styles.indexOf(i->getLineType());
    lineTypeProperty->setValue(index);
    AddProperty(lineTypeProperty, VAbstractTool::AttrTypeLine);
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

        if (name.isEmpty())
        {
            idToProperty[VAbstractTool::AttrName]->setValue(i->name());
        }
        else
        {
            //TODO check if label name is unique
            i->setName(name);
        }
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
        case 2: // VAbstractTool::AttrBasePoint
            i->setBasePointId(value.toUInt());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->setTypeLine(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->setFormulaLength(value.value<VFormula>());
            break;
        case 5: // VAbstractTool::AttrAngle
            i->setFormulaAngle(value.value<VFormula>());
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
            SetPointName<VToolEndLine>(value.toString());
            break;
        case 6: // VAbstractTool::AttrFirstPoint
            i->setFirstPointId(value.toUInt());
            break;
        case 7: // VAbstractTool::AttrSecondPoint
            i->setSecondPointId(value.toUInt());
            break;
        case 3: // VAbstractTool::AttrTypeLine
            i->setTypeLine(value.toString());
            break;
        case 4: // VAbstractTool::AttrLength
            i->setFormulaLength(value.value<VFormula>());
            break;
        default:
            qWarning()<<"Unknown property type. id = "<<id;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VToolOptionsPropertyBrowser::PropertiesList() const
{
    QStringList attr{VAbstractTool::AttrName,   /* 0 */
                QLatin1String("position"),      /* 1 */
                VAbstractTool::AttrBasePoint,   /* 2 */
                VAbstractTool::AttrTypeLine,    /* 3 */
                VAbstractTool::AttrLength,      /* 4 */
                VAbstractTool::AttrAngle,       /* 5 */
                VAbstractTool::AttrFirstPoint,  /* 6 */
                VAbstractTool::AttrSecondPoint};/* 7 */
    return attr;
}

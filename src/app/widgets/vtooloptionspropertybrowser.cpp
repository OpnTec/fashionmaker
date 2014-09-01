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
            i->setBasePointId(value.toUInt());
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
void VToolOptionsPropertyBrowser::ChangeDataToolArc(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolBisector(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutArc(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSpline(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolCutSplinePath(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolHeight(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLine(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolLineIntersect(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolNormal(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfContact(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolPointOfIntersection(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolShoulderPoint(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSpline(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolSplinePath(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ChangeDataToolTriangle(VProperty *property)
{

}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSinglePoint(QGraphicsItem *item)
{
    VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(item);
    TreeView->setTitle(tr("Base point"));

    AddPropertyPointName(i, tr("Point name"));

    VPointFProperty* itemPosition = new VPointFProperty(tr("Position"));
    itemPosition->setValue(i->pos());
    AddProperty(itemPosition, QLatin1String("position"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolEndLine(QGraphicsItem *item)
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(item);
    TreeView->setTitle(tr("Point at distance and angle"));

    AddPropertyPointName(i, tr("Point name"));
    AddPropertyPointsList(i, tr("Base point"), i->getBasePointId(), VAbstractTool::AttrBasePoint);
    AddPropertyLineType(i, tr("Line type"));
    AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
    AddPropertyFormula(tr("Angle"), i->getFormulaAngle(), VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolAlongLine(QGraphicsItem *item)
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(item);
    TreeView->setTitle(tr("Point at distance along line"));

    AddPropertyPointName(i, tr("Point name"));
    AddPropertyPointsList(i, tr("First point"), i->getBasePointId(), VAbstractTool::AttrFirstPoint);
    AddPropertyPointsList(i, tr("Second point"), i->getSecondPointId(), VAbstractTool::AttrSecondPoint);
    AddPropertyLineType(i, tr("Line type"));
    AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolArc(QGraphicsItem *item)
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolBisector(QGraphicsItem *item)
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutArc(QGraphicsItem *item)
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSpline(QGraphicsItem *item)
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolCutSplinePath(QGraphicsItem *item)
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolHeight(QGraphicsItem *item)
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLine(QGraphicsItem *item)
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolLineIntersect(QGraphicsItem *item)
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolNormal(QGraphicsItem *item)
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfContact(QGraphicsItem *item)
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolPointOfIntersection(QGraphicsItem *item)
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolShoulderPoint(QGraphicsItem *item)
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSpline(QGraphicsItem *item)
{
    VToolSpline *i = qgraphicsitem_cast<VToolSpline *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolSplinePath(QGraphicsItem *item)
{
    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowOptionsToolTriangle(QGraphicsItem *item)
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(item);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSinglePoint()
{
    VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(currentItem);
    TreeView->setTitle(tr("Base point"));

    AddPropertyPointName(i, tr("Point name"));

    VPointFProperty* itemPosition = new VPointFProperty(tr("Position"));
    itemPosition->setValue(i->pos());
    AddProperty(itemPosition, QLatin1String("position"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolEndLine()
{
    VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
    TreeView->setTitle(tr("Point at distance and angle"));

    AddPropertyPointName(i, tr("Point name"));
    AddPropertyPointsList(i, tr("Base point"), i->getBasePointId(), VAbstractTool::AttrBasePoint);
    AddPropertyLineType(i, tr("Line type"));
    AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
    AddPropertyFormula(tr("Angle"), i->getFormulaAngle(), VAbstractTool::AttrAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolAlongLine()
{
    VToolAlongLine *i = qgraphicsitem_cast<VToolAlongLine *>(currentItem);
    TreeView->setTitle(tr("Point at distance along line"));

    AddPropertyPointName(i, tr("Point name"));
    AddPropertyPointsList(i, tr("First point"), i->getBasePointId(), VAbstractTool::AttrFirstPoint);
    AddPropertyPointsList(i, tr("Second point"), i->getSecondPointId(), VAbstractTool::AttrSecondPoint);
    AddPropertyLineType(i, tr("Line type"));
    AddPropertyFormula(tr("Length"), i->getFormulaLength(), VAbstractTool::AttrLength);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolArc()
{
    VToolArc *i = qgraphicsitem_cast<VToolArc *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolBisector()
{
    VToolBisector *i = qgraphicsitem_cast<VToolBisector *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutArc()
{
    VToolCutArc *i = qgraphicsitem_cast<VToolCutArc *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSpline()
{
    VToolCutSpline *i = qgraphicsitem_cast<VToolCutSpline *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolCutSplinePath()
{
    VToolCutSplinePath *i = qgraphicsitem_cast<VToolCutSplinePath *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolHeight()
{
    VToolHeight *i = qgraphicsitem_cast<VToolHeight *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLine()
{
    VToolLine *i = qgraphicsitem_cast<VToolLine *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolLineIntersect()
{
    VToolLineIntersect *i = qgraphicsitem_cast<VToolLineIntersect *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolNormal()
{
    VToolNormal *i = qgraphicsitem_cast<VToolNormal *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfContact()
{
    VToolPointOfContact *i = qgraphicsitem_cast<VToolPointOfContact *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolPointOfIntersection()
{
    VToolPointOfIntersection *i = qgraphicsitem_cast<VToolPointOfIntersection *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolShoulderPoint()
{
    VToolShoulderPoint *i = qgraphicsitem_cast<VToolShoulderPoint *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSpline()
{
    VToolSpline *i = qgraphicsitem_cast<VToolSpline *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolSplinePath()
{
    VToolSplinePath *i = qgraphicsitem_cast<VToolSplinePath *>(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::UpdateOptionsToolTriangle()
{
    VToolTriangle *i = qgraphicsitem_cast<VToolTriangle *>(currentItem);
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

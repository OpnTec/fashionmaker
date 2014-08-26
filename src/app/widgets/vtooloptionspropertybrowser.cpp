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
        return;
    }

    ShowItemOptions(currentItem);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::userChangedData(VProperty *property)
{
    if (!propertyToId.contains(property))
    {
        return;
    }

    if (!currentItem)
    {
        return;
    }

    QVariant variant = property->data(VProperty::DPC_Data);

    QString id = propertyToId[property];
    switch (currentItem->type())
    {
        case VToolSinglePoint::Type:
        {
            if (id == QLatin1String("name"))
            {
                if (VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(currentItem))
                {
                    if (variant.toString() == i->name())
                    {
                        return;
                    }

                    if (variant.toString().isEmpty())
                    {
                        idToProperty[QLatin1String("name")]->setValue(i->name());
                    }
                    else
                    {
                        //TODO check if label name is unique
                        i->setName(variant.toString());
                    }
                }
            }
            else if (id == QLatin1String("posX"))
            {
                currentItem->setX(variant.toDouble());
            }
            else if (id == QLatin1String("posY"))
            {
                currentItem->setY(variant.toDouble());
            }
            break;
        }
//        case VGraphicsSimpleTextItem::Type:
//            ShowItemOptions(currentItem->parentItem());
//            break;
//        case VControlPointSpline::Type:
//            ShowItemOptions(currentItem->parentItem());
//            break;
        default:
            break;
    }
    qApp->getSceneView()->update();
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
            idToProperty[QLatin1String("name")]->setValue(i->name());
            idToProperty[QLatin1String("posX")]->setValue(i->x());
            idToProperty[QLatin1String("posY")]->setValue(i->y());
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
void VToolOptionsPropertyBrowser::AddProperty(VProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    PropertyModel->addProperty(property, QLatin1String("name"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowItemOptions(QGraphicsItem *item)
{
    switch (item->type())
    {
        case VToolSinglePoint::Type:
        {
            VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(item);

            VProperty* itemName = new VProperty(tr("Point name"));
            itemName->setValue(i->name());
            AddProperty(itemName, QLatin1String("name"));

            VDoubleProperty* positionX = new VDoubleProperty(tr("Position X"));
            positionX->setValue(i->x());
            AddProperty(positionX, QLatin1String("posX"));

            VDoubleProperty* positionY = new VDoubleProperty(tr("Position Y"));
            positionY->setValue(i->y());
            AddProperty(positionY, QLatin1String("posY"));

//            QtVariantProperty *position = variantManager->addProperty(QVariant::PointF, tr("Position"));
//            position->setValue(i->pos());
//            AddProperty(position, QLatin1String("position"));
//            mainGroup->addSubProperty(position);
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

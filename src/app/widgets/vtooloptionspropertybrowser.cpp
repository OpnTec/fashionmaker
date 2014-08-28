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
        QDockWidget *parent = qobject_cast<QDockWidget *>(this->parent());
        parent->setWindowTitle(tr("Tool options"));
        return;
    }

    ShowItemOptions(currentItem);
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

    QVariant variant = prop->data(VProperty::DPC_Data, Qt::DisplayRole);

    QString id = propertyToId[prop];
    switch (currentItem->type())
    {
        case VToolSinglePoint::Type:
        {
            if (id == QLatin1String("name"))
            {
                SetPointName<VToolSinglePoint>(variant.toString());
            }
            else if (id == QLatin1String("position"))
            {
                currentItem->setPos(variant.toPointF());
            }
//            else if (id == QLatin1String("list"))
//            {
//                qDebug()<<prop->data(VProperty::DPC_Data, Qt::DisplayRole);
//            }
            break;
        }
        case VToolEndLine::Type:
        {
            if (id == QLatin1String("name"))
            {
                SetPointName<VToolEndLine>(variant.toString());
            }
            else if (id == QLatin1String("basePoint"))
            {
                VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
                i->setBasePointId(variant.toUInt());
            }
            break;
        }
        default:
            break;
    }
    qApp->getSceneView()->update();
}

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
            idToProperty[QLatin1String("name")]->setValue(i->name());
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
            idToProperty[QLatin1String("position")]->setValue(i->pos());
            break;
        }
        case VToolEndLine::Type:
        {
            VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(currentItem);
            idToProperty[QLatin1String("name")]->setValue(i->name());
            idToProperty[QLatin1String("basePoint")]->setValue(i->getBasePointId());

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
    PropertyModel->addProperty(property, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolOptionsPropertyBrowser::ShowItemOptions(QGraphicsItem *item)
{
    switch (item->type())
    {
        case VToolSinglePoint::Type:
        {
            VToolSinglePoint *i = qgraphicsitem_cast<VToolSinglePoint *>(item);
            QDockWidget *parent = qobject_cast<QDockWidget *>(this->parent());
            parent->setWindowTitle(tr("Tool options (Base point)"));

            VProperty* itemName = new VProperty(tr("Point name"));
            itemName->setValue(i->name());
            AddProperty(itemName, QLatin1String("name"));

            VPointFProperty* itemPosition = new VPointFProperty(tr("Position"));
            itemPosition->setValue(i->pos());
            AddProperty(itemPosition, QLatin1String("position"));

            break;
        }
        case VToolEndLine::Type:
        {
            VToolEndLine *i = qgraphicsitem_cast<VToolEndLine *>(item);
            QDockWidget *parent = qobject_cast<QDockWidget *>(this->parent());
            parent->setWindowTitle(tr("Tool options (End of line)"));

            VProperty* itemName = new VProperty(tr("Point name"));
            itemName->setValue(i->name());
            AddProperty(itemName, QLatin1String("name"));

            VObjectProperty *pointsProperty = new VObjectProperty(tr("Base point"));
            QMap<QString, quint32> pointsList = i->PointsList();
            pointsProperty->setObjectsList(pointsList);
            pointsProperty->setValue(i->getBasePointId());
            AddProperty(pointsProperty, QLatin1String("basePoint"));

//            VEnumProperty *enumProperty = new VEnumProperty(tr("list"));
//            QStringList list = QStringList()<<"a1"<<"a2"<<"a3";
//            enumProperty->setLiterals(list);
//            AddProperty(enumProperty, QLatin1String("list"));
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

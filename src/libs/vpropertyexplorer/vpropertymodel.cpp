/************************************************************************
 **
 **  @file   vpropertymodel.cpp
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#include "vpropertymodel.h"

#include <QList>

#include "vproperty.h"
#include "vpropertyset.h"

using namespace VPE;

#include "vpropertymodel_p.h"

VPropertyModel::VPropertyModel(VPropertyModelPrivate *d, QObject *parent)
    : QAbstractItemModel(parent), d_ptr(d)
{
}


VPropertyModel::VPropertyModel(QObject * parent) :
    QAbstractItemModel(parent), d_ptr(new VPropertyModelPrivate())
{
}

VPropertyModel::~VPropertyModel()
{
    delete d_ptr->Properties;
    delete d_ptr;
}

//! Adds the property to the model and attaches it to the parentid
bool VPropertyModel::addProperty(VProperty* property, const QString& id, const QString &parentid, bool emitsignals)
{
    if (!property)
    {
        return false;
    }

    if (!d_ptr->Properties) // If not existant, create property set
    {
        d_ptr->Properties = new VPropertySet();
    }

    if (emitsignals)
    {
        VProperty* tmpParent = getProperty(parentid);
        int tmpRow = tmpParent != nullptr ? tmpParent->getRowCount() : d_ptr->Properties->getRootPropertyCount();
        beginInsertRows((tmpParent != nullptr ? getIndexFromProperty(tmpParent) : QModelIndex()), tmpRow, tmpRow);
    }

    d_ptr->Properties->addProperty(property, id, parentid);

    if (emitsignals)
    {
        endInsertRows();
    }

    return true;

}

//! Creates a property and adds it to the model
VProperty* VPropertyModel::createProperty(const QString& id, const QString& name, const QString& parentid,
                                          const QVariant& data)
{
    VProperty* tmpProp = new VProperty(name);
    tmpProp->setValue(data);
    if (addProperty(tmpProp, id, parentid))
    {
        return tmpProp;
    }
    else
        return nullptr;
}

//! Gets a property by it's ID
VProperty* VPropertyModel::getProperty(const QString& id)
{
    return d_ptr->Properties != nullptr ? d_ptr->Properties->getProperty(id) : nullptr;
}

//! Returns the model index at row/column
QModelIndex VPropertyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (d_ptr->Properties == nullptr || (parent.isValid() && parent.column() > 1))
    {
            return QModelIndex();
    }

    if (parent.isValid())
    {
        // Get the parent index
        VProperty* parentItem = getProperty(parent);
        if (parentItem)
        {
            VProperty* childItem = parentItem->getChild(row);
            if (childItem)
            {
                return createIndex(row, column, childItem);
            }
        }
    }
    else if (row >= 0 && row < d_ptr->Properties->count())
    {
        return createIndex(row, column, d_ptr->Properties->getRootProperty(row));
    }

    return QModelIndex();
}

//! Returns the parent of one model index
QModelIndex VPropertyModel::parent ( const QModelIndex & index ) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    VProperty* childItem = getProperty(index);
    if (childItem)
    {
        VProperty* parentItem = childItem->getParent();
        if (parentItem)
        {
            VProperty* grandParentItem = parentItem->getParent();
            int parents_row = grandParentItem != nullptr ? grandParentItem->getChildRow(parentItem)
                                                         : d_ptr->Properties->getRootProperties().indexOf(parentItem);

            if (parents_row >= 0)
            {
                return createIndex(parents_row, 0, parentItem);
            }
        }
    }

    return QModelIndex();
}

//! Returns the item flags for the given index
Qt::ItemFlags VPropertyModel::flags (const QModelIndex& index) const
{
    VProperty* tmpProperty = getProperty(index);
    if (!tmpProperty)
    {
        return Qt::NoItemFlags;
    }
    else
        return tmpProperty->flags(index.column());
}

//! Sets the role data for the item at index to value
bool VPropertyModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
    VProperty* tmpProperty = getProperty(index);
    if (index.column() == 1 && tmpProperty)
    {
        bool tmpHasChanged = tmpProperty->setData(value, role);
        if (tmpProperty->getUpdateParent() && tmpHasChanged)
        {   // If neccessary, update the parent as well
            QModelIndex tmpParentIndex = parent(index);
            emit dataChanged(tmpParentIndex, tmpParentIndex);
        }

        if (tmpHasChanged)
        {
            emit onDataChangedByEditor(tmpProperty);
        }
    }


    return true;
}


//! Returns the data of an model index
QVariant VPropertyModel::data ( const QModelIndex & index, int role ) const
{
    VProperty* tmpProperty = getProperty(index);
    if (!tmpProperty)
    {
        return QVariant();
    }
    else
        return tmpProperty->data(index.column(), role);
}


QVariant VPropertyModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        // Header data
        if (section == 0)
        {
            return d_ptr->HeadlineProperty;
        }
        else if (section == 1)
        {
            return d_ptr->HeadlineValue;
        }
    }
    else if (role == Qt::DisplayRole)
    {
        return QVariant(section);
    }

    return QVariant();
}


//! Returns the number of rows
int VPropertyModel::rowCount ( const QModelIndex & parent ) const
{
    if (parent.isValid())
    {
        VProperty* tmpParent = getProperty(parent);
        if (tmpParent)
        {
            return tmpParent->getRowCount();
        }
    }

    // Return the root property count
    if (d_ptr->Properties)
    {
        return d_ptr->Properties->getRootPropertyCount();
    }

    return 0;
}


//! Returns the number of columns
int VPropertyModel::columnCount ( const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 2;
}


//! Gets a property by its ModelIndex
VProperty* VPropertyModel::getProperty(const QModelIndex &index) const
{
    if (index.isValid())
    {
        VProperty* prop = static_cast<VProperty*>(index.internalPointer());

        if (prop)
        {
            return prop;
        }
    }
    return nullptr;
}

QString VPropertyModel::getPropertyID(const VProperty *prop) const
{
    return d_ptr->Properties != nullptr ? d_ptr->Properties->getPropertyID(prop) : QString();
}

QModelIndex VPropertyModel::getIndexFromProperty(VProperty* property, int column) const
{
    if (!property || column > columnCount() || column < 0)
    {
        return QModelIndex();
    }

    VProperty* parentItem = property->getParent();
    int row = 0;

    if (parentItem)
    {
        row = parentItem->getChildRow(property);
    }

    return createIndex(row, column, property);
}


void VPropertyModel::onDataChangedByModel(VProperty* property)
{
    QModelIndex tmpIndex = getIndexFromProperty(property, 1);
    if (tmpIndex.isValid())
    {
        emit dataChanged(tmpIndex, tmpIndex);
        emit onDataChangedByEditor(property);
    }
}

const VPropertySet *VPropertyModel::getPropertySet() const
{
    return d_ptr->Properties;
}

void VPropertyModel::clear(bool emit_signals)
{
    setPropertySet(nullptr, emit_signals);
}

VPropertySet *VPropertyModel::takePropertySet(VPropertySet *new_property_set, bool emit_signals)
{
    VPropertySet* tmpOldPropertySet = d_ptr->Properties;

    if (emit_signals)
    {
        emit beginResetModel();
    }
    d_ptr->Properties = new_property_set;
    if (emit_signals)
    {
        emit endResetModel();
    }

    return tmpOldPropertySet;
}

void VPropertyModel::setPropertySet(VPropertySet *property_set, bool emit_signals)
{
    VPropertySet* tmpOldPropertySet = takePropertySet(property_set, emit_signals);
    delete tmpOldPropertySet;
}

VProperty *VPropertyModel::takeProperty(const QString &id)
{
    QModelIndex tmpIndex = getIndexFromProperty(getProperty(id));
    if (d_ptr->Properties && tmpIndex.isValid())
    {
        beginRemoveRows(tmpIndex.parent(), tmpIndex.row(), tmpIndex.row());
        VProperty* tmpProp = d_ptr->Properties->takeProperty(id);
        endRemoveRows();
        return tmpProp;
    }

    return nullptr;
}

void VPropertyModel::removeProperty(const QString &id)
{
    QModelIndex tmpIndex = getIndexFromProperty(getProperty(id));
    if (d_ptr->Properties && tmpIndex.isValid())
    {
        beginRemoveRows(tmpIndex.parent(), tmpIndex.row(), tmpIndex.row());
        d_ptr->Properties->removeProperty(id);
        endRemoveRows();
    }
}

/************************************************************************
 **
 **  @file   vpropertyset.cpp
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

#include "vpropertyset.h"
using namespace VPE;

#include "vpropertyset_p.h"


VPropertySet::VPropertySet()
    : d_ptr(new VPropertySetPrivate())
{
}


VPropertySet::~VPropertySet()
{
    // Delete all the properties
    clear(true);

    delete d_ptr;
}

bool VPropertySet::addProperty(VProperty *property, const QString &id, const QString &parentid)
{
    // Check if the property to add is not a null pointer
    if(!property)
        return false;

    VProperty* tmpParent = parentid.isEmpty() ? NULL : getProperty(parentid);
    return addProperty(property, id, tmpParent);
}

bool VPropertySet::addProperty(VProperty *property, const QString &id, VProperty *parent_property)
{
    // Check if the property to add is not a null pointer
    if(!property)
        return false;

    QString tmpOldID = getPropertyID(property);
    if(!tmpOldID.isEmpty())
        d_ptr->Properties.remove(tmpOldID);

    if(parent_property)
        parent_property->addChild(property);
    else {
        d_ptr->RootProperties.append(property);
        if(property->getParent())
            property->getParent()->removeChild(property);
    }

    if(!id.isEmpty())
        d_ptr->Properties.insert(id, property);

    return true;
}

bool VPropertySet::hasProperty(VProperty *property) const
{
    if(!property)
        return false;

    return hasProperty(property, NULL);
}

VProperty *VPropertySet::getProperty(const QString &id) const
{
    return d_ptr->Properties.value(id, NULL);
}

VProperty *VPropertySet::takeProperty(const QString &id)
{
    VProperty* tmpProp = getProperty(id);
    removeProperty(tmpProp, false);

    // Return the property
    return tmpProp;
}

void VPropertySet::removeProperty(const QString &id)
{
    VProperty* tmpProp = takeProperty(id);
    if(tmpProp)
        delete tmpProp;
}

void VPropertySet::removeProperty(VProperty* prop, bool delete_property)
{
    // Remove all the children
    removePropertyFromSet(prop);

    // Remove from parent and optionally delete
    if(prop) {
        prop->setParent(NULL);

        if(delete_property)
            delete prop;
    }
}

int VPropertySet::count() const
{
    return d_ptr->Properties.count();
}

void VPropertySet::clear(bool delete_properties)
{
    d_ptr->Properties.clear();
    while(!d_ptr->RootProperties.isEmpty()) {
        VProperty* tmpProp = d_ptr->RootProperties.takeLast();
        if(tmpProp != nullptr && delete_properties) {
            delete tmpProp;
        }
    }
}

QString VPropertySet::getPropertyID(const VProperty *prop, bool look_for_parent_id) const
{
    QString tmpResult;
    const VProperty* tmpCurrentProp = prop;

    while(tmpCurrentProp && (look_for_parent_id || prop == tmpCurrentProp) && tmpResult.isEmpty()) {

        // todo: The following code doesn't work, because .key() doesn't accept a const VProperty* pointer ...
        //tmpResult = d_ptr->Properties.key(tmpCurrentProp, QString());

        // ... which is why we need the code below
        for (QMap<QString, VProperty*>::const_iterator i = d_ptr->Properties.constBegin(); i != d_ptr->Properties.constEnd(); ++i) {
            if(tmpCurrentProp == (*i))
                return i.key();
        }

        tmpCurrentProp = tmpCurrentProp->getParent();
    }

    return tmpResult;
}

const QMap<QString, VProperty *> &VPropertySet::getPropertiesMap() const
{
    return d_ptr->Properties;
}

const QList<VProperty *> &VPropertySet::getRootProperties() const
{
    return d_ptr->RootProperties;
}

VProperty *VPropertySet::getRootProperty(int row) const
{
    return d_ptr->RootProperties.value(row, NULL);
}

int VPropertySet::getRootPropertyCount() const
{
    return d_ptr->RootProperties.count();
}

VPropertySet* VPropertySet::clone() const
{
    VPropertySet* tmpResult = new VPropertySet();

    foreach(VProperty* tmpProperty, d_ptr->RootProperties)
        cloneProperty(tmpProperty, NULL, tmpResult);


    return tmpResult;
}



bool VPropertySet::hasProperty(VProperty *property, VProperty *parent) const
{
    if(!property)
        return false;

    const QList<VProperty*>& tmpChildrenList = (parent != NULL ? parent->getChildren() : d_ptr->RootProperties);
    foreach(VProperty* tmpProp, tmpChildrenList) {
        if(!tmpProp)
            continue;
        else if(tmpProp == property || hasProperty(property, tmpProp))
            return true;
    }

    return false;
}

void VPropertySet::cloneProperty(VProperty* property_to_clone, VProperty *parent_property, VPropertySet *output_set) const
{
    if(!output_set || !property_to_clone || !hasProperty(property_to_clone))
        return;

    QString tmpID = getPropertyID(property_to_clone, false);
    VProperty* tmpNewProperty = property_to_clone->clone(false);	// We want to clone the children ourselves (because of the IDs)

    output_set->addProperty(tmpNewProperty, tmpID, parent_property);
    for(int i = 0; i < property_to_clone->getRowCount(); ++i)
        cloneProperty(property_to_clone->getChild(i), tmpNewProperty, output_set);
}

void VPropertySet::removePropertyFromSet(VProperty *prop)
{
    // Remove all the children
    foreach(VProperty* tmpChild, prop->getChildren())
        removeProperty(tmpChild);


    QList<QString> tmpKeys = d_ptr->Properties.keys(prop);
    foreach(const QString& tmpID, tmpKeys)
        d_ptr->Properties.remove(tmpID);

    // Remove from list
    d_ptr->RootProperties.removeAll(prop);
}

/************************************************************************
 **
 **  @file   vserializedproperty.cpp
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

#include "vserializedproperty.h"

#include <QForeachContainer>
#include <QtGlobal>

#include "vproperty.h"
#include "vpropertyset.h"

VPE::VSerializedProperty::VSerializedProperty()
    : ID(), Type(), Value(), Children()
{}

/*! Creates a new VSerializedProperty from an existing property

*/
VPE::VSerializedProperty::VSerializedProperty(const VProperty* property, const VPropertySet* set)
    : ID(), Type(property ? property->type() : QString()), Value(property ? property->getValue() : QVariant()),
      Children()
{
    if (set)
    {
        ID = set->getPropertyID(property);

        initChildren(property, set);
    }
}

VPE::VSerializedProperty::VSerializedProperty(const VProperty *property, const QString &id, const VPropertySet *set)
    : ID(id), Type(property ? property->type() : QString()), Value(property ? property->getValue() : QVariant()),
      Children()
{
    initChildren(property, set);
}

VPE::VSerializedProperty::VSerializedProperty(const QString &id, const QString &type, const QVariant &value)
    : ID(id), Type(type), Value(value), Children()
{
}

void VPE::VSerializedProperty::initChildren(const VProperty *property, const VPropertySet *set)
{
    if (property && set)
    {
        const QList<VProperty*>& tmpChildren = property->getChildren();
        foreach(const VProperty* tmpChild, tmpChildren)
        {
            QString tmpChildID = set->getPropertyID(property);
            Children.append(VSerializedProperty(tmpChild, tmpChildID, set));
        }
    }
}

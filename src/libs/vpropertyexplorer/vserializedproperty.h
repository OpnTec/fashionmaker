/************************************************************************
 **
 **  @file   vserializedproperty.h
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

#ifndef VSERIALIZEDPROPERTY_H
#define VSERIALIZEDPROPERTY_H

#include "vpropertyexplorer_global.h"

#include <QString>
#include <QList>
#include <QVariant>

#include "vproperty.h"
#include "vpropertyset.h"

namespace VPE {


class VPROPERTYEXPLORERSHARED_EXPORT VSerializedProperty
{
public:
    //! Constructor
    VSerializedProperty();

    //! Constructor
    VSerializedProperty(const VProperty* property, const VPropertySet* set);

    //! Constructor
    VSerializedProperty(const VProperty* property, const QString& id, const VPropertySet* set);

    //! Constructor
    VSerializedProperty(const QString& id, const QString& type, const QVariant& value);

    //! Destructor
    ~VSerializedProperty();

    //! The property type
    QString ID;

    //! The property type
    QString Type;

    //! The serialized value of the property
    QVariant Value;

    //! List of child properties
    QList<VSerializedProperty> Children;

private:
    void initChildren(const VProperty* property, const VPropertySet* set);
};

}

#endif // VSERIALIZEDPROPERTY_H

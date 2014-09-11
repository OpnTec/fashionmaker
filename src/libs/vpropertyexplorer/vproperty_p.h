/************************************************************************
 **
 **  @file   vproperty_p.h
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

#ifndef VPROPERTY_P_H
#define VPROPERTY_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QVariant>
#include <QString>
#include "vproperty.h"

namespace VPE
{

class VPropertyPrivate
{
public:
    //! The property's value.
    //! This does not have to be used by subclasses, but it makes sense in cases where QVariant supports
    //! the data type. Also, this can be used as cache, so that when the data() function gets called by
    //! the model, the data does not have to be converted in a QVariant every time.
    QVariant VariantValue;

    //! Property name
    QString Name;

    //! Description
    QString Description;

    //! Specifies whether the property is empty or not
    bool IsEmpty;

    //! Stores the property type
    QVariant::Type PropertyVariantType;

    //! Stores whether the views have to update the parent of this property if it changes
    bool UpdateParent;

    //! Stores whether the views have to update the children of this property if it changes
    bool UpdateChildren;

    //! The parent property
    VProperty* Parent;

    QWidget* editor;

    Property type;

    //! List of child properties
    QList<VProperty*> Children;

    //! Constructor passing name and type
    VPropertyPrivate(const QString& name, QVariant::Type type)
        : VariantValue(type), Name(name), Description(QString()), IsEmpty(false), PropertyVariantType(type),
          UpdateParent(false), UpdateChildren(false), Parent(nullptr), editor(nullptr), type(Property::Simple),
          Children(QList<VProperty*>())
    {}

    //! Constructor
    VPropertyPrivate()
        : VariantValue(), Name(), Description(QString()), IsEmpty(false), PropertyVariantType(QVariant::Invalid),
          UpdateParent(false), UpdateChildren(false), Parent(nullptr), editor(nullptr), type(Property::Simple),
          Children(QList<VProperty*>())
    {}
private:
    Q_DISABLE_COPY(VPropertyPrivate)
};

}

#endif // VPROPERTY_P_H

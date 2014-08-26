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

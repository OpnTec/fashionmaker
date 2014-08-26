#include "vserializedproperty.h"

using namespace VPE;

VSerializedProperty::VSerializedProperty()
    : ID(), Type(), Value()
{
}

/*! Creates a new VSerializedProperty from an existing property

*/
VSerializedProperty::VSerializedProperty(const VProperty* property, const VPropertySet* set)
    : ID(), Type(property ? property->type() : QString()), Value(property ? property->getValue() : QVariant())
{
    if(set) {
        ID = set->getPropertyID(property);

        initChildren(property, set);
    }
}

VSerializedProperty::VSerializedProperty(const VProperty *property, const QString &id, const VPropertySet *set)
    : ID(id), Type(property ? property->type() : QString()), Value(property ? property->getValue() : QVariant())
{
    initChildren(property, set);
}

VSerializedProperty::VSerializedProperty(const QString &id, const QString &type, const QVariant &value)
    : ID(id), Type(type), Value(value)
{
}

VPE::VSerializedProperty::~VSerializedProperty()
{
    //
}

void VSerializedProperty::initChildren(const VProperty *property, const VPropertySet *set)
{
    if(property && set) {
        const QList<VProperty*>& tmpChildren = property->getChildren();
        foreach(const VProperty* tmpChild, tmpChildren) {
            QString tmpChildID = set->getPropertyID(property);
            Children.append(VSerializedProperty(tmpChild, tmpChildID, set));
        }
    }
}


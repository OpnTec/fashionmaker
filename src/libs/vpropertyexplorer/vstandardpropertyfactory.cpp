#include "vstandardpropertyfactory.h"

#include "vpropertyfactorymanager.h"

// Supported Properties
#include "vproperty.h"
#include "plugins/vboolproperty.h"
#include "plugins/vcolorproperty.h"
#include "plugins/vemptyproperty.h"
#include "plugins/venumproperty.h"
#include "plugins/vfileproperty.h"
#include "plugins/vnumberproperty.h"
#include "plugins/vshortcutproperty.h"
#include "plugins/Vector3d/vvector3dproperty.h"


using namespace VPE;

VStandardPropertyFactory::VStandardPropertyFactory()
    : VAbstractPropertyFactory()
{
}

VStandardPropertyFactory::VStandardPropertyFactory(VPropertyFactoryManager *manager)
    : VAbstractPropertyFactory()
{
    if(manager) {
        manager->registerFactory("string", this);
        manager->registerFactory("bool", this);
        manager->registerFactory("color", this);
        manager->registerFactory("empty", this);
        manager->registerFactory("enum", this);
        manager->registerFactory("file", this);
        manager->registerFactory("integer", this);
        manager->registerFactory("double", this);
        manager->registerFactory("shortcut", this);
        manager->registerFactory("vector3d", this);
    }
}

VProperty *VStandardPropertyFactory::createProperty(const QString &type, const QString &name)
{
    if(type == QString("string")) {
        return new VProperty(name);
    } else if(type == QString("bool")) {
        return new VBoolProperty(name);
    } else if(type == QString("color")) {
        return new QColorProperty(name);
    } else if(type == QString("empty")) {
        return new VEmptyProperty(name);
    } else if(type == QString("enum")) {
        return new VEnumProperty(name);
    } else if(type == QString("file")) {
        return new VFileProperty(name);
    } else if(type == QString("integer")) {
        return new VIntegerProperty(name);
    } else if(type == QString("double")) {
        return new VDoubleProperty(name);
    } else if(type == QString("shortcut")) {
        return new VShortcutProperty(name);
    } else if(type == QString("vector3d")) {
        return new QVector3DProperty(name);
    }
    else
        return nullptr;
}

#include "vpropertyfactorymanager.h"
#include "vpropertyfactorymanager_p.h"

#include "vstandardpropertyfactory.h"
#include "vproperty.h"

using namespace VPE;


VPropertyFactoryManager* VPropertyFactoryManager::DefaultManager = NULL;

VPropertyFactoryManager::VPropertyFactoryManager(QObject *parent)
    : QObject(parent), d_ptr(new VPropertyFactoryManagerPrivate())
{

}

VPropertyFactoryManager::~VPropertyFactoryManager()
{
    // Delete all factories
    QList<VAbstractPropertyFactory*> tmpFactories = d_ptr->Factories.values();
    while(!tmpFactories.isEmpty()) {
        VAbstractPropertyFactory* tmpFactory = tmpFactories.takeLast();
        tmpFactories.removeAll(tmpFactory);
        delete tmpFactory;
    }


    delete d_ptr;
    if(this == DefaultManager)
        DefaultManager = NULL;
}

void VPropertyFactoryManager::registerFactory(const QString& type, VAbstractPropertyFactory* factory)
{
    if(type.isEmpty())
        return;

    // Remove old factory
    unregisterFactory(getFactory(type), type, true);
    // Register new one
    d_ptr->Factories[type] = factory;
}

void VPropertyFactoryManager::unregisterFactory(VAbstractPropertyFactory* factory, const QString& type, bool delete_if_unused)
{
    if(!factory)
        return;

    if(!type.isEmpty()) {
        // Remove all occurances
        QString tmpKey;
        do {
            tmpKey = d_ptr->Factories.key(factory, QString());
            if(!tmpKey.isEmpty()) d_ptr->Factories.remove(tmpKey);
        } while(!tmpKey.isEmpty());
    } else {
        // Only remove one type
        if(d_ptr->Factories.value(type, NULL) == factory)
            d_ptr->Factories.remove(type);
    }

    if(delete_if_unused && !isRegistered(factory))
        delete factory;
}

bool VPropertyFactoryManager::isRegistered(VAbstractPropertyFactory* factory)
{
    return (!d_ptr->Factories.key(factory, QString()).isEmpty());
}

VAbstractPropertyFactory* VPropertyFactoryManager::getFactory(const QString& type)
{
    return d_ptr->Factories.value(type, NULL);
}


VProperty* VPropertyFactoryManager::createProperty(const QString& type, const QString& name, const QString& description, const QString &default_value)
{
    VAbstractPropertyFactory* tmpFactory = getFactory(type);
    VProperty* tmpResult = NULL;
    if(tmpFactory) {
        tmpResult = tmpFactory->createProperty(type, name);

        if(tmpResult) {
            tmpResult->setDescription(description);

            if(!default_value.isEmpty())
                tmpResult->deserialize(default_value);
        }
    }

    return tmpResult;
}

VPropertyFactoryManager *VPropertyFactoryManager::getDefaultManager()
{
    if(!DefaultManager) {
        DefaultManager = new VPropertyFactoryManager();
        /*VStandardPropertyFactory* tmpStandardProp = */new VStandardPropertyFactory(DefaultManager);
    }

    return DefaultManager;
}

QStringList VPropertyFactoryManager::getSupportedTypes()
{
    return d_ptr->Factories.keys();
}

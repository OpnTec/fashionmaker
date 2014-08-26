#ifndef VASTANDARDPROPERTYFACTORY_H
#define VASTANDARDPROPERTYFACTORY_H

#include "vpropertyexplorer_global.h"
#include "vabstractpropertyfactory.h"

namespace VPE {

class VProperty;

class VPropertyFactoryManager;

//! The standard property factory is able to create all the properties that are included in QtPropertyExplorer
//! by default.
class VPROPERTYEXPLORERSHARED_EXPORT VStandardPropertyFactory : public VAbstractPropertyFactory
{
public:
    //! Constructor
    VStandardPropertyFactory();

    //! Constructor
    //! \param manager Registers this factory at the manager for all it's types
    VStandardPropertyFactory(VPropertyFactoryManager* manager);

    //! Creates a new property of a certain type and assigns a name and description (otionally)
    //! \param type The type of the property as string
    //! \return Returns the created property or NULL if it couldn't be be created
    virtual VProperty* createProperty(const QString& type, const QString &name);
};

}

#endif // VASTANDARDPROPERTYFACTORY_H

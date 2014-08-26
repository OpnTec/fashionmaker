#ifndef VABSTRACTPROPERTYFACTORY_H
#define VABSTRACTPROPERTYFACTORY_H

#include "vpropertyexplorer_global.h"
#include <QObject>

namespace VPE {

class VProperty;

class VPROPERTYEXPLORERSHARED_EXPORT VAbstractPropertyFactory
{
public:
    //! Empty virtual destructor
    virtual ~VAbstractPropertyFactory() {}

    //! Creates a new property of a certain type and assigns a name and description (otionally)
    //! \param type The type of the property as string
    //! \param name The property's name
    //! \return Returns the created property or NULL if it couldn't be be created
    virtual VProperty* createProperty(const QString& type, const QString &name) = 0;
};

}

#endif // VABSTRACTPROPERTYFACTORY_H

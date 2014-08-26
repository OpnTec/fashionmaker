#ifndef VPROPERTYFACTORYMANAGER_H
#define VPROPERTYFACTORYMANAGER_H

#include "vpropertyexplorer_global.h"
#include <QObject>

namespace VPE {

class VAbstractPropertyFactory;
class VPropertyFactoryManagerPrivate;
class VProperty;

class VPROPERTYEXPLORERSHARED_EXPORT VPropertyFactoryManager : public QObject
{
    Q_OBJECT
public:
    //! Constructor
    VPropertyFactoryManager(QObject* parent = nullptr);

    //! Destructor
    virtual ~VPropertyFactoryManager();

    //! Register a factory to the factory manager
    //! Note that the manager takes ownership of the factory, so don't delete it.
    //! You can unregister a factory using unregisterFactory()
    void registerFactory(const QString& type, VAbstractPropertyFactory* factory);

    //! Removes a factory from the manager.
    //! \param factory The factory to unregister
    //! \param type The type from which to remove the factory. If this is empty, all the types the factory is registered for are being removed
    //! \param delete_if_unused Determines whether the factory should be deleted, if it not used anymore by this manager. Default: true. Otherwise, if the factory is unused by this manager, ownership is being passed on.
    void unregisterFactory(VAbstractPropertyFactory* factory, const QString& type = QString(),
                           bool delete_if_unused = true);

    //! Returns whether a factory is registered (and thus owned) by this factory manager
    bool isRegistered(VAbstractPropertyFactory* factory);

    //! Returns a pointer to a factory registered for a certain type
    //! \param type The type to return the factory for
    //! \return Returns NULL, if there is none registered for this type
    VAbstractPropertyFactory* getFactory(const QString& type);

    //! Creates a new property of a certain type and assigns a name and description (otionally)
    //! \param type The type of the property as string
    //! \param name The name of the property
    //! \param description The property's description. Optional.
    //! \param default_value The properties initial value as string. Optional.
    //! \return Returns the created property or NULL if it couldn't be be created
    VProperty* createProperty(const QString& type, const QString& name, const QString& description = QString(),
                              const QString& default_value = QString());

    //! Returns the default manager.
    static VPropertyFactoryManager* getDefaultManager();

    //! Returns a list of all supported property types
    QStringList getSupportedTypes();

protected:
    VPropertyFactoryManagerPrivate* d_ptr;

    //! The default manager
    static VPropertyFactoryManager* DefaultManager;
};

}

#endif // VPROPERTYFACTORYMANAGER_H

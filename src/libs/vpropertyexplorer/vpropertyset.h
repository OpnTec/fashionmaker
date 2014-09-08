#ifndef VPROPERTYSET_H
#define VPROPERTYSET_H

#include "vpropertyexplorer_global.h"

#include <QMap>
#include <QString>

#include "vproperty.h"

namespace VPE {

// Forward declaration
class VPropertySetPrivate;

// todo: better description
//! \brief	VPropertySet is a simple class for managing a set of properties.
//!			If you don't need all the Model-functionality, chose this class
//!			over VPropertyModel.
//!

//!
class VPROPERTYEXPLORERSHARED_EXPORT VPropertySet
{
public:
    //! Default constructor, creating an empty property set
    explicit VPropertySet();

    //! Destructor
    virtual ~VPropertySet();

    //! Adds the property to the model and attaches it to the parentid. Note that if the property has a parent which is not part of this set, it will be removed from that parent.
    //! \param property The property to add
    //! \param id The property ID. If id is empty, the property will not be accessable by it's id but still be added. If the property was filed under another ID before, that will no longer be valid.
    //! \param parentid The property's ID to which to add the property as child. Pass empty string to add it to the root properties.
    virtual bool addProperty(VProperty* property, const QString& id, const QString& parentid);

    //! Adds the property to the model and attaches it to the parent property.
    //! \param property The property to add
    //! \param id The property ID. If id is empty, the property will not be accessable by it's id but still be added. If the property was filed under another ID before, that will no longer be valid.
    //! \param parentid The property to which to add the property as child. Pass NULL to add it to the root properties.
    virtual bool addProperty(VProperty* property, const QString& id, VProperty* parent_property = nullptr);

    //! Checks whether a property belongs to this set and returns the result
    //! \param property The property to check for
    //! \return True, if the property is part of this set, false otherwise
    virtual bool hasProperty(VProperty* property) const;

    //! Gets a property by it's ID
    virtual VProperty* getProperty(const QString& id) const;

    //! Removes a property from the set and returns it
    virtual VProperty* takeProperty(const QString& id);

    //! Removes a property from the set and deletes it
    virtual void removeProperty(const QString& id);

    //! Removes a property from the set and deletes it optionally
    virtual void removeProperty(VProperty* prop, bool delete_property = true);

    //! Returns the number of properties with in ID that are directly accessable by getProperty()
    virtual int count() const;

    //! Clears the set and (optionally) deletes all properties
    //! \param delete_properties Set this to false, if you don't want the properties to get deleted.
    virtual void clear(bool delete_properties = true);

    //! Returns the ID of the property within the set
    //! The concept of property IDs is, that the object that manages the properties
    //! and not the properties themselves handle the IDs.
    //! \param prop The property of which to get the ID.
    //! \param look_for_parent_id If this is TRUE and the property has no ID, all the parent properties are checked.
    //! \return Returns the ID under which the property is stored within the set
    virtual QString getPropertyID(const VProperty* prop, bool look_for_parent_id = true) const;

    //! Returns a const reference to the map of properties
    const QMap<QString, VProperty*>& getPropertiesMap() const;

    //! Returns a const reference to the list of root properties
    const QList<VProperty*>& getRootProperties() const;

    //! Returns the root property in a certain row
    //! \param row The root row in which to look for the root property
    VProperty* getRootProperty(int row) const;

    //! Returns the number of independent properties
    int getRootPropertyCount() const;

    //! Clones the property set
    VPropertySet* clone() const;

protected:
    //! Checks whether a property belongs to this set and returns the result
    //! \param property The property to check for
    //! \param parent The parent property from which to start checking all the children
    //! \return True, if the property is part of this set, false otherwise
    virtual bool hasProperty(VProperty* property, VProperty* parent) const;

    //! Clones a property into another property set
    void cloneProperty(VProperty* property_to_clone, VProperty* parent_property, VPropertySet* output_set) const;

    //! Recursivly removes a property's child properties from the set, but not from the parent
    virtual void removePropertyFromSet(VProperty* prop);

    //! The data
    VPropertySetPrivate* d_ptr;
};

}

#endif // VPROPERTYMODEL_H

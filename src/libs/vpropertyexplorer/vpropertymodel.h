/************************************************************************
 **
 **  @file   vpropertymodel.h
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

#ifndef VPROPERTYMODEL_H
#define VPROPERTYMODEL_H

#include "vpropertyexplorer_global.h"

#include <QAbstractItemModel>
#include <QMap>
#include <QString>

#include "vproperty.h"

namespace VPE
{

class VPropertyModelPrivate;
class VPropertySet;

//! \brief  This is the base model for managing all the properties
//!         and passing them to the view.
//!
//! When you create your own "proxy models", this is the place to
//! start: just subclass VPropertyModel and extend the new class.
//! Have a look at existing examples of proxies.
//!
//! <strong>Note that in this context, the term "proxy model" does not refer
//! to VProxyModel as that is another concept.</strong>
//! The idea behind "proxy models" in the QtPropertyExplorer framework
//! is to provide an convenient interface which takes data as your
//! application (or a third-party-library) provides it, and converts this
//! data to VProperty-objects, manage them and produce output for the views.
//!
//! In most cases, you will not need to rewrite the basic functions of
//! QAbstractItemModel, as VPropertyModel provides standard implementations
//! to work with. Thus, instead of subclassing VPropertyModel, it is also
//! possible to use VPropertyModel directly (as it is not an abstract class).
//! This might be more convenient in some cases.
class VPROPERTYEXPLORERSHARED_EXPORT VPropertyModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit VPropertyModel(QObject * parent = 0);
    virtual ~VPropertyModel();

    //! Adds the property to the model and attaches it to the parentid
    //! \param emitsignals If this is set to false, this function will not call beginInsertRows() and endInsertRows(),
    //! so it has to be called from a subclass
    virtual bool addProperty(VProperty* property, const QString& id, const QString& parentid = QString(),
                             bool emitsignals = true);

    //! Creates a property and adds it to the model
    virtual VProperty* createProperty(const QString& id, const QString& name, const QString& parentid = QString(),
                                      const QVariant& data = QVariant());

    //! Gets a property by it's ID
    virtual VProperty* getProperty(const QString& id);

    //! Returns the item flags for the given index
    virtual Qt::ItemFlags flags (const QModelIndex& index) const;

    //! Sets the role data for the item at index to value
    virtual bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    //! Returns the model index at row/column
    virtual QModelIndex index (int row, int column, const QModelIndex & parent = QModelIndex() ) const;

    //! Returns the parent of one model index
    virtual QModelIndex parent (const QModelIndex& index) const;

    //! Returns the data of an model index
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

    //! Returns the data for the given role and section in the header with the specified orientation.
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //! Returns the number of rows
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    //! Returns the number of columns
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

    //! Gets a property by its ModelIndex
    //! \param index The modelIndex of the property.
    //! \return Returns the property with the given index, or NULL if none such property exists
    virtual VProperty* getProperty(const QModelIndex &index) const;

    //! Returns the ID of the property within the model
    //! The concept of property IDs is, that the object that manages the properties
    //! and not the properties themselves handle the IDs.
    //! \return Returns the ID under which the property is stored within the model
    virtual QString getPropertyID(const VProperty* prop) const;

    //! Returns a const pointer to the property set managed by this model. If you want to manipulate the property set,
    //! either use the methods provided by the model or use takePropertySet() and setPropertySet().
    //! \return A constant pointer to the property set or NULL if there currently is none.
    virtual const VPropertySet* getPropertySet() const;

    //! Clears the model, deletes the property set managed by this model.
    //! \param emit_signals Default: true. Set this to false if you want to prevent the model from emmiting the reset
    //! model signals
    virtual void clear(bool emit_signals = true);

    //! Removes the current property set and returns it. If new_property_set is set, the old one will be replaced by the
    //! new one
    //! \param new_property_set The new property set to replace the old one with. Default: NULL
    //! \param emit_signals Default: true. Set this to false if you want to prevent the model from emmiting the reset
    //! model signals
    //! \return A constant pointer to the property set or NULL if there currently is none.
    virtual VPropertySet* takePropertySet(VPropertySet* new_property_set = nullptr, bool emit_signals = true);

    //! Sets a new property set. The model will take ownership of the property set. The old property set will be
    //! deleted.
    //! \param property_set The new property set. Setting this to NULL has the same effect as calling clear.
    //! \param emit_signals Default: true. Set this to false if you want to prevent the model from emmiting the reset
    //! model signals
    virtual void setPropertySet(VPropertySet* property_set, bool emit_signals = true);

    //! Removes a property from the model and returns it
    virtual VProperty* takeProperty(const QString& id);

    //! Removes a property from the model and deletes it
    virtual void removeProperty(const QString& id);

signals:
    //! This signal is being emitted, when the setData method is being called
    void onDataChangedByEditor(VProperty* property);

public slots:
    //! This function causes the views to update the property
    void onDataChangedByModel(VProperty* property);

protected:
    //! Gets a property by its ModelIndex
    virtual QModelIndex getIndexFromProperty(VProperty* property, int column = 0) const;

    //! Protected constructor passing the private object
    VPropertyModel(VPropertyModelPrivate* d, QObject* parent = 0);

    //! The model data
    VPropertyModelPrivate* d_ptr;

private:
    Q_DISABLE_COPY(VPropertyModel)
};

}

#endif // VPROPERTYMODEL_H

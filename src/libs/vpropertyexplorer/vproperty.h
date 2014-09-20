/************************************************************************
 **
 **  @file   vproperty.h
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

#ifndef VPROPERTY_H
#define VPROPERTY_H

#include "vpropertyexplorer_global.h"

#include <QVariant>
#include <QString>
#include <QSharedPointer>
#include <QStyleOptionViewItem>
#include <QAbstractItemDelegate>
#include <QEvent>

namespace VPE
{

enum class Property : char{Simple, Complex};

static const int MyCustomEventType = 1099;

class VPROPERTYEXPLORERSHARED_EXPORT UserChangeEvent : public QEvent
{
public:
    UserChangeEvent() : QEvent(static_cast<QEvent::Type>(MyCustomEventType)) {}
    virtual ~UserChangeEvent();
};

class VPropertyPrivate;

class VPROPERTYEXPLORERSHARED_EXPORT VProperty : public QObject
{
    Q_OBJECT
public:
    enum DPC_DisplayColumn
    {
        DPC_Name = 0,
        DPC_Data
    };

    //! Standard constructor, takes a name and a parent property as argument
    explicit VProperty(const QString& name, QVariant::Type type = QVariant::String);

    //! Destructor
    virtual ~VProperty();

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const;

    //! This is used by the model to set the data
    //! \param data The data to set
    //! \param role The role. Default is Qt::EditRole
    //! \return Returns true, if the data was changed, false if not.
    virtual bool setData (const QVariant& data, int role = Qt::EditRole);

    //! This is called by the delegate when the property value is being drawn.
    //! The standard implementation doesn't do anything.
    //! If you reimplement this in a sub property, make sure to return true or the delegate will draw the item.
    virtual bool paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index,
                       const QAbstractItemDelegate* delegate) const;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and
    //! slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                  const QAbstractItemDelegate* delegate);

    //! Sets the property's data to the editor (returns false, if the standard delegate should do that)
    virtual bool setEditorData(QWidget* editor);

    //! Gets the data from the widget
    virtual QVariant getEditorData(const QWidget* editor) const;

    //! Returns item flags
    virtual Qt::ItemFlags flags(int column = DPC_Name) const;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value);

    //! Returns the value of the property as a QVariant
    virtual QVariant getValue() const;

    //! Serializes the value to a string
    virtual QString serialize() const;

    //! Deserializes the value from a string
    virtual void deserialize(const QString& value);

    // The following functions are experimental and not yet implemented.
    /*//! Returns a pointer to the data stored and handled by this property. In most cases this function shouldn't be
    //! used.
    //! \return Returns a void pointer to the data. Not all properties have to support this. By default, this
    //! implementation returns a NULL pointer.
    virtual void* getDataPointer();

    //! Sets the data.
    //! \return Returns a void pointer to the data. Not all properties have to support this. By default, this
    //! implementation returns a NULL pointer.
    virtual bool setDataPointer(void* pointer);*/

    //! Sets the name of the property
    virtual void setName(const QString& name);

    //! Gets the name of the property
    virtual QString getName() const;

    //! Sets the name of the property
    virtual void setDescription(const QString& desc);

    //! Gets the name of the property
    virtual QString getDescription() const;

    //! Adds a child to this property
    virtual int addChild(VProperty* child);

    //! Returns a reference to the list of children
    virtual QList<VProperty*>& getChildren();

    //! Returns a reference to the list of children
    virtual const QList<VProperty*>& getChildren() const;

    //! Returns the child at a certain row
    virtual VProperty* getChild(int row) const;

    //! Gets the number of children
    virtual int getRowCount() const;

    //! Gets the parent of this property
    virtual VProperty* getParent() const;

    //! Sets the parent of this property
    virtual void setParent(VProperty* parent);

    //! Removes a child from the children list, doesn't delete the child!
    virtual void removeChild(VProperty* child);

    //! Returns the row the child has
    virtual int getChildRow(VProperty* child) const;

    //! Returns whether the views have to update the parent of this property if it changes
    virtual bool getUpdateParent() const;

    //! Returns whether the views have to update the children of this property if it changes
    virtual bool getUpdateChildren() const;

    //! Sets whether the views should update Parents or children after this property changes
    virtual void setUpdateBehaviour(bool update_parent, bool update_children);

    //! Sets the settings by calling the overloaded setSetting(const QString& key, const QVariant& value) for each item
    //! in the map.
    virtual void setSettings(const QMap<QString, QVariant>& settings);

    //! Get the settings.
    virtual QMap<QString, QVariant> getSettings() const;

    //! Sets the settings. This function has to be implemented in a subclass in order to have an effect
    virtual void setSetting(const QString& key, const QVariant& value);

    //! Get the settings. This function has to be implemented in a subclass in order to have an effect
    virtual QVariant getSetting(const QString& key) const;

    //! Returns the list of keys of the property's settings
    virtual QStringList getSettingKeys() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = nullptr) const;

    Property propertyType() const;
    void     setPropertyType(const Property &type);

    virtual void UpdateParent(const QVariant &value);
public slots:
    virtual void ValueChildChanged(const QVariant &value, int typeForParent);
signals:
    void childChanged(const QVariant &value, int typeForParent);

protected:
    //! Protected constructor
    VProperty(VPropertyPrivate* d);

    //! The protected structure holding the member variables (to assure binary compatibility)
    VPropertyPrivate* d_ptr;

private:
    // Provide access functions for the d_ptr
    Q_DECLARE_PRIVATE(VProperty)
    Q_DISABLE_COPY(VProperty)
};

}

#endif // VPROPERTY_H

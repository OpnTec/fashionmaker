#ifndef VENUMPROPERTY_H
#define VENUMPROPERTY_H

#include "vproperty.h"

#include <QStringList>

namespace VPE{

class VPROPERTYEXPLORERSHARED_EXPORT VEnumProperty : public VProperty
{
public:
    //! Constructor
    VEnumProperty(const QString& name);

    //! Destructor
    ~VEnumProperty() {}

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    //! Gets the data from the widget
    virtual QVariant getEditorData(QWidget* editor) const;

    //! Sets the enumeration literals
    virtual void setLiterals(const QStringList &literals);

    //! Get the settings. This function has to be implemented in a subclass in order to have an effect
    virtual QStringList getLiterals() const;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value);

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = nullptr) const;

    //! Sets the settings. Available settings:
    //!
    //! key: "literals" - value: "item1;;item2;;item3"
    virtual void setSetting(const QString& key, const QVariant& value);

    //! Get the settings. This function has to be implemented in a subclass in order to have an effect
    virtual QVariant getSetting(const QString& key) const;

    //! Returns the list of keys of the property's settings
    virtual QStringList getSettingKeys() const;

protected:
    //! The list of possible options to choose frome
    QStringList EnumerationLiterals;
    // No use of d-pointer in this case, because it is unlikely this will change. If it does, we can still add other members by reimplementing the VPropertyPrivate class without touching this header file.
};

}

#endif // VENUMPROPERTY_H

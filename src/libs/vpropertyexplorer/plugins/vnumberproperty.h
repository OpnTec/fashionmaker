#ifndef VNUMBERPROPERTY_H
#define VNUMBERPROPERTY_H

#include "vpropertyexplorer_global.h"
#include "vproperty.h"

namespace VPE {


//! Class for holding an integer property
class VPROPERTYEXPLORERSHARED_EXPORT VIntegerProperty : public VProperty
{
public:
    VIntegerProperty(const QString& name, const QMap<QString, QVariant>& settings);

    VIntegerProperty(const QString& name);

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    //! Gets the data from the widget
    virtual QVariant getEditorData(QWidget* editor) const;

    //! Sets the settings of a basic integer property
    //! \param minimum The minimum value
    //! \param maxiumum The maximum value
    virtual void setSettings(int minimum, int maxiumum);

    //! Sets the settings. Available settings:
    //!
    //! key: "Min" - value: Minimum number as integer
    //! key: "Max" - value: Maximum number as integer
    virtual void setSetting(const QString& key, const QVariant& value);

    //! Get the settings. This function has to be implemented in a subclass in order to have an effect
    virtual QVariant getSetting(const QString& key) const;

    //! Returns the list of keys of the property's settings
    virtual QStringList getSettingKeys() const;

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = NULL) const;

protected:
    int Min, Max;

    static const int StandardMin;// = -1000000;
    static const int StandardMax;// = 1000000;
};


//! Class for holding a double property
class VPROPERTYEXPLORERSHARED_EXPORT VDoubleProperty : public VIntegerProperty
{
public:
    VDoubleProperty(const QString& name, const QMap<QString, QVariant>& settings);

    VDoubleProperty(const QString& name);

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    //! Gets the data from the widget
    virtual QVariant getEditorData(QWidget* editor) const;

    //! Sets the settings of a double property
    //! \param minimum The minimum value
    //! \param maxiumum The maximum value
    //! \param precision The number of decimal places
    virtual void setSettings(double minimum, double maxiumum, int precision);

    //! Sets the settings. Available settings:
    //!
    //! key: "Min" - value: Minimum number as integer
    //! key: "Max" - value: Maximum number as integer
    virtual void setSetting(const QString& key, const QVariant& value);

    //! Get the settings. This function has to be implemented in a subclass in order to have an effect
    virtual QVariant getSetting(const QString& key) const;

    //! Returns the list of keys of the property's settings
    virtual QStringList getSettingKeys() const;

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = NULL) const;

protected:
    //! Number of decimals after the decimal point
    int Precision;

    const static double StandardPrecision;// = 5;
};


}

#endif // VNUMBERPROPERTY_H

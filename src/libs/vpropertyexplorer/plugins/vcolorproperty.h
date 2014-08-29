#ifndef VCOLORPROPERTY_H
#define VCOLORPROPERTY_H

#include "vpropertyexplorer_global.h"

#include "vproperty.h"

namespace VPE {

class VPROPERTYEXPLORERSHARED_EXPORT VColorProperty : public VProperty
{

public:
    VColorProperty(const QString &name);

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    //! Sets the property's data to the editor (returns false, if the standard delegate should do that)
    virtual bool setEditorData(QWidget* editor);

    //! Gets the data from the widget
    virtual QVariant getEditorData(QWidget* editor) const;

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = NULL) const;

};

}

#endif // VCOLORPROPERTY_H

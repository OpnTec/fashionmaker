#include "vcolorproperty.h"
#include "vcolorpropertyeditor.h"

#include "vproperty_p.h"

using namespace VPE;

QColorProperty::QColorProperty(const QString &name) :
    VProperty(name, QVariant::Color)
{
}


//! Get the data how it should be displayed
QVariant QColorProperty::data (int column, int role) const
{
    if(column == DPC_Data && (Qt::DisplayRole == role))
        return VColorPropertyEditor::getColorString(d_ptr->VariantValue.value<QColor>());
    else if(Qt::EditRole == role)
        return QVariant();
    else if(column == DPC_Data && (Qt::DecorationRole == role))
        return VColorPropertyEditor::getColorPixmap(d_ptr->VariantValue.value<QColor>());
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* QColorProperty::createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    VColorPropertyEditor* tmpWidget = new VColorPropertyEditor(parent);
    tmpWidget->setColor(d_ptr->VariantValue.value<QColor>());
    return tmpWidget;
}

//! Sets the property's data to the editor (returns false, if the standard delegate should do that)
bool QColorProperty::setEditorData(QWidget* editor)
{
    VColorPropertyEditor* tmpWidget = qobject_cast<VColorPropertyEditor*>(editor);
    if(tmpWidget)
        tmpWidget->setColor(d_ptr->VariantValue.value<QColor>());
    else
        return false;

    return true;
}

//! Gets the data from the widget
QVariant QColorProperty::getEditorData(QWidget* editor) const
{
    VColorPropertyEditor* tmpWidget = qobject_cast<VColorPropertyEditor*>(editor);
    if(tmpWidget)
        return tmpWidget->getColor();

    return QVariant();
}

QString QColorProperty::type() const
{
    return "color";
}

VProperty *QColorProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new QColorProperty(getName()));
}

/************************************************************************
 **
 **  @file   vcolorproperty.cpp
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

#include "vcolorproperty.h"
#include "vcolorpropertyeditor.h"

#include "vproperty_p.h"

using namespace VPE;

VColorProperty::VColorProperty(const QString &name) :
    VProperty(name, QVariant::Color)
{
}


//! Get the data how it should be displayed
QVariant VColorProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role))
    {
        return VColorPropertyEditor::getColorString(d_ptr->VariantValue.value<QColor>());
    }
    else if (Qt::EditRole == role)
    {
        return QVariant();
    }
    else if (column == DPC_Data && (Qt::DecorationRole == role))
    {
        return VColorPropertyEditor::getColorPixmap(d_ptr->VariantValue.value<QColor>());
    }
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VColorProperty::createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                      const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    VColorPropertyEditor* tmpWidget = new VColorPropertyEditor(parent);
    tmpWidget->setColor(d_ptr->VariantValue.value<QColor>());
    return tmpWidget;
}

//! Sets the property's data to the editor (returns false, if the standard delegate should do that)
bool VColorProperty::setEditorData(QWidget* editor)
{
    VColorPropertyEditor* tmpWidget = qobject_cast<VColorPropertyEditor*>(editor);
    if (tmpWidget)
    {
        tmpWidget->setColor(d_ptr->VariantValue.value<QColor>());
    }
    else
        return false;

    return true;
}

//! Gets the data from the widget
QVariant VColorProperty::getEditorData(QWidget* editor) const
{
    VColorPropertyEditor* tmpWidget = qobject_cast<VColorPropertyEditor*>(editor);
    if (tmpWidget)
    {
        return tmpWidget->getColor();
    }

    return QVariant();
}

QString VColorProperty::type() const
{
    return "color";
}

VProperty *VColorProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VColorProperty(getName()));
}

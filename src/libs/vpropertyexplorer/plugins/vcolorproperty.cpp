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

#include <QColor>
#include <QLocale>
#include <QPixmap>
#include <QWidget>

#include "../vproperty_p.h"
#include "vcolorpropertyeditor.h"

class QAbstractItemDelegate;
class QStyleOptionViewItem;

VPE::VColorProperty::VColorProperty(const QString &name) :
    VProperty(name, QVariant::Color)
{
}


//! Get the data how it should be displayed
QVariant VPE::VColorProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role))
    {
        return VColorPropertyEditor::GetColorString(d_ptr->VariantValue.value<QColor>());
    }
    else if (Qt::EditRole == role)
    {
        return QVariant();
    }
    else if (column == DPC_Data && (Qt::DecorationRole == role))
    {
        return VColorPropertyEditor::GetColorPixmap(d_ptr->VariantValue.value<QColor>());
    }
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VPE::VColorProperty::createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                           const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options)
    Q_UNUSED(delegate)

    VColorPropertyEditor* tmpWidget = new VColorPropertyEditor(parent);
    tmpWidget->setLocale(parent->locale());
    tmpWidget->SetColor(d_ptr->VariantValue.value<QColor>());
    return tmpWidget;
}

//! Sets the property's data to the editor (returns false, if the standard delegate should do that)
bool VPE::VColorProperty::setEditorData(QWidget* editor)
{
    VColorPropertyEditor* tmpWidget = qobject_cast<VColorPropertyEditor*>(editor);
    if (tmpWidget)
    {
        tmpWidget->SetColor(d_ptr->VariantValue.value<QColor>());
    }
    else
        return false;

    return true;
}

//! Gets the data from the widget
QVariant VPE::VColorProperty::getEditorData(const QWidget *editor) const
{
    const VColorPropertyEditor* tmpWidget = qobject_cast<const VColorPropertyEditor*>(editor);
    if (tmpWidget)
    {
        return tmpWidget->GetColor();
    }

    return QVariant();
}

QString VPE::VColorProperty::type() const
{
    return "color";
}

VPE::VProperty *VPE::VColorProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VColorProperty(getName()));
}

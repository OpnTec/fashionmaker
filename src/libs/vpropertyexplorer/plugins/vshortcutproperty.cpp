/************************************************************************
 **
 **  @file   vshortcutproperty.cpp
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

#include "vshortcutproperty.h"

#include <QAbstractItemDelegate>
#include <QKeySequence>
#include <QLocale>
#include <QWidget>

#include "../vproperty_p.h"
#include "vshortcutpropertyeditor.h"

class QStyleOptionViewItem;

VPE::VShortcutProperty::VShortcutProperty(const QString& name)
    : VProperty(name, QVariant::String)
{

}

VPE::VShortcutProperty::~VShortcutProperty()
{
    //
}

QVariant VPE::VShortcutProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        return d_ptr->VariantValue;
    }
    else
        return VProperty::data(column, role);
}


QWidget* VPE::VShortcutProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                              const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options)

    VShortcutEditWidget* tmpWidget = new VShortcutEditWidget(parent);
    if (delegate)
    {
        VShortcutEditWidget::connect(tmpWidget, SIGNAL(commitData(QWidget*)), delegate, SIGNAL(commitData(QWidget*)));
    }
    tmpWidget->setLocale(parent->locale());
    return tmpWidget;
}


bool VPE::VShortcutProperty::setEditorData(QWidget* editor)
{
    VShortcutEditWidget* tmpWidget = qobject_cast<VShortcutEditWidget*>(editor);
    if (tmpWidget)
    {
        tmpWidget->setShortcut(d_ptr->VariantValue.toString(), false);
    }
    else
        return false;

    return true;
}


QVariant VPE::VShortcutProperty::getEditorData(const QWidget *editor) const
{
    const VShortcutEditWidget* tmpWidget = qobject_cast<const VShortcutEditWidget*>(editor);
    if (tmpWidget)
    {
        return tmpWidget->getShortcutAsString();
    }

    return QVariant();
}


QString VPE::VShortcutProperty::type() const
{
    return "shortcut";
}

VPE::VProperty* VPE::VShortcutProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VShortcutProperty(getName()));
}

void VPE::VShortcutProperty::setValue(const QVariant &value)
{
    VProperty::setValue(QKeySequence::fromString(value.toString()).toString());
}

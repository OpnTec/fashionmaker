/************************************************************************
 **
 **  @file   venumproperty.cpp
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

#include "venumproperty.h"
#include "../vproperty_p.h"

#include <QComboBox>
#include <QCoreApplication>

using namespace VPE;

VEnumProperty::VEnumProperty(const QString& name)
    : VProperty(name, QVariant::Int)
{
    VProperty::d_ptr->VariantValue = 0;
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);
}


//! Get the data how it should be displayed
QVariant VEnumProperty::data (int column, int role) const
{
    if (EnumerationLiterals.empty())
    {
        return QVariant();
    }

    int tmpIndex = VProperty::d_ptr->VariantValue.toInt();

    if (tmpIndex < 0 || tmpIndex >= EnumerationLiterals.count())
    {
        tmpIndex = 0;
    }

    if (column == DPC_Data && Qt::DisplayRole == role)
    {
        return EnumerationLiterals.at(tmpIndex);
    }
    else if (column == DPC_Data && Qt::EditRole == role)
    {
        return tmpIndex;
    }
    else
        return VProperty::data(column, role);
}


//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VEnumProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                     const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QComboBox* tmpEditor = new QComboBox(parent);
    tmpEditor->clear();
    tmpEditor->addItems(EnumerationLiterals);
    tmpEditor->setCurrentIndex(VProperty::d_ptr->VariantValue.toInt());
    connect(tmpEditor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                     &VEnumProperty::currentIndexChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

//! Gets the data from the widget
QVariant VEnumProperty::getEditorData(const QWidget *editor) const
{
    const QComboBox* tmpEditor = qobject_cast<const QComboBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->currentIndex();
    }

    return QVariant(0);
}

//! Sets the enumeration literals
void VEnumProperty::setLiterals(const QStringList& literals)
{
    EnumerationLiterals = literals;
}

//! Get the settings. This function has to be implemented in a subclass in order to have an effect
QStringList VEnumProperty::getLiterals() const
{
    return EnumerationLiterals;
}

//! Sets the value of the property
void VEnumProperty::setValue(const QVariant& value)
{
    int tmpIndex = value.toInt();

    if (tmpIndex < 0 || tmpIndex >= EnumerationLiterals.count())
    {
        tmpIndex = 0;
    }

    VProperty::d_ptr->VariantValue = tmpIndex;
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);

    if (VProperty::d_ptr->editor != nullptr)
    {
        setEditorData(VProperty::d_ptr->editor);
    }
}

QString VEnumProperty::type() const
{
    return "enum";
}

VProperty* VEnumProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VEnumProperty(getName()));
}

void VEnumProperty::setSetting(const QString& key, const QVariant& value)
{
    if (key == "literals")
    {
        setLiterals(value.toString().split(";;"));
    }
}

QVariant VEnumProperty::getSetting(const QString& key) const
{
    if (key == "literals")
    {
        return getLiterals().join(";;");
    }
    else
        return VProperty::getSetting(key);
}

QStringList VEnumProperty::getSettingKeys() const
{
    return QStringList("literals");
}

void VEnumProperty::currentIndexChanged(int index)
{
    Q_UNUSED(index)
    UserChangeEvent *event = new UserChangeEvent();
    QCoreApplication::postEvent ( VProperty::d_ptr->editor, event );
}

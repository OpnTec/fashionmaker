/************************************************************************
 **
 **  @file   vobjectproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
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

#include "vobjectproperty.h"
#include "../vproperty_p.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>

using namespace VPE;

VObjectProperty::VObjectProperty(const QString& name)
    : VProperty(name, QVariant::Int), objects()
{
    VProperty::d_ptr->VariantValue = 0;
    VProperty::d_ptr->VariantValue.convert(QVariant::UInt);
}

//! Get the data how it should be displayed
QVariant VObjectProperty::data (int column, int role) const
{
    if (objects.empty())
    {
        return QVariant();
    }

    QComboBox* tmpEditor = qobject_cast<QComboBox*>(VProperty::d_ptr->editor);

    if (column == DPC_Data && Qt::DisplayRole == role)
    {
        return VProperty::d_ptr->VariantValue;
    }
    else if (column == DPC_Data && Qt::EditRole == role)
    {
        return tmpEditor->currentIndex();
    }
    else
        return VProperty::data(column, role);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VObjectProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                       const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QComboBox* tmpEditor = new QComboBox(parent);
    tmpEditor->clear();
    tmpEditor->setLocale(parent->locale());
    FillList(tmpEditor, objects);
    tmpEditor->setCurrentIndex(tmpEditor->findData(VProperty::d_ptr->VariantValue.toUInt()));
    connect(tmpEditor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                     &VObjectProperty::currentIndexChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

bool VObjectProperty::setEditorData(QWidget *editor)
{
    if (!editor)
    {
        return false;
    }

    QComboBox* tmpEditor = qobject_cast<QComboBox*>(editor);
    if (tmpEditor)
    {
        quint32 objId = VProperty::d_ptr->VariantValue.toUInt();
        qint32 tmpIndex = tmpEditor->findData(objId);

        if (tmpIndex == -1)
        {
            tmpIndex = 0;
        }
        tmpEditor->blockSignals(true);
        tmpEditor->setCurrentIndex(tmpIndex);
        tmpEditor->blockSignals(false);
        return true;
    }

    return false;
}

//! Gets the data from the widget
QVariant VObjectProperty::getEditorData(const QWidget *editor) const
{
    const QComboBox* tmpEditor = qobject_cast<const QComboBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->itemData(tmpEditor->currentIndex());
    }

    return QVariant(0);
}

//! Sets the objects list
// cppcheck-suppress unusedFunction
void VObjectProperty::setObjectsList(const QMap<QString, quint32> &objects)
{
    this->objects = objects;
}

//! Get the settings. This function has to be implemented in a subclass in order to have an effect
// cppcheck-suppress unusedFunction
QMap<QString, quint32> VObjectProperty::getObjects() const
{
    return objects;
}

//! Sets the value of the property
void VObjectProperty::setValue(const QVariant& value)
{
    VProperty::d_ptr->VariantValue = value;
    VProperty::d_ptr->VariantValue.convert(QVariant::UInt);

    if (VProperty::d_ptr->editor != nullptr)
    {
        setEditorData(VProperty::d_ptr->editor);
    }
}

QString VObjectProperty::type() const
{
    return "objectList";
}

VProperty* VObjectProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VObjectProperty(getName()));
}

void VObjectProperty::currentIndexChanged(int index)
{
    Q_UNUSED(index)
    UserChangeEvent *event = new UserChangeEvent();
    QCoreApplication::postEvent ( VProperty::d_ptr->editor, event );
}

void VObjectProperty::FillList(QComboBox *box, const QMap<QString, quint32> &list) const
{
    box->clear();

    QMap<QString, quint32>::const_iterator i;
    for (i = list.constBegin(); i != list.constEnd(); ++i)
    {
        box->addItem(i.key(), i.value());
    }
}

/************************************************************************
 **
 **  @file   vstringproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 8, 2014
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

#include "vstringproperty.h"

#include <QLineEdit>
#include <QSizePolicy>

#include "vproperty_p.h"

using namespace VPE;


VPE::VStringProperty::VStringProperty(const QString &name, const QMap<QString, QVariant> &settings)
    : VProperty(name, QVariant::String), readOnly(false)
{
    VProperty::setSettings(settings);
    d_ptr->VariantValue.setValue(QStringLiteral(""));
    d_ptr->VariantValue.convert(QVariant::String);
}

VPE::VStringProperty::VStringProperty(const QString &name)
    : VProperty(name), readOnly(false)
{
    d_ptr->VariantValue.setValue(QStringLiteral(""));
    d_ptr->VariantValue.convert(QVariant::String);
}

QWidget *VPE::VStringProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &options,
                                            const QAbstractItemDelegate *delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QLineEdit* tmpEditor = new QLineEdit(parent);
    tmpEditor->setReadOnly(readOnly);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setText(d_ptr->VariantValue.toString());

    d_ptr->editor = tmpEditor;
    return d_ptr->editor;
}

QVariant VPE::VStringProperty::getEditorData(QWidget *editor) const
{
    QLineEdit* tmpEditor = qobject_cast<QLineEdit*>(editor);
    if(tmpEditor)
        return tmpEditor->text();

    return QVariant(QStringLiteral(""));
}

void VPE::VStringProperty::setReadOnly(bool readOnly)
{
    this->readOnly = readOnly;
}

void VPE::VStringProperty::setSetting(const QString &key, const QVariant &value)
{
    if(key == "ReadOnly")
        setReadOnly(value.toBool());
}

QVariant VPE::VStringProperty::getSetting(const QString &key) const
{
    if(key == "ReadOnly")
        return readOnly;
    else
        return VProperty::getSetting(key);
}

QStringList VPE::VStringProperty::getSettingKeys() const
{
    return QStringList("ReadOnly");
}

QString VPE::VStringProperty::type() const
{
    return "string";
}

VPE::VProperty *VPE::VStringProperty::clone(bool include_children, VPE::VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VStringProperty(getName()));
}

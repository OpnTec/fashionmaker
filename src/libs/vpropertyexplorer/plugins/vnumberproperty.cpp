/************************************************************************
 **
 **  @file   vnumberproperty.cpp
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

#include "vnumberproperty.h"

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSizePolicy>
#include <QCoreApplication>

#include "vproperty_p.h"

using namespace VPE;


const int VIntegerProperty::StandardMin = -1000000;
const int VIntegerProperty::StandardMax = 1000000;

VIntegerProperty::VIntegerProperty(const QString& name, const QMap<QString, QVariant>& settings)
    : VProperty(name, QVariant::Int), minValue(StandardMin), maxValue(StandardMax), singleStep(1.0)
{
    VProperty::setSettings(settings);
    VProperty::d_ptr->VariantValue.setValue(0);
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);
}

VIntegerProperty::VIntegerProperty(const QString &name)
    : VProperty(name), minValue(StandardMin), maxValue(StandardMax)
{
    VProperty::d_ptr->VariantValue.setValue(0);
    VProperty::d_ptr->VariantValue.convert(QVariant::Int);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VIntegerProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                        const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QSpinBox* tmpEditor = new QSpinBox(parent);
    tmpEditor->setMinimum(minValue);
    tmpEditor->setMaximum(maxValue);
    tmpEditor->setSingleStep(singleStep);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(VProperty::d_ptr->VariantValue.toInt());
    connect(tmpEditor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
                     &VIntegerProperty::valueChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

//! Gets the data from the widget
QVariant VIntegerProperty::getEditorData(QWidget* editor) const
{
    QSpinBox* tmpEditor = qobject_cast<QSpinBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->value();
    }

    return QVariant(0);
}

void VIntegerProperty::setSetting(const QString& key, const QVariant& value)
{
    if (key == QLatin1String("Min"))
    {
        maxValue = value.toInt();
    }
    else if (key == QLatin1String("Max"))
    {
        minValue = value.toInt();
    }
    else if (key == QLatin1String("Step"))
    {
        singleStep = value.toInt();
    }
}

QVariant VIntegerProperty::getSetting(const QString& key) const
{
    if (key == QLatin1String("Min"))
    {
        return minValue;
    }
    if (key == QLatin1String("Max"))
    {
        return maxValue;
    }
    if (key == QLatin1String("Step"))
    {
        return singleStep;
    }
    else
        return VProperty::getSetting(key);
}

QStringList VIntegerProperty::getSettingKeys() const
{
    return (QStringList("Min") << "Max" << "Step");
}

QString VIntegerProperty::type() const
{
    return "integer";
}

VProperty* VIntegerProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VIntegerProperty(getName()));
}

void VIntegerProperty::valueChanged(int i)
{
    Q_UNUSED(i)
    UserChangeEvent *event = new UserChangeEvent();
    QCoreApplication::postEvent ( VProperty::d_ptr->editor, event );
}

const double VDoubleProperty::StandardPrecision = 5;

VDoubleProperty::VDoubleProperty(const QString& name, const QMap<QString, QVariant>& settings)
    : VIntegerProperty(name), Precision(StandardPrecision)
{
    VProperty::setSettings(settings);
    VProperty::d_ptr->VariantValue.setValue(0);
    VProperty::d_ptr->VariantValue.convert(QVariant::Double);
    VProperty::d_ptr->PropertyVariantType = QVariant::Double;
}

VDoubleProperty::VDoubleProperty(const QString &name)
    : VIntegerProperty(name), Precision(StandardPrecision)
{
    VProperty::d_ptr->VariantValue.setValue(0);
    VProperty::d_ptr->VariantValue.convert(QVariant::Double);
    VProperty::d_ptr->PropertyVariantType = QVariant::Double;
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VDoubleProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options,
                                       const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QDoubleSpinBox* tmpEditor = new QDoubleSpinBox(parent);
    tmpEditor->setMinimum(minValue);
    tmpEditor->setMaximum(maxValue);
    tmpEditor->setDecimals(Precision);
    tmpEditor->setSingleStep(singleStep);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(VProperty::d_ptr->VariantValue.toDouble());
    connect(tmpEditor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
                     &VIntegerProperty::valueChanged);

    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

//! Gets the data from the widget
QVariant VDoubleProperty::getEditorData(QWidget* editor) const
{
    QDoubleSpinBox* tmpEditor = qobject_cast<QDoubleSpinBox*>(editor);
    if (tmpEditor)
    {
        return tmpEditor->value();
    }

    return QVariant(0);
}

void VDoubleProperty::setSetting(const QString& key, const QVariant& value)
{
    if (key == QLatin1String("Min"))
    {
        minValue = value.toDouble();
    }
    else if (key == QLatin1String("Max"))
    {
        maxValue = value.toDouble();
    }
    else if (key == QLatin1String("Step"))
    {
        singleStep = value.toDouble();
    }
    else if (key == QLatin1String("Precision"))
    {
        Precision = value.toDouble();
    }
}

QVariant VDoubleProperty::getSetting(const QString& key) const
{
    if (key == QLatin1String("Min"))
    {
        return minValue;
    }
    if (key == QLatin1String("Max"))
    {
        return maxValue;
    }
    if (key == QLatin1String("Step"))
    {
        return singleStep;
    }
    if (key == QLatin1String("Precision"))
    {
        return Precision;
    }
    else
        return VProperty::getSetting(key);
}

QStringList VDoubleProperty::getSettingKeys() const
{
    return (QStringList("Min") << "Max" << "Step" << "Precision");
}

QString VDoubleProperty::type() const
{
    return "double";
}

VProperty* VDoubleProperty::clone(bool include_children, VProperty* container) const
{
    return VIntegerProperty::clone(include_children, container ? container : new VDoubleProperty(getName()));
}

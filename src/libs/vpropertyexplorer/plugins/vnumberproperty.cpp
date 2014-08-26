#include "vnumberproperty.h"

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSizePolicy>

#include "vproperty_p.h"

using namespace VPE;


const int VIntegerProperty::StandardMin = -1000000;
const int VIntegerProperty::StandardMax = 1000000;

VIntegerProperty::VIntegerProperty(const QString& name, const QMap<QString, QVariant>& settings)
    : VProperty(name, QVariant::Int), Min(StandardMin), Max(StandardMax)
{
    VProperty::setSettings(settings);
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::Int);
}

VIntegerProperty::VIntegerProperty(const QString &name)
    : VProperty(name), Min(StandardMin), Max(StandardMax)
{
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::Int);
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VIntegerProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    QSpinBox* tmpEditor = new QSpinBox(parent);
    tmpEditor->setMinimum(Min);
    tmpEditor->setMaximum(Max);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(d_ptr->VariantValue.toInt());

    d_ptr->editor = tmpEditor;
    return d_ptr->editor;
}

//! Gets the data from the widget
QVariant VIntegerProperty::getEditorData(QWidget* editor) const
{
    QSpinBox* tmpEditor = qobject_cast<QSpinBox*>(editor);
    if(tmpEditor)
        return tmpEditor->value();

    return QVariant(0);
}

void VIntegerProperty::setSettings(int minimum, int maxiumum)
{
    Min = minimum;
    Max = maxiumum;
}

void VIntegerProperty::setSetting(const QString& key, const QVariant& value)
{
    if(key == "Min")
        setSettings(value.toInt(), Max);
    else if(key == "Max")
        setSettings(Min, value.toInt());
}

QVariant VIntegerProperty::getSetting(const QString& key) const
{
    if(key == "Min")
        return Min;
    if(key == "Max")
        return Max;
    else
        return VProperty::getSetting(key);
}

QStringList VIntegerProperty::getSettingKeys() const
{
    return (QStringList("Min") << "Max");
}

QString VIntegerProperty::type() const
{
    return "integer";
}

VProperty* VIntegerProperty::clone(bool include_children, VProperty* container) const
{
    return VProperty::clone(include_children, container ? container : new VIntegerProperty(getName()));
}





const double VDoubleProperty::StandardPrecision = 5;

VDoubleProperty::VDoubleProperty(const QString& name, const QMap<QString, QVariant>& settings)
    : VIntegerProperty(name), Precision(StandardPrecision)
{
    VProperty::setSettings(settings);
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::Double);
    d_ptr->PropertyVariantType = QVariant::Double;
}

VDoubleProperty::VDoubleProperty(const QString &name)
    : VIntegerProperty(name), Precision(StandardPrecision)
{
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::Double);
    d_ptr->PropertyVariantType = QVariant::Double;
}

//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VDoubleProperty::createEditor(QWidget * parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);
    QDoubleSpinBox* tmpEditor = new QDoubleSpinBox(parent);
    tmpEditor->setMinimum(Min);
    tmpEditor->setMaximum(Max);
    tmpEditor->setDecimals(Precision);
    tmpEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tmpEditor->setValue(d_ptr->VariantValue.toDouble());

    d_ptr->editor = tmpEditor;
    return d_ptr->editor;
}

//! Gets the data from the widget
QVariant VDoubleProperty::getEditorData(QWidget* editor) const
{
    QDoubleSpinBox* tmpEditor = qobject_cast<QDoubleSpinBox*>(editor);
    if(tmpEditor)
        return tmpEditor->value();

    return QVariant(0);
}

void VDoubleProperty::setSettings(double minimum, double maxiumum, int precision)
{
    VIntegerProperty::setSettings(minimum, maxiumum);
    Precision = precision;
}

void VDoubleProperty::setSetting(const QString& key, const QVariant& value)
{
    if(key == "Min")
        setSettings(value.toDouble(), Max, Precision);
    else if(key == "Max")
        setSettings(Min, value.toDouble(), Precision);
    else if(key == "Precision")
        setSettings(Min, Max, value.toDouble());
}

QVariant VDoubleProperty::getSetting(const QString& key) const
{
    if(key == "Min")
        return Min;
    if(key == "Max")
        return Max;
    if(key == "Precision")
        return Precision;
    else
        return VProperty::getSetting(key);
}

QStringList VDoubleProperty::getSettingKeys() const
{
    return (QStringList("Min") << "Max" << "Precision");
}

QString VDoubleProperty::type() const
{
    return "double";
}

VProperty* VDoubleProperty::clone(bool include_children, VProperty* container) const
{
    return VIntegerProperty::clone(include_children, container ? container : new VDoubleProperty(getName()));
}

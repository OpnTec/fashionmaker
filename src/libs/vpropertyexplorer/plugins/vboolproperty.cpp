#include "vboolproperty.h"

#include <QCheckBox>
#include <QApplication>
#include <QStyle>
#include <QSizePolicy>
#include <QObject>
#include <QSpinBox>

#include "vproperty_p.h"

using namespace VPE;

QVariant VBoolProperty::TrueText;
QVariant VBoolProperty::FalseText;

VBoolProperty::VBoolProperty(const QString& name) :
    VProperty(name, QVariant::Bool)
{
    d_ptr->VariantValue.setValue(false);
    d_ptr->VariantValue.convert(QVariant::Bool);

    // I'm not sure, how Qt handles the translations...
    if(TrueText.isNull()) TrueText = QObject::tr("True");
    if(TrueText.isNull()) FalseText = QObject::tr("False");
}


//! Get the data how it should be displayed
QVariant VBoolProperty::data (int column, int role) const
{
    if(column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
        return d_ptr->VariantValue.toBool() ? TrueText : FalseText;
    if(column == DPC_Data && Qt::CheckStateRole == role)
        return d_ptr->VariantValue.toBool() ? Qt::Checked : Qt::Unchecked;
    else
        return VProperty::data(column, role);
}

bool VBoolProperty::setData(const QVariant &data, int role)
{
    if(Qt::CheckStateRole == role)
    {
        d_ptr->VariantValue = (Qt::Checked == static_cast<Qt::CheckState>(data.toInt())); return true;
    }

    return false;
}

//! Returns item flags
Qt::ItemFlags VBoolProperty::flags(int column) const
{
    if(column == DPC_Data)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return VProperty::flags(column);
}

QString VBoolProperty::type() const
{
    return "bool";
}

VProperty *VBoolProperty::clone(bool include_children, VProperty *container) const
{
    return VProperty::clone(include_children, container ? container : new VBoolProperty(getName()));
}

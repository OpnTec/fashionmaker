/************************************************************************
 **
 **  @file   vformulaproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vformulaproperty.h"
#include "vformulapropertyeditor.h"

#include "../vpropertyexplorer/vproperty_p.h"
#include "vformulapropertyeditor.h"
#include "../vpropertyexplorer/vproperties.h"
#include "../vpatterndb/vformula.h"
#include "../vmisc/vabstractapplication.h"

enum class ChildType : char {Invalid = 0, Value = 1, Formula = 2};

using namespace VPE;

//---------------------------------------------------------------------------------------------------------------------
VFormulaProperty::VFormulaProperty(const QString &name)
    : VProperty(name, static_cast<QVariant::Type>(VFormula::FormulaTypeId()))
{
    d_ptr->type = Property::Complex;

    VStringProperty* tmpValue = new VStringProperty(tr("Value"));
    addChild(tmpValue);
    tmpValue->setUpdateBehaviour(true, false);
    tmpValue->setReadOnly(true);
    tmpValue->setOsSeparator(qApp->Settings()->GetOsSeparator());
    tmpValue->setTypeForParent(static_cast<int>(ChildType::Value));

    VStringProperty* tmpFormula = new VStringProperty(tr("Formula"));
    addChild(tmpFormula);
    tmpFormula->setClearButtonEnable(true);
    tmpFormula->setUpdateBehaviour(true, false);
    tmpFormula->setOsSeparator(qApp->Settings()->GetOsSeparator());
    tmpFormula->setTypeForParent(static_cast<int>(ChildType::Formula));

    setValue(0);
}

//---------------------------------------------------------------------------------------------------------------------
//! Get the data how it should be displayed
QVariant VFormulaProperty::data (int column, int role) const
{
    if (column == DPC_Data && (Qt::DisplayRole == role || Qt::EditRole == role))
    {
        return getValue();
    }
    else
    {
        return VProperty::data(column, role);
    }
}

//---------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags VFormulaProperty::flags(int column) const
{
    if (column == DPC_Name || column == DPC_Data)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::NoItemFlags;
    }
}

//---------------------------------------------------------------------------------------------------------------------
//! Returns an editor widget, or NULL if it doesn't supply one
QWidget* VFormulaProperty::createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                        const QAbstractItemDelegate* delegate)
{
    Q_UNUSED(options);
    Q_UNUSED(delegate);

    VFormula formula = VProperty::d_ptr->VariantValue.value<VFormula>();
    VFormulaPropertyEditor* tmpEditor = new VFormulaPropertyEditor(parent);
    tmpEditor->setLocale(parent->locale());
    tmpEditor->SetFormula(formula);
    VProperty::d_ptr->editor = tmpEditor;
    return VProperty::d_ptr->editor;
}

//---------------------------------------------------------------------------------------------------------------------
//! Sets the property's data to the editor (returns false, if the standard delegate should do that)
bool VFormulaProperty::setEditorData(QWidget* editor)
{
    VFormulaPropertyEditor* tmpWidget = qobject_cast<VFormulaPropertyEditor*>(editor);
    if (tmpWidget)
    {
        VFormula formula = VProperty::d_ptr->VariantValue.value<VFormula>();
        tmpWidget->SetFormula(formula);
    }
    else
        return false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
//! Gets the data from the widget
QVariant VFormulaProperty::getEditorData(const QWidget *editor) const
{
    const VFormulaPropertyEditor* tmpWidget = qobject_cast<const VFormulaPropertyEditor*>(editor);
    if (tmpWidget)
    {
        QVariant value;
        value.setValue(tmpWidget->GetFormula());
        return value;
    }

    return QVariant();
}

//---------------------------------------------------------------------------------------------------------------------
QString VFormulaProperty::type() const
{
    return "formula";
}

//---------------------------------------------------------------------------------------------------------------------
VProperty *VFormulaProperty::clone(bool include_children, VProperty *container) const
{
    if (!container)
    {
        container = new VFormulaProperty(getName());

        if (!include_children)
        {
            QList<VProperty*> tmpChildren = container->getChildren();
            foreach (VProperty* tmpChild, tmpChildren)
            {
                container->removeChild(tmpChild);
                delete tmpChild;
            }
        }
    }

    return VProperty::clone(false, container);  // Child

}

//---------------------------------------------------------------------------------------------------------------------
void VFormulaProperty::setValue(const QVariant &value)
{
    VFormula tmpFormula = value.value<VFormula>();
    SetFormula(tmpFormula);
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VFormulaProperty::getValue() const
{
    VFormula tmpFormula = GetFormula();
    QVariant value;
    value.setValue(tmpFormula);
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VFormulaProperty::GetFormula() const
{
    return VProperty::d_ptr->VariantValue.value<VFormula>();
}

//---------------------------------------------------------------------------------------------------------------------
void VFormulaProperty::SetFormula(const VFormula &formula)
{
    if (d_ptr->Children.count() < 2)
    {
        return;
    }

    QVariant value;
    value.setValue(formula);
    value.convert(VFormula::FormulaTypeId());
    VProperty::d_ptr->VariantValue = value;

    QVariant tmpValue(formula.getStringValue());
    tmpValue.convert(QVariant::String);

    QVariant tmpFormula(formula.GetFormula());
    tmpFormula.convert(QVariant::String);

    VProperty::d_ptr->Children.at(0)->setValue(tmpValue);
    VProperty::d_ptr->Children.at(1)->setValue(tmpFormula);

    if (VProperty::d_ptr->editor != nullptr)
    {
        setEditorData(VProperty::d_ptr->editor);
    }
}

void VFormulaProperty::ValueChildChanged(const QVariant &value, int typeForParent)
{
    if (typeForParent == static_cast<int>(ChildType::Formula))
    {
        VFormula newFormula = GetFormula();
        newFormula.SetFormula(value.toString(), FormulaType::FromUser);
        SetFormula(newFormula);
    }
}

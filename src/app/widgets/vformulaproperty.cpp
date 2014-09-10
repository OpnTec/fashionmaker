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
 **  Copyright (C) 2014 Valentina project
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

#include "../libs/vpropertyexplorer/vproperty_p.h"
#include "vformulapropertyeditor.h"
#include "../libs/vpropertyexplorer/vproperties.h"
#include "../container/vformula.h"

enum class ChildType : char {Invalid = 0, Value = 1, Formula = 2};

using namespace VPE;

//---------------------------------------------------------------------------------------------------------------------
VFormulaProperty::VFormulaProperty(const QString &name) :
    VProperty(name, static_cast<QVariant::Type>(VFormula::FormulaTypeId()))
{
    d_ptr->type = Property::Complex;

    VStringProperty* tmpValue = new VStringProperty(QStringLiteral("Value"));
    addChild(tmpValue);
    tmpValue->setUpdateBehaviour(true, false);
    tmpValue->setReadOnly(true);
    tmpValue->setTypeForParent(static_cast<int>(ChildType::Value));

    VStringProperty* tmpFormula = new VStringProperty(QStringLiteral("Formula"));
    addChild(tmpFormula);
    tmpFormula->setUpdateBehaviour(true, false);
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

    tmpEditor->setFormula(formula);
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
        tmpWidget->setFormula(formula);
    }
    else
        return false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
//! Gets the data from the widget
QVariant VFormulaProperty::getEditorData(QWidget* editor) const
{
    VFormulaPropertyEditor* tmpWidget = qobject_cast<VFormulaPropertyEditor*>(editor);
    if (tmpWidget)
    {
        QVariant value;
        value.setValue(tmpWidget->getFormula());
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
    setFormula(tmpFormula);
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VFormulaProperty::getValue() const
{
    VFormula tmpFormula = getFormula();
    QVariant value;
    value.setValue(tmpFormula);
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VFormulaProperty::getFormula() const
{
    return VProperty::d_ptr->VariantValue.value<VFormula>();
}

//---------------------------------------------------------------------------------------------------------------------
void VFormulaProperty::setFormula(const VFormula &formula)
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

    QVariant tmpFormula(formula.getFormula());
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
        VFormula newFormula = getFormula();
        newFormula.setFormula(value.toString());
        setFormula(newFormula);
    }
}

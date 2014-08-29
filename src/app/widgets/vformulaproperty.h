/************************************************************************
 **
 **  @file   vformulaproperty.h
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

#ifndef VFORMULAPROPERTY_H
#define VFORMULAPROPERTY_H

#include "../libs/vpropertyexplorer/vproperty.h"

class VFormula;

using namespace VPE;

class VFormulaProperty : public VProperty
{

public:
    VFormulaProperty(const QString &name);

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const;

    //! Returns item flags
    Qt::ItemFlags flags(int column = DPC_Name) const;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    //! Sets the property's data to the editor (returns false, if the standard delegate should do that)
    virtual bool setEditorData(QWidget* editor);

    //! Gets the data from the widget
    virtual QVariant getEditorData(QWidget* editor) const;

    //! Returns a string containing the type of the property
    virtual QString type() const;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = NULL) const;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value);

    //! Returns the value of the property as a QVariant
    virtual QVariant getValue() const;

    //! Returns the formula
    virtual VFormula getFormula() const;

    //! Sets the formula
    virtual void setFormula(const VFormula &formula);

public slots:
    virtual void ValueChildChanged(const QVariant &value, int typeForParent);

};

#endif // VFORMULAPROPERTY_H

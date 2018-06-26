/************************************************************************
 **
 **  @file   vformulaproperty.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VFORMULAPROPERTY_H
#define VFORMULAPROPERTY_H

#include "../vpropertyexplorer/vproperty.h"

class VFormula;

class VFormulaProperty : public VPE::VProperty
{
    Q_OBJECT
public:
    explicit VFormulaProperty(const QString &name);

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const override;

    //! Returns item flags
    virtual Qt::ItemFlags flags(int column = DPC_Name) const override;

    //! Returns an editor widget, or NULL if it doesn't supply one
    //! \param parent The widget to which the editor will be added as a child
    //! \options Render options
    //! \delegate A pointer to the QAbstractItemDelegate requesting the editor. This can be used to connect signals and
    //! slots.
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options,
                                  const QAbstractItemDelegate* delegate) override;

    //! Sets the property's data to the editor (returns false, if the standard delegate should do that)
    virtual bool setEditorData(QWidget* editor) override;

    //! Gets the data from the widget
    virtual QVariant getEditorData(const QWidget* editor) const override;

    //! Returns a string containing the type of the property
    virtual QString type() const override;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    Q_REQUIRED_RESULT virtual VProperty* clone(bool include_children = true,
                                               VProperty* container = nullptr) const override;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value) override;

    //! Returns the value of the property as a QVariant
    virtual QVariant getValue() const override;

    //! Returns the formula
    VFormula GetFormula() const;

    //! Sets the formula
    void SetFormula(const VFormula &formula);

public slots:
    virtual void ValueChildChanged(const QVariant &value, int typeForParent) override;

};

#endif // VFORMULAPROPERTY_H

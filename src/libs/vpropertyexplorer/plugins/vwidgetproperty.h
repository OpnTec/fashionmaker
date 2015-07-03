/************************************************************************
 **
 **  @file   vwidgetproperty.h
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

#ifndef VWIDGETROPERTY_H
#define VWIDGETROPERTY_H

#include "../vpropertyexplorer_global.h"

#include "vemptyproperty.h"

#include <QPointer>

namespace VPE
{

// todo: this way, this class doesn't really make sense. What we have to do is pass a widget factory instead of the
// actual widget!

//! This property holds a QWidget and displays it, if the view supports that. If not, it will behave like an empty
//! property
class VPROPERTYEXPLORERSHARED_EXPORT VWidgetProperty : public VEmptyProperty
{
public:
    //! Constructor
    VWidgetProperty(const QString &name, QWidget* widget = nullptr);

    //! The destructor
    virtual ~VWidgetProperty() Q_DECL_OVERRIDE;

    //! Returns the widget held by this property
    QWidget* getWidget() const;

    //! Sets the widget for this property. If there is already an old one, it will be deleted.
    void setWidget(QWidget* widget);

    //! Returns a string containing the type of the property
    virtual QString type() const Q_DECL_OVERRIDE;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = nullptr) const Q_DECL_OVERRIDE;
};

}

#endif // VWIDGETROPERTY_H

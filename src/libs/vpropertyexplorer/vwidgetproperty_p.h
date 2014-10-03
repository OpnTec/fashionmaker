/************************************************************************
 **
 **  @file   vwidgetproperty_p.h
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

#ifndef VWIDGETPROPERTY_P_H
#define VWIDGETPROPERTY_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vproperty_p.h"

#include <QPointer>
#include <QWidget>

namespace VPE
{

#ifdef Q_CC_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wweak-vtables"
#endif

class VWidgetPropertyPrivate : public VPropertyPrivate
{
public:
    //! The widget to show
    QPointer<QWidget> Widget;

    //! Constructor passing name and type
    VWidgetPropertyPrivate(const QString& name, QVariant::Type type, QWidget* widget = nullptr)
        : VPropertyPrivate(name, type), Widget(widget) {}

    //! Constructor
    VWidgetPropertyPrivate()
        : VPropertyPrivate(), Widget(nullptr) {}

    //! Destructor
    virtual ~VWidgetPropertyPrivate()
    {
        if (Widget)
        {
            Widget->deleteLater();
        }
    }
};

#ifdef Q_CC_CLANG
    #pragma clang diagnostic pop
#endif

}

#endif // VWIDGETPROPERTY_P_H

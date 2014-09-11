/************************************************************************
 **
 **  @file   vpropertyformview_p.h
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

#ifndef VPROPERTYFORMVIEW_P_H
#define VPROPERTYFORMVIEW_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vpropertyformwidget_p.h"

namespace VPE
{

class VPropertyModel;
class VPropertySet;

class VPropertyFormViewPrivate : public VPropertyFormWidgetPrivate
{
public:
    //! The current property model
    VPropertyModel* Model;

    //! The currently used property set
    VPropertySet* PropertySet;

    //! Determines whether the widget needs to be rebuild
    bool NeedsRebuild;

    //! Helper variable
    bool IgnoreDataChangedSignal;

    VPropertyFormViewPrivate()
        : VPropertyFormWidgetPrivate(), Model(NULL), PropertySet(NULL), NeedsRebuild(false),
          IgnoreDataChangedSignal(false) {}

    VPropertyFormViewPrivate(VPropertyModel* prop_model)
        : VPropertyFormWidgetPrivate(), Model(prop_model), PropertySet(NULL), NeedsRebuild(false),
          IgnoreDataChangedSignal(false) {}

    VPropertyFormViewPrivate(VPropertySet* prop_set)
        : VPropertyFormWidgetPrivate(), Model(NULL), PropertySet(prop_set), NeedsRebuild(false),
          IgnoreDataChangedSignal(false) {}
};

}

#endif // VPROPERTYFORMVIEW_P_H

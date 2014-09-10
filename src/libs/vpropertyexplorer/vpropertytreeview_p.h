/************************************************************************
 **
 **  @file   vpropertytreeview_p.h
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

#ifndef VPROPERTYTREEVIEW_P_H
#define VPROPERTYTREEVIEW_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE
{

class VPropertyDelegate;

class VPropertyTreeViewPrivate
{
public:
    //! Property Delegate
    VPropertyDelegate* PropertyDelegate;

    //! Constructor
    VPropertyTreeViewPrivate(VPropertyDelegate* delegate)
        : PropertyDelegate(delegate) {}

    //! Constructor
    VPropertyTreeViewPrivate()
        : PropertyDelegate(nullptr) {}
};

}

#endif // VPROPERTYTREEVIEW_P_H

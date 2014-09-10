/************************************************************************
 **
 **  @file   vfileproperty_p.h
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

#ifndef VFILEPROPERTY_P_H
#define VFILEPROPERTY_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include "vproperty_p.h"

namespace VPE
{

class VFilePropertyPrivate : public VPropertyPrivate
{
public:
    //! File filters
    QString FileFilters;

    //! Determines whether the file property is a file or a directory. Default: false
    bool Directory;


    //! Constructor passing name and type
    VFilePropertyPrivate(const QString& name, QVariant::Type type, bool directory = false)
        : VPropertyPrivate(name, type), FileFilters(), Directory(directory) {}

    //! Constructor
    VFilePropertyPrivate()
        : VPropertyPrivate(), FileFilters(), Directory(false) {}
};

}

#endif // VFILEPROPERTY_P_H

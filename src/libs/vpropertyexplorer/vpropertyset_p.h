/************************************************************************
 **
 **  @file   vpropertyset_p.h
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

#ifndef VPROPERTYSET_P_H
#define VPROPERTYSET_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE
{

class VProperty;

class VPropertySetPrivate
{
public:
    //! Property map (ID, Property)
    QMap<QString, VProperty*> Properties; // All the Properties managed by this model are being stored in this map for
    //quick access

    //! List containing the root properties
    QList<VProperty*> RootProperties;

    //! Constructor
    VPropertySetPrivate()
    {
    }
};

}

#endif // VPROPERTYMODEL_P_H

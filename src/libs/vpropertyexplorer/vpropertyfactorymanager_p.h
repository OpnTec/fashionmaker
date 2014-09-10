/************************************************************************
 **
 **  @file   vpropertyfactorymanager_p.h
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

#ifndef VPROPERTYFACTORYMANAGER_P_H
#define VPROPERTYFACTORYMANAGER_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>

namespace VPE {

class VAbstractPropertyFactory;

class VPropertyFactoryManagerPrivate {
public:
    QMap<QString, VAbstractPropertyFactory*> Factories;

};

}

#endif // VPROPERTYFACTORYMANAGER_P_H

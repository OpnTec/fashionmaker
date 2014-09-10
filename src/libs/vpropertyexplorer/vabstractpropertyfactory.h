/************************************************************************
 **
 **  @file   vabstractpropertyfactory.h
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

#ifndef VABSTRACTPROPERTYFACTORY_H
#define VABSTRACTPROPERTYFACTORY_H

#include "vpropertyexplorer_global.h"
#include <QObject>

namespace VPE
{

class VProperty;

class VPROPERTYEXPLORERSHARED_EXPORT VAbstractPropertyFactory
{
public:
    //! Empty virtual destructor
    virtual ~VAbstractPropertyFactory() {}

    //! Creates a new property of a certain type and assigns a name and description (otionally)
    //! \param type The type of the property as string
    //! \param name The property's name
    //! \return Returns the created property or NULL if it couldn't be be created
    virtual VProperty* createProperty(const QString& type, const QString &name) = 0;
};

}

#endif // VABSTRACTPROPERTYFACTORY_H

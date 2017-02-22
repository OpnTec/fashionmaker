/************************************************************************
 **
 **  @file   vstandardpropertyfactory.h
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

#ifndef VASTANDARDPROPERTYFACTORY_H
#define VASTANDARDPROPERTYFACTORY_H

#include <qcompilerdetection.h>
#include <QString>

#include "vabstractpropertyfactory.h"
#include "vpropertyexplorer_global.h"

namespace VPE
{

class VPropertyFactoryManager;

//! The standard property factory is able to create all the properties that are included in VPropertyExplorer
//! by default.
class VPROPERTYEXPLORERSHARED_EXPORT VStandardPropertyFactory : public VAbstractPropertyFactory
{
public:
    //! Constructor
    VStandardPropertyFactory();

    //! Constructor
    //! \param manager Registers this factory at the manager for all it's types
    explicit VStandardPropertyFactory(VPropertyFactoryManager* manager);

    //! Creates a new property of a certain type and assigns a name and description (otionally)
    //! \param type The type of the property as string
    //! \return Returns the created property or NULL if it couldn't be be created
    virtual VProperty* createProperty(const QString& type, const QString &name) Q_DECL_OVERRIDE Q_REQUIRED_RESULT;
};

}

#endif // VASTANDARDPROPERTYFACTORY_H

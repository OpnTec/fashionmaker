/************************************************************************
 **
 **  @file   vpropertymodel_p.h
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

#ifndef VPROPERTYMODEL_P_H
#define VPROPERTYMODEL_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QMap>
#include <QString>
#include <QObject>

namespace VPE
{

class VProperty;
class VPropertySet;

class VPropertyModelPrivate
{
public:
    //! The property set holding the properties
    VPropertySet* Properties;

    //! The header data for the property name column
    QString HeadlineProperty;

    //! The header data for the value column
    QString HeadlineValue;

    //! Constructor
    VPropertyModelPrivate()
        : Properties(nullptr), HeadlineProperty(QString()), HeadlineValue(QString())
    {
        //: The text that appears in the first column header
        HeadlineProperty = QObject::tr("Property");

        //: The text that appears in the second column header
        HeadlineValue = QObject::tr("Value");
    }

private:
    Q_DISABLE_COPY(VPropertyModelPrivate)
};

}

#endif // VPROPERTYMODEL_P_H

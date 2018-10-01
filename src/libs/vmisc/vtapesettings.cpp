/************************************************************************
 **
 **  @file   vtapesettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vtapesettings.h"

#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVariant>
#include <QGlobalStatic>

namespace
{
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingDataBaseGeometry, (QLatin1String("database/geometry")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingDefHeight, (QLatin1String("gradation/defHeight")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingDefSize, (QLatin1String("gradation/defSize")))
}

//---------------------------------------------------------------------------------------------------------------------
VTapeSettings::VTapeSettings(Format format, Scope scope, const QString &organization, const QString &application,
                             QObject *parent)
    :VCommonSettings(format, scope, organization, application, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VTapeSettings::GetDataBaseGeometry() const
{
    return value(*settingDataBaseGeometry).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDataBaseGeometry(const QByteArray &value)
{
    setValue(*settingDataBaseGeometry, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDefHeight(int value)
{
    setValue(*settingDefHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetDefHeight() const
{
    return value(*settingDefHeight, 176).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDefSize(int value)
{
    setValue(*settingDefSize, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetDefSize() const
{
    return value(*settingDefSize, 50).toInt();
}

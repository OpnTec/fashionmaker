/************************************************************************
 **
 **  @file   vtapesettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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
#include <QTextCodec>
#include <QVariant>

const QString VTapeSettings::SettingDataBaseGeometry    = QStringLiteral("database/geometry");

const QString VTapeSettings::SettingDefHeight = QStringLiteral("gradation/defHeight");
const QString VTapeSettings::SettingDefSize   = QStringLiteral("gradation/defHeight");

const QString VTapeSettings::SettingCSVWithHeader = QStringLiteral("csv/withHeader");
const QString VTapeSettings::SettingCSVCodec      = QStringLiteral("csv/withCodec");
const QString VTapeSettings::SettingCSVSeparator  = QStringLiteral("csv/withSeparator");

//---------------------------------------------------------------------------------------------------------------------
VTapeSettings::VTapeSettings(Format format, Scope scope, const QString &organization, const QString &application,
                             QObject *parent)
    :VCommonSettings(format, scope, organization, application, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VTapeSettings::GetDataBaseGeometry() const
{
    return value(SettingDataBaseGeometry).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDataBaseGeometry(const QByteArray &value)
{
    setValue(SettingDataBaseGeometry, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDefHeight(int value)
{
    setValue(SettingDefHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetDefHeight() const
{
    return value(SettingDefHeight, 176).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetDefSize(int value)
{
    setValue(SettingDefSize, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetDefSize() const
{
    return value(SettingDefSize, 50).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetCSVWithHeader(bool withHeader)
{
    setValue(SettingCSVWithHeader, withHeader);
}

//---------------------------------------------------------------------------------------------------------------------
bool VTapeSettings::GetCSVWithHeader() const
{
    return value(SettingCSVWithHeader, GetDefCSVWithHeader()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VTapeSettings::GetDefCSVWithHeader() const
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetCSVCodec(int mib)
{
    setValue(SettingCSVCodec, mib);
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetCSVCodec() const
{
    return value(SettingCSVCodec, GetDefCSVCodec()).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
int VTapeSettings::GetDefCSVCodec() const
{
    return QTextCodec::codecForLocale()->mibEnum();
}

//---------------------------------------------------------------------------------------------------------------------
void VTapeSettings::SetCSVSeparator(const QChar &separator)
{
    switch(separator.toLatin1())
    {
        case '\t':
            setValue(SettingCSVSeparator, 0);
            break;
        case ';':
            setValue(SettingCSVSeparator, 1);
            break;
        case ' ':
            setValue(SettingCSVSeparator, 2);
            break;
        case ',':
        default:
            setValue(SettingCSVSeparator, 3);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar VTapeSettings::GetCSVSeparator() const
{
    const quint8 separator = static_cast<quint8>(value(SettingCSVSeparator, 3).toUInt());
    switch(separator)
    {
        case 0:
            return QChar('\t');
            break;
        case 1:
            return QChar(';');
            break;
        case 2:
            return QChar(' ');
            break;
        case 3:
        default:
            return QChar(',');
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar VTapeSettings::GetDefCSVSeparator() const
{
    return QChar(',');
}

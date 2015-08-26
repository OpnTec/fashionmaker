/************************************************************************
 **
 **  @file   VVSTConverter.cpp
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

#include "vvstconverter.h"
#include "exception/vexception.h"

#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VVSTConverter::MeasurementMinVerStr = QStringLiteral("0.3.0");
const QString VVSTConverter::MeasurementMaxVerStr = QStringLiteral("0.4.0");
const QString VVSTConverter::CurrentSchema        = QStringLiteral("://schema/standard_measurements/v0.4.0.xsd");

//---------------------------------------------------------------------------------------------------------------------
VVSTConverter::VVSTConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VVSTConverter::~VVSTConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VVSTConverter::MinVer() const
{
    return GetVersion(MeasurementMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VVSTConverter::MaxVer() const
{
    return GetVersion(MeasurementMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000300):
            return QStringLiteral("://schema/standard_measurements/v0.3.0.xsd");
        case (0x000400):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000300):
            {
                ToV0_4_0();
                const QString schema = XSDSchema(0x000400);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000400):
                break;
            default:
                break;
        }
    }
    catch (VException &e)
    {
        QString error;
        const QString backupFileName = fileName +".backup";
        if (SafeCopy(backupFileName, fileName, error) == false)
        {
            const QString errorMsg(tr("Error restoring backup file: %1.").arg(error));
            VException excep(errorMsg);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }

        QFile file(backupFileName);
        file.remove();

        throw;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_0()
{
    SetVersion(QStringLiteral("0.4.0"));
    Save();
}

/************************************************************************
 **
 **  @file   vvitconverter.cpp
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

#include "vvitconverter.h"
#include "exception/vexception.h"

#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VVITConverter::MeasurementMinVerStr = QStringLiteral("0.2.0");
const QString VVITConverter::MeasurementMaxVerStr = QStringLiteral("0.3.0");
const QString VVITConverter::CurrentSchema        = QStringLiteral("://schema/individual_measurements/v0.3.0.xsd");

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::VVITConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::~VVITConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VVITConverter::MinVer() const
{
    return GetVersion(MeasurementMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VVITConverter::MaxVer() const
{
    return GetVersion(MeasurementMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000200):
            return QStringLiteral("://schema/individual_measurements/v0.2.0.xsd");
        case (0x000300):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000200):
            {
                ToV0_3_0();
                const QString schema = XSDSchema(0x000300);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000300):
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
void VVITConverter::ToV0_3_0()
{
    SetVersion(QStringLiteral("0.3.0"));
    Save();
}

/************************************************************************
 **
 **  @file   vpatternconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vpatternconverter.h"
#include "exception/vexception.h"

#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VPatternConverter::PatternMinVerStr = QStringLiteral("0.1.0");
const QString VPatternConverter::PatternMaxVerStr = QStringLiteral("0.1.3");
const QString VPatternConverter::CurrentSchema    = QStringLiteral("://schema/pattern/v0.1.3.xsd");

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::VPatternConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::~VPatternConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VPatternConverter::MinVer() const
{
    return GetVersion(PatternMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternConverter::MaxVer() const
{
    return GetVersion(PatternMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::MinVerStr() const
{
    return PatternMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::MaxVerStr() const
{
    return PatternMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000100):
            return QStringLiteral("://schema/pattern/v0.1.0.xsd");
        case (0x000101):
            return QStringLiteral("://schema/pattern/v0.1.1.xsd");
        case (0x000102):
            return QStringLiteral("://schema/pattern/v0.1.2.xsd");
        case (0x000103):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000100):
            {
                ToV0_1_1();
                const QString schema = XSDSchema(0x000101);
                ValidateXML(schema, fileName);
                // continue conversion
            }
            case (0x000101):
            {
                ToV0_1_2();
                const QString schema = XSDSchema(0x000102);
                ValidateXML(schema, fileName);
                // continue conversion
            }
            case (0x000102):
            {
                ToV0_1_3();
                const QString schema = XSDSchema(0x000103);
                ValidateXML(schema, fileName);
                // continue conversion
            }
            case (0x000103):
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
void VPatternConverter::ToV0_1_1()
{
    SetVersion(QStringLiteral("0.1.1"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_2()
{
    SetVersion(QStringLiteral("0.1.2"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_3()
{
    SetVersion(QStringLiteral("0.1.3"));
    Save();
}

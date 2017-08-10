/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "vlabeltemplateconverter.h"

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VLabelTemplateConverter::LabelTemplateMinVerStr = QStringLiteral("1.0.0");
const QString VLabelTemplateConverter::LabelTemplateMaxVerStr = QStringLiteral("1.0.0");
const QString VLabelTemplateConverter::CurrentSchema          = QStringLiteral("://schema/label_template/v1.0.0.xsd");

//VLabelTemplateConverter::LabelTemplateMinVer; // <== DON'T FORGET TO UPDATE TOO!!!!
//VLabelTemplateConverter::LabelTemplateMaxVer; // <== DON'T FORGET TO UPDATE TOO!!!!

//---------------------------------------------------------------------------------------------------------------------
VLabelTemplateConverter::VLabelTemplateConverter(const QString &fileName)
    : VAbstractConverter(fileName)
{
    ValidateInputFile(CurrentSchema);
}

//---------------------------------------------------------------------------------------------------------------------
int VLabelTemplateConverter::MinVer() const
{
    return LabelTemplateMinVer;
}

//---------------------------------------------------------------------------------------------------------------------
int VLabelTemplateConverter::MaxVer() const
{
    return LabelTemplateMaxVer;
}

//---------------------------------------------------------------------------------------------------------------------
QString VLabelTemplateConverter::MinVerStr() const
{
    return LabelTemplateMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VLabelTemplateConverter::MaxVerStr() const
{
    return LabelTemplateMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VLabelTemplateConverter::XSDSchema(int ver) const
{
    switch (ver)
    {
        case (0x010000):
            return CurrentSchema;
        default:
            InvalidVersion(ver);
            break;
    }
    return QString();//unreachable code
}

//---------------------------------------------------------------------------------------------------------------------
void VLabelTemplateConverter::ApplyPatches()
{
    switch (m_ver)
    {
        case (0x010000):
            break;
        default:
            InvalidVersion(m_ver);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VLabelTemplateConverter::DowngradeToCurrentMaxVersion()
{
    SetVersion(LabelTemplateMaxVerStr);
    Save();
}

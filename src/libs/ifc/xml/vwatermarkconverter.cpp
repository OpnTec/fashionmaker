/************************************************************************
 **
 **  @file   vwatermarkconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 12, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#include "vwatermarkconverter.h"

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VWatermarkConverter::WatermarkMinVerStr = QStringLiteral("1.0.0");
const QString VWatermarkConverter::WatermarkMaxVerStr = QStringLiteral("1.0.0");
const QString VWatermarkConverter::CurrentSchema          = QStringLiteral("://schema/watermark/v1.0.0.xsd");

//VWatermarkConverter::WatermarkMinVer; // <== DON'T FORGET TO UPDATE TOO!!!!
//VWatermarkConverter::WatermarkMaxVer; // <== DON'T FORGET TO UPDATE TOO!!!!

//---------------------------------------------------------------------------------------------------------------------
VWatermarkConverter::VWatermarkConverter(const QString &fileName)
    : VAbstractConverter(fileName)
{
    ValidateInputFile(CurrentSchema);
}

//---------------------------------------------------------------------------------------------------------------------
int VWatermarkConverter::MinVer() const
{
    return WatermarkMinVer;
}

//---------------------------------------------------------------------------------------------------------------------
int VWatermarkConverter::MaxVer() const
{
    return WatermarkMaxVer;
}

//---------------------------------------------------------------------------------------------------------------------
QString VWatermarkConverter::MinVerStr() const
{
    return WatermarkMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VWatermarkConverter::MaxVerStr() const
{
    return WatermarkMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VWatermarkConverter::XSDSchema(int ver) const
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
void VWatermarkConverter::ApplyPatches()
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
void VWatermarkConverter::DowngradeToCurrentMaxVersion()
{
    SetVersion(WatermarkMaxVerStr);
    Save();
}

/************************************************************************
 **
 **  @file   vwatermarkconverter.h
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
#ifndef VWATERMARKCONVERTER_H
#define VWATERMARKCONVERTER_H

#include "vabstractconverter.h"

class VWatermarkConverter : public VAbstractConverter
{
public:
    explicit VWatermarkConverter(const QString &fileName);
    virtual ~VWatermarkConverter() Q_DECL_EQ_DEFAULT;

    static const QString WatermarkMaxVerStr;
    static const QString CurrentSchema;
    static Q_DECL_CONSTEXPR const int WatermarkMinVer = FORMAT_VERSION(1, 0, 0);
    static Q_DECL_CONSTEXPR const int WatermarkMaxVer = FORMAT_VERSION(1, 0, 0);

protected:
    virtual int     MinVer() const override;
    virtual int     MaxVer() const override;

    virtual QString MinVerStr() const override;
    virtual QString MaxVerStr() const override;

    virtual QString XSDSchema(int ver) const override;
    virtual void    ApplyPatches() override;
    virtual void    DowngradeToCurrentMaxVersion() override;

    virtual bool IsReadOnly() const override {return false;}

private:
    Q_DISABLE_COPY(VWatermarkConverter)
    static const QString WatermarkMinVerStr;
};

#endif // VWATERMARKCONVERTER_H

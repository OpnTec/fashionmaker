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

#ifndef VLABELTEMPLATECONVERTER_H
#define VLABELTEMPLATECONVERTER_H

#include "vabstractconverter.h"

class VLabelTemplateConverter : public VAbstractConverter
{
public:
    explicit VLabelTemplateConverter(const QString &fileName);
    virtual ~VLabelTemplateConverter() Q_DECL_EQ_DEFAULT;

    static const QString LabelTemplateMaxVerStr;
    static const QString CurrentSchema;
    static Q_DECL_CONSTEXPR const int LabelTemplateMinVer = CONVERTER_VERSION_CHECK(1, 0, 0);
    static Q_DECL_CONSTEXPR const int LabelTemplateMaxVer = CONVERTER_VERSION_CHECK(1, 0, 0);

protected:
    virtual int     MinVer() const Q_DECL_OVERRIDE;
    virtual int     MaxVer() const Q_DECL_OVERRIDE;

    virtual QString MinVerStr() const Q_DECL_OVERRIDE;
    virtual QString MaxVerStr() const Q_DECL_OVERRIDE;

    virtual QString XSDSchema(int ver) const Q_DECL_OVERRIDE;
    virtual void    ApplyPatches() Q_DECL_OVERRIDE;
    virtual void    DowngradeToCurrentMaxVersion() Q_DECL_OVERRIDE;

    virtual bool IsReadOnly() const Q_DECL_OVERRIDE {return false;}

private:
    Q_DISABLE_COPY(VLabelTemplateConverter)
    static const QString LabelTemplateMinVerStr;
};

#endif // VLABELTEMPLATECONVERTER_H

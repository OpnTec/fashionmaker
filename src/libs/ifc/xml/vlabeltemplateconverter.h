/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
    static Q_DECL_CONSTEXPR const int LabelTemplateMinVer = FORMAT_VERSION(1, 0, 0);
    static Q_DECL_CONSTEXPR const int LabelTemplateMaxVer = FORMAT_VERSION(1, 0, 0);

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
    Q_DISABLE_COPY(VLabelTemplateConverter)
    static const QString LabelTemplateMinVerStr;
};

#endif // VLABELTEMPLATECONVERTER_H

/************************************************************************
 **
 **  @file   vpatternconverter.h
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

#ifndef VPATTERNCONVERTER_H
#define VPATTERNCONVERTER_H

#include "vabstractconverter.h"

class VPatternConverter : public VAbstractConverter
{
    Q_DECLARE_TR_FUNCTIONS(VPatternConverter)
public:
    VPatternConverter(const QString &fileName);
    virtual ~VPatternConverter();

    static const QString    PatternMaxVerStr;
    static const QString    CurrentSchema;

protected:
    virtual int     MinVer() const;
    virtual int     MaxVer() const;

    virtual QString MinVerStr() const;
    virtual QString MaxVerStr() const;

    QString         XSDSchema(int ver) const;
    virtual void    ApplyPatches();

private:
    Q_DISABLE_COPY(VPatternConverter)
    static const QString    PatternMinVerStr;

    void ToV0_1_1();
    void ToV0_1_2();
    void ToV0_1_3();
    void ToV0_1_4();
};

#endif // VPATTERNCONVERTER_H

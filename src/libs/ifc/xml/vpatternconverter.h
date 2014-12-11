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
 **  Copyright (C) 2014 Valentina project
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
public:
    VPatternConverter(const QString &fileName);
    virtual ~VPatternConverter();

    static QString CurrentSchema();
    static const QString    PatternMaxVerStr;

protected:
    virtual int     MinVer() const;
    virtual int     MaxVer() const;

    virtual QString MinVerStr() const;
    virtual QString MaxVerStr() const;
private:
    Q_DISABLE_COPY(VPatternConverter)
    static const QString    PatternMinVerStr;
};

#endif // VPATTERNCONVERTER_H

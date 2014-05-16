/************************************************************************
 **
 **  @file   vstandardmeasurements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 3, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VSTANDARDMEASUREMENTS_H
#define VSTANDARDMEASUREMENTS_H

#include "vdomdocument.h"

class VStandardMeasurements:public VDomDocument
{
public:
    VStandardMeasurements(VContainer *data);
    Valentina::Units Unit();
    QString Description();
    void Measurements();
    void SetSize();
    void SetHeight();
    static const QString TagMeasurement;
    static const QString TagDescription;
    static const QString TagSize;
    static const QString TagHeight;
    static const QString AttrValue;
    static const QString AttrSize_increace;
    static const QString AttrHeight_increase;
private:
    void Measurement(const QString &tag);
};

#endif // VSTANDARDMEASUREMENTS_H

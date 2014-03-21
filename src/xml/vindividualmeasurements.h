/************************************************************************
 **
 **  @file   vindividualmeasurements.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#ifndef VINDIVIDUALMEASUREMENTS_H
#define VINDIVIDUALMEASUREMENTS_H

#include "vdomdocument.h"

class VIndividualMeasurements:public VDomDocument
{
public:
    VIndividualMeasurements(VContainer *data);
    Valentina::Units Unit();
    void Measurements();
    void Measurement(const QString &tag);
    QString Language();
    QString FamilyName();
    QString GivenName();
    QString BirthDate();
    QString Sex();
    static const QString AttrIgnore;
    static const QString AttrName;
    static const QString AttrM_number;
    static const QString AttrGui_text;
    static const QString AttrValue;
    static const QString AttrDescription;
    static const QString AttrLang;
    static const QString AttrFamily_name;
    static const QString AttrGiven_name;
    static const QString AttrBirth_date;
    static const QString AttrSex;
};

#endif // VINDIVIDUALMEASUREMENTS_H

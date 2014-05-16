/************************************************************************
 **
 **  @file   vindividualmeasurements.h
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

#ifndef VINDIVIDUALMEASUREMENTS_H
#define VINDIVIDUALMEASUREMENTS_H

#include "vdomdocument.h"
#include <QDate>

class VIndividualMeasurements:public VDomDocument
{
public:
    enum Sex { Male, Female };
    Q_DECLARE_FLAGS(Genders, Sex)

    VIndividualMeasurements(VContainer *data);
    Valentina::Units Unit() const;
    void             setUnit(const Valentina::Units &unit);
    void             Measurements();
    QString          FamilyName() const;
    void             setFamilyName(const QString &text);
    QString          GivenName() const;
    void             setGivenName(const QString &text);
    QDate            BirthDate() const;
    void             setBirthDate(const QDate &date);
    VIndividualMeasurements::Genders Sex() const;
    void             setSex(const VIndividualMeasurements::Genders &sex);
    QString          Mail() const;
    void             setMail(const QString &text);
    static const QString AttrValue;
    static const QString TagFamily_name;
    static const QString TagGiven_name;
    static const QString TagBirth_date;
    static const QString TagSex;
    static const QString TagUnit;
    static const QString TagEmail;
    static const QString SexMale;
    static const QString SexFemale;

    static QString   GenderToStr(const VIndividualMeasurements::Genders &sex);
    static VIndividualMeasurements::Genders StrToGender(const QString &sex);
private:
    void             Measurement(const QString &tag);
};

Q_DECLARE_OPERATORS_FOR_FLAGS( VIndividualMeasurements::Genders )

#endif // VINDIVIDUALMEASUREMENTS_H

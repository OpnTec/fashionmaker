/************************************************************************
 **
 **  @file   vindividualmeasurements.cpp
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

#include "vindividualmeasurements.h"
#include "../widgets/vapplication.h"
#include <QDate>

const QString VIndividualMeasurements::TagFamily_name = QStringLiteral("family-name");
const QString VIndividualMeasurements::TagGiven_name  = QStringLiteral("given-name");
const QString VIndividualMeasurements::TagBirth_date  = QStringLiteral("birth-date");
const QString VIndividualMeasurements::TagSex         = QStringLiteral("sex");
const QString VIndividualMeasurements::TagEmail       = QStringLiteral("email");
const QString VIndividualMeasurements::SexMale        = QStringLiteral("male");
const QString VIndividualMeasurements::SexFemale      = QStringLiteral("female");

//---------------------------------------------------------------------------------------------------------------------
VIndividualMeasurements::VIndividualMeasurements(VContainer *data)
    :VAbstractMeasurements(data)
{
    SCASSERT(data != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
VIndividualMeasurements::~VIndividualMeasurements()
{}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setUnit(const Unit &unit)
{
    setTagText(TagUnit, UnitsToStr(unit));
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::Measurements()
{
    VAbstractMeasurements::Measurements();
    //heights
    Measurement(height_M);
    //extended
    Measurement(size_M);
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::ReadMeasurement(const QDomElement &domElement, const QString &tag)
{
    qreal value = GetParametrDouble(domElement, AttrValue, "0.0");
    value = UnitConvertor(value, MUnit(), qApp->patternUnit());
    data->AddVariable(tag, new VMeasurement(tag, value, qApp->GuiText(tag), qApp->Description(tag), tag));
}

//---------------------------------------------------------------------------------------------------------------------
QString VIndividualMeasurements::FamilyName() const
{
    return UniqueTagText(TagFamily_name, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setFamilyName(const QString &text)
{
    setTagText(TagFamily_name, text);
}

//---------------------------------------------------------------------------------------------------------------------
QString VIndividualMeasurements::GivenName() const
{
    return UniqueTagText(TagGiven_name, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setGivenName(const QString &text)
{
    setTagText(TagGiven_name, text);
}

//---------------------------------------------------------------------------------------------------------------------
QDate VIndividualMeasurements::BirthDate() const
{
    const QString date = UniqueTagText(TagBirth_date, "1900-01-01");
    return QDate::fromString(date, "yyyy-MM-dd");
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setBirthDate(const QDate &date)
{
    setTagText(TagBirth_date, date.toString("yyyy-MM-dd"));
}

//---------------------------------------------------------------------------------------------------------------------
SexType VIndividualMeasurements::Sex() const
{
    return StrToGender(UniqueTagText(TagSex, ""));
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setSex(const SexType &sex)
{
    setTagText(TagSex, GenderToStr(sex));
}

//---------------------------------------------------------------------------------------------------------------------
QString VIndividualMeasurements::Mail() const
{
    return UniqueTagText(TagEmail, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setMail(const QString &text)
{
    setTagText(TagEmail, text);
}

//---------------------------------------------------------------------------------------------------------------------
QString VIndividualMeasurements::GenderToStr(const SexType &sex)
{
    switch (sex)
    {
        case SexType::Male:
            return SexMale;
        case SexType::Female:
            return SexFemale;
        default:
            return SexMale;
    }
}

//---------------------------------------------------------------------------------------------------------------------
SexType VIndividualMeasurements::StrToGender(const QString &sex)
{
    QStringList genders{SexMale, SexFemale};
    switch (genders.indexOf(sex))
    {
        case 0: // SexMale
            return SexType::Male;
        case 1: // SexFemale
            return SexType::Female;
        default:
            return SexType::Male;
    }
}

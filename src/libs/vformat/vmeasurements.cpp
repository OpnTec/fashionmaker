/************************************************************************
 **
 **  @file   vmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"

const QString VMeasurements::TagVST              = QStringLiteral("vst");
const QString VMeasurements::TagVIT              = QStringLiteral("vit");
const QString VMeasurements::TagBodyMeasurements = QStringLiteral("body-measurements");
const QString VMeasurements::TagUnit             = QStringLiteral("unit");
const QString VMeasurements::TagNotes            = QStringLiteral("notes");
const QString VMeasurements::TagSize             = QStringLiteral("size");
const QString VMeasurements::TagHeight           = QStringLiteral("height");
const QString VMeasurements::TagPersonal         = QStringLiteral("personal");
const QString VMeasurements::TagFamilyName       = QStringLiteral("family-name");
const QString VMeasurements::TagGivenName        = QStringLiteral("given-name");
const QString VMeasurements::TagBirthDate        = QStringLiteral("birth-date");
const QString VMeasurements::TagSex              = QStringLiteral("sex");
const QString VMeasurements::TagEmail            = QStringLiteral("email");
const QString VMeasurements::TagReadOnly         = QStringLiteral("read-only");

const QString VMeasurements::AttrBase = QStringLiteral("base");

const QString VMeasurements::SexMale    = QStringLiteral("male");
const QString VMeasurements::SexFemale  = QStringLiteral("female");
const QString VMeasurements::SexUnknown = QStringLiteral("unknown");

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Unknown)
{
    SCASSERT(data != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(Unit unit, VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Individual)
{
    SCASSERT(data != nullptr);

    CreateEmptyIndividualFile(unit);
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(Unit unit, int baseSize, int baseHeight, VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Standard)
{
    SCASSERT(data != nullptr);

    CreateEmptyStandardFile(unit, baseSize, baseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::~VMeasurements()
{
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType VMeasurements::Type() const
{
    return type;
}

//---------------------------------------------------------------------------------------------------------------------
Unit VMeasurements::MUnit() const
{
    const QString unit = UniqueTagText(TagUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
int VMeasurements::BaseSize() const
{
    if (type == MeasurementsType::Standard)
    {
        return static_cast<int>(UniqueTagAttr(TagSize, AttrBase, 50));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VMeasurements::BaseHeight() const
{
    if (type == MeasurementsType::Standard)
    {
        return static_cast<int>(UniqueTagAttr(TagHeight, AttrBase, 176));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::Notes() const
{
    return UniqueTagText(TagNotes, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetNotes(const QString &text)
{
    setTagText(TagNotes, text);
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::FamilyName() const
{
    return UniqueTagText(TagFamilyName, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetFamilyName(const QString &text)
{
    setTagText(TagFamilyName, text);
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::GivenName() const
{
    return UniqueTagText(TagGivenName, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetGivenName(const QString &text)
{
    setTagText(TagGivenName, text);
}

//---------------------------------------------------------------------------------------------------------------------
QDate VMeasurements::BirthDate() const
{
    const QString date = UniqueTagText(TagBirthDate, "1900-01-01");
    return QDate::fromString(date, "yyyy-MM-dd");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetBirthDate(const QDate &date)
{
    setTagText(TagBirthDate, date.toString("yyyy-MM-dd"));
}

//---------------------------------------------------------------------------------------------------------------------
SexType VMeasurements::Sex() const
{
    return StrToGender(UniqueTagText(TagSex, ""));
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetSex(const SexType &sex)
{
    setTagText(TagSex, GenderToStr(sex));
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::Email() const
{
    return UniqueTagText(TagEmail, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetEmail(const QString &text)
{
    setTagText(TagEmail, text);
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurements::ReadOnly() const
{
    if (UniqueTagText(TagReadOnly, "false") == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetReadOnly(bool ro)
{
    if (ro)
    {
        setTagText(TagReadOnly, "true");
    }
    else
    {
        setTagText(TagReadOnly, "false");
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::GenderToStr(const SexType &sex)
{
    switch (sex)
    {
        case SexType::Male:
            return SexMale;
        case SexType::Female:
            return SexFemale;
        case SexType::Unknown:
            return SexUnknown;
        default:
            return SexUnknown;
    }
}

//---------------------------------------------------------------------------------------------------------------------
SexType VMeasurements::StrToGender(const QString &sex)
{
    QStringList genders = QStringList() << SexMale << SexFemale << SexUnknown;
    switch (genders.indexOf(sex))
    {
        case 0: // SexMale
            return SexType::Male;
        case 1: // SexFemale
            return SexType::Female;
        case 2: // SexUnknown
            return SexType::Unknown;
        default:
            return SexType::Unknown;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::CreateEmptyStandardFile(Unit unit, int baseSize, int baseHeight)
{
    this->clear();
    QDomElement mElement = this->createElement(TagVST);

    mElement.appendChild(createComment("Measurements created with Valentina (http://www.valentina-project.org/)."));

    QDomElement version = createElement(TagVersion);
    const QDomText newNodeText = createTextNode(VVSTConverter::MeasurementMaxVerStr);
    version.appendChild(newNodeText);
    mElement.appendChild(version);

    QDomElement ro = createElement(TagReadOnly);
    const QDomText roNodeText = createTextNode(false);
    ro.appendChild(roNodeText);
    mElement.appendChild(ro);

    mElement.appendChild(createElement(TagNotes));

    QDomElement mUnit = createElement(TagUnit);
    const QDomText unitText = createTextNode(UnitsToStr(unit));
    mUnit.appendChild(unitText);
    mElement.appendChild(mUnit);

    QDomElement size = createElement(TagSize);
    SetAttribute(size, AttrBase, QString().setNum(baseSize));
    mElement.appendChild(size);

    QDomElement height = createElement(TagHeight);
    SetAttribute(height, AttrBase, QString().setNum(baseHeight));
    mElement.appendChild(height);

    mElement.appendChild(createElement(TagBodyMeasurements));

    this->appendChild(mElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::CreateEmptyIndividualFile(Unit unit)
{
    this->clear();
    QDomElement mElement = this->createElement(TagVIT);

    mElement.appendChild(createComment("Measurements created with Valentina (http://www.valentina-project.org/)."));

    QDomElement version = createElement(TagVersion);
    const QDomText newNodeText = createTextNode(VVITConverter::MeasurementMaxVerStr);
    version.appendChild(newNodeText);
    mElement.appendChild(version);

    QDomElement ro = createElement(TagReadOnly);
    const QDomText roNodeText = createTextNode(false);
    ro.appendChild(roNodeText);
    mElement.appendChild(ro);

    mElement.appendChild(createElement(TagNotes));

    QDomElement mUnit = createElement(TagUnit);
    mUnit.appendChild(createTextNode(UnitsToStr(unit)));
    mElement.appendChild(mUnit);

    QDomElement personal = createElement(TagPersonal);
    personal.appendChild(createElement(TagGivenName));
    personal.appendChild(createElement(TagBirthDate));
    personal.appendChild(createElement(TagSex));
    personal.appendChild(createElement(TagEmail));
    mElement.appendChild(personal);

    mElement.appendChild(createElement(TagBodyMeasurements));

    this->appendChild(mElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurements::UniqueTagAttr(const QString &tag, const QString &attr, qreal defValue) const
{
    const qreal defVal = UnitConvertor(defValue, Unit::Cm, MUnit());

    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        return defVal;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                return GetParametrDouble(domElement, attr, QString("%1").arg(defVal));
            }
        }
    }
    return defVal;
}

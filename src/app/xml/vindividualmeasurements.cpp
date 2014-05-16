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

const QString VIndividualMeasurements::AttrValue      = QStringLiteral("value");
const QString VIndividualMeasurements::TagFamily_name = QStringLiteral("family-name");
const QString VIndividualMeasurements::TagGiven_name  = QStringLiteral("given-name");
const QString VIndividualMeasurements::TagBirth_date  = QStringLiteral("birth-date");
const QString VIndividualMeasurements::TagSex         = QStringLiteral("sex");
const QString VIndividualMeasurements::TagUnit        = QStringLiteral("unit");
const QString VIndividualMeasurements::TagEmail       = QStringLiteral("email");
const QString VIndividualMeasurements::SexMale        = QStringLiteral("male");
const QString VIndividualMeasurements::SexFemale      = QStringLiteral("female");

//---------------------------------------------------------------------------------------------------------------------
VIndividualMeasurements::VIndividualMeasurements(VContainer *data):VDomDocument(data)
{}

//---------------------------------------------------------------------------------------------------------------------
Valentina::Units VIndividualMeasurements::Unit() const
{
    const QString unit = UniqueTagText(TagUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setUnit(const Valentina::Units &unit)
{
    setTagText(TagUnit, UnitsToStr(unit));
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::Measurements()
{
    //head and neck
    Measurement(headGirth);
    Measurement(midNeckGirth);
    Measurement(neckBaseGirth);
    Measurement(headAndNeckLength);
    //torso
    Measurement(centerFrontWaistLength);
    Measurement(centerBackWaistLength);
    Measurement(shoulderLength);
    Measurement(sideWaistLength);
    Measurement(trunkLength);
    Measurement(shoulderGirth);
    Measurement(upperChestGirth);
    Measurement(bustGirth);
    Measurement(underBustGirth);
    Measurement(waistGirth);
    Measurement(highHipGirth);
    Measurement(hipGirth);
    Measurement(upperFrontChestWidth);
    Measurement(frontChestWidth);
    Measurement(acrossFrontShoulderWidth);
    Measurement(acrossBackShoulderWidth);
    Measurement(upperBackWidth);
    Measurement(backWidth);
    Measurement(bustpointToBustpoint);
    Measurement(halterBustpointToBustpoint);
    Measurement(neckToBustpoint);
    Measurement(crotchLength);
    Measurement(riseHeight);
    Measurement(shoulderDrop);
    Measurement(shoulderSlopeDegrees);
    Measurement(frontShoulderSlopeLength);
    Measurement(backShoulderSlopeLength);
    Measurement(frontShoulderToWaistLength);
    Measurement(backShoulderToWaistLength);
    Measurement(frontNeckArc);
    Measurement(backNeckArc);
    Measurement(frontUpperChestArc);
    Measurement(backUpperChestArc);
    Measurement(frontWaistArc);
    Measurement(backWaistArc);
    Measurement(frontUpperHipArc);
    Measurement(backUpperHipArc);
    Measurement(frontHipArc);
    Measurement(backHipArc);
    Measurement(chestSlope);
    Measurement(backSlope);
    Measurement(frontWaistSlope);
    Measurement(backWaistSlope);
    Measurement(frontNeckToUpperChestHeight);
    Measurement(frontNeckToBustHeight);
    //arm
    Measurement(armscyeGirth);
    Measurement(elbowGirth);
    Measurement(upperArmGirth);
    Measurement(wristGirth);
    Measurement(scyeDepth);
    Measurement(shoulderAndArmLength);
    Measurement(underarmLength);
    Measurement(cervicaleToWristLength);
    Measurement(shoulderToElbowLength);
    Measurement(armLength);
    //hand
    Measurement(handWidth);
    Measurement(handLength);
    Measurement(handGirth);
    //leg
    Measurement(thighGirth);
    Measurement(midThighGirth);
    Measurement(kneeGirth);
    Measurement(calfGirth);
    Measurement(ankleGirth);
    Measurement(kneeHeight);
    Measurement(ankleHeight);
    //foot
    Measurement(footWidth);
    Measurement(footLength);
    //heights
    Measurement(height);
    Measurement(cervicaleHeight);
    Measurement(cervicaleToKneeHeight);
    Measurement(waistHeight);
    Measurement(highHipHeight);
    Measurement(hipHeight);
    Measurement(waistToHipHeight);
    Measurement(waistToKneeHeight);
    Measurement(crotchHeight);
    //extended
    Measurement(size);
    Measurement(heightFrontNeckBasePoint);
    Measurement(heightBaseNeckSidePoint);
    Measurement(heightShoulderPoint);
    Measurement(heightNipplePoint);
    Measurement(heightBackAngleAxilla);
    Measurement(heightScapularPoint);
    Measurement(heightUnderButtockFolds);
    Measurement(hipsExcludingProtrudingAbdomen);
    Measurement(girthFootInstep);
    Measurement(sideWaistToFloor);
    Measurement(frontWaistToFloor);
    Measurement(arcThroughGroinArea);
    Measurement(waistToPlaneSeat);
    Measurement(neckToRadialPoint);
    Measurement(neckToThirdFinger);
    Measurement(neckToFirstLineChestCircumference);
    Measurement(frontWaistLength);
    Measurement(arcThroughShoulderJoint);
    Measurement(neckToBackLineChestCircumference);
    Measurement(waistToNeckSide);
    Measurement(arcLengthUpperBody);
    Measurement(chestWidth);
    Measurement(anteroposteriorDiameterHands);
    Measurement(heightClavicularPoint);
    Measurement(heightArmholeSlash);
    Measurement(slashShoulderHeight);
    Measurement(halfGirthNeck);
    Measurement(halfGirthNeckForShirts);
    Measurement(halfGirthChestFirst);
    Measurement(halfGirthChestSecond);
    Measurement(halfGirthChestThird);
    Measurement(halfGirthWaist);
    Measurement(halfGirthHipsConsideringProtrudingAbdomen);
    Measurement(halfGirthHipsExcludingProtrudingAbdomen);
    Measurement(girthKneeFlexedFeet);
    Measurement(neckTransverseDiameter);
    Measurement(frontSlashShoulderHeight);
    Measurement(neckToFrontWaistLine);
    Measurement(handVerticalDiameter);
    Measurement(neckToKneePoint);
    Measurement(waistToKnee);
    Measurement(shoulderHeight);
    Measurement(headHeight);
    Measurement(bodyPosition);
    Measurement(arcBehindShoulderGirdle);
    Measurement(neckToNeckBase);
    Measurement(depthWaistFirst);
    Measurement(depthWaistSecond);
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::Measurement(const QString &tag)
{
    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        qDebug()<<"Measurement" << tag <<"doesn't exist"<<Q_FUNC_INFO;
        return;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, AttrValue, "0.0");

                if (Unit() == Valentina::Mm)//Convert to Cm.
                {
                    value = value / 10.0;
                }
                data->AddMeasurement(tag, VMeasurement(value, qApp->GuiText(tag), qApp->Description(tag), tag));
            }
        }
    }
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
VIndividualMeasurements::Genders VIndividualMeasurements::Sex() const
{
    return StrToGender(UniqueTagText(TagSex, ""));
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setSex(const VIndividualMeasurements::Genders &sex)
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
QString VIndividualMeasurements::GenderToStr(const VIndividualMeasurements::Genders &sex)
{
    switch (sex)
    {
        case Male:
            return SexMale;
        case Female:
            return SexFemale;
        default:
            return SexMale;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VIndividualMeasurements::Genders VIndividualMeasurements::StrToGender(const QString &sex)
{
    QStringList genders;
    genders << SexMale << SexFemale;
    switch (genders.indexOf(sex))
    {
        case 0: // SexMale
            return Male;
        case 1: // SexFemale
            return Female;
        default:
            return Male;
    }
}

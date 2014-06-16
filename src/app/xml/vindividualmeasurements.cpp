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
VIndividualMeasurements::~VIndividualMeasurements()
{}

//---------------------------------------------------------------------------------------------------------------------
Unit VIndividualMeasurements::MUnit() const
{
    const QString unit = UniqueTagText(TagUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::setUnit(const Unit &unit)
{
    setTagText(TagUnit, UnitsToStr(unit));
}

//---------------------------------------------------------------------------------------------------------------------
void VIndividualMeasurements::Measurements()
{
    //head and neck
    Measurement(headGirth_M);
    Measurement(midNeckGirth_M);
    Measurement(neckBaseGirth_M);
    Measurement(headAndNeckLength_M);
    //torso
    Measurement(centerFrontWaistLength_M);
    Measurement(centerBackWaistLength_M);
    Measurement(shoulderLength_M);
    Measurement(sideWaistLength_M);
    Measurement(trunkLength_M);
    Measurement(shoulderGirth_M);
    Measurement(upperChestGirth_M);
    Measurement(bustGirth_M);
    Measurement(underBustGirth_M);
    Measurement(waistGirth_M);
    Measurement(highHipGirth_M);
    Measurement(hipGirth_M);
    Measurement(upperFrontChestWidth_M);
    Measurement(frontChestWidth_M);
    Measurement(acrossFrontShoulderWidth_M);
    Measurement(acrossBackShoulderWidth_M);
    Measurement(upperBackWidth_M);
    Measurement(backWidth_M);
    Measurement(bustpointToBustpoint_M);
    Measurement(halterBustpointToBustpoint_M);
    Measurement(neckToBustpoint_M);
    Measurement(crotchLength_M);
    Measurement(riseHeight_M);
    Measurement(shoulderDrop_M);
    Measurement(shoulderSlopeDegrees_M);
    Measurement(frontShoulderSlopeLength_M);
    Measurement(backShoulderSlopeLength_M);
    Measurement(frontShoulderToWaistLength_M);
    Measurement(backShoulderToWaistLength_M);
    Measurement(frontNeckArc_M);
    Measurement(backNeckArc_M);
    Measurement(frontUpperChestArc_M);
    Measurement(backUpperChestArc_M);
    Measurement(frontWaistArc_M);
    Measurement(backWaistArc_M);
    Measurement(frontUpperHipArc_M);
    Measurement(backUpperHipArc_M);
    Measurement(frontHipArc_M);
    Measurement(backHipArc_M);
    Measurement(chestSlope_M);
    Measurement(backSlope_M);
    Measurement(frontWaistSlope_M);
    Measurement(backWaistSlope_M);
    Measurement(frontNeckToUpperChestHeight_M);
    Measurement(frontNeckToBustHeight_M);
    //arm
    Measurement(armscyeGirth_M);
    Measurement(elbowGirth_M);
    Measurement(upperArmGirth_M);
    Measurement(wristGirth_M);
    Measurement(scyeDepth_M);
    Measurement(shoulderAndArmLength_M);
    Measurement(underarmLength_M);
    Measurement(cervicaleToWristLength_M);
    Measurement(shoulderToElbowLength_M);
    Measurement(armLength_M);
    //hand
    Measurement(handWidth_M);
    Measurement(handLength_M);
    Measurement(handGirth_M);
    //leg
    Measurement(thighGirth_M);
    Measurement(midThighGirth_M);
    Measurement(kneeGirth_M);
    Measurement(calfGirth_M);
    Measurement(ankleGirth_M);
    Measurement(kneeHeight_M);
    Measurement(ankleHeight_M);
    //foot
    Measurement(footWidth_M);
    Measurement(footLength_M);
    //heights
    Measurement(height_M);
    Measurement(cervicaleHeight_M);
    Measurement(cervicaleToKneeHeight_M);
    Measurement(waistHeight_M);
    Measurement(highHipHeight_M);
    Measurement(hipHeight_M);
    Measurement(waistToHipHeight_M);
    Measurement(waistToKneeHeight_M);
    Measurement(crotchHeight_M);
    //extended
    Measurement(size_M);
    Measurement(heightFrontNeckBasePoint_M);
    Measurement(heightBaseNeckSidePoint_M);
    Measurement(heightShoulderPoint_M);
    Measurement(heightNipplePoint_M);
    Measurement(heightBackAngleAxilla_M);
    Measurement(heightScapularPoint_M);
    Measurement(heightUnderButtockFolds_M);
    Measurement(hipsExcludingProtrudingAbdomen_M);
    Measurement(girthFootInstep_M);
    Measurement(sideWaistToFloor_M);
    Measurement(frontWaistToFloor_M);
    Measurement(arcThroughGroinArea_M);
    Measurement(waistToPlaneSeat_M);
    Measurement(neckToRadialPoint_M);
    Measurement(neckToThirdFinger_M);
    Measurement(neckToFirstLineChestCircumference_M);
    Measurement(frontWaistLength_M);
    Measurement(arcThroughShoulderJoint_M);
    Measurement(neckToBackLineChestCircumference_M);
    Measurement(waistToNeckSide_M);
    Measurement(arcLengthUpperBody_M);
    Measurement(chestWidth_M);
    Measurement(anteroposteriorDiameterHands_M);
    Measurement(heightClavicularPoint_M);
    Measurement(heightArmholeSlash_M);
    Measurement(slashShoulderHeight_M);
    Measurement(halfGirthNeck_M);
    Measurement(halfGirthNeckForShirts_M);
    Measurement(halfGirthChestFirst_M);
    Measurement(halfGirthChestSecond_M);
    Measurement(halfGirthChestThird_M);
    Measurement(halfGirthWaist_M);
    Measurement(halfGirthHipsConsideringProtrudingAbdomen_M);
    Measurement(halfGirthHipsExcludingProtrudingAbdomen_M);
    Measurement(girthKneeFlexedFeet_M);
    Measurement(neckTransverseDiameter_M);
    Measurement(frontSlashShoulderHeight_M);
    Measurement(neckToFrontWaistLine_M);
    Measurement(handVerticalDiameter_M);
    Measurement(neckToKneePoint_M);
    Measurement(waistToKnee_M);
    Measurement(shoulderHeight_M);
    Measurement(headHeight_M);
    Measurement(bodyPosition_M);
    Measurement(arcBehindShoulderGirdle_M);
    Measurement(neckToNeckBase_M);
    Measurement(depthWaistFirst_M);
    Measurement(depthWaistSecond_M);
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

                if (MUnit() == Unit::Mm)//Convert to Cm.
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

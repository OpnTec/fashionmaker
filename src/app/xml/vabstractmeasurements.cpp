/************************************************************************
 **
 **  @file   vabstractmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 6, 2014
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

#include "vabstractmeasurements.h"

const QString VAbstractMeasurements::TagUnit   = QStringLiteral("unit");
const QString VAbstractMeasurements::AttrValue = QStringLiteral("value");

//---------------------------------------------------------------------------------------------------------------------
VAbstractMeasurements::VAbstractMeasurements(VContainer *data)
    :VDomDocument(data)
{
    SCASSERT(data != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractMeasurements::~VAbstractMeasurements()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Measurements help read all measurements.
 *
 * Virtual method that keep all common measurements name. Measurements height and size create in different way in
 * individual and standard measurements, see VIndividualMeasurements::Measurements().
 */
void VAbstractMeasurements::Measurements()
{
    //Set of measurements common for both standard.
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
    Measurement(frontWaistToUpperChest_M);
    Measurement(frontWaistToLowerBreast_M);
    Measurement(backWaistToUpperChest_M);
    Measurement(strapLength_M);
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
    Measurement(cervicaleHeight_M);
    Measurement(cervicaleToKneeHeight_M);
    Measurement(waistHeight_M);
    Measurement(highHipHeight_M);
    Measurement(hipHeight_M);
    Measurement(waistToHipHeight_M);
    Measurement(waistToKneeHeight_M);
    Measurement(crotchHeight_M);
    //extended
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
Unit VAbstractMeasurements::MUnit() const
{
    const QString unit = UniqueTagText(TagUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractMeasurements::UnitConvertor(qreal value, const Unit &from, const Unit &to)
{
    switch (from)
    {
        case Unit::Mm:
            switch (to)
            {
                case Unit::Mm:
                    return value;
                case Unit::Cm:
                    return value / 10.0;
                case Unit::Inch:
                    return value / 25.4;
                default:
                    break;
            }
            break;
        case Unit::Cm:
            switch (to)
            {
                case Unit::Mm:
                    return value * 10.0;
                case Unit::Cm:
                    return value;
                case Unit::Inch:
                    return value / 2.54;
                default:
                    break;
            }
            break;
        case Unit::Inch:
            switch (to)
            {
                case Unit::Mm:
                    return value * 25.4;
                case Unit::Cm:
                    return value * 2.54;
                case Unit::Inch:
                    return value;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMeasurements::Measurement(const QString &tag)
{
    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        qWarning()<<"Measurement" << tag <<"doesn't exist";
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
                ReadMeasurement(domElement, tag);
            }
        }
    }
}

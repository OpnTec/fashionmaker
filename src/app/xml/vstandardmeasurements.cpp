/************************************************************************
 **
 **  @file   vstandardmeasurements.cpp
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

#include "vstandardmeasurements.h"
#include <QDebug>
#include "../widgets/vapplication.h"

const QString VStandardMeasurements::TagDescription      = QStringLiteral("description");
const QString VStandardMeasurements::TagSize             = QStringLiteral("size");
const QString VStandardMeasurements::TagHeight           = QStringLiteral("height");
const QString VStandardMeasurements::AttrValue           = QStringLiteral("value");
const QString VStandardMeasurements::AttrSize_increace   = QStringLiteral("size_increace");
const QString VStandardMeasurements::AttrHeight_increase = QStringLiteral("height_increase");

//---------------------------------------------------------------------------------------------------------------------
VStandardMeasurements::VStandardMeasurements(VContainer *data):VDomDocument(data)
{}

//---------------------------------------------------------------------------------------------------------------------
Valentina::Units VStandardMeasurements::Unit()
{
    const QString unit = UniqueTagText(AttrUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
QString VStandardMeasurements::Description()
{
    const QString desc = UniqueTagText(TagDescription, "");
    if (desc.isEmpty())
    {
        qDebug()<<"Empty description in standard table."<<Q_FUNC_INFO;
    }
    return desc;
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::Measurements()
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
    Measurement(cervicaleHeight);
    Measurement(cervicaleToKneeHeight);
    Measurement(waistHeight);
    Measurement(highHipHeight);
    Measurement(hipHeight);
    Measurement(waistToHipHeight);
    Measurement(waistToKneeHeight);
    Measurement(crotchHeight);
    //extended
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
void VStandardMeasurements::Measurement(const QString &tag)
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
                const qreal value = GetParametrDouble(domElement, AttrValue, "0.0");
                const qreal size_increace = GetParametrDouble(domElement, AttrSize_increace, "0.0");
                const qreal height_increase = GetParametrDouble(domElement, AttrHeight_increase, "0.0");

                if (Unit() == Valentina::Mm)// Convert to Cm.
                {
                    data->AddMeasurement(tag, VMeasurement(value/10.0, size_increace/10.0, height_increase/10.0,
                                                           qApp->GuiText(tag), qApp->Description(tag), tag));
                }
                else// Cm or inch.
                {
                    data->AddMeasurement(tag, VMeasurement(value, size_increace, height_increase, qApp->GuiText(tag),
                                                           qApp->Description(tag), tag));
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetSize()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagSize);
    if (nodeList.isEmpty())
    {
        data->SetSize(50);
        data->SetSizeName(size);
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, AttrValue, "50.0");
                if (Unit() == Valentina::Mm)// Convert to Cm.
                {
                    value = value/10.0;
                }
                data->SetSize(value);
                data->SetSizeName(size);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetHeight()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagHeight);
    if (nodeList.isEmpty())
    {
        data->SetHeight(176);
        data->SetHeightName(height);
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, AttrValue, "176.0");
                if (Unit() == Valentina::Mm)// Convert to Cm.
                {
                    value = value / 10.0;
                }
                data->SetHeight(value);
                data->SetHeightName(height);
            }
        }
    }
}

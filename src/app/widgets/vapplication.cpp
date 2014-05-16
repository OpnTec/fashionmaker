/************************************************************************
 **
 **  @file   vapplication.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vapplication.h"
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionbadid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionwrongid.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>

const qreal VApplication::PrintDPI = 96.0;

#define DefWidth 1.2//mm

//---------------------------------------------------------------------------------------------------------------------
VApplication::VApplication(int &argc, char **argv)
    : QApplication(argc, argv), _patternUnit(Valentina::Cm), _patternType(Pattern::Individual),
      _widthMainLine(DefWidth), _widthHairLine(DefWidth/3.0), measurements(QMap<QString, VTranslation>()),
      guiTexts(QMap<QString, VTranslation>()), descriptions(QMap<QString, VTranslation>()),
      variables(QMap<QString, VTranslation>()), functions(QMap<QString, VTranslation>())
{
    InitLineWidth();
    InitMeasurements();
    InitVariables();
    InitFunctions();
}

//---------------------------------------------------------------------------------------------------------------------
// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file. Program will be terminated."));
        abort();
    }
    catch (const VExceptionBadId &e)
    {
        e.CriticalMessageBox(tr("Error bad id. Program will be terminated."));
        abort();
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value. Program will be terminated."));
        abort();
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter. Program will be terminated."));
        abort();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id. Program will be terminated."));
        abort();
    }
    catch (const VException &e)
    {
        e.CriticalMessageBox(tr("Something's wrong!!"));
        return true;
    }
    catch (std::exception& e)
    {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::toPixel(double unit) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (unit / 25.4) * PrintDPI;
        break;
    case Valentina::Cm:
        result = ((unit * 10.0) / 25.4) * PrintDPI;
        break;
    case Valentina::Inch:
        result = unit * PrintDPI;
        break;
    default:
        break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
double VApplication::fromPixel(double pix) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (pix / PrintDPI) * 25.4;
        break;
    case Valentina::Cm:
        result = ((pix / PrintDPI) * 25.4) / 10.0;
        break;
    case Valentina::Inch:
        result = pix / PrintDPI;
        break;
    default:
        break;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::pathToTables() const
{
    if (_patternType == Pattern::Individual)
    {
        return QStringLiteral("://tables/individual/individual.vit");
    }
    else
    {
        #ifdef Q_OS_WIN
            return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
        #else
            #ifdef QT_DEBUG
                return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
            #else
                QDir dir(QApplication::applicationDirPath() + QStringLiteral("/tables/standard"));
                if(dir.exist())
                {
                    return dir.absolutePath();
                }
                else
                {
                    return QStringLiteral("/usr/share/valentina/tables/standard");
                }
            #endif
        #endif
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::translationsPath() const
{
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
#else
#ifdef QT_DEBUG
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
    #else
        QDir dir(QApplication::applicationDirPath() + QStringLiteral("/translations"));
        if(dir.exist())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/translations");
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitLineWidth()
{
    switch (_patternUnit)
    {
        case Valentina::Mm:
            _widthMainLine = DefWidth;
            break;
        case Valentina::Cm:
            _widthMainLine = DefWidth/10.0;
            break;
        case Valentina::Inch:
            _widthMainLine = DefWidth/25.4;
            break;
        default:
            _widthMainLine = DefWidth;
            break;
    }
    _widthHairLine = _widthMainLine/3.0;
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitMeasurements()
{
    // head_and_neck
    InitMeasurement(headGirth, QStringLiteral("Head girth"), QStringLiteral("Around fullest part of Head"));

    InitMeasurement(midNeckGirth, QStringLiteral("Mid-neck girth"), QStringLiteral("Around middle part of Neck"));

    InitMeasurement(neckBaseGirth, QStringLiteral("Neck Base girth"), QStringLiteral("Around Neck at base"));

    InitMeasurement(headAndNeckLength, QStringLiteral("Head and Neck length"),
                    QStringLiteral("Vertical Distance from Crown to Nape"));

    // torso
    InitMeasurement(centerFrontWaistLength, QStringLiteral("Front Center length"),
                    QStringLiteral("Front Neck Center over tape at Bustline to Front Waist Center"));

    InitMeasurement(centerBackWaistLength, QStringLiteral("Back Center length"),
                    QStringLiteral("Back Neck Center to Back Waist Center"));

    InitMeasurement(shoulderLength, QStringLiteral("Shoulder length"), QStringLiteral("NeckPoint to ShoulderTip"));

    InitMeasurement(sideWaistLength, QStringLiteral("Side Waist length"), QStringLiteral("Armpit to Waist side"));

    InitMeasurement(trunkLength, QStringLiteral("Trunk length"),
        QStringLiteral("Around Body from middle of Shoulder length to BustPoint to Crotch up back to beginning point"));

    InitMeasurement(shoulderGirth, QStringLiteral("Shoulder girth"),
             QStringLiteral("Around Arms and Torso, at bicep level parallel to floor, with arms hanging at the sides"));

    InitMeasurement(upperChestGirth, QStringLiteral("Upper Chest girth"),
    QStringLiteral("Around Chest at Armfold level, will be parallel to floor across back, will not be parallel to floor"
                   " across front chest"));

    InitMeasurement(bustGirth, QStringLiteral("Bust girth"),
                    QStringLiteral("Around fullest part of Bust, parallel to floor"));

    InitMeasurement(underBustGirth, QStringLiteral("Under Bust girth"),
                    QStringLiteral("Around Chest below the Bust, parallel to floor"));

    InitMeasurement(waistGirth, QStringLiteral("Waist girth"),
    QStringLiteral("Tie a string around smallest part of waist, keep string tied while taking meaasurements. Not"
                   " usually parallel to floor for front waist or back waist."));

    InitMeasurement(highHipGirth, QStringLiteral("HighHip girth"), QStringLiteral("Around HighHip, parallel to floor"));

    InitMeasurement(hipGirth, QStringLiteral("Hip girth"), QStringLiteral("Around Hip, parallel to floor"));

    InitMeasurement(upperFrontChestWidth, QStringLiteral("Front Upper Chest width"),
                    QStringLiteral("Across Front UpperChest, smallest width from armscye to armscye"));

    InitMeasurement(frontChestWidth, QStringLiteral("Front Chest width"),
                    QStringLiteral("Across Front Chest, from armfold to armfold"));

    InitMeasurement(acrossFrontShoulderWidth, QStringLiteral("Front Across Shoulder width"),
                    QStringLiteral("From ShoulderTip to ShoulderTip, across Front"));

    InitMeasurement(acrossBackShoulderWidth, QStringLiteral("Back Across Shoulder width"),
                    QStringLiteral("From ShoulderTip to ShoulderTip, across Back"));

    InitMeasurement(acrossBackShoulderWidth, QStringLiteral("Back Upper Chest width"),
                    QStringLiteral("Across Back UpperChest, smallest width from armscye to armscye"));

    InitMeasurement(backWidth, QStringLiteral("Back Chest width"),
                    QStringLiteral("Across Back Chest, from armfold to armfold"));

    InitMeasurement(bustpointToBustpoint, QStringLiteral("BustPoint to BustPoint"),
                    QStringLiteral("Distance between BustPoints, across Chest"));

    InitMeasurement(halterBustpointToBustpoint, QStringLiteral("Halter Bustpoint to Bustpoint"),
                    QStringLiteral("Distance from Bustpoint, behind neck, down to Bustpoint"));

    InitMeasurement(neckToBustpoint, QStringLiteral("NeckPoint to BustPoint"),
                    QStringLiteral("From NeckPoint to BustPoint"));

    InitMeasurement(crotchLength, QStringLiteral("Crotch length"),
                    QStringLiteral("From Front Waist Center, down to crotch, up to Back Waist Center"));

    InitMeasurement(riseHeight, QStringLiteral("Rise height"),
                    QStringLiteral("Sit on hard chair, measure from side waist straight down to chair bottom"));

    InitMeasurement(shoulderDrop, QStringLiteral("Shoulder Drop"),
                    QStringLiteral("Vertical Distance from NeckPoint level to ShoulderTip level"));

    InitMeasurement(shoulderSlopeDegrees, QStringLiteral("Shoulder Slope degrees"),
                    QStringLiteral("Degrees of angle from NeckPoint to ShoulderTip – requires goniometer"));

    InitMeasurement(frontShoulderSlopeLength, QStringLiteral("Front Shoulder Balance"),
                    QStringLiteral("ShoulderTip to Front Waist Center"));

    InitMeasurement(backShoulderSlopeLength, QStringLiteral("Back Shoulder Balance"),
                    QStringLiteral("ShoulderTip to Back Waist Center"));

    InitMeasurement(frontShoulderToWaistLength, QStringLiteral("Front Full Length"),
                    QStringLiteral("NeckPoint straight down front chest to Waistline"));

    InitMeasurement(backShoulderToWaistLength, QStringLiteral("Back Full Length"),
                    QStringLiteral("Back NeckPoint straight down back chest to Waistline"));

    InitMeasurement(frontNeckArc, QStringLiteral("Front Neck arc"),
                    QStringLiteral("NeckPoint to NeckPoint through Front Neck Center"));

    InitMeasurement(backNeckArc, QStringLiteral("Back Neck arc"), QStringLiteral("NeckPoint to NeckPoint across Nape"));

    InitMeasurement(frontUpperChestArc, QStringLiteral("front_upper-bust_arc"), QStringLiteral("front_upper-bust_arc"));

    InitMeasurement(backUpperChestArc, QStringLiteral("Back UpperBust arc"),
                    QStringLiteral("Back UpperBust side to side"));

    InitMeasurement(frontWaistArc, QStringLiteral("Front Waist arc"), QStringLiteral("Front Waist side to side"));

    InitMeasurement(backWaistArc, QStringLiteral("Back Waist arc"), QStringLiteral("Back Waist side to side"));

    InitMeasurement(frontUpperHipArc, QStringLiteral("Front UpperHip arc"),
                    QStringLiteral("Front UpperHip side to side"));

    InitMeasurement(backUpperHipArc, QStringLiteral("Back UpperHip arc"), QStringLiteral("Back UpperHip side to side"));

    InitMeasurement(frontHipArc, QStringLiteral("Front Hip arc"), QStringLiteral("Front Hip side to side"));

    InitMeasurement(backHipArc, QStringLiteral("Back Hip arc"), QStringLiteral("Back Hip side to side"));

    InitMeasurement(chestSlope, QStringLiteral("Chest Balance"), QStringLiteral("NeckPoint to Front ArmfoldPoint"));

    InitMeasurement(backSlope, QStringLiteral("Back Balance"), QStringLiteral("NeckPoint to Back ArmfoldPoint"));

    InitMeasurement(frontWaistSlope, QStringLiteral("Front Waist Balance"),
                    QStringLiteral("NeckPoint across Front Chest to Waist side"));

    InitMeasurement(backWaistSlope, QStringLiteral("Back Waist Balance"),
                    QStringLiteral("NeckPoint across Back Chest to Waist side"));

    InitMeasurement(frontNeckToUpperChestHeight, QStringLiteral("Front UpperChest height"),
                    QStringLiteral("Front Neck Center straight down to UpperChest line"));

    InitMeasurement(frontNeckToBustHeight, QStringLiteral("Bust height"),
                    QStringLiteral("Front Neck Center straight down to Bust line"));

    // arm
    InitMeasurement(armscyeGirth, QStringLiteral("Armscye Girth"), QStringLiteral("Around Armscye"));

    InitMeasurement(elbowGirth, QStringLiteral("Elbow Girth"), QStringLiteral("Around Elbow with elbow bent"));

    InitMeasurement(upperArmGirth, QStringLiteral("Upperarm Girth"), QStringLiteral("Around UpperArm"));

    InitMeasurement(wristGirth, QStringLiteral("Wrist girth"), QStringLiteral("Around Wrist"));

    InitMeasurement(scyeDepth, QStringLiteral("Armscye depth"),
                    QStringLiteral("Nape straight down to UnderBust line (same as Back UpperBust height)"));

    InitMeasurement(shoulderAndArmLength, QStringLiteral("Shoulder and Arm length"),
                    QStringLiteral("NeckPoint to ShoulderTip to Wrist, with elbow bent and hand on hip"));

    InitMeasurement(underarmLength, QStringLiteral("Underarm length"),
                    QStringLiteral("Armpit to Wrist, with arm straight and hanging at side"));

    InitMeasurement(cervicaleToWristLength, QStringLiteral("Nape to wrist length"),
                    QStringLiteral("Nape to Wrist, with elbow bent and hand on hip"));

    InitMeasurement(shoulderToElbowLength, QStringLiteral("Elbow length"),
                    QStringLiteral("ShoulderTip to Elbow, with elbow bent and hand on hip"));

    InitMeasurement(armLength, QStringLiteral("Arm length"),
                    QStringLiteral("ShoulderTip to Wrist, with elbow bent and hand on hip"));

    // hand
    InitMeasurement(handWidth, QStringLiteral("Hand width"), QStringLiteral("Hand side to side"));

    InitMeasurement(handLength, QStringLiteral("Hand length"), QStringLiteral("Hand Middle Finger tip to wrist"));

    InitMeasurement(handGirth, QStringLiteral("Hand girth"), QStringLiteral("Around Hand"));

    // leg
    InitMeasurement(thighGirth, QStringLiteral("Thigh girth"), QStringLiteral("Around Thigh"));

    InitMeasurement(midThighGirth, QStringLiteral("Midthigh girth"), QStringLiteral("Around MidThigh"));

    InitMeasurement(kneeGirth, QStringLiteral("Knee girth"), QStringLiteral("Around Knee"));

    InitMeasurement(calfGirth, QStringLiteral("Calf girth"), QStringLiteral("Around Calf"));

    InitMeasurement(ankleGirth, QStringLiteral("Ankle girth"), QStringLiteral("Around Ankle"));

    InitMeasurement(kneeHeight, QStringLiteral("Knee height"), QStringLiteral("Knee to Floor"));

    InitMeasurement(ankleHeight, QStringLiteral("Ankle height"), QStringLiteral("Ankle to Floor"));

    // foot
    InitMeasurement(footWidth, QStringLiteral("Foot width"), QStringLiteral("Widest part of Foot side to side"));

    InitMeasurement(footLength, QStringLiteral("Foot length"),
                    QStringLiteral("Tip of Longest Toe straight to back of heel"));

    // heights
    InitMeasurement(height, QStringLiteral("Total Height"), QStringLiteral("Top of head to floor"));

    InitMeasurement(cervicaleHeight, QStringLiteral("Nape height"), QStringLiteral("Nape to Floor"));

    InitMeasurement(cervicaleToKneeHeight, QStringLiteral("Nape to knee height"), QStringLiteral("Nape to Knee"));

    InitMeasurement(waistHeight, QStringLiteral("Waist height"), QStringLiteral("Waist side to floor"));

    InitMeasurement(highHipHeight, QStringLiteral("HighHip height"), QStringLiteral("HighHip side to Floor"));

    InitMeasurement(hipHeight, QStringLiteral("Hip height"), QStringLiteral("Hip side to Floor"));

    InitMeasurement(waistToHipHeight, QStringLiteral("Waist to Hip height"), QStringLiteral("Waist side to Hip"));

    InitMeasurement(waistToKneeHeight, QStringLiteral("Waist to Knee height"), QStringLiteral("Waist side to Knee"));

    InitMeasurement(crotchHeight, QStringLiteral("Crotch height/Inseam"),
                    QStringLiteral("Crotch to Floor along inside leg"));
    //extended
    InitMeasurement(heightFrontNeckBasePoint, QStringLiteral("Height front neck base point"),
                    QStringLiteral("Height of the point base of the neck in front"));

    InitMeasurement(heightBaseNeckSidePoint, QStringLiteral("Height base neck side point"),
                    QStringLiteral("Height of the base of the neck side point"));

    InitMeasurement(heightShoulderPoint, QStringLiteral("Height shoulder point"),
                    QStringLiteral("The height of the shoulder point"));

    InitMeasurement(heightNipplePoint, QStringLiteral("Height nipple point"), QStringLiteral("Height nipple point"));

    InitMeasurement(heightBackAngleAxilla, QStringLiteral("Height back angle axilla"),
                    QStringLiteral("Height back angle axilla"));

    InitMeasurement(heightScapularPoint, QStringLiteral("Height scapular point"),
                    QStringLiteral("Height scapular point"));

    InitMeasurement(heightUnderButtockFolds, QStringLiteral("Height under buttock folds"),
                    QStringLiteral("Height under buttock folds"));

    InitMeasurement(hipsExcludingProtrudingAbdomen, QStringLiteral("Hips excluding protruding abdomen"),
                    QStringLiteral("Hips excluding protruding abdomen"));

    InitMeasurement(girthFootInstep, QStringLiteral("Girth foot instep"), QStringLiteral("Girth foot instep"));

    InitMeasurement(sideWaistToFloor, QStringLiteral("Side waist to floor"),
                    QStringLiteral("The distance from the side waist to floor"));

    InitMeasurement(frontWaistToFloor, QStringLiteral("Front waist to floor"),
                    QStringLiteral("The distance from the front waist to floor"));

    InitMeasurement(arcThroughGroinArea, QStringLiteral("Arc through groin area"),
                    QStringLiteral("Arc through groin area"));

    InitMeasurement(waistToPlaneSeat, QStringLiteral("Waist to plane seat"),
                    QStringLiteral("The distance from the waist to the plane seat"));

    InitMeasurement(neckToRadialPoint, QStringLiteral("Neck to radial point"),
                    QStringLiteral("The distance from the base of the neck to the side of the radial point"));

    InitMeasurement(neckToThirdFinger, QStringLiteral("Neck to third finger"),
                    QStringLiteral("Distance from the base of the neck side point to the end of the third finger"));

    InitMeasurement(neckToFirstLineChestCircumference, QStringLiteral("Neck to first line chest circumference"),
                    QStringLiteral("The distance from the base of the neck to the side of the first line in front of "
                                   "chest circumference"));

    InitMeasurement(frontWaistLength, QStringLiteral("Front waist length"),
                    QStringLiteral("The distance from the base of the neck to the waist side front (waist length in "
                                   "the front)"));

    InitMeasurement(arcThroughShoulderJoint, QStringLiteral("Arc through shoulder joint"),
                    QStringLiteral("Arc through the highest point of the shoulder joint"));

    InitMeasurement(neckToBackLineChestCircumference, QStringLiteral("Neck to back line chest circumference"),
                    QStringLiteral("The distance from the base of the neck to the back line of chest circumference of "
                                   "the first and the second based on ledge vanes"));

    InitMeasurement(waistToNeckSide, QStringLiteral("Waist to neck side"),
                    QStringLiteral("The distance from the waist to the back base of the neck side point"));

    InitMeasurement(arcLengthUpperBody, QStringLiteral("Arc length upper body"),
                    QStringLiteral("Arc length of the upper body through the base of the neck side point"));

    InitMeasurement(chestWidth, QStringLiteral("Chest width"), QStringLiteral("Chest width"));

    InitMeasurement(anteroposteriorDiameterHands, QStringLiteral("Anteroposterior diameter hands"),
                    QStringLiteral("Anteroposterior diameter of the hands"));

    InitMeasurement(heightClavicularPoint, QStringLiteral("Height clavicular point"),
                    QStringLiteral("Height clavicular point"));

    InitMeasurement(heightArmholeSlash, QStringLiteral("Height armhole slash"),
                    QStringLiteral("The distance from the point to the cervical level of the posterior angle of the "
                                   "front armpit (underarm height oblique)"));

    InitMeasurement(slashShoulderHeight, QStringLiteral("Slash shoulder height"),
                    QStringLiteral("Slash shoulder height"));

    InitMeasurement(halfGirthNeck, QStringLiteral("Half girth neck"), QStringLiteral("Half girth neck"));

    InitMeasurement(halfGirthNeckForShirts, QStringLiteral("Half girth neck for shirts"),
                    QStringLiteral("Half girth neck for shirts"));

    InitMeasurement(halfGirthChestFirst, QStringLiteral("Half girth chest first"),
                    QStringLiteral("Half girth chest first"));

    InitMeasurement(halfGirthChestSecond, QStringLiteral("Half girth chest second"),
                    QStringLiteral("Half girth chest second"));

    InitMeasurement(halfGirthChestThird, QStringLiteral("Half girth chest third"),
                    QStringLiteral("Half girth chest third"));

    InitMeasurement(halfGirthWaist, QStringLiteral("Half girth waist"), QStringLiteral("Half girth waist"));

    InitMeasurement(halfGirthHipsConsideringProtrudingAbdomen,
                    QStringLiteral("Half girth hips considering protruding abdomen"),
                    QStringLiteral("Half girth hips considering protruding abdomen"));

    InitMeasurement(halfGirthHipsExcludingProtrudingAbdomen,
                    QStringLiteral("Half girth hips excluding protruding abdomen"),
                    QStringLiteral("Half girth hips excluding protruding abdomen"));

    InitMeasurement(girthKneeFlexedFeet, QStringLiteral("Girth knee flexed feet"),
                    QStringLiteral("Girth knee flexed feet"));

    InitMeasurement(neckTransverseDiameter, QStringLiteral("Neck transverse diameter"),
                    QStringLiteral("Neck transverse diameter"));

    InitMeasurement(frontSlashShoulderHeight, QStringLiteral("Front slash shoulder height"),
                    QStringLiteral("Front slash shoulder height"));

    InitMeasurement(neckToFrontWaistLine, QStringLiteral("Neck to front waist line"),
                    QStringLiteral("The distance from the base of the neck to the waist line front"));

    InitMeasurement(handVerticalDiameter, QStringLiteral("Hand vertical diameter"),
                    QStringLiteral("Hand vertical diameter"));

    InitMeasurement(neckToKneePoint, QStringLiteral("Neck to knee point"),
                    QStringLiteral("Distance from neck to knee point"));

    InitMeasurement(waistToKnee, QStringLiteral("Waist to knee"),
                    QStringLiteral("The distance from the waist to the knee"));

    InitMeasurement(shoulderHeight, QStringLiteral("Shoulder height"), QStringLiteral("Shoulder height"));

    InitMeasurement(headHeight, QStringLiteral("Head height"), QStringLiteral("Head height"));

    InitMeasurement(bodyPosition, QStringLiteral("Body position"), QStringLiteral("Body position"));

    InitMeasurement(arcBehindShoulderGirdle, QStringLiteral("Arc behind shoulder girdle"),
                    QStringLiteral("Arc behind the shoulder girdle"));

    InitMeasurement(neckToNeckBase, QStringLiteral("Neck to neck base"),
                    QStringLiteral("Distance from neck point to point on the base of the neck side neck girth "
                                   "measurement line"));

    InitMeasurement(depthWaistFirst, QStringLiteral("Depth waist first"), QStringLiteral("Depth waist first"));

    InitMeasurement(depthWaistSecond, QStringLiteral("Depth waist second"), QStringLiteral("Depth waist second"));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitMeasurement(const QString &measurement, const QString &guiText, const QString &desc)
{
    const QString context = QStringLiteral("Measurements");
    const QString disambiguation1 = QStringLiteral("Short measurement name. Don't use math symbols in name!!!!");
    const QString disambiguation2 = QStringLiteral("Full measurement name");
    const QString disambiguation3 = QStringLiteral("Full measurement description");

    measurements.insert(measurement, VTranslation::translate(context, measurement, disambiguation1));
    guiTexts.insert(measurement, VTranslation::translate(context, guiText, disambiguation2));
    descriptions.insert(measurement, VTranslation::translate(context, desc, disambiguation3));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitVariables()
{
    const QString context = QStringLiteral("Variables");
    const QString disambiguation = QStringLiteral("Left symbol _ in name");

    variables.insert(line_, VTranslation::translate(context, line_, disambiguation));
    variables.insert(angleLine_, VTranslation::translate(context, angleLine_, disambiguation));
    variables.insert(arc_, VTranslation::translate(context, arc_, disambiguation));
    variables.insert(spl_, VTranslation::translate(context, spl_, disambiguation));
    variables.insert(splPath, VTranslation::translate(context, splPath, disambiguation));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::InitFunctions()
{
    const QString context = QStringLiteral("Functions");

    functions.insert(sin_F, VTranslation::translate(context, sin_F, QStringLiteral("sine function")));
    functions.insert(cos_F, VTranslation::translate(context, cos_F, QStringLiteral("cosine function")));
    functions.insert(tan_F, VTranslation::translate(context, tan_F, QStringLiteral("tangens function")));
    functions.insert(asin_F, VTranslation::translate(context, asin_F, QStringLiteral("arcus sine function")));
    functions.insert(acos_F, VTranslation::translate(context, acos_F, QStringLiteral("arcus cosine function")));
    functions.insert(atan_F, VTranslation::translate(context, atan_F, QStringLiteral("arcus tangens function")));
    functions.insert(sinh_F, VTranslation::translate(context, sinh_F, QStringLiteral("hyperbolic sine function")));
    functions.insert(cosh_F, VTranslation::translate(context, cosh_F, QStringLiteral("hyperbolic cosine")));
    functions.insert(tanh_F, VTranslation::translate(context, tanh_F, QStringLiteral("hyperbolic tangens function")));
    functions.insert(asinh_F, VTranslation::translate(context, asinh_F,
                                                      QStringLiteral("hyperbolic arcus sine function")));
    functions.insert(acosh_F, VTranslation::translate(context, acosh_F,
                                                      QStringLiteral("hyperbolic arcus tangens function")));
    functions.insert(atanh_F, VTranslation::translate(context, atanh_F,
                                                      QStringLiteral("hyperbolic arcur tangens function")));
    functions.insert(log2_F, VTranslation::translate(context, log2_F, QStringLiteral("logarithm to the base 2")));
    functions.insert(log10_F, VTranslation::translate(context, log10_F, QStringLiteral("logarithm to the base 10")));
    functions.insert(log_F, VTranslation::translate(context, log_F, QStringLiteral("logarithm to the base 10")));
    functions.insert(ln_F, VTranslation::translate(context, ln_F, QStringLiteral("logarithm to base e (2.71828...)")));
    functions.insert(exp_F, VTranslation::translate(context, exp_F, QStringLiteral("e raised to the power of x")));
    functions.insert(sqrt_F, VTranslation::translate(context, sqrt_F, QStringLiteral("square root of a value")));
    functions.insert(sign_F, VTranslation::translate(context, sign_F,
                                                     QStringLiteral("sign function -1 if x<0; 1 if x>0")));
    functions.insert(rint_F, VTranslation::translate(context, rint_F, QStringLiteral("round to nearest integer")));
    functions.insert(abs_F, VTranslation::translate(context, abs_F, QStringLiteral("absolute value")));
    functions.insert(min_F, VTranslation::translate(context, min_F, QStringLiteral("min of all arguments")));
    functions.insert(max_F, VTranslation::translate(context, max_F, QStringLiteral("max of all arguments")));
    functions.insert(sum_F, VTranslation::translate(context, sum_F, QStringLiteral("sum of all arguments")));
    functions.insert(avg_F, VTranslation::translate(context, avg_F, QStringLiteral("mean value of all arguments")));
}

//---------------------------------------------------------------------------------------------------------------------
void VApplication::setPatternUnit(const Valentina::Units &patternUnit)
{
    _patternUnit = patternUnit;
    InitLineWidth();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Measurement(const QString &measurement) const
{
    return measurements.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::GuiText(const QString &measurement) const
{
    return guiTexts.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Description(const QString &measurement) const
{
    return descriptions.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Variable(const QString &name) const
{
    return variables.value(name).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VApplication::Function(const QString &name) const
{
    return functions.value(name).translate();
}

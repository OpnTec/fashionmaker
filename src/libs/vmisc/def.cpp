/************************************************************************
 **
 **  @file   def.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2015
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

#include "def.h"

#include <QApplication>

// Keep synchronize all names with initialization in VApllication class!!!!!
//measurements
//head and neck
const QString headGirth_M                   = QStringLiteral("head_girth");
const QString midNeckGirth_M                = QStringLiteral("mid_neck_girth");
const QString neckBaseGirth_M               = QStringLiteral("neck_base_girth");
const QString headAndNeckLength_M           = QStringLiteral("head_and_neck_length");
//torso
const QString centerFrontWaistLength_M      = QStringLiteral("center_front_waist_length");
const QString centerBackWaistLength_M       = QStringLiteral("center_back_waist_length");
const QString shoulderLength_M              = QStringLiteral("shoulder_length");
const QString sideWaistLength_M             = QStringLiteral("side_waist_length");
const QString trunkLength_M                 = QStringLiteral("trunk_length");
const QString shoulderGirth_M               = QStringLiteral("shoulder_girth");
const QString upperChestGirth_M             = QStringLiteral("upper_chest_girth");
const QString bustGirth_M                   = QStringLiteral("bust_girth");
const QString underBustGirth_M              = QStringLiteral("under_bust_girth");
const QString waistGirth_M                  = QStringLiteral("waist_girth");
const QString highHipGirth_M                = QStringLiteral("high_hip_girth");
const QString hipGirth_M                    = QStringLiteral("hip_girth");
const QString upperFrontChestWidth_M        = QStringLiteral("upper_front_chest_width");
const QString frontChestWidth_M             = QStringLiteral("front_chest_width");
const QString acrossFrontShoulderWidth_M    = QStringLiteral("across_front_shoulder_width");
const QString acrossBackShoulderWidth_M     = QStringLiteral("across_back_shoulder_width");
const QString upperBackWidth_M              = QStringLiteral("upper_back_width");
const QString backWidth_M                   = QStringLiteral("back_width");
const QString bustpointToBustpoint_M        = QStringLiteral("bustpoint_to_bustpoint");
const QString halterBustpointToBustpoint_M  = QStringLiteral("halter_bustpoint_to_bustpoint");
const QString neckToBustpoint_M             = QStringLiteral("neck_to_bustpoint");
const QString crotchLength_M                = QStringLiteral("crotch_length");
const QString riseHeight_M                  = QStringLiteral("rise_height");
const QString shoulderDrop_M                = QStringLiteral("shoulder_drop");
const QString shoulderSlopeDegrees_M        = QStringLiteral("shoulder_slope_degrees");
const QString frontShoulderSlopeLength_M    = QStringLiteral("front_shoulder_slope_length");
const QString backShoulderSlopeLength_M     = QStringLiteral("back_shoulder_slope_length");
const QString frontShoulderToWaistLength_M  = QStringLiteral("front_shoulder_to_waist_length");
const QString backShoulderToWaistLength_M   = QStringLiteral("back_shoulder_to_waist_length");
const QString frontNeckArc_M                = QStringLiteral("front_neck_arc");
const QString backNeckArc_M                 = QStringLiteral("back_neck_arc");
const QString frontUpperChestArc_M          = QStringLiteral("front_upper_chest_arc");
const QString backUpperChestArc_M           = QStringLiteral("back_upper_chest_arc");
const QString frontWaistArc_M               = QStringLiteral("front_waist_arc");
const QString backWaistArc_M                = QStringLiteral("back_waist_arc");
const QString frontUpperHipArc_M            = QStringLiteral("front_upper_hip_arc");
const QString backUpperHipArc_M             = QStringLiteral("back_upper_hip_arc");
const QString frontHipArc_M                 = QStringLiteral("front_hip_arc");
const QString backHipArc_M                  = QStringLiteral("back_hip_arc");
const QString chestSlope_M                  = QStringLiteral("chest_slope");
const QString backSlope_M                   = QStringLiteral("back_slope");
const QString frontWaistSlope_M             = QStringLiteral("front_waist_slope");
const QString backWaistSlope_M              = QStringLiteral("back_waist_slope");
const QString frontNeckToUpperChestHeight_M = QStringLiteral("front_neck_to_upper_chest_height");
const QString frontNeckToBustHeight_M       = QStringLiteral("front_neck_to_bust_height");
const QString frontWaistToUpperChest_M      = QStringLiteral("front_waist_to_upper_chest");
const QString frontWaistToLowerBreast_M     = QStringLiteral("front_waist_to_lower_breast");
const QString backWaistToUpperChest_M       = QStringLiteral("back_waist_to_upper_chest");
const QString strapLength_M                 = QStringLiteral("strap_length");
//arm
const QString armscyeGirth_M                = QStringLiteral("armscye_girth");
const QString elbowGirth_M                  = QStringLiteral("elbow_girth");
const QString upperArmGirth_M               = QStringLiteral("upper_arm_girth");
const QString wristGirth_M                  = QStringLiteral("wrist_girth");
const QString scyeDepth_M                   = QStringLiteral("scye_depth");
const QString shoulderAndArmLength_M        = QStringLiteral("shoulder_and_arm_length");
const QString underarmLength_M              = QStringLiteral("underarm_length");
const QString cervicaleToWristLength_M      = QStringLiteral("cervicale_to_wrist_length");
const QString shoulderToElbowLength_M       = QStringLiteral("shoulder_to_elbow_length");
const QString armLength_M                   = QStringLiteral("arm_length");
//hand
const QString handWidth_M                   = QStringLiteral("hand_width");
const QString handLength_M                  = QStringLiteral("hand_length");
const QString handGirth_M                   = QStringLiteral("hand_girth");
//leg
const QString thighGirth_M                  = QStringLiteral("thigh_girth");
const QString midThighGirth_M               = QStringLiteral("mid_thigh_girth");
const QString kneeGirth_M                   = QStringLiteral("knee_girth");
const QString calfGirth_M                   = QStringLiteral("calf_girth");
const QString ankleGirth_M                  = QStringLiteral("ankle_girth");
const QString kneeHeight_M                  = QStringLiteral("knee_height");
const QString ankleHeight_M                 = QStringLiteral("ankle_height");
//foot
const QString footWidth_M                   = QStringLiteral("foot_width");
const QString footLength_M                  = QStringLiteral("foot_length");
//heights
const QString height_M                      = QStringLiteral("height");
const QString cervicaleHeight_M             = QStringLiteral("cervicale_height");
const QString cervicaleToKneeHeight_M       = QStringLiteral("cervicale_to_knee_height");
const QString waistHeight_M                 = QStringLiteral("waist_height");
const QString highHipHeight_M               = QStringLiteral("high_hip_height");
const QString hipHeight_M                   = QStringLiteral("hip_height");
const QString waistToHipHeight_M            = QStringLiteral("waist_to_hip_height");
const QString waistToKneeHeight_M           = QStringLiteral("waist_to_knee_height");
const QString crotchHeight_M                = QStringLiteral("crotch_height");
//extended measurements
const QString size_M                              = QStringLiteral("size");
const QString heightFrontNeckBasePoint_M          = QStringLiteral("height_front_neck_base_point");
const QString heightBaseNeckSidePoint_M           = QStringLiteral("height_base_neck_side_point");
const QString heightShoulderPoint_M               = QStringLiteral("height_shoulder_point");
const QString heightNipplePoint_M                 = QStringLiteral("height_nipple_point");
const QString heightBackAngleAxilla_M             = QStringLiteral("height_back_angle_axilla");
const QString heightScapularPoint_M               = QStringLiteral("height_scapular_point");
const QString heightUnderButtockFolds_M           = QStringLiteral("height_under_buttock_folds");
const QString hipsExcludingProtrudingAbdomen_M    = QStringLiteral("hips_excluding_protruding_abdomen");
const QString girthFootInstep_M                   = QStringLiteral("girth_foot_instep");
const QString sideWaistToFloor_M                  = QStringLiteral("side_waist_to_floor");
const QString frontWaistToFloor_M                 = QStringLiteral("front_waist_to_floor");
const QString arcThroughGroinArea_M               = QStringLiteral("arc_through_groin_area");
const QString waistToPlaneSeat_M                  = QStringLiteral("waist_to_plane_seat");
const QString neckToRadialPoint_M                 = QStringLiteral("neck_to_radial_point");
const QString neckToThirdFinger_M                 = QStringLiteral("neck_to_third_finger");
const QString neckToFirstLineChestCircumference_M = QStringLiteral("neck_to_first_line_chest_circumference");
const QString frontWaistLength_M                  = QStringLiteral("front_waist_length");
const QString arcThroughShoulderJoint_M           = QStringLiteral("arc_through_shoulder_joint");
const QString neckToBackLineChestCircumference_M  = QStringLiteral("neck_to_back_line_chest_circumference");
const QString waistToNeckSide_M                   = QStringLiteral("waist_to_neck_side");
const QString arcLengthUpperBody_M                = QStringLiteral("arc_length_upper_body");
const QString chestWidth_M                        = QStringLiteral("chest_width");
const QString anteroposteriorDiameterHands_M      = QStringLiteral("anteroposterior_diameter_hands");
const QString heightClavicularPoint_M             = QStringLiteral("height_clavicular_point");
const QString heightArmholeSlash_M                = QStringLiteral("height_armhole_slash");
const QString slashShoulderHeight_M               = QStringLiteral("slash_shoulder_height");
const QString halfGirthNeck_M                     = QStringLiteral("half_girth_neck");
const QString halfGirthNeckForShirts_M            = QStringLiteral("half_girth_neck_for_shirts");
const QString halfGirthChestFirst_M               = QStringLiteral("half_girth_chest_first");
const QString halfGirthChestSecond_M              = QStringLiteral("half_girth_chest_second");
const QString halfGirthChestThird_M               = QStringLiteral("half_girth_chest_third");
const QString halfGirthWaist_M                    = QStringLiteral("half_girth_waist");
const QString halfGirthHipsConsideringProtrudingAbdomen_M
= QStringLiteral("half_girth_hips_considering_protruding_abdomen");
const QString halfGirthHipsExcludingProtrudingAbdomen_M
= QStringLiteral("half_girth_hips_excluding_protruding_abdomen");
const QString girthKneeFlexedFeet_M               = QStringLiteral("girth_knee_flexed_feet");
const QString neckTransverseDiameter_M            = QStringLiteral("neck_transverse_diameter");
const QString frontSlashShoulderHeight_M          = QStringLiteral("front_slash_shoulder_height");
const QString neckToFrontWaistLine_M              = QStringLiteral("neck_to_front_waist_line");
const QString handVerticalDiameter_M              = QStringLiteral("hand_vertical_diameter");
const QString neckToKneePoint_M                   = QStringLiteral("neck_to_knee_point");
const QString waistToKnee_M                       = QStringLiteral("waist_to_knee");
const QString shoulderHeight_M                    = QStringLiteral("shoulder_height");
const QString headHeight_M                        = QStringLiteral("head_height");
const QString bodyPosition_M                      = QStringLiteral("body_position");
const QString arcBehindShoulderGirdle_M           = QStringLiteral("arc_behind_shoulder_girdle");
const QString neckToNeckBase_M                    = QStringLiteral("neck_to_neck_base");
const QString depthWaistFirst_M                   = QStringLiteral("depth_waist_first");
const QString depthWaistSecond_M                  = QStringLiteral("depth_waist_second");

//variables
const QString line_       = QStringLiteral("Line_");
const QString angleLine_  = QStringLiteral("AngleLine_");

//functions
const QString sin_F   = QStringLiteral("sin");
const QString cos_F   = QStringLiteral("cos");
const QString tan_F   = QStringLiteral("tan");
const QString asin_F  = QStringLiteral("asin");
const QString acos_F  = QStringLiteral("acos");
const QString atan_F  = QStringLiteral("atan");
const QString sinh_F  = QStringLiteral("sinh");
const QString cosh_F  = QStringLiteral("cosh");
const QString tanh_F  = QStringLiteral("tanh");
const QString asinh_F = QStringLiteral("asinh");
const QString acosh_F = QStringLiteral("acosh");
const QString atanh_F = QStringLiteral("atanh");
const QString log2_F  = QStringLiteral("log2");
const QString log10_F = QStringLiteral("log10");
const QString log_F   = QStringLiteral("log");
const QString ln_F    = QStringLiteral("ln");
const QString exp_F   = QStringLiteral("exp");
const QString sqrt_F  = QStringLiteral("sqrt");
const QString sign_F  = QStringLiteral("sign");
const QString rint_F  = QStringLiteral("rint");
const QString abs_F   = QStringLiteral("abs");
const QString min_F   = QStringLiteral("min");
const QString max_F   = QStringLiteral("max");
const QString sum_F   = QStringLiteral("sum");
const QString avg_F   = QStringLiteral("avg");
const QString fmod_F  = QStringLiteral("fmod");

const QStringList builInFunctions = QStringList() << sin_F  << cos_F   << tan_F  << asin_F  << acos_F  << atan_F
                                                  << sinh_F << cosh_F  << tanh_F << asinh_F << acosh_F << atanh_F
                                                  << log2_F << log10_F << log_F  << ln_F    << exp_F   << sqrt_F
                                                  << sign_F << rint_F  << abs_F  << min_F   << max_F   << sum_F
                                                  << avg_F << fmod_F;

// Postfix operators
const QString cm_Oprt = QStringLiteral("cm");
const QString mm_Oprt = QStringLiteral("mm");
const QString in_Oprt = QStringLiteral("in");

const QString cursorArrowOpenHand = QStringLiteral("://cursor/cursor-arrow-openhand.png");
const QString cursorArrowCloseHand = QStringLiteral("://cursor/cursor-arrow-closehand.png");

// From documantation: If you use QStringLiteral you should avoid declaring the same literal in multiple places: This
// furthermore blows up the binary sizes.
const QString degreeSymbol = QStringLiteral("°");

//---------------------------------------------------------------------------------------------------------------------
void SetOverrideCursor(const QString &pixmapPath, int hotX, int hotY)
{
#ifndef QT_NO_CURSOR
    QPixmap oldPixmap;
    if (QCursor *oldCursor = QGuiApplication::overrideCursor())
    {
        oldPixmap = oldCursor->pixmap();
    }
    QPixmap newPixmap(pixmapPath);

    QImage oldImage = oldPixmap.toImage();
    QImage newImage = newPixmap.toImage();

    if (oldImage != newImage )
    {
        QApplication::setOverrideCursor(QCursor(newPixmap, hotX, hotY));
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void RestoreOverrideCursor(const QString &pixmapPath)
{
#ifndef QT_NO_CURSOR
    QPixmap oldPixmap;
    if (QCursor *oldCursor = QGuiApplication::overrideCursor())
    {
        oldPixmap = oldCursor->pixmap();
    }
    QPixmap newPixmap(pixmapPath);

    QImage oldImage = oldPixmap.toImage();
    QImage newImage = newPixmap.toImage();

    if (oldImage == newImage )
    {
        QApplication::restoreOverrideCursor();
    }
#endif
}

const qreal PrintDPI = 96.0;

//---------------------------------------------------------------------------------------------------------------------
double ToPixel(double val, const Unit &unit)
{
    switch (unit)
    {
        case Unit::Mm:
            return (val / 25.4) * PrintDPI;
        case Unit::Cm:
            return ((val * 10.0) / 25.4) * PrintDPI;
        case Unit::Inch:
            return val * PrintDPI;
        case Unit::Px:
            return val;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
double FromPixel(double pix, const Unit &unit)
{
    switch (unit)
    {
        case Unit::Mm:
            return (pix / PrintDPI) * 25.4;
        case Unit::Cm:
            return ((pix / PrintDPI) * 25.4) / 10.0;
        case Unit::Inch:
            return pix / PrintDPI;
        case Unit::Px:
            return pix;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
qreal UnitConvertor(qreal value, const Unit &from, const Unit &to)
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
                case Unit::Px:
                    return (value / 25.4) * PrintDPI;
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
                case Unit::Px:
                    return ((value * 10.0) / 25.4) * PrintDPI;
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
                case Unit::Px:
                    return value * PrintDPI;
                default:
                    break;
            }
            break;
        case Unit::Px:
            switch (to)
            {
                case Unit::Mm:
                    return (value / PrintDPI) * 25.4;
                case Unit::Cm:
                    return ((value / PrintDPI) * 25.4) / 10.0;
                case Unit::Inch:
                    return value / PrintDPI;
                case Unit::Px:
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
void CheckFactor(qreal &oldFactor, const qreal &Newfactor)
{
    if (Newfactor <= 2 && Newfactor >= 0.5)
    {
        oldFactor = Newfactor;
    }
}

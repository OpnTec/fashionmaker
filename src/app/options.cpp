/************************************************************************
 **
 **  @file   options.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 5, 2014
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

#include "options.h"

//measurements
const QString headGirth                   = QStringLiteral("head_girth");
const QString midNeckGirth                = QStringLiteral("mid_neck_girth");
const QString neckBaseGirth               = QStringLiteral("neck_base_girth");
const QString headAndNeckLength           = QStringLiteral("head_and_neck_length");
const QString centerFrontWaistLength      = QStringLiteral("center_front_waist_length");
const QString centerBackWaistLength       = QStringLiteral("center_back_waist_length");
const QString shoulderLength              = QStringLiteral("shoulder_length");
const QString sideWaistLength             = QStringLiteral("side_waist_length");
const QString trunkLength                 = QStringLiteral("trunk_length");
const QString shoulderGirth               = QStringLiteral("shoulder_girth");
const QString upperChestGirth             = QStringLiteral("upper_chest_girth");
const QString bustGirth                   = QStringLiteral("bust_girth");
const QString underBustGirth              = QStringLiteral("under_bust_girth");
const QString waistGirth                  = QStringLiteral("waist_girth");
const QString highHipGirth                = QStringLiteral("high_hip_girth");
const QString hipGirth                    = QStringLiteral("hip_girth");
const QString upperFrontChestWidth        = QStringLiteral("upper_front_chest_width");
const QString frontChestWidth             = QStringLiteral("front_chest_width");
const QString acrossFrontShoulderWidth    = QStringLiteral("across_front_shoulder_width");
const QString acrossBackShoulderWidth     = QStringLiteral("across_back_shoulder_width");
const QString upperBackWidth              = QStringLiteral("upper_back_width");
const QString backWidth                   = QStringLiteral("back_width");
const QString bustpointToBustpoint        = QStringLiteral("bustpoint_to_bustpoint");
const QString halterBustpointToBustpoint  = QStringLiteral("halter_bustpoint_to_bustpoint");
const QString neckToBustpoint             = QStringLiteral("neck_to_bustpoint");
const QString crotchLength                = QStringLiteral("crotch_length");
const QString riseHeight                  = QStringLiteral("rise_height");
const QString shoulderDrop                = QStringLiteral("shoulder_drop");
const QString shoulderSlopeDegrees        = QStringLiteral("shoulder_slope_degrees");
const QString frontShoulderSlopeLength    = QStringLiteral("front_shoulder_slope_length");
const QString backShoulderSlopeLength     = QStringLiteral("back_shoulder_slope_length");
const QString frontShoulderToWaistLength  = QStringLiteral("front_shoulder_to_waist_length");
const QString backShoulderToWaistLength   = QStringLiteral("back_shoulder_to_waist_length");
const QString frontNeckArc                = QStringLiteral("front_neck_arc");
const QString backNeckArc                 = QStringLiteral("back_neck_arc");
const QString frontUpperChestArc          = QStringLiteral("front_upper_chest_arc");
const QString backUpperChestArc           = QStringLiteral("back_upper_chest_arc");
const QString frontWaistArc               = QStringLiteral("front_waist_arc");
const QString backWaistArc                = QStringLiteral("back_waist_arc");
const QString frontUpperHipArc            = QStringLiteral("front_upper_hip_arc");
const QString backUpperHipArc             = QStringLiteral("back_upper_hip_arc");
const QString frontHipArc                 = QStringLiteral("front_hip_arc");
const QString backHipArc                  = QStringLiteral("back_hip_arc");
const QString chestSlope                  = QStringLiteral("chest_slope");
const QString backSlope                   = QStringLiteral("back_slope");
const QString frontWaistSlope             = QStringLiteral("front_waist_slope");
const QString backWaistSlope              = QStringLiteral("back_waist_slope");
const QString frontNeckToUpperChestHeight = QStringLiteral("front_neck_to_upper_chest_height");
const QString frontNeckToBustHeight       = QStringLiteral("front_neck_to_bust_height");
const QString armscyeGirth                = QStringLiteral("armscye_girth");
const QString elbowGirth                  = QStringLiteral("elbow_girth");
const QString upperArmGirth               = QStringLiteral("upper_arm_girth");
const QString wristGirth                  = QStringLiteral("wrist_girth");
const QString scyeDepth                   = QStringLiteral("scye_depth");
const QString shoulderAndArmLength        = QStringLiteral("shoulder_and_arm_length");
const QString underarmLength              = QStringLiteral("underarm_length");
const QString cervicaleToWristLength      = QStringLiteral("cervicale_to_wrist_length");
const QString shoulderToElbowLength       = QStringLiteral("shoulder_to_elbow_length");
const QString armLength                   = QStringLiteral("arm_length");
const QString handWidth                   = QStringLiteral("hand_width");
const QString handLength                  = QStringLiteral("hand_length");
const QString handGirth                   = QStringLiteral("hand_girth");
const QString thighGirth                  = QStringLiteral("thigh_girth");
const QString midThighGirth               = QStringLiteral("mid_thigh_girth");
const QString kneeGirth                   = QStringLiteral("knee_girth");
const QString calfGirth                   = QStringLiteral("calf_girth");
const QString ankleGirth                  = QStringLiteral("ankle_girth");
const QString kneeHeight                  = QStringLiteral("knee_height");
const QString ankleHeight                 = QStringLiteral("ankle_height");
const QString footWidth                   = QStringLiteral("foot_width");
const QString footLength                  = QStringLiteral("foot_length");
const QString height                      = QStringLiteral("height");
const QString cervicaleHeight             = QStringLiteral("cervicale_height");
const QString cervicaleToKneeHeight       = QStringLiteral("cervicale_to_knee_height");
const QString waistHeight                 = QStringLiteral("waist_height");
const QString highHipHeight               = QStringLiteral("high_hip_height");
const QString hipHeight                   = QStringLiteral("hip_height");
const QString waistToHipHeight            = QStringLiteral("waist_to_hip_height");
const QString waistToKneeHeight           = QStringLiteral("waist_to_knee_height");
const QString crotchHeight                = QStringLiteral("crotch_height");
//extended measurements
const QString size                              = QStringLiteral("size");
const QString heightFrontNeckBasePoint          = QStringLiteral("height_front_neck_base_point");
const QString heightBaseNeckSidePoint           = QStringLiteral("height_base_neck_side_point");
const QString heightShoulderPoint               = QStringLiteral("height_shoulder_point");
const QString heightNipplePoint                 = QStringLiteral("height_nipple_point");
const QString heightBackAngleAxilla             = QStringLiteral("height_back_angle_axilla");
const QString heightScapularPoint               = QStringLiteral("height_scapular_point");
const QString heightUnderButtockFolds           = QStringLiteral("height_under_buttock_folds");
const QString hipsExcludingProtrudingAbdomen    = QStringLiteral("hips_excluding_protruding_abdomen");
const QString girthFootInstep                   = QStringLiteral("girth_foot_instep");
const QString sideWaistToFloor                  = QStringLiteral("side_waist_to_floor");
const QString frontWaistToFloor                 = QStringLiteral("front_waist_to_floor");
const QString arcThroughGroinArea               = QStringLiteral("arc_through_groin_area");
const QString waistToPlaneSeat                  = QStringLiteral("waist_to_plane_seat");
const QString neckToRadialPoint                 = QStringLiteral("neck_to_radial_point");
const QString neckToThirdFinger                 = QStringLiteral("neck_to_third_finger");
const QString neckToFirstLineChestCircumference = QStringLiteral("neck_to_first_line_chest_circumference");
const QString frontWaistLength                  = QStringLiteral("front_waist_length");
const QString arcThroughShoulderJoint           = QStringLiteral("arc_through_shoulder_joint");
const QString neckToBackLineChestCircumference  = QStringLiteral("neck_to_back_line_chest_circumference");
const QString waistToNeckSide                   = QStringLiteral("waist_to_neck_side");
const QString arcLengthUpperBody                = QStringLiteral("arc_length_upper_body");
const QString chestWidth                        = QStringLiteral("chest_width");
const QString anteroposteriorDiameterHands      = QStringLiteral("anteroposterior_diameter_hands");
const QString heightClavicularPoint             = QStringLiteral("height_clavicular_point");
const QString heightArmholeSlash                = QStringLiteral("height_armhole_slash");
const QString slashShoulderHeight               = QStringLiteral("slash_shoulder_height");
const QString halfGirthNeck                     = QStringLiteral("half_girth_neck");
const QString halfGirthNeckForShirts            = QStringLiteral("half_girth_neck_for_shirts");
const QString halfGirthChestFirst               = QStringLiteral("half_girth_chest_first");
const QString halfGirthChestSecond              = QStringLiteral("half_girth_chest_second");
const QString halfGirthChestThird               = QStringLiteral("half_girth_chest_third");
const QString halfGirthWaist                    = QStringLiteral("half_girth_waist");
const QString halfGirthHipsConsideringProtrudingAbdomen
= QStringLiteral("half_girth_hips_considering_protruding_abdomen");
const QString halfGirthHipsExcludingProtrudingAbdomen = QStringLiteral("half_girth_hips_excluding_protruding_abdomen");
const QString girthKneeFlexedFeet               = QStringLiteral("girth_knee_flexed_feet");
const QString neckTransverseDiameter            = QStringLiteral("neck_transverse_diameter");
const QString frontSlashShoulderHeight          = QStringLiteral("front_slash_shoulder_height");
const QString neckToFrontWaistLine              = QStringLiteral("neck_to_front_waist_line");
const QString handVerticalDiameter              = QStringLiteral("hand_vertical_diameter");
const QString neckToKneePoint                   = QStringLiteral("neck_to_knee_point");
const QString waistToKnee                       = QStringLiteral("waist_to_knee");
const QString shoulderHeight                    = QStringLiteral("shoulder_height");
const QString headHeight                        = QStringLiteral("head_height");
const QString bodyPosition                      = QStringLiteral("body_position");
const QString arcBehindShoulderGirdle           = QStringLiteral("arc_behind_shoulder_girdle");
const QString neckToNeckBase                    = QStringLiteral("neck_to_neck_base");
const QString depthWaistFirst                   = QStringLiteral("depth_waist_first");
const QString depthWaistSecond                  = QStringLiteral("depth_waist_second");

//variables
const QString line_      = QStringLiteral("Line_");
const QString angleLine_ = QStringLiteral("AngleLine_");
const QString arc_       = QStringLiteral("Arc_");
const QString spl_       = QStringLiteral("Spl_");
const QString splPath    = QStringLiteral("splPath");

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

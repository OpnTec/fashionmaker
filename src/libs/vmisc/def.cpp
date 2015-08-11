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

// Keep synchronize all names with initialization in VTranslateVars class!!!!!
//measurements
// Need for standard table
const QString size_M = QStringLiteral("size");
// A
const QString height_M                = QStringLiteral("height");                    // A01
const QString heightNeckBack_M        = QStringLiteral("height_neck_back");          // A02
const QString heightScapula_M         = QStringLiteral("height_scapula");            // A03
const QString heightArmpit_M          = QStringLiteral("height_armpit");             // A04
const QString heightWaistSide_M       = QStringLiteral("height_waist_side");         // A05
const QString heightHip_M             = QStringLiteral("height_hip");                // A06
const QString heightGlutealFold_M     = QStringLiteral("height_gluteal_fold");       // A07
const QString heightKnee_M            = QStringLiteral("height_knee");               // A08
const QString heightCalf_M            = QStringLiteral("height_calf");               // A09
const QString heightAnkleHigh_M       = QStringLiteral("height_ankle_high");         // A10
const QString heightAnkle_M           = QStringLiteral("height_ankle");              // A11
const QString heightHighhip_M         = QStringLiteral("height_highhip");            // A12
const QString heightWaistFront_M      = QStringLiteral("height_waist_front");        // A13
const QString heightBustpoint_M       = QStringLiteral("height_bustpoint");          // A14
const QString heightShoulderTip_M     = QStringLiteral("height_shoulder_tip");       // A15
const QString heightNeckFront_M       = QStringLiteral("height_neck_front");         // A16
const QString heightNeckSide_M        = QStringLiteral("height_neck_side");          // A17
const QString heightNeckBackToKnee_M  = QStringLiteral("height_neck_back_to_knee");  // A18
const QString heightWaistSideToKnee_M = QStringLiteral("height_waist_side_to_knee"); // A19
const QString heightWaistSideToHip_M  = QStringLiteral("height_waist_side_to_hip");  // A20
// B
const QString widthShoulder_M     = QStringLiteral("width_shoulder");       // B01
const QString widthBust_M         = QStringLiteral("width_bust");           // B02
const QString widthWaist_M        = QStringLiteral("width_waist");          // B03
const QString widthHip_M          = QStringLiteral("width_hip");            // B04
const QString widthAbdomenToHip_M = QStringLiteral("width_abdomen_to_hip"); // B05
// C
const QString indentNeckBack_M  = QStringLiteral("indent_neck_back");  // C01
const QString indentWaistBack_M = QStringLiteral("indent_waist_back"); // C02
const QString indentAnkleHigh_M = QStringLiteral("indent_ankle_high"); // C03
// D
const QString neckMidCirc_M        = QStringLiteral("neck_mid_circ");        // D01
const QString neckCirc_M           = QStringLiteral("neck_circ");            // D02
const QString highbustCirc_M       = QStringLiteral("highbust_circ");        // D03
const QString bustCirc_M           = QStringLiteral("bust_circ");            // D04
const QString lowbustCirc_M        = QStringLiteral("lowbust_circ");         // D05
const QString ribCirc_M            = QStringLiteral("rib_circ");             // D06
const QString waistCirc_M          = QStringLiteral("waist_circ");           // D07
const QString highhipCirc_M        = QStringLiteral("highhip_circ");         // D08
const QString hipCirc_M            = QStringLiteral("hip_circ");             // D09
const QString neckArcF_M           = QStringLiteral("neck_arc_f");           // D10
const QString highbustArcF_M       = QStringLiteral("highbust_arc_f");       // D11
const QString bustArcF_M           = QStringLiteral("bust_arc_f");           // D12
const QString lowbustArcF_M        = QStringLiteral("lowbust_arc_f");        // D13
const QString ribArcF_M            = QStringLiteral("rib_arc_f");            // D14
const QString waistArcF_M          = QStringLiteral("waist_arc_f");          // D15
const QString highhipArcF_M        = QStringLiteral("highhip_arc_f");        // D16
const QString hipArcF_M            = QStringLiteral("hip_arc_f");            // D17
const QString neckArcB_M           = QStringLiteral("neck_arc_b");           // D18
const QString highbustArcB_M       = QStringLiteral("highbust_arc_b");       // D19
const QString bustArcB_M           = QStringLiteral("bust_arc_b");           // D20
const QString lowbustArcB_M        = QStringLiteral("lowbust_arc_b");        // D21
const QString ribArcB_M            = QStringLiteral("rib_arc_b");            // D22
const QString waistArcB_M          = QStringLiteral("waist_arc_b");          // D23
const QString highhipArcB_M        = QStringLiteral("highhip_arc_b");        // D24
const QString hipArcB_M            = QStringLiteral("hip_arc_b");            // D25
const QString neckArcHalfF_M       = QStringLiteral("neck_arc_half_f");      // D26
const QString highbustArcHalfF_M   = QStringLiteral("highbust_arc_half_f");  // D27
const QString bustArcHalfF_M       = QStringLiteral("bust_arc_half_f");      // D28
const QString lowbustArcHalfF_M    = QStringLiteral("lowbust_arc_half_f");   // D29
const QString ribArcHalfF_M        = QStringLiteral("rib_arc_half_f");       // D30
const QString waistArcHalfF_M      = QStringLiteral("waist_arc_half_f");     // D31
const QString highhipArcHalfF_M    = QStringLiteral("highhip_arc_half_f");   // D32
const QString hipArcHalfF_M        = QStringLiteral("hip_arc_half_f");       // D33
const QString neckArcHalfB_M       = QStringLiteral("neck_arc_half_b");      // D34
const QString highbustArcHalfB_M   = QStringLiteral("highbust_arc_half_b");  // D35
const QString bustArcHalfB_M       = QStringLiteral("bust_arc_half_b");      // D36
const QString lowbustArcHalfB_M    = QStringLiteral("lowbust_arc_half_b");   // D37
const QString ribArcHalfB_M        = QStringLiteral("rib_arc_half_b");       // D38
const QString waistArcHalfB_M      = QStringLiteral("waist_arc_half_b");     // D39
const QString highhipArcHalfB_M    = QStringLiteral("highhip_arc_half_b");   // D40
const QString hipArcHalfB_M        = QStringLiteral("hip_arc_half_b");       // D41
const QString hipWithAbdomenArcF_M = QStringLiteral("hip_with_abdomen_arc"); // D42
const QString bodyArmfoldCirc_M    = QStringLiteral("body_armfold_circ");    // D43
const QString bodyBustCirc_M       = QStringLiteral("body_bust_circ");       // D44
const QString bodyTorsoCirc_M      = QStringLiteral("body_torso_circ");      // D45
// E
const QString neckFrontToWaistF_M             = QStringLiteral("neck_front_to_waist_f");             // E01
const QString neckFrontToWaistFlatF_M         = QStringLiteral("neck_front_to_waist_flat_f");        // E02
const QString armpitToWaistSide_M             = QStringLiteral("armpit_to_waist_side");              // E03
const QString shoulderTipToWaistSideF_M       = QStringLiteral("shoulder_tip_to_waist_side_f");      // E04
const QString neckSideToWaistF_M              = QStringLiteral("neck_side_to_waist_f");              // E05
const QString neckSideToWaistBustpointF_M     = QStringLiteral("neck_side_to_waist_bustpoint_f");    // E06
const QString shoulderTipToWaistSideB_M       = QStringLiteral("shoulder_tip_to_waist_side_b");      // E07
const QString neckSideToWaistB_M              = QStringLiteral("neck_side_to_waist_b");              // E08
const QString neckBackToWaistB_M              = QStringLiteral("neck_back_to_waist_b");              // E09
const QString neckSideToWaistBladepointB_M    = QStringLiteral("neck_side_to_waist_bladepoint_b");   // E10
const QString shoulderTipToArmfoldF_M         = QStringLiteral("shoulder_tip_to_armfold_f");         // E11
const QString neckSideToBustF_M               = QStringLiteral("neck_side_to_bust_f");               // E12
const QString neckSideToHighbustF_M           = QStringLiteral("neck_side_to_highbust_f");           // E13
const QString shoulderCenterToHighbustF_M     = QStringLiteral("shoulder_center_to_highbust_f");     // E14
const QString neckFrontToHighbustF_M          = QStringLiteral("neck_front_to_highbust_f");          // E15
const QString neckFrontToBustF_M              = QStringLiteral("neck_front_to_bust_f");              // E16
const QString lowbustToWaistF_M               = QStringLiteral("lowbust_to_waist_f");                // E17
const QString shoulderTipToArmfoldB_M         = QStringLiteral("shoulder_tip_to_armfold_b");         // E18
const QString neckSideToBustB_M               = QStringLiteral("neck_side_to_bust_b");               // E19
const QString neckSideToHighbustB_M           = QStringLiteral("neck_side_to_highbust_b");           // E20
const QString shoulderCenterToHighbustB_M     = QStringLiteral("shoulder_center_to_highbust_b");     // E21
const QString neckBackToHighbustB_M           = QStringLiteral("neck_back_to_highbust_b");           // E22
const QString neckBackToBustB_M               = QStringLiteral("neck_back_to_bust_b");               // E23
const QString lowbustToWaistB_M               = QStringLiteral("lowbust_to_waist_b");                // E24
const QString waistToHighhipF_M               = QStringLiteral("waist_to_highhip_f");                // E25
const QString waistToHipF_M                   = QStringLiteral("waist_to_hip_f");                    // E26
const QString waistToHighhipSide_M            = QStringLiteral("waist_to_highhip_side");             // E27
const QString waistToHighhipB_M               = QStringLiteral("waist_to_highhip_b");                // E28
const QString waistToHipB_M                   = QStringLiteral("waist_to_hip_b");                    // E29
const QString waistToHipSide_M                = QStringLiteral("waist_to_hip_side");                 // E30
const QString shoulderSlopeNeckSideLength_M   = QStringLiteral("shoulder_slope_neck_side_length");   // E31
const QString shoulderSlopeNeckSideAngle_M    = QStringLiteral("shoulder_slope_neck_side_angle");    // E32
const QString shoulderSlopeNeckBackHeight_M   = QStringLiteral("shoulder_slope_neck_back_height");   // E33
const QString shoulderSlopeNeckBackAngle_M    = QStringLiteral("shoulder_slope_neck_back_angle");    // E34
const QString shoulderSlopeShoulderTipAngle_M = QStringLiteral("shoulder_slope_shoulder_tip_angle"); // E35
const QString highbustToWaistF_M              = QStringLiteral("highbust_to_waist_f");               // E36
const QString highbustToWaistB_M              = QStringLiteral("highbust_to_waist_b");               // E37
// F
const QString shoulderLength_M                    = QStringLiteral("shoulder_length");                     // F01
const QString shoulderWidthF_M                    = QStringLiteral("shoulder_width_f");                    // F02
const QString acrossChestF_M                      = QStringLiteral("across_chest_f");                      // F03
const QString armfoldToArmfoldF_M                 = QStringLiteral("armfold_to_armfold_f");                // F04
const QString shoulderWidthB_M                    = QStringLiteral("shoulder_width_b");                    // F05
const QString acrossBackB_M                       = QStringLiteral("across_back_b");                       // F06
const QString armfoldToArmfoldB_M                 = QStringLiteral("armfold_to_armfold_b");                // F07
const QString shoulderTipToShoulderTipHalfF_M = QStringLiteral("shoulder_tip_to_shoulder_tip_half_f"); // F08
const QString acrossChestHalfF_M                 = QStringLiteral("across_chest_half_f");                 // F09
const QString shoulderTipToShoulderTipHalfB_M = QStringLiteral("shoulder_tip_to_shoulder_tip_half_b"); // F10
const QString acrossBackHalfB_M                  = QStringLiteral("across_back_half_b");                  // F11
const QString neckFrontToShoulderTipF_M           = QStringLiteral("neck_front_to_shoulder_tip_f");        // F12
const QString neckBackToShoulderTipB_M            = QStringLiteral("neck_back_to_shoulder_tip_b");         // F13
const QString neckWidth_M                         = QStringLiteral("neck_width");	                       // F14
// G
const QString bustpointToBustpoint_M       = QStringLiteral("bustpoint_to_bustpoint");        // G01
const QString bustpointToNeckSide_M        = QStringLiteral("bustpoint_to_neck_side");        // G02
const QString bustpointToLowbust_M         = QStringLiteral("bustpoint_to_lowbust");          // G03
const QString bustpointToWaist_M           = QStringLiteral("bustpoint_to_waist");            // G04
const QString bustpointToBustpointHalf_M   = QStringLiteral("bustpoint_to_bustpoint_half");   // G05
const QString bustpointToBustpointHalter_M = QStringLiteral("bustpoint_to_bustpoint_halter"); // G06
const QString bustpointToShoulderTip_M     = QStringLiteral("bustpoint_to_shoulder_tip");     // G07
const QString bustpointToWaistFront_M      = QStringLiteral("bustpoint_to_waist_front");      // G08
// H
const QString shoulderTipToWaistFront_M = QStringLiteral("shoulder_tip_to_waist_front"); // H01
const QString neckFrontToWaistSide_M    = QStringLiteral("neck_front_to_waist_side");    // H02
const QString neckSideToWaistSideF_M    = QStringLiteral("neck_side_to_waist_side_f");   // H03
const QString neckSideToArmfoldF_M      = QStringLiteral("neck_side_to_armfold_f");      // H04
const QString neckSideToArmpitF_M       = QStringLiteral("neck_side_to_armpit_f");       // H05
const QString neckSideToBustSideF_M     = QStringLiteral("neck_side_to_bust_side_f");    // H06
const QString shoulderTipToWaistBack_M  = QStringLiteral("shoulder_tip_to_waist_back");  // H07
const QString neckBackToWaistSide_M     = QStringLiteral("neck_back_to_waist_side");     // H08
const QString neckSideToWaistSideB_M    = QStringLiteral("neck_side_to_waist_side_b");   // H09
const QString neckSideToArmfoldB_M      = QStringLiteral("neck_side_to_armfold_b");      // H10
const QString neckSideToArmpitB_M       = QStringLiteral("neck_side_to_armpit_b");       // H11
const QString neckSideToBustSideB_M     = QStringLiteral("neck_side_to_bust_side_b");    // H12
// I
const QString armShoulderTipToWristBent_M   = QStringLiteral("arm_shoulder_tip_to_wrist_bent");   // I01
const QString armShoulderTipToElbowBent_M   = QStringLiteral(" arm_shoulder_tip_to_elbow_bent");  // I02
const QString armElbowToWristBent_M         = QStringLiteral("arm_elbow_to_wrist_bent");          // I03
const QString armElbowCircBent_M            = QStringLiteral("arm_elbow_circ_bent");              // I04
const QString armShoulderTipToWrist_M       = QStringLiteral("arm_shoulder_tip_to_wrist");        // I05
const QString armShoulderTipToElbow_M       = QStringLiteral("arm_shoulder_tip_to_elbow");        // I06
const QString armElbowToWrist_M             = QStringLiteral("arm_elbow_to_wrist");               // I07
const QString armArmpitToWrist_M            = QStringLiteral("arm_armpit_to_wrist");              // I08
const QString armArmpitToElbow_M            = QStringLiteral("arm_armpit_to_elbow");              // I09
const QString armElbowToWristInside_M       = QStringLiteral("arm_elbow_to_wrist_inside");        // I10
const QString armUpperCirc_M                = QStringLiteral("arm_upper_circ");                   // I11
const QString armAboveElbowCirc_M           = QStringLiteral("arm_above_elbow_circ");             // I12
const QString armElbowCirc_M                = QStringLiteral("arm_elbow_circ");                   // I13
const QString armLowerCirc_M                = QStringLiteral("arm_lower_circ");                   // I14
const QString armWristCirc_M                = QStringLiteral("arm_wrist_circ");                   // I15
const QString armShoulderTipToArmfoldLine_M = QStringLiteral("arm_shoulder_tip_to_armfold_line"); // I16
const QString armscyeCirc_M                 = QStringLiteral("armscye_circ");                     // I17
const QString armscyeLength_M               = QStringLiteral("armscye_length");                   // I18
const QString armscyeWidth_M                = QStringLiteral("armscye_width");	                  // I19
const QString armNeckSideToFingerTip_M      = QStringLiteral("arm_neck_side_to_finger_tip");      // I20
const QString armNeckSideToWrist_M          = QStringLiteral("arm_neck_side_to_wrist");           // I21
// J
const QString legCrotchToFloor_M    = QStringLiteral("leg_crotch_to_floor");     // J01
const QString legWaistSideToFloor_M = QStringLiteral("leg_waist_side_to_floor"); // J02
const QString legWaistSideToKnee_M  = QStringLiteral("leg_waist_side_to_knee");  // J03
const QString legThighUpperCirc_M   = QStringLiteral("leg_thigh_upper_circ");	 // J04
const QString legThighMidCirc_M     = QStringLiteral("leg_thigh_mid_circ");      // J05
const QString legKneeCirc_M         = QStringLiteral("leg_knee_circ");	         // J06
const QString legKneeSmallCirc_M    = QStringLiteral("leg_knee_small_circ");     // J07
const QString legCalfCirc_M         = QStringLiteral("leg_calf_circ");	         // J08
const QString legAnkleHighCirc_M    = QStringLiteral("leg_ankle_high_circ");     // J09
const QString legAnkleCirc_M        = QStringLiteral("leg_ankle_circ");          // J10
const QString legKneeCircBent_M     = QStringLiteral("leg_knee_circ_bent");	     // J11
const QString legAnkleDiagCirc_M    = QStringLiteral("leg_ankle_diag_circ");     // J12
const QString legCrotchToAnkle_M    = QStringLiteral("leg_crotch_to_ankle");     // J13
const QString legWaistSideToAnkle_M = QStringLiteral("leg_waist_side_to_ankle"); // J14
// K
const QString crotchLength_M   = QStringLiteral("crotch_length");    // K01
const QString crotchLengthB_M  = QStringLiteral("crotch_length_b");  // K02
const QString crotchLengthF_M  = QStringLiteral("crotch_length_f");  // K03
const QString riseLengthSide_M = QStringLiteral("rise_length_side"); // K04
const QString riseLengthDiag_M = QStringLiteral("rise_length_diag"); // K05
const QString riseLengthB_M    = QStringLiteral("rise_length_b");    // K06
const QString riseLengthF_M    = QStringLiteral("rise_length_f");    // K07
// L
const QString handPalmLength_M = QStringLiteral("hand_palm_length"); // L01
const QString handLength_M     = QStringLiteral("hand_length");      // L02
const QString handPalmWidth_M  = QStringLiteral("hand_palm_width");  // L03
const QString handPalmCirc_M   = QStringLiteral("hand_palm_circ");   // L04
const QString handCirc_M       = QStringLiteral("hand_circ");	     // L05
// M
const QString footWidth_M      = QStringLiteral("foot_width");       // M01
const QString footLength_M     = QStringLiteral("foot_length");      // M02
const QString footCirc_M       = QStringLiteral("foot_circ");        // M03
const QString footInstepCirc_M = QStringLiteral("foot_instep_circ"); // M04
// N
const QString headCirc_M            = QStringLiteral("head_circ");	             // N01
const QString headLength_M          = QStringLiteral("head_length");             // N02
const QString headDepth_M           = QStringLiteral("head_depth");	             // N03
const QString headWidth_M           = QStringLiteral("head_width");	             // N04
const QString headCrownToNeckBack_M = QStringLiteral("head_crown_to_neck_back"); // N05
const QString headChinToNeckBack_M  = QStringLiteral("head_chin_to_neck_back");  // N06
// O
const QString neckBackToWaistFront_M            = QStringLiteral("neck_back_to_waist_front");	           // O01
const QString waistToWaistHalter_M              = QStringLiteral("waist_to_waist_halter");	               // O02
const QString waistNaturalCirc_M                = QStringLiteral("waist_natural_circ");	                   // O03
const QString waistNaturalArcF_M                = QStringLiteral("waist_natural_arc_f");                   // O04
const QString waistNaturalArcB_M                = QStringLiteral("waist_natural_arc_b");                   // O05
const QString waistToNaturalWaistF_M            = QStringLiteral("waist_to_natural_waist_f");              // O06
const QString waistToNaturalWaistB_M            = QStringLiteral("waist_to_natural_waist_b");              // O07
const QString armNeckBackToElbowBent_M          = QStringLiteral("arm_neck_back_to_elbow_bent");           // O08
const QString armNeckBackToWristBent_M          = QStringLiteral("arm_neck_back_to_wrist_bent");           // O09
const QString armNeckSideToElbowBent_M          = QStringLiteral("arm_neck_side_to_elbow_bent");	       // O10
const QString armNeckSideToWristBent_M          = QStringLiteral("arm_neck_side_to_wrist_bent");	       // O11
const QString armAcrossBackCenterToElbowBent_M  = QStringLiteral("arm_across_back_center_to_elbow_bent");  // O12
const QString armAcrossBackCenterToWristBent_M  = QStringLiteral("arm_across_back_center_to_wrist_bent");  // O13
const QString armArmscyeBackCenterToWristBent_M = QStringLiteral("arm_armscye_back_center_to_wrist_bent"); // O14
// P
const QString armfoldToArmfoldBust_M              = QStringLiteral("armfold_to_armfold_bust");                    // P01
const QString armfoldToBustFront_M                = QStringLiteral("armfold_to_bust_front");	                  // P02
const QString neckBackToBustFront_M               = QStringLiteral("neck_back_to_bust_front");	                  // P03
const QString neckBackToArmfoldFront_M            = QStringLiteral("neck_back_to_armfold_front");                 // P04
const QString neckBackToArmfoldFrontToWaistSide_M = QStringLiteral("neck_back_to_armfold_front_to_waist_side");   // P05
const QString highbustBackOverShoulderToArmfoldFront_M
                                                = QStringLiteral("highbust_back_over_shoulder_to_armfold_front"); // P06
const QString highbustBackOverShoulderToWaistFront_M
                                                  = QStringLiteral("highbust_back_over_shoulder_to_waist_front"); // P07
const QString neckBackToArmfoldFrontToNeckBack_M  = QStringLiteral("neck_back_to_armfold_front_to_neck_back");    // P08
const QString acrossBackCenterToArmfoldFrontToAcrossBackCenter_M
                                   = QStringLiteral("across_back_center_to_armfold_front_to_across_back_center"); // P09
const QString neckBackToArmfoldFrontToHighbustBack_M
                                                 = QStringLiteral("neck_back_to_armfold_front_to_highbust_back"); // P10
const QString highbustBOverShoulderToHighbustF_M = QStringLiteral("highbust_b_over_shoulder_to_highbust_f");      // P11
const QString armscyeArc_M                       = QStringLiteral("armscye_arc");	                              // P12
// Q
const QString dartWidthShoulder_M = QStringLiteral("dart_width_shoulder"); // Q01
const QString dartWidthBust_M     = QStringLiteral("dart_width_bust");     // Q02
const QString dartWidthWaist_M    = QStringLiteral("dart_width_waist");    // Q03

// pattern making systems codes
const QString p0_S = QStringLiteral("p0");
const QString p1_S = QStringLiteral("p1");
const QString p2_S = QStringLiteral("p2");
const QString p3_S = QStringLiteral("p3");
const QString p4_S = QStringLiteral("p4");
const QString p5_S = QStringLiteral("p5");
const QString p6_S = QStringLiteral("p6");
const QString p7_S = QStringLiteral("p7");
const QString p8_S = QStringLiteral("p8");
const QString p9_S = QStringLiteral("p9");
const QString p10_S = QStringLiteral("p10");
const QString p11_S = QStringLiteral("p11");
const QString p12_S = QStringLiteral("p12");
const QString p13_S = QStringLiteral("p13");
const QString p14_S = QStringLiteral("p14");
const QString p15_S = QStringLiteral("p15");
const QString p16_S = QStringLiteral("p16");
const QString p17_S = QStringLiteral("p17");
const QString p18_S = QStringLiteral("p18");
const QString p19_S = QStringLiteral("p19");
const QString p20_S = QStringLiteral("p20");
const QString p21_S = QStringLiteral("p21");
const QString p22_S = QStringLiteral("p22");
const QString p23_S = QStringLiteral("p23");
const QString p24_S = QStringLiteral("p24");
const QString p25_S = QStringLiteral("p25");
const QString p26_S = QStringLiteral("p26");
const QString p27_S = QStringLiteral("p27");
const QString p28_S = QStringLiteral("p28");
const QString p29_S = QStringLiteral("p29");
const QString p30_S = QStringLiteral("p30");
const QString p31_S = QStringLiteral("p31");
const QString p32_S = QStringLiteral("p32");
const QString p33_S = QStringLiteral("p33");
const QString p34_S = QStringLiteral("p34");
const QString p35_S = QStringLiteral("p35");
const QString p36_S = QStringLiteral("p36");
const QString p37_S = QStringLiteral("p37");
const QString p38_S = QStringLiteral("p38");
const QString p39_S = QStringLiteral("p39");
const QString p40_S = QStringLiteral("p40");
const QString p41_S = QStringLiteral("p41");
const QString p42_S = QStringLiteral("p42");
const QString p43_S = QStringLiteral("p43");
const QString p44_S = QStringLiteral("p44");
const QString p45_S = QStringLiteral("p45");
const QString p46_S = QStringLiteral("p46");
const QString p47_S = QStringLiteral("p47");
const QString p48_S = QStringLiteral("p48");
const QString p49_S = QStringLiteral("p49");
const QString p50_S = QStringLiteral("p50");
const QString p51_S = QStringLiteral("p51");
const QString p52_S = QStringLiteral("p52");
const QString p53_S = QStringLiteral("p53");
const QString p54_S = QStringLiteral("p54");

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

const QStringList builInPostfixOperators = QStringList() << cm_Oprt
                                                         << mm_Oprt
                                                         << in_Oprt;

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

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupA()
{
    const QStringList list = QStringList() << height_M                // A01
                                           << heightNeckBack_M        // A02
                                           << heightScapula_M         // A03
                                           << heightArmpit_M          // A04
                                           << heightWaistSide_M       // A05
                                           << heightHip_M             // A06
                                           << heightGlutealFold_M     // A07
                                           << heightKnee_M            // A08
                                           << heightCalf_M            // A09
                                           << heightAnkleHigh_M       // A10
                                           << heightAnkle_M           // A11
                                           << heightHighhip_M         // A12
                                           << heightWaistFront_M      // A13
                                           << heightBustpoint_M       // A14
                                           << heightShoulderTip_M     // A15
                                           << heightNeckFront_M       // A16
                                           << heightNeckSide_M        // A17
                                           << heightNeckBackToKnee_M  // A18
                                           << heightWaistSideToKnee_M // A19
                                           << heightWaistSideToHip_M; // A20
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupB()
{
    const QStringList list = QStringList() << widthShoulder_M      // B01
                                           << widthBust_M          // B02
                                           << widthWaist_M         // B03
                                           << widthHip_M           // B04
                                           << widthAbdomenToHip_M; // B05

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupC()
{
    const QStringList list = QStringList() << indentNeckBack_M   // C01
                                           << indentWaistBack_M  // C02
                                           << indentAnkleHigh_M; // C03

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupD()
{
    const QStringList list = QStringList() << neckMidCirc_M         // D01
                                           << neckCirc_M            // D02
                                           << highbustCirc_M        // D03
                                           << bustCirc_M            // D04
                                           << lowbustCirc_M         // D05
                                           << ribCirc_M             // D06
                                           << waistCirc_M           // D07
                                           << highhipCirc_M         // D08
                                           << hipCirc_M             // D09
                                           << neckArcF_M            // D10
                                           << highbustArcF_M        // D11
                                           << bustArcF_M            // D12
                                           << lowbustArcF_M         // D13
                                           << ribArcF_M             // D14
                                           << waistArcF_M           // D15
                                           << highhipArcF_M         // D16
                                           << hipArcF_M             // D17
                                           << neckArcB_M            // D18
                                           << highbustArcB_M        // D19
                                           << bustArcB_M            // D20
                                           << lowbustArcB_M         // D21
                                           << ribArcB_M             // D22
                                           << waistArcB_M           // D23
                                           << highhipArcB_M         // D24
                                           << hipArcB_M             // D25
                                           << neckArcHalfF_M        // D26
                                           << highbustArcHalfF_M    // D27
                                           << bustArcHalfF_M        // D28
                                           << lowbustArcHalfF_M     // D29
                                           << ribArcHalfF_M         // D30
                                           << waistArcHalfF_M       // D31
                                           << highhipArcHalfF_M     // D32
                                           << hipArcHalfF_M         // D33
                                           << neckArcHalfB_M        // D34
                                           << highbustArcHalfB_M    // D35
                                           << bustArcHalfB_M        // D36
                                           << lowbustArcHalfB_M     // D37
                                           << ribArcHalfB_M         // D38
                                           << waistArcHalfB_M       // D39
                                           << highhipArcHalfB_M     // D40
                                           << hipArcHalfB_M         // D41
                                           << hipWithAbdomenArcF_M  // D42
                                           << bodyArmfoldCirc_M     // D43
                                           << bodyBustCirc_M        // D44
                                           << bodyTorsoCirc_M;      // D45

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupE()
{
    const QStringList list = QStringList() << neckFrontToWaistF_M              // E01
                                           << neckFrontToWaistFlatF_M          // E02
                                           << armpitToWaistSide_M              // E03
                                           << shoulderTipToWaistSideF_M        // E04
                                           << neckSideToWaistF_M               // E05
                                           << neckSideToWaistBustpointF_M      // E06
                                           << shoulderTipToWaistSideB_M        // E07
                                           << neckSideToWaistB_M               // E08
                                           << neckBackToWaistB_M               // E09
                                           << neckSideToWaistBladepointB_M     // E10
                                           << shoulderTipToArmfoldF_M          // E11
                                           << neckSideToBustF_M                // E12
                                           << neckSideToHighbustF_M            // E13
                                           << shoulderCenterToHighbustF_M      // E14
                                           << neckFrontToHighbustF_M           // E15
                                           << neckFrontToBustF_M               // E16
                                           << lowbustToWaistF_M                // E17
                                           << shoulderTipToArmfoldB_M          // E18
                                           << neckSideToBustB_M                // E19
                                           << neckSideToHighbustB_M            // E20
                                           << shoulderCenterToHighbustB_M      // E21
                                           << neckBackToHighbustB_M            // E22
                                           << neckBackToBustB_M                // E23
                                           << lowbustToWaistB_M                // E24
                                           << waistToHighhipF_M                // E25
                                           << waistToHipF_M                    // E26
                                           << waistToHighhipSide_M             // E27
                                           << waistToHighhipB_M                // E28
                                           << waistToHipB_M                    // E29
                                           << waistToHipSide_M                 // E30
                                           << shoulderSlopeNeckSideLength_M    // E31
                                           << shoulderSlopeNeckSideAngle_M     // E32
                                           << shoulderSlopeNeckBackHeight_M    // E33
                                           << shoulderSlopeNeckBackAngle_M     // E34
                                           << shoulderSlopeShoulderTipAngle_M  // E35
                                           << highbustToWaistF_M               // E36
                                           << highbustToWaistB_M;              // E37

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupF()
{
    const QStringList list = QStringList() << shoulderLength_M                 // F01
                                           << shoulderWidthF_M                 // F02
                                           << acrossChestF_M                   // F03
                                           << armfoldToArmfoldF_M              // F04
                                           << shoulderWidthB_M                 // F05
                                           << acrossBackB_M                    // F06
                                           << armfoldToArmfoldB_M              // F07
                                           << shoulderTipToShoulderTipHalfF_M  // F08
                                           << acrossChestHalfF_M               // F09
                                           << shoulderTipToShoulderTipHalfB_M  // F10
                                           << acrossBackHalfB_M                // F11
                                           << neckFrontToShoulderTipF_M        // F12
                                           << neckBackToShoulderTipB_M         // F13
                                           << neckWidth_M;	                   // F14

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupG()
{
    const QStringList list = QStringList() << bustpointToBustpoint_M        // G01
                                           << bustpointToNeckSide_M         // G02
                                           << bustpointToLowbust_M          // G03
                                           << bustpointToWaist_M            // G04
                                           << bustpointToBustpointHalf_M    // G05
                                           << bustpointToBustpointHalter_M  // G06
                                           << bustpointToShoulderTip_M      // G07
                                           << bustpointToWaistFront_M;      // G08

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupH()
{
    const QStringList list = QStringList() << shoulderTipToWaistFront_M  // H01
                                           << neckFrontToWaistSide_M     // H02
                                           << neckSideToWaistSideF_M     // H03
                                           << neckSideToArmfoldF_M       // H04
                                           << neckSideToArmpitF_M        // H05
                                           << neckSideToBustSideF_M      // H06
                                           << shoulderTipToWaistBack_M   // H07
                                           << neckBackToWaistSide_M      // H08
                                           << neckSideToWaistSideB_M     // H09
                                           << neckSideToArmfoldB_M       // H10
                                           << neckSideToArmpitB_M        // H11
                                           << neckSideToBustSideB_M;     // H12

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupI()
{
    const QStringList list = QStringList() << armShoulderTipToWristBent_M    // I01
                                           << armShoulderTipToElbowBent_M    // I02
                                           << armElbowToWristBent_M          // I03
                                           << armElbowCircBent_M             // I04
                                           << armShoulderTipToWrist_M        // I05
                                           << armShoulderTipToElbow_M        // I06
                                           << armElbowToWrist_M              // I07
                                           << armArmpitToWrist_M             // I08
                                           << armArmpitToElbow_M             // I09
                                           << armElbowToWristInside_M        // I10
                                           << armUpperCirc_M                 // I11
                                           << armAboveElbowCirc_M            // I12
                                           << armElbowCirc_M                 // I13
                                           << armLowerCirc_M                 // I14
                                           << armWristCirc_M                 // I15
                                           << armShoulderTipToArmfoldLine_M  // I16
                                           << armscyeCirc_M                  // I17
                                           << armscyeLength_M                // I18
                                           << armscyeWidth_M                 // I19
                                           << armNeckSideToFingerTip_M       // I20
                                           << armNeckSideToWrist_M;          // I21

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupJ()
{
    const QStringList list = QStringList() << legCrotchToFloor_M     // J01
                                           << legWaistSideToFloor_M  // J02
                                           << legWaistSideToKnee_M   // J03
                                           << legThighUpperCirc_M    // J04
                                           << legThighMidCirc_M      // J05
                                           << legKneeCirc_M          // J06
                                           << legKneeSmallCirc_M     // J07
                                           << legCalfCirc_M          // J08
                                           << legAnkleHighCirc_M     // J09
                                           << legAnkleCirc_M         // J10
                                           << legKneeCircBent_M      // J11
                                           << legAnkleDiagCirc_M     // J12
                                           << legCrotchToAnkle_M     // J13
                                           << legWaistSideToAnkle_M; // J14

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupK()
{
    const QStringList list = QStringList() << crotchLength_M    // K01
                                           << crotchLengthB_M   // K02
                                           << crotchLengthF_M   // K03
                                           << riseLengthSide_M  // K04
                                           << riseLengthDiag_M  // K05
                                           << riseLengthB_M     // K06
                                           << riseLengthF_M;    // K07

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupL()
{
    const QStringList list = QStringList() << handPalmLength_M  // L01
                                           << handLength_M      // L02
                                           << handPalmWidth_M   // L03
                                           << handPalmCirc_M    // L04
                                           << handCirc_M;       // L05

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupM()
{
    const QStringList list = QStringList() << footWidth_M       // M01
                                           << footLength_M      // M02
                                           << footCirc_M        // M03
                                           << footInstepCirc_M; // M04

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupN()
{
    const QStringList list = QStringList() << headCirc_M 	        // N01
                                           << headLength_M          // N02
                                           << headDepth_M 	        // N03
                                           << headWidth_M 	        // N04
                                           << headCrownToNeckBack_M // N05
                                           << headChinToNeckBack_M; // N06

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupO()
{
    const QStringList list = QStringList() << neckBackToWaistFront_M 	         // O01
                                           << waistToWaistHalter_M 	             // O02
                                           << waistNaturalCirc_M 	             // O03
                                           << waistNaturalArcF_M                 // O04
                                           << waistNaturalArcB_M                 // O05
                                           << waistToNaturalWaistF_M             // O06
                                           << waistToNaturalWaistB_M             // O07
                                           << armNeckBackToElbowBent_M           // O08
                                           << armNeckBackToWristBent_M           // O09
                                           << armNeckSideToElbowBent_M 	         // O10
                                           << armNeckSideToWristBent_M 	         // O11
                                           << armAcrossBackCenterToElbowBent_M   // O12
                                           << armAcrossBackCenterToWristBent_M   // O13
                                           << armArmscyeBackCenterToWristBent_M; // O14

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupP()
{
    const QStringList list = QStringList() << armfoldToArmfoldBust_M                             // P01
                                           << armfoldToBustFront_M 	                             // P02
                                           << neckBackToBustFront_M 	                         // P03
                                           << neckBackToArmfoldFront_M 	                         // P04
                                           << neckBackToArmfoldFrontToWaistSide_M                // P05
                                           << highbustBackOverShoulderToArmfoldFront_M           // P06
                                           << highbustBackOverShoulderToWaistFront_M             // P07
                                           << neckBackToArmfoldFrontToNeckBack_M                 // P08
                                           << acrossBackCenterToArmfoldFrontToAcrossBackCenter_M // P09
                                           << neckBackToArmfoldFrontToHighbustBack_M             // P10
                                           << highbustBOverShoulderToHighbustF_M 	             // P11
                                           << armscyeArc_M;	                                     // P12

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupQ()
{
    const QStringList list = QStringList() << dartWidthShoulder_M // Q01
                                           << dartWidthBust_M 	  // Q02
                                           << dartWidthWaist_M;   // Q03

    return list;
}

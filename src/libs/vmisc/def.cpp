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
#include "vabstractapplication.h"
#include "../vpatterndb/vtranslatevars.h"

#include <QComboBox>

// Keep synchronize all names with initialization in VTranslateVars class!!!!!
//measurements
// Need for standard table
const QString size_M = QStringLiteral("size");
// A
const QString height_M                    = QStringLiteral("height");                         // A01
const QString heightNeckBack_M            = QStringLiteral("height_neck_back");               // A02
const QString heightScapula_M             = QStringLiteral("height_scapula");                 // A03
const QString heightArmpit_M              = QStringLiteral("height_armpit");                  // A04
const QString heightWaistSide_M           = QStringLiteral("height_waist_side");              // A05
const QString heightHip_M                 = QStringLiteral("height_hip");                     // A06
const QString heightGlutealFold_M         = QStringLiteral("height_gluteal_fold");            // A07
const QString heightKnee_M                = QStringLiteral("height_knee");                    // A08
const QString heightCalf_M                = QStringLiteral("height_calf");                    // A09
const QString heightAnkleHigh_M           = QStringLiteral("height_ankle_high");              // A10
const QString heightAnkle_M               = QStringLiteral("height_ankle");                   // A11
const QString heightHighhip_M             = QStringLiteral("height_highhip");                 // A12
const QString heightWaistFront_M          = QStringLiteral("height_waist_front");             // A13
const QString heightBustpoint_M           = QStringLiteral("height_bustpoint");               // A14
const QString heightShoulderTip_M         = QStringLiteral("height_shoulder_tip");            // A15
const QString heightNeckFront_M           = QStringLiteral("height_neck_front");              // A16
const QString heightNeckSide_M            = QStringLiteral("height_neck_side");               // A17
const QString heightNeckBackToKnee_M      = QStringLiteral("height_neck_back_to_knee");       // A18
const QString heightWaistSideToKnee_M     = QStringLiteral("height_waist_side_to_knee");      // A19
const QString heightWaistSideToHip_M      = QStringLiteral("height_waist_side_to_hip");       // A20
const QString heightKneeToAnkle_M         = QStringLiteral("height_knee_to_ankle");           // A21
const QString heightNeckBackToWaistSide_M = QStringLiteral("height_neck_back_to_waist_side"); // A22
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
const QString handPalmLength_M = QStringLiteral("hand_palm_length"); // D01
const QString handLength_M     = QStringLiteral("hand_length");      // D02
const QString handPalmWidth_M  = QStringLiteral("hand_palm_width");  // D03
const QString handPalmCirc_M   = QStringLiteral("hand_palm_circ");   // D04
const QString handCirc_M       = QStringLiteral("hand_circ");        // D05
// E
const QString footWidth_M      = QStringLiteral("foot_width");       // E01
const QString footLength_M     = QStringLiteral("foot_length");      // E02
const QString footCirc_M       = QStringLiteral("foot_circ");        // E03
const QString footInstepCirc_M = QStringLiteral("foot_instep_circ"); // E04
// F
const QString headCirc_M            = QStringLiteral("head_circ");	             // F01
const QString headLength_M          = QStringLiteral("head_length");             // F02
const QString headDepth_M           = QStringLiteral("head_depth");	             // F03
const QString headWidth_M           = QStringLiteral("head_width");	             // F04
const QString headCrownToNeckBack_M = QStringLiteral("head_crown_to_neck_back"); // F05
const QString headChinToNeckBack_M  = QStringLiteral("head_chin_to_neck_back");  // F06
// G
const QString neckMidCirc_M        = QStringLiteral("neck_mid_circ");          // G01
const QString neckCirc_M           = QStringLiteral("neck_circ");              // G02
const QString highbustCirc_M       = QStringLiteral("highbust_circ");          // G03
const QString bustCirc_M           = QStringLiteral("bust_circ");              // G04
const QString lowbustCirc_M        = QStringLiteral("lowbust_circ");           // G05
const QString ribCirc_M            = QStringLiteral("rib_circ");               // G06
const QString waistCirc_M          = QStringLiteral("waist_circ");             // G07
const QString highhipCirc_M        = QStringLiteral("highhip_circ");           // G08
const QString hipCirc_M            = QStringLiteral("hip_circ");               // G09
const QString neckArcF_M           = QStringLiteral("neck_arc_f");             // G10
const QString highbustArcF_M       = QStringLiteral("highbust_arc_f");         // G11
const QString bustArcF_M           = QStringLiteral("bust_arc_f");             // G12
const QString lowbustArcF_M        = QStringLiteral("lowbust_arc_f");          // G13
const QString ribArcF_M            = QStringLiteral("rib_arc_f");              // G14
const QString waistArcF_M          = QStringLiteral("waist_arc_f");            // G15
const QString highhipArcF_M        = QStringLiteral("highhip_arc_f");          // G16
const QString hipArcF_M            = QStringLiteral("hip_arc_f");              // G17
const QString neckArcHalfF_M       = QStringLiteral("neck_arc_half_f");        // G18
const QString highbustArcHalfF_M   = QStringLiteral("highbust_arc_half_f");    // G19
const QString bustArcHalfF_M       = QStringLiteral("bust_arc_half_f");        // G20
const QString lowbustArcHalfF_M    = QStringLiteral("lowbust_arc_half_f");     // G21
const QString ribArcHalfF_M        = QStringLiteral("rib_arc_half_f");         // G22
const QString waistArcHalfF_M      = QStringLiteral("waist_arc_half_f");       // G23
const QString highhipArcHalfF_M    = QStringLiteral("highhip_arc_half_f");     // G24
const QString hipArcHalfF_M        = QStringLiteral("hip_arc_half_f");         // G25
const QString neckArcB_M           = QStringLiteral("neck_arc_b");             // G26
const QString highbustArcB_M       = QStringLiteral("highbust_arc_b");         // G27
const QString bustArcB_M           = QStringLiteral("bust_arc_b");             // G28
const QString lowbustArcB_M        = QStringLiteral("lowbust_arc_b");          // G29
const QString ribArcB_M            = QStringLiteral("rib_arc_b");              // G30
const QString waistArcB_M          = QStringLiteral("waist_arc_b");            // G31
const QString highhipArcB_M        = QStringLiteral("highhip_arc_b");          // G32
const QString hipArcB_M            = QStringLiteral("hip_arc_b");              // G33
const QString neckArcHalfB_M       = QStringLiteral("neck_arc_half_b");        // G34
const QString highbustArcHalfB_M   = QStringLiteral("highbust_arc_half_b");    // G35
const QString bustArcHalfB_M       = QStringLiteral("bust_arc_half_b");        // G36
const QString lowbustArcHalfB_M    = QStringLiteral("lowbust_arc_half_b");     // G37
const QString ribArcHalfB_M        = QStringLiteral("rib_arc_half_b");         // G38
const QString waistArcHalfB_M      = QStringLiteral("waist_arc_half_b");       // G39
const QString highhipArcHalfB_M    = QStringLiteral("highhip_arc_half_b");     // G40
const QString hipArcHalfB_M        = QStringLiteral("hip_arc_half_b");         // G41
const QString hipWithAbdomenArcF_M = QStringLiteral("hip_with_abdomen_arc_f"); // G42
const QString bodyArmfoldCirc_M    = QStringLiteral("body_armfold_circ");      // G43
const QString bodyBustCirc_M       = QStringLiteral("body_bust_circ");         // G44
const QString bodyTorsoCirc_M      = QStringLiteral("body_torso_circ");        // G45
// H
const QString neckFrontToWaistF_M             = QStringLiteral("neck_front_to_waist_f");             // H01
const QString neckFrontToWaistFlatF_M         = QStringLiteral("neck_front_to_waist_flat_f");        // H02
const QString armpitToWaistSide_M             = QStringLiteral("armpit_to_waist_side");              // H03
const QString shoulderTipToWaistSideF_M       = QStringLiteral("shoulder_tip_to_waist_side_f");      // H04
const QString neckSideToWaistF_M              = QStringLiteral("neck_side_to_waist_f");              // H05
const QString neckSideToWaistBustpointF_M     = QStringLiteral("neck_side_to_waist_bustpoint_f");    // H06
const QString neckFrontToHighbustF_M          = QStringLiteral("neck_front_to_highbust_f");          // H07
const QString highbustToWaistF_M              = QStringLiteral("highbust_to_waist_f");               // H08
const QString neckFrontToBustF_M              = QStringLiteral("neck_front_to_bust_f");              // H09
const QString bustToWaistF_M                  = QStringLiteral("bust_to_waist_f");                   // H10
const QString lowbustToWaistF_M               = QStringLiteral("lowbust_to_waist_f");                // H11
const QString ribToWaistSide_M                = QStringLiteral("rib_to_waist_side");                 // H12
const QString shoulderTipToArmfoldF_M         = QStringLiteral("shoulder_tip_to_armfold_f");         // H13
const QString neckSideToBustF_M               = QStringLiteral("neck_side_to_bust_f");               // H14
const QString neckSideToHighbustF_M           = QStringLiteral("neck_side_to_highbust_f");           // H15
const QString shoulderCenterToHighbustF_M     = QStringLiteral("shoulder_center_to_highbust_f");     // H16
const QString shoulderTipToWaistSideB_M       = QStringLiteral("shoulder_tip_to_waist_side_b");      // H17
const QString neckSideToWaistB_M              = QStringLiteral("neck_side_to_waist_b");              // H18
const QString neckBackToWaistB_M              = QStringLiteral("neck_back_to_waist_b");              // H19
const QString neckSideToWaistScapulaB_M       = QStringLiteral("neck_side_to_waist_scapula_b");      // H20
const QString neckBackToHighbustB_M           = QStringLiteral("neck_back_to_highbust_b");           // H21
const QString highbustToWaistB_M              = QStringLiteral("highbust_to_waist_b");               // H22
const QString neckBackToBustB_M               = QStringLiteral("neck_back_to_bust_b");               // H23
const QString bustToWaistB_M                  = QStringLiteral("bust_to_waist_b");                   // H24
const QString lowbustToWaistB_M               = QStringLiteral("lowbust_to_waist_b");                // H25
const QString shoulderTipToArmfoldB_M         = QStringLiteral("shoulder_tip_to_armfold_b");         // H26
const QString neckSideToBustB_M               = QStringLiteral("neck_side_to_bust_b");               // H27
const QString neckSideToHighbustB_M           = QStringLiteral("neck_side_to_highbust_b");           // H28
const QString shoulderCenterToHighbustB_M     = QStringLiteral("shoulder_center_to_highbust_b");     // H29
const QString waistToHighhipF_M               = QStringLiteral("waist_to_highhip_f");                // H30
const QString waistToHipF_M                   = QStringLiteral("waist_to_hip_f");                    // H31
const QString waistToHighhipSide_M            = QStringLiteral("waist_to_highhip_side");             // H32
const QString waistToHighhipB_M               = QStringLiteral("waist_to_highhip_b");                // H33
const QString waistToHipB_M                   = QStringLiteral("waist_to_hip_b");                    // H34
const QString waistToHipSide_M                = QStringLiteral("waist_to_hip_side");                 // H35
const QString shoulderSlopeNeckSideAngle_M    = QStringLiteral("shoulder_slope_neck_side_angle");    // H36
const QString shoulderSlopeNeckSideLength_M   = QStringLiteral("shoulder_slope_neck_side_length");   // H37
const QString shoulderSlopeNeckBackAngle_M    = QStringLiteral("shoulder_slope_neck_back_angle");    // H38
const QString shoulderSlopeNeckBackHeight_M   = QStringLiteral("shoulder_slope_neck_back_height");   // H39
const QString shoulderSlopeShoulderTipAngle_M = QStringLiteral("shoulder_slope_shoulder_tip_angle"); // H40
// I
const QString shoulderLength_M                = QStringLiteral("shoulder_length");                     // I01
const QString shoulderTipToShoulderTipF_M     = QStringLiteral("shoulder_tip_to_shoulder_tip_f");      // I02
const QString acrossChestF_M                  = QStringLiteral("across_chest_f");                      // I03
const QString armfoldToArmfoldF_M             = QStringLiteral("armfold_to_armfold_f");                // I04
const QString shoulderTipToShoulderTipHalfF_M = QStringLiteral("shoulder_tip_to_shoulder_tip_half_f"); // I05
const QString acrossChestHalfF_M              = QStringLiteral("across_chest_half_f");                 // I06
const QString shoulderTipToShoulderTipB_M     = QStringLiteral("shoulder_tip_to_shoulder_tip_b");      // I07
const QString acrossBackB_M                   = QStringLiteral("across_back_b");                       // I08
const QString armfoldToArmfoldB_M             = QStringLiteral("armfold_to_armfold_b");                // I09
const QString shoulderTipToShoulderTipHalfB_M = QStringLiteral("shoulder_tip_to_shoulder_tip_half_b"); // I10
const QString acrossBackHalfB_M               = QStringLiteral("across_back_half_b");                  // I11
const QString neckFrontToShoulderTipF_M       = QStringLiteral("neck_front_to_shoulder_tip_f");        // I12
const QString neckBackToShoulderTipB_M        = QStringLiteral("neck_back_to_shoulder_tip_b");         // I13
const QString neckWidth_M                     = QStringLiteral("neck_width");                          // I14
// J
const QString bustpointToBustpoint_M       = QStringLiteral("bustpoint_to_bustpoint");        // J01
const QString bustpointToNeckSide_M        = QStringLiteral("bustpoint_to_neck_side");        // J02
const QString bustpointToLowbust_M         = QStringLiteral("bustpoint_to_lowbust");          // J03
const QString bustpointToWaist_M           = QStringLiteral("bustpoint_to_waist");            // J04
const QString bustpointToBustpointHalf_M   = QStringLiteral("bustpoint_to_bustpoint_half");   // J05
const QString bustpointNeckSideToWaist_M   = QStringLiteral("bustpoint_neck_side_to_waist");  // J06
const QString bustpointToShoulderTip_M     = QStringLiteral("bustpoint_to_shoulder_tip");     // J07
const QString bustpointToWaistFront_M      = QStringLiteral("bustpoint_to_waist_front");      // J08
const QString bustpointToBustpointHalter_M = QStringLiteral("bustpoint_to_bustpoint_halter"); // J09
// K
const QString shoulderTipToWaistFront_M       = QStringLiteral("shoulder_tip_to_waist_front");        // K01
const QString neckFrontToWaistSide_M          = QStringLiteral("neck_front_to_waist_side");           // K02
const QString neckSideToWaistSideF_M          = QStringLiteral("neck_side_to_waist_side_f");          // K03
const QString shoulderTipToWaistBack_M        = QStringLiteral("shoulder_tip_to_waist_back");         // K04
const QString shoulderTipToWaistB_1inOffset_M = QStringLiteral("shoulder_tip_to_waist_b_1in_offset"); // K05
const QString neckBackToWaistSide_M           = QStringLiteral("neck_back_to_waist_side");            // K06
const QString neckSideToWaistSideB_M          = QStringLiteral("neck_side_to_waist_side_b");          // K07
const QString neckSideToArmfoldF_M            = QStringLiteral("neck_side_to_armfold_f");             // K08
const QString neckSideToArmpitF_M             = QStringLiteral("neck_side_to_armpit_f");              // K09
const QString neckSideToBustSideF_M           = QStringLiteral("neck_side_to_bust_side_f");           // K10
const QString neckSideToArmfoldB_M            = QStringLiteral("neck_side_to_armfold_b");             // K11
const QString neckSideToArmpitB_M             = QStringLiteral("neck_side_to_armpit_b");              // K12
const QString neckSideToBustSideB_M           = QStringLiteral("neck_side_to_bust_side_b");           // K13
// L
const QString armShoulderTipToWristBent_M   = QStringLiteral("arm_shoulder_tip_to_wrist_bent");   // L01
const QString armShoulderTipToElbowBent_M   = QStringLiteral("arm_shoulder_tip_to_elbow_bent");   // L02
const QString armElbowToWristBent_M         = QStringLiteral("arm_elbow_to_wrist_bent");          // L03
const QString armElbowCircBent_M            = QStringLiteral("arm_elbow_circ_bent");              // L04
const QString armShoulderTipToWrist_M       = QStringLiteral("arm_shoulder_tip_to_wrist");        // L05
const QString armShoulderTipToElbow_M       = QStringLiteral("arm_shoulder_tip_to_elbow");        // L06
const QString armElbowToWrist_M             = QStringLiteral("arm_elbow_to_wrist");               // L07
const QString armArmpitToWrist_M            = QStringLiteral("arm_armpit_to_wrist");              // L08
const QString armArmpitToElbow_M            = QStringLiteral("arm_armpit_to_elbow");              // L09
const QString armElbowToWristInside_M       = QStringLiteral("arm_elbow_to_wrist_inside");        // L10
const QString armUpperCirc_M                = QStringLiteral("arm_upper_circ");                   // L11
const QString armAboveElbowCirc_M           = QStringLiteral("arm_above_elbow_circ");             // L12
const QString armElbowCirc_M                = QStringLiteral("arm_elbow_circ");                   // L13
const QString armLowerCirc_M                = QStringLiteral("arm_lower_circ");                   // L14
const QString armWristCirc_M                = QStringLiteral("arm_wrist_circ");                   // L15
const QString armShoulderTipToArmfoldLine_M = QStringLiteral("arm_shoulder_tip_to_armfold_line"); // L16
const QString armNeckSideToWrist_M          = QStringLiteral("arm_neck_side_to_wrist");           // L17
const QString armNeckSideToFingerTip_M      = QStringLiteral("arm_neck_side_to_finger_tip");      // L18
const QString armscyeCirc_M                 = QStringLiteral("armscye_circ");                     // L19
const QString armscyeLength_M               = QStringLiteral("armscye_length");                   // L20
const QString armscyeWidth_M                = QStringLiteral("armscye_width");	                  // L21
// M
const QString legCrotchToFloor_M    = QStringLiteral("leg_crotch_to_floor");     // M01
const QString legWaistSideToFloor_M = QStringLiteral("leg_waist_side_to_floor"); // M02
const QString legThighUpperCirc_M   = QStringLiteral("leg_thigh_upper_circ");	 // M03
const QString legThighMidCirc_M     = QStringLiteral("leg_thigh_mid_circ");      // M04
const QString legKneeCirc_M         = QStringLiteral("leg_knee_circ");	         // M05
const QString legKneeSmallCirc_M    = QStringLiteral("leg_knee_small_circ");     // M06
const QString legCalfCirc_M         = QStringLiteral("leg_calf_circ");	         // M07
const QString legAnkleHighCirc_M    = QStringLiteral("leg_ankle_high_circ");     // M08
const QString legAnkleCirc_M        = QStringLiteral("leg_ankle_circ");          // M09
const QString legKneeCircBent_M     = QStringLiteral("leg_knee_circ_bent");	     // M10
const QString legAnkleDiagCirc_M    = QStringLiteral("leg_ankle_diag_circ");     // M11
const QString legCrotchToAnkle_M    = QStringLiteral("leg_crotch_to_ankle");     // M12
const QString legWaistSideToAnkle_M = QStringLiteral("leg_waist_side_to_ankle"); // M13
const QString legWaistSideToKnee_M  = QStringLiteral("leg_waist_side_to_knee");  // M14
// N
const QString crotchLength_M   = QStringLiteral("crotch_length");    // N01
const QString crotchLengthB_M  = QStringLiteral("crotch_length_b");  // N02
const QString crotchLengthF_M  = QStringLiteral("crotch_length_f");  // N03
const QString riseLengthSide_M = QStringLiteral("rise_length_side"); // N04
const QString riseLengthDiag_M = QStringLiteral("rise_length_diag"); // N05
const QString riseLengthB_M    = QStringLiteral("rise_length_b");    // N06
const QString riseLengthF_M    = QStringLiteral("rise_length_f");    // N07
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
const QString neckBackToBustFront_M               = QStringLiteral("neck_back_to_bust_front");	                  // P01
const QString neckBackToArmfoldFront_M            = QStringLiteral("neck_back_to_armfold_front");                 // P02
const QString neckBackToArmfoldFrontToWaistSide_M = QStringLiteral("neck_back_to_armfold_front_to_waist_side");   // P03
const QString highbustBackOverShoulderToArmfoldFront_M
                                                = QStringLiteral("highbust_back_over_shoulder_to_armfold_front"); // P04
const QString highbustBackOverShoulderToWaistFront_M
                                                  = QStringLiteral("highbust_back_over_shoulder_to_waist_front"); // P05
const QString neckBackToArmfoldFrontToNeckBack_M  = QStringLiteral("neck_back_to_armfold_front_to_neck_back");    // P06
const QString acrossBackCenterToArmfoldFrontToAcrossBackCenter_M
                                   = QStringLiteral("across_back_center_to_armfold_front_to_across_back_center"); // P07
const QString neckBackToArmfoldFrontToHighbustBack_M
                                                 = QStringLiteral("neck_back_to_armfold_front_to_highbust_back"); // P08
const QString armfoldToArmfoldBust_M              = QStringLiteral("armfold_to_armfold_bust");                    // P09
const QString armfoldToBustFront_M                = QStringLiteral("armfold_to_bust_front");	                  // P10
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
const QString p998_S = QStringLiteral("p998");

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
#else
    Q_UNUSED(pixmapPath);
    Q_UNUSED(hotX);
    Q_UNUSED(hotY);
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
#else
    Q_UNUSED(pixmapPath);
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
    const QStringList list = QStringList() << height_M                     // A01
                                           << heightNeckBack_M             // A02
                                           << heightScapula_M              // A03
                                           << heightArmpit_M               // A04
                                           << heightWaistSide_M            // A05
                                           << heightHip_M                  // A06
                                           << heightGlutealFold_M          // A07
                                           << heightKnee_M                 // A08
                                           << heightCalf_M                 // A09
                                           << heightAnkleHigh_M            // A10
                                           << heightAnkle_M                // A11
                                           << heightHighhip_M              // A12
                                           << heightWaistFront_M           // A13
                                           << heightBustpoint_M            // A14
                                           << heightShoulderTip_M          // A15
                                           << heightNeckFront_M            // A16
                                           << heightNeckSide_M             // A17
                                           << heightNeckBackToKnee_M       // A18
                                           << heightWaistSideToKnee_M      // A19
                                           << heightWaistSideToHip_M       // A20
                                           << heightKneeToAnkle_M          // A21
                                           << heightNeckBackToWaistSide_M; // A22
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
    const QStringList list = QStringList() << handPalmLength_M // D01
                                           << handLength_M     // D02
                                           << handPalmWidth_M  // D03
                                           << handPalmCirc_M   // D04
                                           << handCirc_M;      // D05

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupE()
{
    const QStringList list = QStringList() << footWidth_M       // E01
                                           << footLength_M      // E02
                                           << footCirc_M        // E03
                                           << footInstepCirc_M; // E04

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupF()
{
    const QStringList list = QStringList() << headCirc_M	        // F01
                                           << headLength_M          // F02
                                           << headDepth_M	        // F03
                                           << headWidth_M	        // F04
                                           << headCrownToNeckBack_M // F05
                                           << headChinToNeckBack_M; // F06

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupG()
{
    const QStringList list = QStringList() << neckMidCirc_M        // G01
                                           << neckCirc_M           // G02
                                           << highbustCirc_M       // G03
                                           << bustCirc_M           // G04
                                           << lowbustCirc_M        // G05
                                           << ribCirc_M            // G06
                                           << waistCirc_M          // G07
                                           << highhipCirc_M        // G08
                                           << hipCirc_M            // G09
                                           << neckArcF_M           // G10
                                           << highbustArcF_M       // G11
                                           << bustArcF_M           // G12
                                           << lowbustArcF_M        // G13
                                           << ribArcF_M            // G14
                                           << waistArcF_M          // G15
                                           << highhipArcF_M        // G16
                                           << hipArcF_M            // G17
                                           << neckArcHalfF_M       // G18
                                           << highbustArcHalfF_M   // G19
                                           << bustArcHalfF_M       // G20
                                           << lowbustArcHalfF_M    // G21
                                           << ribArcHalfF_M        // G22
                                           << waistArcHalfF_M      // G23
                                           << highhipArcHalfF_M    // G24
                                           << hipArcHalfF_M        // G25
                                           << neckArcB_M           // G26
                                           << highbustArcB_M       // G27
                                           << bustArcB_M           // G28
                                           << lowbustArcB_M        // G29
                                           << ribArcB_M            // G30
                                           << waistArcB_M          // G31
                                           << highhipArcB_M        // G32
                                           << hipArcB_M            // G33
                                           << neckArcHalfB_M       // G34
                                           << highbustArcHalfB_M   // G35
                                           << bustArcHalfB_M       // G36
                                           << lowbustArcHalfB_M    // G37
                                           << ribArcHalfB_M        // G38
                                           << waistArcHalfB_M      // G39
                                           << highhipArcHalfB_M    // G40
                                           << hipArcHalfB_M        // G41
                                           << hipWithAbdomenArcF_M // G42
                                           << bodyArmfoldCirc_M    // G43
                                           << bodyBustCirc_M       // G44
                                           << bodyTorsoCirc_M;     // G45

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupH()
{
    const QStringList list = QStringList() << neckFrontToWaistF_M              // H01
                                           << neckFrontToWaistFlatF_M          // H02
                                           << armpitToWaistSide_M              // H03
                                           << shoulderTipToWaistSideF_M        // H04
                                           << neckSideToWaistF_M               // H05
                                           << neckSideToWaistBustpointF_M      // H06
                                           << neckFrontToHighbustF_M           // H07
                                           << highbustToWaistF_M               // H08
                                           << neckFrontToBustF_M               // H09
                                           << bustToWaistF_M                   // H10
                                           << lowbustToWaistF_M                // H11
                                           << ribToWaistSide_M                 // H12
                                           << shoulderTipToArmfoldF_M          // H13
                                           << neckSideToBustF_M                // H14
                                           << neckSideToHighbustF_M            // H15
                                           << shoulderCenterToHighbustF_M      // H16
                                           << shoulderTipToWaistSideB_M        // H17
                                           << neckSideToWaistB_M               // H18
                                           << neckBackToWaistB_M               // H19
                                           << neckSideToWaistScapulaB_M        // H20
                                           << neckBackToHighbustB_M            // H21
                                           << highbustToWaistB_M               // H22
                                           << neckBackToBustB_M                // H23
                                           << bustToWaistB_M                   // H24
                                           << lowbustToWaistB_M                // H25
                                           << shoulderTipToArmfoldB_M          // H26
                                           << neckSideToBustB_M                // H27
                                           << neckSideToHighbustB_M            // H28
                                           << shoulderCenterToHighbustB_M      // H29
                                           << waistToHighhipF_M                // H30
                                           << waistToHipF_M                    // H31
                                           << waistToHighhipSide_M             // H32
                                           << waistToHighhipB_M                // H33
                                           << waistToHipB_M                    // H34
                                           << waistToHipSide_M                 // H35
                                           << shoulderSlopeNeckSideAngle_M     // H36
                                           << shoulderSlopeNeckSideLength_M    // H37
                                           << shoulderSlopeNeckBackAngle_M     // H38
                                           << shoulderSlopeNeckBackHeight_M    // H39
                                           << shoulderSlopeShoulderTipAngle_M; // H40

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupI()
{
    const QStringList list = QStringList() << shoulderLength_M                // I01
                                           << shoulderTipToShoulderTipF_M     // I02
                                           << acrossChestF_M                  // I03
                                           << armfoldToArmfoldF_M             // I04
                                           << shoulderTipToShoulderTipHalfF_M // I05
                                           << acrossChestHalfF_M              // I06
                                           << shoulderTipToShoulderTipB_M     // I07
                                           << acrossBackB_M                   // I08
                                           << armfoldToArmfoldB_M             // I09
                                           << shoulderTipToShoulderTipHalfB_M // I10
                                           << acrossBackHalfB_M               // I11
                                           << neckFrontToShoulderTipF_M       // I12
                                           << neckBackToShoulderTipB_M        // I13
                                           << neckWidth_M;                    // I14

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupJ()
{
    const QStringList list = QStringList() << bustpointToBustpoint_M        // J01
                                           << bustpointToNeckSide_M         // J02
                                           << bustpointToLowbust_M          // J03
                                           << bustpointToWaist_M            // J04
                                           << bustpointToBustpointHalf_M    // J05
                                           << bustpointNeckSideToWaist_M    // J06
                                           << bustpointToShoulderTip_M      // J07
                                           << bustpointToWaistFront_M       // J08
                                           << bustpointToBustpointHalter_M; // J09

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupK()
{
    const QStringList list = QStringList() << shoulderTipToWaistFront_M       // K01
                                           << neckFrontToWaistSide_M          // K02
                                           << neckSideToWaistSideF_M          // K03
                                           << shoulderTipToWaistBack_M        // K04
                                           << shoulderTipToWaistB_1inOffset_M // K05
                                           << neckBackToWaistSide_M           // K06
                                           << neckSideToWaistSideB_M          // K07
                                           << neckSideToArmfoldF_M            // K08
                                           << neckSideToArmpitF_M             // K09
                                           << neckSideToBustSideF_M           // K10
                                           << neckSideToArmfoldB_M            // K11
                                           << neckSideToArmpitB_M             // K12
                                           << neckSideToBustSideB_M;          // K13

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupL()
{
    const QStringList list = QStringList() << armShoulderTipToWristBent_M   // L01
                                           << armShoulderTipToElbowBent_M   // L02
                                           << armElbowToWristBent_M         // L03
                                           << armElbowCircBent_M            // L04
                                           << armShoulderTipToWrist_M       // L05
                                           << armShoulderTipToElbow_M       // L06
                                           << armElbowToWrist_M             // L07
                                           << armArmpitToWrist_M            // L08
                                           << armArmpitToElbow_M            // L09
                                           << armElbowToWristInside_M       // L10
                                           << armUpperCirc_M                // L11
                                           << armAboveElbowCirc_M           // L12
                                           << armElbowCirc_M                // L13
                                           << armLowerCirc_M                // L14
                                           << armWristCirc_M                // L15
                                           << armShoulderTipToArmfoldLine_M // L16
                                           << armNeckSideToWrist_M          // L17
                                           << armNeckSideToFingerTip_M      // L18
                                           << armscyeCirc_M                 // L19
                                           << armscyeLength_M               // L20
                                           << armscyeWidth_M;	            // L21

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupM()
{
    const QStringList list = QStringList() << legCrotchToFloor_M    // M01
                                           << legWaistSideToFloor_M // M02
                                           << legThighUpperCirc_M	// M03
                                           << legThighMidCirc_M     // M04
                                           << legKneeCirc_M	        // M05
                                           << legKneeSmallCirc_M    // M06
                                           << legCalfCirc_M	        // M07
                                           << legAnkleHighCirc_M    // M08
                                           << legAnkleCirc_M        // M09
                                           << legKneeCircBent_M	    // M10
                                           << legAnkleDiagCirc_M    // M11
                                           << legCrotchToAnkle_M    // M12
                                           << legWaistSideToAnkle_M // M13
                                           << legWaistSideToKnee_M; // M14

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListGroupN()
{
    const QStringList list = QStringList() << crotchLength_M   // N01
                                           << crotchLengthB_M  // N02
                                           << crotchLengthF_M  // N03
                                           << riseLengthSide_M // N04
                                           << riseLengthDiag_M // N05
                                           << riseLengthB_M    // N06
                                           << riseLengthF_M;   // N07

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
    const QStringList list = QStringList() << neckBackToBustFront_M	                             // P01
                                           << neckBackToArmfoldFront_M                           // P02
                                           << neckBackToArmfoldFrontToWaistSide_M                // P03
                                           << highbustBackOverShoulderToArmfoldFront_M           // P04
                                           << highbustBackOverShoulderToWaistFront_M             // P05
                                           << neckBackToArmfoldFrontToNeckBack_M                 // P06
                                           << acrossBackCenterToArmfoldFrontToAcrossBackCenter_M // P07
                                           << neckBackToArmfoldFrontToHighbustBack_M             // P08
                                           << armfoldToArmfoldBust_M                             // P09
                                           << armfoldToBustFront_M	                             // P10
                                           << highbustBOverShoulderToHighbustF_M                 // P11
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

//---------------------------------------------------------------------------------------------------------------------
QStringList SupportedLocales()
{
    const QStringList locales = QStringList() << QStringLiteral("ru_RU")
                                              << QStringLiteral("uk_UA")
                                              << QStringLiteral("de_DE")
                                              << QStringLiteral("cs_CZ")
                                              << QStringLiteral("he_IL")
                                              << QStringLiteral("fr_FR")
                                              << QStringLiteral("it_IT")
                                              << QStringLiteral("nl_NL")
                                              << QStringLiteral("id_ID")
                                              << QStringLiteral("es_ES")
                                              << QStringLiteral("fi_FI")
                                              << QStringLiteral("en_US");

    return locales;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList AllGroupNames()
{
    const QStringList originalNames = QStringList() << ListGroupA()
                                                    << ListGroupB()
                                                    << ListGroupC()
                                                    << ListGroupD()
                                                    << ListGroupE()
                                                    << ListGroupF()
                                                    << ListGroupG()
                                                    << ListGroupH()
                                                    << ListGroupI()
                                                    << ListGroupJ()
                                                    << ListGroupK()
                                                    << ListGroupL()
                                                    << ListGroupM()
                                                    << ListGroupN()
                                                    << ListGroupO()
                                                    << ListGroupP()
                                                    << ListGroupQ()
                                                    << size_M
                                                    << height_M;

    return originalNames;
}

//---------------------------------------------------------------------------------------------------------------------
void InitPMSystems(QComboBox *systemCombo)
{
    const QStringList listSystems = ListPMSystems();
    QMap<QString, QString> systems;
    for (int i = 0; i < listSystems.size()-1; ++i)
    {
        systems.insert(qApp->TrVars()->PMSystemName(listSystems.at(i)), listSystems.at(i));
    }

// * The default option (blank field or 'None') should appear at the top of the list.
// * The list should be sorted alphabetically so users can find their system easily.

    SCASSERT(systemCombo != nullptr);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(listSystems.at(listSystems.size()-1)),
                         listSystems.at(listSystems.size()-1));

    QMap<QString, QString>::const_iterator i = systems.constBegin();
    while (i != systems.constEnd())
    {
        systemCombo->addItem(i.key(), i.value());
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ListPMSystems()
{
    QStringList list;
    list << p0_S
         << p1_S
         << p2_S
         << p3_S
         << p4_S
         << p5_S
         << p6_S
         << p7_S
         << p8_S
         << p9_S
         << p10_S
         << p11_S
         << p12_S
         << p13_S
         << p14_S
         << p15_S
         << p16_S
         << p17_S
         << p18_S
         << p19_S
         << p20_S
         << p21_S
         << p22_S
         << p23_S
         << p24_S
         << p25_S
         << p26_S
         << p27_S
         << p28_S
         << p29_S
         << p30_S
         << p31_S
         << p32_S
         << p33_S
         << p34_S
         << p35_S
         << p36_S
         << p37_S
         << p38_S
         << p39_S
         << p40_S
         << p41_S
         << p42_S
         << p43_S
         << p44_S
         << p45_S
         << p46_S
         << p47_S
         << p48_S
         << p49_S
         << p50_S
         << p51_S
         << p52_S
         << p53_S
         << p54_S
         << p998_S;

    return list;
}

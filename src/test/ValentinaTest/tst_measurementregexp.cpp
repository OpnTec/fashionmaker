/************************************************************************
 **
 **  @file   tst_measurementregexp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2015
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

#include "tst_measurementregexp.h"
#include "../qmuparser/qmudef.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../ifc/ifcdef.h"

#include <QtTest>
#include <QTranslator>

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::TST_MeasurementRegExp(QObject *parent)
    :AbstractTest(parent),
      pmsTranslator(nullptr),
      vTranslator(nullptr),
      trMs(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::~TST_MeasurementRegExp()
{
    delete pmsTranslator;
    delete vTranslator;
    delete trMs;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestOriginalMeasurementNamesRegExp()
{
    const QStringList originalNames = AllGroupNames();
    const QRegularExpression re(NameRegExp());

    foreach(const QString &str, originalNames)
    {
        QCOMPARE(re.match(str).hasMatch(), true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestVariableStrings_data()
{
    PrepareMeasurementData();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestVariableStrings()
{
    QFETCH(QString, system);
    QFETCH(QString, locale);

    const int res = LoadTranslation(system, locale);

    switch(res)
    {
        case ErrorInstall:
        case ErrorSize:
        case ErrorLoad:
        {
            const QString message = QString("Can't to check translation. System = %1, locale = %2")
                    .arg(system)
                    .arg(locale);
            QSKIP(qUtf8Printable(message));
            break;
        }
        case NoError:
        {
            CheckRegExpNames();
            CheckIsNamesUnique();
            CheckNoOriginalNamesInTranslation();
            CheckUnderlineExists();
            CheckInternalVaribleRegExp();

            RemoveTrMeasurements(system, locale);
            RemoveTrVariables(locale);
            break;
        }
        default:
            QWARN("Unexpected state");
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestCorrectOrderMeasurement_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("expect");

    QTest::newRow("A01") << height_M << "height";
    QTest::newRow("A02") << heightNeckBack_M << "height_neck_back";
    QTest::newRow("A03") << heightScapula_M << "height_scapula";
    QTest::newRow("A04") << heightArmpit_M << "height_armpit";
    QTest::newRow("A05") << heightWaistSide_M << "height_waist_side";
    QTest::newRow("A06") << heightHip_M << "height_hip";
    QTest::newRow("A07") << heightGlutealFold_M << "height_gluteal_fold";
    QTest::newRow("A08") << heightKnee_M << "height_knee";
    QTest::newRow("A09") << heightCalf_M << "height_calf";
    QTest::newRow("A10") << heightAnkleHigh_M << "height_ankle_high";
    QTest::newRow("A11") << heightAnkle_M << "height_ankle";
    QTest::newRow("A12") << heightHighhip_M << "height_highhip";
    QTest::newRow("A13") << heightWaistFront_M << "height_waist_front";
    QTest::newRow("A14") << heightBustpoint_M << "height_bustpoint";
    QTest::newRow("A15") << heightShoulderTip_M << "height_shoulder_tip";
    QTest::newRow("A16") << heightNeckFront_M << "height_neck_front";
    QTest::newRow("A17") << heightNeckSide_M << "height_neck_side";
    QTest::newRow("A18") << heightNeckBackToKnee_M << "height_neck_back_to_knee";
    QTest::newRow("A19") << heightWaistSideToKnee_M << "height_waist_side_to_knee";
    QTest::newRow("A20") << heightWaistSideToHip_M << "height_waist_side_to_hip";
    QTest::newRow("A21") << heightKneeToAnkle_M << "height_knee_to_ankle";
    QTest::newRow("A22") << heightNeckBackToWaistSide_M << "height_neck_back_to_waist_side";
    QTest::newRow("A23") << heightWaistBack_M << "height_waist_back";
    QTest::newRow("B01") << widthShoulder_M << "width_shoulder";
    QTest::newRow("B02") << widthBust_M << "width_bust";
    QTest::newRow("B03") << widthWaist_M << "width_waist";
    QTest::newRow("B04") << widthHip_M << "width_hip";
    QTest::newRow("B05") << widthAbdomenToHip_M << "width_abdomen_to_hip";
    QTest::newRow("C01") << indentNeckBack_M << "indent_neck_back";
    QTest::newRow("C02") << indentWaistBack_M << "indent_waist_back";
    QTest::newRow("C03") << indentAnkleHigh_M << "indent_ankle_high";
    QTest::newRow("D01") << handPalmLength_M << "hand_palm_length";
    QTest::newRow("D02") << handLength_M << "hand_length";
    QTest::newRow("D03") << handPalmWidth_M << "hand_palm_width";
    QTest::newRow("D04") << handPalmCirc_M << "hand_palm_circ";
    QTest::newRow("D05") << handCirc_M << "hand_circ";
    QTest::newRow("E01") << footWidth_M << "foot_width";
    QTest::newRow("E02") << footLength_M << "foot_length";
    QTest::newRow("E03") << footCirc_M << "foot_circ";
    QTest::newRow("E04") << footInstepCirc_M << "foot_instep_circ";
    QTest::newRow("F01") << headCirc_M << "head_circ";
    QTest::newRow("F02") << headLength_M << "head_length";
    QTest::newRow("F03") << headDepth_M << "head_depth";
    QTest::newRow("F04") << headWidth_M << "head_width";
    QTest::newRow("F05") << headCrownToNeckBack_M << "head_crown_to_neck_back";
    QTest::newRow("F06") << headChinToNeckBack_M << "head_chin_to_neck_back";
    QTest::newRow("G01") << neckMidCirc_M << "neck_mid_circ";
    QTest::newRow("G02") << neckCirc_M << "neck_circ";
    QTest::newRow("G03") << highbustCirc_M << "highbust_circ";
    QTest::newRow("G04") << bustCirc_M << "bust_circ";
    QTest::newRow("G05") << lowbustCirc_M << "lowbust_circ";
    QTest::newRow("G06") << ribCirc_M << "rib_circ";
    QTest::newRow("G07") << waistCirc_M << "waist_circ";
    QTest::newRow("G08") << highhipCirc_M << "highhip_circ";
    QTest::newRow("G09") << hipCirc_M << "hip_circ";
    QTest::newRow("G10") << neckArcF_M << "neck_arc_f";
    QTest::newRow("G11") << highbustArcF_M << "highbust_arc_f";
    QTest::newRow("G12") << bustArcF_M << "bust_arc_f";
    QTest::newRow("G13") << lowbustArcF_M << "lowbust_arc_f";
    QTest::newRow("G14") << ribArcF_M << "rib_arc_f";
    QTest::newRow("G15") << waistArcF_M << "waist_arc_f";
    QTest::newRow("G16") << highhipArcF_M << "highhip_arc_f";
    QTest::newRow("G17") << hipArcF_M << "hip_arc_f";
    QTest::newRow("G18") << neckArcHalfF_M << "neck_arc_half_f";
    QTest::newRow("G19") << highbustArcHalfF_M << "highbust_arc_half_f";
    QTest::newRow("G20") << bustArcHalfF_M << "bust_arc_half_f";
    QTest::newRow("G21") << lowbustArcHalfF_M << "lowbust_arc_half_f";
    QTest::newRow("G22") << ribArcHalfF_M << "rib_arc_half_f";
    QTest::newRow("G23") << waistArcHalfF_M << "waist_arc_half_f";
    QTest::newRow("G24") << highhipArcHalfF_M << "highhip_arc_half_f";
    QTest::newRow("G25") << hipArcHalfF_M << "hip_arc_half_f";
    QTest::newRow("G26") << neckArcB_M << "neck_arc_b";
    QTest::newRow("G27") << highbustArcB_M << "highbust_arc_b";
    QTest::newRow("G28") << bustArcB_M << "bust_arc_b";
    QTest::newRow("G29") << lowbustArcB_M << "lowbust_arc_b";
    QTest::newRow("G30") << ribArcB_M << "rib_arc_b";
    QTest::newRow("G31") << waistArcB_M << "waist_arc_b";
    QTest::newRow("G32") << highhipArcB_M << "highhip_arc_b";
    QTest::newRow("G33") << hipArcB_M << "hip_arc_b";
    QTest::newRow("G34") << neckArcHalfB_M << "neck_arc_half_b";
    QTest::newRow("G35") << highbustArcHalfB_M << "highbust_arc_half_b";
    QTest::newRow("G36") << bustArcHalfB_M << "bust_arc_half_b";
    QTest::newRow("G37") << lowbustArcHalfB_M << "lowbust_arc_half_b";
    QTest::newRow("G38") << ribArcHalfB_M << "rib_arc_half_b";
    QTest::newRow("G39") << waistArcHalfB_M << "waist_arc_half_b";
    QTest::newRow("G40") << highhipArcHalfB_M << "highhip_arc_half_b";
    QTest::newRow("G41") << hipArcHalfB_M << "hip_arc_half_b";
    QTest::newRow("G42") << hipWithAbdomenArcF_M << "hip_with_abdomen_arc_f";
    QTest::newRow("G43") << bodyArmfoldCirc_M << "body_armfold_circ";
    QTest::newRow("G44") << bodyBustCirc_M << "body_bust_circ";
    QTest::newRow("G45") << bodyTorsoCirc_M << "body_torso_circ";
    QTest::newRow("G46") << hipCircWithAbdomen_M << "hip_circ_with_abdomen";
    QTest::newRow("H01") << neckFrontToWaistF_M << "neck_front_to_waist_f";
    QTest::newRow("H02") << neckFrontToWaistFlatF_M << "neck_front_to_waist_flat_f";
    QTest::newRow("H03") << armpitToWaistSide_M << "armpit_to_waist_side";
    QTest::newRow("H04") << shoulderTipToWaistSideF_M << "shoulder_tip_to_waist_side_f";
    QTest::newRow("H05") << neckSideToWaistF_M << "neck_side_to_waist_f";
    QTest::newRow("H06") << neckSideToWaistBustpointF_M << "neck_side_to_waist_bustpoint_f";
    QTest::newRow("H07") << neckFrontToHighbustF_M << "neck_front_to_highbust_f";
    QTest::newRow("H08") << highbustToWaistF_M << "highbust_to_waist_f";
    QTest::newRow("H09") << neckFrontToBustF_M << "neck_front_to_bust_f";
    QTest::newRow("H10") << bustToWaistF_M << "bust_to_waist_f";
    QTest::newRow("H11") << lowbustToWaistF_M << "lowbust_to_waist_f";
    QTest::newRow("H12") << ribToWaistSide_M << "rib_to_waist_side";
    QTest::newRow("H13") << shoulderTipToArmfoldF_M << "shoulder_tip_to_armfold_f";
    QTest::newRow("H14") << neckSideToBustF_M << "neck_side_to_bust_f";
    QTest::newRow("H15") << neckSideToHighbustF_M << "neck_side_to_highbust_f";
    QTest::newRow("H16") << shoulderCenterToHighbustF_M << "shoulder_center_to_highbust_f";
    QTest::newRow("H17") << shoulderTipToWaistSideB_M << "shoulder_tip_to_waist_side_b";
    QTest::newRow("H18") << neckSideToWaistB_M << "neck_side_to_waist_b";
    QTest::newRow("H19") << neckBackToWaistB_M << "neck_back_to_waist_b";
    QTest::newRow("H20") << neckSideToWaistScapulaB_M << "neck_side_to_waist_scapula_b";
    QTest::newRow("H21") << neckBackToHighbustB_M << "neck_back_to_highbust_b";
    QTest::newRow("H22") << highbustToWaistB_M << "highbust_to_waist_b";
    QTest::newRow("H23") << neckBackToBustB_M << "neck_back_to_bust_b";
    QTest::newRow("H24") << bustToWaistB_M << "bust_to_waist_b";
    QTest::newRow("H25") << lowbustToWaistB_M << "lowbust_to_waist_b";
    QTest::newRow("H26") << shoulderTipToArmfoldB_M << "shoulder_tip_to_armfold_b";
    QTest::newRow("H27") << neckSideToBustB_M << "neck_side_to_bust_b";
    QTest::newRow("H28") << neckSideToHighbustB_M << "neck_side_to_highbust_b";
    QTest::newRow("H29") << shoulderCenterToHighbustB_M << "shoulder_center_to_highbust_b";
    QTest::newRow("H30") << waistToHighhipF_M << "waist_to_highhip_f";
    QTest::newRow("H31") << waistToHipF_M << "waist_to_hip_f";
    QTest::newRow("H32") << waistToHighhipSide_M << "waist_to_highhip_side";
    QTest::newRow("H33") << waistToHighhipB_M << "waist_to_highhip_b";
    QTest::newRow("H34") << waistToHipB_M << "waist_to_hip_b";
    QTest::newRow("H35") << waistToHipSide_M << "waist_to_hip_side";
    QTest::newRow("H36") << shoulderSlopeNeckSideAngle_M << "shoulder_slope_neck_side_angle";
    QTest::newRow("H37") << shoulderSlopeNeckSideLength_M << "shoulder_slope_neck_side_length";
    QTest::newRow("H38") << shoulderSlopeNeckBackAngle_M << "shoulder_slope_neck_back_angle";
    QTest::newRow("H39") << shoulderSlopeNeckBackHeight_M << "shoulder_slope_neck_back_height";
    QTest::newRow("H40") << shoulderSlopeShoulderTipAngle_M << "shoulder_slope_shoulder_tip_angle";
    QTest::newRow("H41") << neckBackToAcrossBack_M << "neck_back_to_across_back";
    QTest::newRow("H42") << acrossBackToWaistB_M << "across_back_to_waist_b";
    QTest::newRow("I01") << shoulderLength_M << "shoulder_length";
    QTest::newRow("I02") << shoulderTipToShoulderTipF_M << "shoulder_tip_to_shoulder_tip_f";
    QTest::newRow("I03") << acrossChestF_M << "across_chest_f";
    QTest::newRow("I04") << armfoldToArmfoldF_M << "armfold_to_armfold_f";
    QTest::newRow("I05") << shoulderTipToShoulderTipHalfF_M << "shoulder_tip_to_shoulder_tip_half_f";
    QTest::newRow("I06") << acrossChestHalfF_M << "across_chest_half_f";
    QTest::newRow("I07") << shoulderTipToShoulderTipB_M << "shoulder_tip_to_shoulder_tip_b";
    QTest::newRow("I08") << acrossBackB_M << "across_back_b";
    QTest::newRow("I09") << armfoldToArmfoldB_M << "armfold_to_armfold_b";
    QTest::newRow("I10") << shoulderTipToShoulderTipHalfB_M << "shoulder_tip_to_shoulder_tip_half_b";
    QTest::newRow("I11") << acrossBackHalfB_M << "across_back_half_b";
    QTest::newRow("I12") << neckFrontToShoulderTipF_M << "neck_front_to_shoulder_tip_f";
    QTest::newRow("I13") << neckBackToShoulderTipB_M << "neck_back_to_shoulder_tip_b";
    QTest::newRow("I14") << neckWidth_M << "neck_width";
    QTest::newRow("J01") << bustpointToBustpoint_M << "bustpoint_to_bustpoint";
    QTest::newRow("J02") << bustpointToNeckSide_M << "bustpoint_to_neck_side";
    QTest::newRow("J03") << bustpointToLowbust_M << "bustpoint_to_lowbust";
    QTest::newRow("J04") << bustpointToWaist_M << "bustpoint_to_waist";
    QTest::newRow("J05") << bustpointToBustpointHalf_M << "bustpoint_to_bustpoint_half";
    QTest::newRow("J06") << bustpointNeckSideToWaist_M << "bustpoint_neck_side_to_waist";
    QTest::newRow("J07") << bustpointToShoulderTip_M << "bustpoint_to_shoulder_tip";
    QTest::newRow("J08") << bustpointToWaistFront_M << "bustpoint_to_waist_front";
    QTest::newRow("J09") << bustpointToBustpointHalter_M << "bustpoint_to_bustpoint_halter";
    QTest::newRow("J10") << bustpointToShoulderCenter_M  << "bustpoint_to_shoulder_center";
    QTest::newRow("K01") << shoulderTipToWaistFront_M << "shoulder_tip_to_waist_front";
    QTest::newRow("K02") << neckFrontToWaistSide_M << "neck_front_to_waist_side";
    QTest::newRow("K03") << neckSideToWaistSideF_M << "neck_side_to_waist_side_f";
    QTest::newRow("K04") << shoulderTipToWaistBack_M << "shoulder_tip_to_waist_back";
    QTest::newRow("K05") << shoulderTipToWaistB_1inOffset_M << "shoulder_tip_to_waist_b_1in_offset";
    QTest::newRow("K06") << neckBackToWaistSide_M << "neck_back_to_waist_side";
    QTest::newRow("K07") << neckSideToWaistSideB_M << "neck_side_to_waist_side_b";
    QTest::newRow("K08") << neckSideToArmfoldF_M << "neck_side_to_armfold_f";
    QTest::newRow("K09") << neckSideToArmpitF_M << "neck_side_to_armpit_f";
    QTest::newRow("K10") << neckSideToBustSideF_M << "neck_side_to_bust_side_f";
    QTest::newRow("K11") << neckSideToArmfoldB_M << "neck_side_to_armfold_b";
    QTest::newRow("K12") << neckSideToArmpitB_M << "neck_side_to_armpit_b";
    QTest::newRow("K13") << neckSideToBustSideB_M << "neck_side_to_bust_side_b";
    QTest::newRow("L01") << armShoulderTipToWristBent_M << "arm_shoulder_tip_to_wrist_bent";
    QTest::newRow("L02") << armShoulderTipToElbowBent_M << "arm_shoulder_tip_to_elbow_bent";
    QTest::newRow("L03") << armElbowToWristBent_M << "arm_elbow_to_wrist_bent";
    QTest::newRow("L04") << armElbowCircBent_M << "arm_elbow_circ_bent";
    QTest::newRow("L05") << armShoulderTipToWrist_M << "arm_shoulder_tip_to_wrist";
    QTest::newRow("L06") << armShoulderTipToElbow_M << "arm_shoulder_tip_to_elbow";
    QTest::newRow("L07") << armElbowToWrist_M << "arm_elbow_to_wrist";
    QTest::newRow("L08") << armArmpitToWrist_M << "arm_armpit_to_wrist";
    QTest::newRow("L09") << armArmpitToElbow_M << "arm_armpit_to_elbow";
    QTest::newRow("L10") << armElbowToWristInside_M << "arm_elbow_to_wrist_inside";
    QTest::newRow("L11") << armUpperCirc_M << "arm_upper_circ";
    QTest::newRow("L12") << armAboveElbowCirc_M << "arm_above_elbow_circ";
    QTest::newRow("L13") << armElbowCirc_M << "arm_elbow_circ";
    QTest::newRow("L14") << armLowerCirc_M << "arm_lower_circ";
    QTest::newRow("L15") << armWristCirc_M << "arm_wrist_circ";
    QTest::newRow("L16") << armShoulderTipToArmfoldLine_M << "arm_shoulder_tip_to_armfold_line";
    QTest::newRow("L17") << armNeckSideToWrist_M << "arm_neck_side_to_wrist";
    QTest::newRow("L18") << armNeckSideToFingerTip_M << "arm_neck_side_to_finger_tip";
    QTest::newRow("L19") << armscyeCirc_M << "armscye_circ";
    QTest::newRow("L20") << armscyeLength_M << "armscye_length";
    QTest::newRow("L21") << armscyeWidth_M << "armscye_width";
    QTest::newRow("L22") << armNeckSideToOuterElbow_M << "arm_neck_side_to_outer_elbow";
    QTest::newRow("M01") << legCrotchToFloor_M << "leg_crotch_to_floor";
    QTest::newRow("M02") << legWaistSideToFloor_M << "leg_waist_side_to_floor";
    QTest::newRow("M03") << legThighUpperCirc_M << "leg_thigh_upper_circ";
    QTest::newRow("M04") << legThighMidCirc_M << "leg_thigh_mid_circ";
    QTest::newRow("M05") << legKneeCirc_M << "leg_knee_circ";
    QTest::newRow("M06") << legKneeSmallCirc_M << "leg_knee_small_circ";
    QTest::newRow("M07") << legCalfCirc_M << "leg_calf_circ";
    QTest::newRow("M08") << legAnkleHighCirc_M << "leg_ankle_high_circ";
    QTest::newRow("M09") << legAnkleCirc_M << "leg_ankle_circ";
    QTest::newRow("M10") << legKneeCircBent_M << "leg_knee_circ_bent";
    QTest::newRow("M11") << legAnkleDiagCirc_M << "leg_ankle_diag_circ";
    QTest::newRow("M12") << legCrotchToAnkle_M << "leg_crotch_to_ankle";
    QTest::newRow("M13") << legWaistSideToAnkle_M << "leg_waist_side_to_ankle";
    QTest::newRow("M14") << legWaistSideToKnee_M << "leg_waist_side_to_knee";
    QTest::newRow("N01") << crotchLength_M << "crotch_length";
    QTest::newRow("N02") << crotchLengthB_M << "crotch_length_b";
    QTest::newRow("N03") << crotchLengthF_M << "crotch_length_f";
    QTest::newRow("N04") << riseLengthSideSitting_M << "rise_length_side_sitting";
    QTest::newRow("N05") << riseLengthDiag_M << "rise_length_diag";
    QTest::newRow("N06") << riseLengthB_M << "rise_length_b";
    QTest::newRow("N07") << riseLengthF_M << "rise_length_f";
    QTest::newRow("N08") << riseLengthSide_M << "rise_length_side";
    QTest::newRow("O01") << neckBackToWaistFront_M << "neck_back_to_waist_front";
    QTest::newRow("O02") << waistToWaistHalter_M << "waist_to_waist_halter";
    QTest::newRow("O03") << waistNaturalCirc_M << "waist_natural_circ";
    QTest::newRow("O04") << waistNaturalArcF_M << "waist_natural_arc_f";
    QTest::newRow("O05") << waistNaturalArcB_M << "waist_natural_arc_b";
    QTest::newRow("O06") << waistToNaturalWaistF_M << "waist_to_natural_waist_f";
    QTest::newRow("O07") << waistToNaturalWaistB_M << "waist_to_natural_waist_b";
    QTest::newRow("O08") << armNeckBackToElbowBent_M << "arm_neck_back_to_elbow_bent";
    QTest::newRow("O09") << armNeckBackToWristBent_M << "arm_neck_back_to_wrist_bent";
    QTest::newRow("O10") << armNeckSideToElbowBent_M << "arm_neck_side_to_elbow_bent";
    QTest::newRow("O11") << armNeckSideToWristBent_M << "arm_neck_side_to_wrist_bent";
    QTest::newRow("O12") << armAcrossBackCenterToElbowBent_M << "arm_across_back_center_to_elbow_bent";
    QTest::newRow("O13") << armAcrossBackCenterToWristBent_M << "arm_across_back_center_to_wrist_bent";
    QTest::newRow("O14") << armArmscyeBackCenterToWristBent_M << "arm_armscye_back_center_to_wrist_bent";
    QTest::newRow("P01") << neckBackToBustFront_M << "neck_back_to_bust_front";
    QTest::newRow("P02") << neckBackToArmfoldFront_M << "neck_back_to_armfold_front";
    QTest::newRow("P03") << neckBackToArmfoldFrontToWaistSide_M << "neck_back_to_armfold_front_to_waist_side";
    QTest::newRow("P04") << highbustBackOverShoulderToArmfoldFront_M << "highbust_back_over_shoulder_to_armfold_front";
    QTest::newRow("P05") << highbustBackOverShoulderToWaistFront_M << "highbust_back_over_shoulder_to_waist_front";
    QTest::newRow("P06") << neckBackToArmfoldFrontToNeckBack_M << "neck_back_to_armfold_front_to_neck_back";
    QTest::newRow("P07") << acrossBackCenterToArmfoldFrontToAcrossBackCenter_M
                         << "across_back_center_to_armfold_front_to_across_back_center";
    QTest::newRow("P08") << neckBackToArmfoldFrontToHighbustBack_M << "neck_back_to_armfold_front_to_highbust_back";
    QTest::newRow("P09") << armfoldToArmfoldBust_M << "armfold_to_armfold_bust";
    QTest::newRow("P10") << armfoldToBustFront_M << "armfold_to_bust_front";
    QTest::newRow("P11") << highbustBOverShoulderToHighbustF_M << "highbust_b_over_shoulder_to_highbust_f";
    QTest::newRow("P12") << armscyeArc_M << "armscye_arc";
    QTest::newRow("Q01") << dartWidthShoulder_M << "dart_width_shoulder";
    QTest::newRow("Q02") << dartWidthBust_M << "dart_width_bust";
    QTest::newRow("Q03") << dartWidthWaist_M << "dart_width_waist";
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestCorrectOrderMeasurement()
{
    QFETCH(QString, name);
    QFETCH(QString, expect);

    QCOMPARE(name, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::PrepareMeasurementData()
{
    const int systemCounts = 56;
    const QStringList locales = SupportedLocales();

    {
        const int combinations = systemCounts * locales.size();

        QDir dir(TranslationsPath());
        const QStringList fileNames = dir.entryList(QStringList("measurements_p*_*.qm"));

        QVERIFY2(combinations == fileNames.size(), "Unexpected count of files.");
    }

    QTest::addColumn<QString>("system");
    QTest::addColumn<QString>("locale");

    for(int s = 0; s < systemCounts; ++s)
    {
        for(int l = 0, sz = locales.size(); l < sz; ++l)
        {
            const QString system = QString("p%1").arg(s);
            const QString locale = locales.at(l);
            const QString tag = QString("Check translation system %1, locale %2").arg(system).arg(locale);
            QTest::newRow(qUtf8Printable(tag)) << system << locale;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadTranslation(const QString &checkedSystem, const QString &checkedLocale)
{
    int state = LoadMeasurements(checkedSystem, checkedLocale);
    if (state != NoError)
    {
        return state;
    }

    state = LoadVariables(checkedLocale);
    if (state != NoError)
    {
        return state;
    }

    InitTrMs();//Very important do this after loading QM files.

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("measurements_%1_%2.qm").arg(checkedSystem).arg(checkedLocale);

    if (QFileInfo(path+QLatin1Literal("/")+file).size() <= 34)
    {
        const QString message = QString("Translation for system = %1 and locale = %2 is empty. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    pmsTranslator = new QTranslator(this);

    if (not pmsTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete pmsTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(pmsTranslator))
    {
        const QString message = QString("Can't install translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete pmsTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadVariables(const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("valentina_%1.qm").arg(checkedLocale);

    if (QFileInfo(path+QLatin1Literal("/")+file).size() <= 34)
    {
        const QString message = QString("Translation variables for locale = %1 is empty. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    vTranslator = new QTranslator(this);

    if (not vTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete vTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(vTranslator))
    {
        const QString message = QString("Can't install translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete vTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::RemoveTrMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    if (not pmsTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(pmsTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation for system = %1 and locale = %2")
                    .arg(checkedSystem)
                    .arg(checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete pmsTranslator;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::RemoveTrVariables(const QString &checkedLocale)
{
    if (not vTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(vTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation variables for locale = %1")
                    .arg(checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete vTranslator;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::InitTrMs()
{
    if (trMs != nullptr)
    {
        trMs->Retranslate();
    }
    else
    {
        trMs = new VTranslateVars();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckRegExpNames() const
{
    const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
    const QRegularExpression re(NameRegExp());

    foreach(const QString &str, originalNames)
    {
        const QString translated = trMs->VarToUser(str);
        if (not re.match(translated).hasMatch())
        {
            const QString message = QString("Original name:'%1', translated name:'%2'").arg(str).arg(translated);
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckIsNamesUnique() const
{
    const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
    QSet<QString> names;

    foreach(const QString &str, originalNames)
    {
        const QString translated = trMs->VarToUser(str);
        if (names.contains(translated))
        {
            const QString message = QString("Name is not unique. Original name:'%1', translated name:'%2'")
                    .arg(str).arg(translated);
            QFAIL(qUtf8Printable(message));
        }
        names.insert(translated);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckNoOriginalNamesInTranslation() const
{
    const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
    QSet<QString> names = QSet<QString>::fromList(originalNames);

    foreach(const QString &str, originalNames)
    {
        const QString translated = trMs->VarToUser(str);
        if (names.contains(translated))
        {
            if (str != translated)
            {
                const QString message = QString("Translation repeat original name from other place. "
                                                "Original name:'%1', translated name:'%2'")
                        .arg(str).arg(translated);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckUnderlineExists() const
{
    QMap<QString, bool> data;

    data.insert(line_, true);
    data.insert(angleLine_, true);
    data.insert(arc_, true);
    data.insert(spl_, true);
    data.insert(splPath, false);
    data.insert(radius_V, false);
    data.insert(radiusArc_, true);
    data.insert(angle1_V, false);
    data.insert(angle2_V, false);
    data.insert(angle1Arc_, true);
    data.insert(angle2Arc_, true);
    data.insert(angle1Spl_, true);
    data.insert(angle2Spl_, true);
    data.insert(angle1SplPath, false);
    data.insert(angle2SplPath, false);
    data.insert(seg_, true);

    //Catch case when new internal variable appears.
    QCOMPARE(data.size(), builInVariables.size());

    auto i = data.constBegin();
    while (i != data.constEnd())
    {
        const QString translated = trMs->InternalVarToUser(i.key());
        if ((translated.right(1) == QLatin1Literal("_")) != i.value())
        {
            const QString message = QString("String '%1' doesn't contain underline. Original string is '%2'")
                    .arg(translated).arg(i.key());
            QFAIL(qUtf8Printable(message));
        }
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckInternalVaribleRegExp() const
{
    const QString regex = QStringLiteral("(.){1,}_(.){1,}$");
    foreach(const QString &var, builInVariables)
    {
        const QString sourceRegex = QLatin1Literal("^") + var + regex;
        const QRegularExpression sourceRe(sourceRegex);

        const QString translated = trMs->InternalVarToUser(var);
        const QString translationRegex = QLatin1Literal("^") + translated + regex;
        const QRegularExpression translationRe(translationRegex);

        const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
        foreach(const QString &str, originalNames)
        {
            if (sourceRe.match(str).hasMatch() || translationRe.match(str).hasMatch())
            {
                const QString message = QString("Invalid original string '%1'").arg(str);
                QFAIL(qUtf8Printable(message));
            }

            const QString translated = trMs->VarToUser(str);
            if (sourceRe.match(translated).hasMatch() || translationRe.match(translated).hasMatch())
            {
                const QString message = QString("Invalid translation string '%1'").arg(translated);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

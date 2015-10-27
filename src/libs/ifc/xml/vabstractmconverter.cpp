/************************************************************************
 **
 **  @file   vabstractmconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 9, 2015
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

#include "vabstractmconverter.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractMConverter::VAbstractMConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractMConverter::~VAbstractMConverter()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMConverter::AddRootComment()
{
    const QString rootComment =
            QStringLiteral("Measurements created with Valentina (http://www.valentina-project.org/).");

    QDomElement rootElement = this->documentElement();
    rootElement.insertBefore(createComment(rootComment), rootElement.firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
QMultiMap<QString, QString> VAbstractMConverter::OldNamesToNewNames_InV0_3_0()
{
    // new name, old name
    QMultiMap<QString, QString> names;

    // A02
    names.insert(QStringLiteral("height_neck_back"), QStringLiteral("cervicale_height"));
    // A03
    names.insert(QStringLiteral("height_scapula"), QStringLiteral("height_scapular_point"));
    // A04
    names.insert(QStringLiteral("height_armpit"), QStringLiteral("height_back_angle_axilla"));
    // A05
    names.insert(QStringLiteral("height_waist_side"), QStringLiteral("waist_height"));
    // A06
    names.insert(QStringLiteral("height_hip"), QStringLiteral("hip_height"));
    // A08
    names.insert(QStringLiteral("height_knee"), QStringLiteral("knee_height"));
    // A11
    names.insert(QStringLiteral("height_ankle"), QStringLiteral("ankle_height"));
    // A12
    names.insert(QStringLiteral("height_highhip"), QStringLiteral("high_hip_height"));
    // A13
    names.insert(QStringLiteral("height_waist_front"), QStringLiteral("front_waist_to_floor"));
    // A14
    names.insert(QStringLiteral("height_bustpoint"), QStringLiteral("height_nipple_point"));
    // A15
    QString name = QStringLiteral("height_shoulder_tip");
    names.insert(name, QStringLiteral("shoulder_height"));
    names.insert(name, QStringLiteral("height_shoulder_point"));
    // A16
    name = QStringLiteral("height_neck_front");
    names.insert(name, QStringLiteral("height_clavicular_point"));
    names.insert(name, QStringLiteral("height_front_neck_base_point"));
    // A17
    names.insert(QStringLiteral("height_neck_side"), QStringLiteral("height_base_neck_side_point"));
    // A18
    name = QStringLiteral("height_neck_back_to_knee");
    names.insert(name, QStringLiteral("neck_to_knee_point"));
    names.insert(name, QStringLiteral("cervicale_to_knee_height"));
    // A19
    names.insert(QStringLiteral("height_waist_side_to_knee"), QStringLiteral("waist_to_knee_height"));
    // A20
    names.insert(QStringLiteral("height_waist_side_to_hip"), QStringLiteral("waist_to_hip_height"));
    // C01
    names.insert(QStringLiteral("indent_neck_back"), QStringLiteral("body_position"));
    // D01
    name = QStringLiteral("neck_mid_circ");
    names.insert(name, QStringLiteral("half_girth_neck_for_shirts"));
    names.insert(name, QStringLiteral("mid_neck_girth"));
    // D02
    names.insert(QStringLiteral("neck_circ"), QStringLiteral("neck_base_girth"));
    // D03
    names.insert(QStringLiteral("highbust_circ"), QStringLiteral("upper_chest_girth"));
    // D04
    names.insert(QStringLiteral("bust_circ"), QStringLiteral("bust_girth"));
    // D05
    names.insert(QStringLiteral("lowbust_circ"), QStringLiteral("under_bust_girth"));
    // D07
    names.insert(QStringLiteral("waist_circ"), QStringLiteral("waist_girth"));
    // D08
    names.insert(QStringLiteral("highhip_circ"), QStringLiteral("high_hip_girth"));
    // D09
    name = QStringLiteral("hip_circ");
    names.insert(name, QStringLiteral("hips_excluding_protruding_abdomen"));
    names.insert(name, QStringLiteral("hip_girth"));
    // D10
    name = QStringLiteral("neck_arc_f");
    names.insert(name, QStringLiteral("half_girth_neck"));
    names.insert(name, QStringLiteral("front_neck_arc"));
    // D11
    name = QStringLiteral("highbust_arc_f");
    names.insert(name, QStringLiteral("half_girth_chest_first"));
    names.insert(name, QStringLiteral("front_upper_chest_arc"));
    // D12
    names.insert(QStringLiteral("bust_arc_f"), QStringLiteral("half_girth_chest_second"));
    // D13
    names.insert(QStringLiteral("lowbust_arc_f"), QStringLiteral("half_girth_chest_third"));
    // D15
    name = QStringLiteral("waist_arc_f");
    names.insert(name, QStringLiteral("half_girth_waist"));
    names.insert(name, QStringLiteral("front_waist_arc"));
    // D16
    names.insert(QStringLiteral("highhip_arc_f"), QStringLiteral("front_upper_hip_arc"));
    // D17
    name = QStringLiteral("hip_arc_f");
    names.insert(name, QStringLiteral("half_girth_hips_excluding_protruding_abdomen"));
    names.insert(name, QStringLiteral("front_hip_arc"));
    // D18
    names.insert(QStringLiteral("neck_arc_b"), QStringLiteral("back_neck_arc"));
    // D19
    names.insert(QStringLiteral("highbust_arc_b"), QStringLiteral("back_upper_chest_arc"));
    // D23
    names.insert(QStringLiteral("waist_arc_b"), QStringLiteral("back_waist_arc"));
    // D24
    names.insert(QStringLiteral("highhip_arc_b"), QStringLiteral("back_upper_hip_arc"));
    // D25
    names.insert(QStringLiteral("hip_arc_b"), QStringLiteral("back_hip_arc"));
    // D42
    names.insert(QStringLiteral("hip_with_abdomen_arc_f"),
                 QStringLiteral("half_girth_hips_considering_protruding_abdomen"));
    // D43
    names.insert(QStringLiteral("body_armfold_circ"), QStringLiteral("shoulder_girth"));
    // D45
    names.insert(QStringLiteral("body_torso_circ"), QStringLiteral("trunk_length"));
    // E01
    name = QStringLiteral("neck_front_to_waist_f");
    names.insert(name, QStringLiteral("front_waist_length"));
    names.insert(name, QStringLiteral("center_front_waist_length"));
    // E03
    names.insert(QStringLiteral("armpit_to_waist_side"), QStringLiteral("side_waist_length"));
    // E05
    name = QStringLiteral("neck_side_to_waist_f");
    names.insert(name, QStringLiteral("neck_to_front_waist_line"));
    names.insert(name, QStringLiteral("waist_to_neck_side"));
    names.insert(name, QStringLiteral("front_shoulder_to_waist_length"));
    // E08
    names.insert(QStringLiteral("neck_side_to_waist_b"), QStringLiteral("back_shoulder_to_waist_length"));
    // E09
    names.insert(QStringLiteral("neck_back_to_waist_b"), QStringLiteral("center_back_waist_length"));
    // E11
    name = QStringLiteral("neck_front_to_highbust_f");
    names.insert(name, QStringLiteral("neck_to_first_line_chest_circumference"));
    names.insert(name, QStringLiteral("front_neck_to_upper_chest_height"));
    // E12
    names.insert(QStringLiteral("neck_front_to_bust_f"), QStringLiteral("front_neck_to_bust_height"));
    // E13
    names.insert(QStringLiteral("highbust_to_waist_f"), QStringLiteral("front_waist_to_upper_chest"));
    // E15
    names.insert(QStringLiteral("lowbust_to_waist_f"), QStringLiteral("front_waist_to_lower_breast"));
    // E17
    names.insert(QStringLiteral("neck_back_to_highbust_b"), QStringLiteral("neck_to_back_line_chest_circumference"));
    // E30
    names.insert(QStringLiteral("waist_to_highhip_f"), QStringLiteral("depth_waist_first"));
    // E31
    names.insert(QStringLiteral("waist_to_hip_f"), QStringLiteral("depth_waist_second"));
    // E36
    names.insert(QStringLiteral("shoulder_slope_neck_side_angle"), QStringLiteral("shoulder_slope_degrees"));
    // E37
    names.insert(QStringLiteral("shoulder_slope_neck_side_length"), QStringLiteral("shoulder_drop"));
    // F01
    names.insert(QStringLiteral("shoulder_length"), QStringLiteral("shoulder_length"));
    // F02
    names.insert(QStringLiteral("shoulder_tip_to_shoulder_tip_f"), QStringLiteral("across_front_shoulder_width"));
    // F03
    names.insert(QStringLiteral("across_chest_f"), QStringLiteral("upper_front_chest_width"));
    // F04
    name = QStringLiteral("armfold_to_armfold_f");
    names.insert(name, QStringLiteral("chest_width"));
    names.insert(name, QStringLiteral("front_chest_width"));
    // F05
    name = QStringLiteral("shoulder_tip_to_shoulder_tip_b");
    names.insert(name, QStringLiteral("arc_behind_shoulder_girdle"));
    names.insert(name, QStringLiteral("across_back_shoulder_width"));
    // F06
    names.insert(QStringLiteral("across_back_b"), QStringLiteral("upper_back_width"));
    // F07
    names.insert(QStringLiteral("armfold_to_armfold_b"), QStringLiteral("back_width"));
    // F14
    names.insert(QStringLiteral("neck_width"), QStringLiteral("neck_transverse_diameter"));
    // G01
    names.insert(QStringLiteral("bustpoint_to_bustpoint"), QStringLiteral("bustpoint_to_bustpoint"));
    // G02
    names.insert(QStringLiteral("bustpoint_to_neck_side"), QStringLiteral("neck_to_bustpoint"));
    // G06
    names.insert(QStringLiteral("bustpoint_to_bustpoint_halter"), QStringLiteral("halter_bustpoint_to_bustpoint"));
    // H01
    name = QStringLiteral("shoulder_tip_to_waist_front");
    names.insert(name, QStringLiteral("front_slash_shoulder_height"));
    names.insert(name, QStringLiteral("front_shoulder_slope_length"));
    // H02
    names.insert(QStringLiteral("neck_front_to_waist_side"), QStringLiteral("front_waist_slope"));
    // H04
    name = QStringLiteral("neck_side_to_armfold_f");
    names.insert(name, QStringLiteral("height_armhole_slash"));
    names.insert(name, QStringLiteral("chest_slope"));
    // H07
    name = QStringLiteral("shoulder_tip_to_waist_back");
    names.insert(name, QStringLiteral("slash_shoulder_height"));
    names.insert(name, QStringLiteral("back_shoulder_slope_length"));
    // H08
    names.insert(QStringLiteral("neck_back_to_waist_side"), QStringLiteral("back_waist_slope"));
    // H10
    names.insert(QStringLiteral("neck_side_to_armfold_b"), QStringLiteral("back_slope"));
    // I05
    names.insert(QStringLiteral("arm_shoulder_tip_to_wrist"), QStringLiteral("arm_length"));
    // I06
    names.insert(QStringLiteral("arm_shoulder_tip_to_elbow"), QStringLiteral("shoulder_to_elbow_length"));
    // I08
    names.insert(QStringLiteral("arm_armpit_to_wrist"), QStringLiteral("underarm_length"));
    // I11
    names.insert(QStringLiteral("arm_upper_circ"), QStringLiteral("upper_arm_girth"));
    // I15
    names.insert(QStringLiteral("arm_wrist_circ"), QStringLiteral("wrist_girth"));
    // I17
    names.insert(QStringLiteral("armscye_circ"), QStringLiteral("armscye_girth"));
    // I19
    names.insert(QStringLiteral("armscye_width"), QStringLiteral("anteroposterior_diameter_hands"));
    // I20
    names.insert(QStringLiteral("arm_neck_side_to_finger_tip"), QStringLiteral("neck_to_third_finger"));
    // I21
    name = QStringLiteral("arm_neck_side_to_wrist");
    names.insert(name, QStringLiteral("neck_to_radial_point"));
    names.insert(name, QStringLiteral("shoulder_and_arm_length"));
    // J01
    names.insert(QStringLiteral("leg_crotch_to_floor"), QStringLiteral("crotch_height"));
    // J02
    names.insert(QStringLiteral("leg_waist_side_to_floor"), QStringLiteral("side_waist_to_floor"));
    // J03
    names.insert(QStringLiteral("leg_waist_side_to_knee"), QStringLiteral("waist_to_knee"));
    // J04
    names.insert(QStringLiteral("leg_thigh_upper_circ"), QStringLiteral("thigh_girth"));
    // J05
    names.insert(QStringLiteral("leg_thigh_mid_circ"), QStringLiteral("mid_thigh_girth"));
    // J06
    names.insert(QStringLiteral("leg_knee_circ"), QStringLiteral("knee_girth"));
    // J08
    names.insert(QStringLiteral("leg_calf_circ"), QStringLiteral("calf_girth"));
    // J10
    names.insert(QStringLiteral("leg_ankle_circ"), QStringLiteral("ankle_girth"));
    // J11
    names.insert(QStringLiteral("leg_knee_circ_bent"), QStringLiteral("girth_knee_flexed_feet"));
    // K01
    name = QStringLiteral("crotch_length");
    names.insert(name, QStringLiteral("arc_through_groin_area"));
    names.insert(name, QStringLiteral("crotch_length"));
    // K04
    name = QStringLiteral("rise_length_side");
    names.insert(name, QStringLiteral("waist_to_plane_seat"));
    names.insert(name, QStringLiteral("rise_height"));
    // L02
    name = QStringLiteral("hand_length");
    names.insert(name, QStringLiteral("hand_vertical_diameter"));
    names.insert(name, QStringLiteral("hand_length"));
    // L03
    names.insert(QStringLiteral("hand_palm_width"), QStringLiteral("hand_width"));
    // L05
    names.insert(QStringLiteral("hand_circ"), QStringLiteral("hand_girth"));
    // M01
    names.insert(QStringLiteral("foot_width"), QStringLiteral("foot_width"));
    // M02
    names.insert(QStringLiteral("foot_length"), QStringLiteral("foot_length"));
    // M04
    names.insert(QStringLiteral("foot_instep_circ"), QStringLiteral("girth_foot_instep"));
    // N02
    names.insert(QStringLiteral("head_length"), QStringLiteral("head_height"));
    // N05
    names.insert(QStringLiteral("head_crown_to_neck_back"), QStringLiteral("head_and_neck_length"));
    // N06
    names.insert(QStringLiteral("head_chin_to_neck_back"), QStringLiteral("neck_to_neck_base"));
    // O02
    names.insert(QStringLiteral("waist_to_waist_halter"), QStringLiteral("arc_length_upper_body"));
    // O09
    names.insert(QStringLiteral("arm_neck_back_to_wrist_bent"), QStringLiteral("cervicale_to_wrist_length"));
    // P11
    names.insert(QStringLiteral("highbust_b_over_shoulder_to_highbust_f"), QStringLiteral("strap_length"));
    // P12
    names.insert(QStringLiteral("armscye_arc"), QStringLiteral("arc_through_shoulder_joint"));

    names.insert(QStringLiteral("head_circ"), QStringLiteral("head_girth"));

    return names;
}

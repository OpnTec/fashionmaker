/************************************************************************
 **
 **  @file   vtranslatemeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 8, 2015
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

#include "vtranslatemeasurements.h"
#include "../vmisc/def.h"

using namespace qmu;

//---------------------------------------------------------------------------------------------------------------------
VTranslateMeasurements::VTranslateMeasurements()
    :measurements(QMap<QString, QmuTranslation>()),
      guiTexts(QMap<QString, QmuTranslation>()),
      descriptions(QMap<QString, QmuTranslation>()),
      numbers(QMap<QString, QString>()),
      formulas(QMap<QString, QString>())
{
    InitMeasurements();
}

//---------------------------------------------------------------------------------------------------------------------
VTranslateMeasurements::~VTranslateMeasurements()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MeasurementsFromUser translate measurement to internal look.
 * @param newFormula [in|out] expression to translate
 * @param position token position
 * @param token token to translate
 * @param bias hold change of length between translated and origin token string
 * @return true if was found measurement with same name.
 */
bool VTranslateMeasurements::MeasurementsFromUser(QString &newFormula, int position, const QString &token,
                                                  int &bias) const
{
    QMap<QString, QmuTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if (token == i.value().translate())
        {
            newFormula.replace(position, token.length(), i.key());
            bias = token.length() - i.key().length();
            return true;
        }
        ++i;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::MToUser(const QString &measurement) const
{
    if (measurements.contains(measurement))
    {
        return measurements.value(measurement).translate();
    }
    else
    {
        return measurement;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::MNumber(const QString &measurement) const
{
    return numbers.value(measurement);
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::MFormula(const QString &measurement) const
{
    return formulas.value(measurement);
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::GuiText(const QString &measurement) const
{
    if (guiTexts.contains(measurement))
    {
        return guiTexts.value(measurement).translate();
    }
    else
    {
        return measurement;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::Description(const QString &measurement) const
{
    if (descriptions.contains(measurement))
    {
        return descriptions.value(measurement).translate();
    }
    else
    {
        return measurement;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::Retranslate()
{
    measurements.clear();
    guiTexts.clear();
    descriptions.clear();
    numbers.clear();
    formulas.clear();
    InitMeasurements();
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitMeasurements()
{
    InitGroupA(); // Direct Height
    InitGroupB(); // Direct Width
    InitGroupC(); // Indentation
    InitGroupD(); // Circumference and Arc
    InitGroupE(); // Vertical
    InitGroupF(); // Horizontal
    InitGroupG(); // Bust
    InitGroupH(); // Balance
    InitGroupI(); // Arm
    InitGroupJ(); // Leg
    InitGroupK(); // Crotch and Rise
    InitGroupL(); // Hand
    InitGroupM(); // Foot
    InitGroupN(); // Head
    InitGroupO(); // Men & Tailoring
    InitGroupP(); // Historical & Specialty
    InitGroupQ(); // Patternmaking measurements
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitMeasurement(const QString &name, const QmuTranslation &m, const QmuTranslation &g,
                                             const QmuTranslation &d, const QString &number, const QString &formula)
{
    measurements.insert(name, m);
    guiTexts.insert(name, g);
    descriptions.insert(name, d);
    numbers.insert(name, number);
    formulas.insert(name, formula);
}

#define translate(source, disambiguation) QmuTranslation::translate("VTranslateMeasurements",(source),(disambiguation))

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupA()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("height", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Total", "Full measurement name.");
    d = translate("Vertical distance from crown of head to floor.", "Full measurement description.");
    InitMeasurement(height_M, m, g, d, "A01");
    //=================================================================================================================
    m = translate("height_neck_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Neck Back", "Full measurement name.");
    d = translate("Vertical distance from the Neck Back (cervicale vertebra) to the floor.",
                  "Full measurement description.");
    InitMeasurement(heightNeckBack_M, m, g, d, "A02");
    //=================================================================================================================
    m = translate("height_scapula", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Scapula", "Full measurement name.");
    d = translate("Vertical distance from the Scapula (Blade point) to the floor.", "Full measurement description.");
    InitMeasurement(heightScapula_M, m, g, d, "A03");
    //=================================================================================================================
    m = translate("height_armpit", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Armpit", "Full measurement name.");
    d = translate("Vertical distance from the Armpit to the floor.", "Full measurement description.");
    InitMeasurement(heightArmpit_M, m, g, d, "A04");
    //=================================================================================================================
    m = translate("height_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Waist Side", "Full measurement name.");
    d = translate("Vertical distance from the Waist Side to the floor.", "Full measurement description.");
    InitMeasurement(heightWaistSide_M, m, g, d, "A05");
    //=================================================================================================================
    m = translate("height_hip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Hip", "Full measurement name.");
    d = translate("Vertical distance from the Hip level to the floor.", "Full measurement description.");
    InitMeasurement(heightHip_M, m, g, d, "A06");
    //=================================================================================================================
    m = translate("height_gluteal_fold", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Gluteal Fold", "Full measurement name.");
    d = translate("Vertical distance from the Gluteal fold, where the Gluteal muscle meets the top of the back thigh, "
                  "to the floor.", "Full measurement description.");
    InitMeasurement(heightGlutealFold_M, m, g, d, "A07");
    //=================================================================================================================
    m = translate("height_knee", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Knee", "Full measurement name.");
    d = translate("Vertical distance from the fold at the back of the Knee to the floor.",
                  "Full measurement description.");
    InitMeasurement(heightKnee_M, m, g, d, "A08");
    //=================================================================================================================
    m = translate("height_calf", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Calf", "Full measurement name.");
    d = translate("Vertical distance from the widest point of the calf to the floor.", "Full measurement description.");
    InitMeasurement(heightCalf_M, m, g, d, "A09");
    //=================================================================================================================
    m = translate("height_ankle_high", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Ankle High", "Full measurement name.");
    d = translate("Vertical distance from the deepest indentation of the back of the ankle to the floor.",
                  "Full measurement description.");
    InitMeasurement(heightAnkleHigh_M, m, g, d, "A10");
    //=================================================================================================================
    m = translate("height_ankle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Ankle", "Full measurement name.");
    d = translate("Vertical distance from point where the front leg meets the foot to the floor.",
                  "Full measurement description.");
    InitMeasurement(heightAnkle_M, m, g, d, "A11");
    //=================================================================================================================
    m = translate("height_highhip","Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Highhip", "Full measurement name.");
    d = translate("Vertical distance from the Highhip level, where front abdomen is most prominent, to the floor.",
                  "Full measurement description.");
    InitMeasurement(heightHighhip_M, m, g, d, "A12");
    //=================================================================================================================
    m = translate("height_waist_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Waist Front", "Full measurement name.");
    d = translate("Vertical distance from the Waist Front to the floor.", "Full measurement description.");
    InitMeasurement(heightWaistFront_M, m, g, d, "A13");
    //=================================================================================================================
    m = translate("height_bustpoint", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Bustpoint", "Full measurement name.");
    d = translate("Vertical distance from Bustpoint to the floor.", "Full measurement description.");
    InitMeasurement(heightBustpoint_M, m, g, d, "A14");
    //=================================================================================================================
    m = translate("height_shoulder_tip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Shoulder Tip", "Full measurement name.");
    d = translate("Vertical distance from the Shoulder Tip to the floor.", "Full measurement description.");
    InitMeasurement(heightShoulderTip_M, m, g, d, "A15");
    //=================================================================================================================
    m = translate("height_neck_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Neck Front", "Full measurement name.");
    d = translate("Vertical distance from the Neck Front to the floor.", "Full measurement description.");
    InitMeasurement(heightNeckFront_M, m, g, d, "A16");
    //=================================================================================================================
    m = translate("height_neck_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Neck Side", "Full measurement name.");
    d = translate("Vertical distance from the Neck Side to the floor.", "Full measurement description.");
    InitMeasurement(heightNeckSide_M, m, g, d, "A17");
    //=================================================================================================================
    m = translate("height_neck_back_to_knee", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Neck Back to Knee", "Full measurement name.");
    d = translate("Vertical distance from the Neck Back (cervicale vertebra) to the fold at the back of the knee.",
                  "Full measurement description.");
    InitMeasurement(heightNeckBackToKnee_M, m, g, d, "A18", "(height_neck_back - height_knee)");
    //=================================================================================================================
    m = translate("height_waist_side_to_knee", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Waist Side to Knee", "Full measurement name.");
    d = translate("Vertical distance from the Waist Side to the fold at the back of the knee.",
                  "Full measurement description.");
    InitMeasurement(heightWaistSideToKnee_M, m, g, d, "A19", "(height_waist_side - height_knee)");
    //=================================================================================================================
    m = translate("height_waist_side_to_hip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Waist Side to Hip", "Full measurement name.");
    d = translate("Vertical distance from the Waist Side to the Hip level.", "Full measurement description.");
    InitMeasurement(heightWaistSideToHip_M, m, g, d, "A20", "(height_waist_side - height_hip)");
    //=================================================================================================================
    m = translate("height_knee_to_ankle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Knee to Ankle", "Full measurement name.");
    d = translate("Vertical distance from the fold at the back of the knee to the point where the front leg meets the "
                  "top of the foot.", "Full measurement description.");
    InitMeasurement(heightKneeToAnkle_M, m, g, d, "A21", "(height_knee - height_ankle)");
    //=================================================================================================================
    m = translate("height_neck_back_to_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Height: Neck Back to Waist Side", "Full measurement name.");
    d = translate("Vertical distance from Neck Back to Waist Side. ('Height: Neck Back' - 'Height: Waist Side').",
                  "Full measurement description.");
    InitMeasurement(heightNeckBackToWaistSide_M, m, g, d, "A22", "(height_neck_back - height_waist_side)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupB()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("width_shoulder", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Width: Shoulder", "Full measurement name.");
    d = translate("Horizontal distance from Shoulder Tip to Shoulder Tip.", "Full measurement description.");
    InitMeasurement(widthShoulder_M, m, g, d, "B01");
    //=================================================================================================================
    m = translate("width_bust", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Width: Bust", "Full measurement name.");
    d = translate("Horizontal distance from Bust Side to Bust Side.", "Full measurement description.");
    InitMeasurement(widthBust_M, m, g, d, "B02");
    //=================================================================================================================
    m = translate("width_waist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Width: Waist", "Full measurement name.");
    d = translate("Horizontal distance from Waist Side to Waist Side.", "Full measurement description.");
    InitMeasurement(widthWaist_M, m, g, d, "B03");
    //=================================================================================================================
    m = translate("width_hip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Width: Hip", "Full measurement name.");
    d = translate("Horizontal distance from Hip Side to Hip Side.", "Full measurement description.");
    InitMeasurement(widthHip_M, m, g, d, "B04");
    //=================================================================================================================
    m = translate("width_abdomen_to_hip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Width: Abdomen to Hip", "Full measurement name.");
    d = translate("Horizontal distance from the greatest abdomen prominence to the greatest hip prominence.",
                  "Full measurement description.");
    InitMeasurement(widthAbdomenToHip_M, m, g, d, "B05");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupC()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("indent_neck_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Indent: Neck Back", "Full measurement name.");
    d = translate("Horizontal distance from Scapula (Blade point) to the Neck Back.", "Full measurement description.");
    InitMeasurement(indentNeckBack_M, m, g, d, "C01");
    //=================================================================================================================
    m = translate("indent_waist_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Indent: Waist Back", "Full measurement name.");
    d = translate("Horizontal distance between a flat stick, placed to touch Hip and Scapula, and Waist Back.",
                  "Full measurement description.");
    InitMeasurement(indentWaistBack_M, m, g, d, "C02");
    //=================================================================================================================
    m = translate("indent_ankle_high", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Indent: Ankle High", "Full measurement name.");
    d = translate("Horizontal Distance betwee a  flat stick, placed perpendicular to Heel, and the greatest "
                  "indentation of Ankle.", "Full measurement description.");
    InitMeasurement(indentAnkleHigh_M, m, g, d, "C03");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupD()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("hand_palm_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hand: Palm length", "Full measurement name.");
    d = translate("Length from Wrist line to base of middle finger.", "Full measurement description.");
    InitMeasurement(handPalmLength_M, m, g, d, "D01");
    //=================================================================================================================
    m = translate("hand_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hand: Length", "Full measurement name.");
    d = translate("Length from Wrist line to end of middle finger.", "Full measurement description.");
    InitMeasurement(handLength_M, m, g, d, "D02");
    //=================================================================================================================
    m = translate("hand_palm_width", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hand: Palm width", "Full measurement name.");
    d = translate("Measure where Palm is widest.", "Full measurement description.");
    InitMeasurement(handPalmWidth_M, m, g, d, "D03");
    //=================================================================================================================
    m = translate("hand_palm_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hand: Palm circumference", "Full measurement name.");
    d = translate("Circumference where Palm is widest.", "Full measurement description.");
    InitMeasurement(handPalmCirc_M, m, g, d, "D04");
    //=================================================================================================================
    m = translate("hand_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hand: Circumference", "Full measurement name.");
    d = translate("Tuck thumb toward smallest finger, bring fingers close together. Measure circumference around "
                  "widest part of hand.", "Full measurement description.");
    InitMeasurement(handCirc_M, m, g, d, "D05");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupE()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("foot_width", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Foot: Width", "Full measurement name.");
    d = translate("Measure at widest part of foot.", "Full measurement description.");
    InitMeasurement(footWidth_M, m, g, d, "E01");
    //=================================================================================================================
    m = translate("foot_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Foot: Length", "Full measurement name.");
    d = translate("Measure from back of heel to end of longest toe.", "Full measurement description.");
    InitMeasurement(footLength_M, m, g, d, "E02");
    //=================================================================================================================
    m = translate("foot_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Foot: Circumference", "Full measurement name.");
    d = translate("Measure circumference around widest part of foot.", "Full measurement description.");
    InitMeasurement(footCirc_M, m, g, d, "E03");
    //=================================================================================================================
    m = translate("foot_instep_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Foot: Instep circumference", "Full measurement name.");
    d = translate("Measure circumference at tallest part of instep.", "Full measurement description.");
    InitMeasurement(footInstepCirc_M, m, g, d, "E04");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupF()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("head_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Circumference", "Full measurement name.");
    d = translate("Measure circumference at largest level of head.", "Full measurement description.");
    InitMeasurement(headCirc_M, m, g, d, "F01");
    //=================================================================================================================
    m = translate("head_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Length", "Full measurement name.");
    d = translate("Vertical distance from Head Crown to bottom of jaw.", "Full measurement description.");
    InitMeasurement(headLength_M, m, g, d, "F02");
    //=================================================================================================================
    m = translate("head_depth", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Depth", "Full measurement name.");
    d = translate("Horizontal distance from front of forehead to back of head.", "Full measurement description.");
    InitMeasurement(headDepth_M, m, g, d, "F03");
    //=================================================================================================================
    m = translate("head_width", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Width", "Full measurement name.");
    d = translate("Horizontal distance from Head Side to Head Side, where Head is widest.",
                  "Full measurement description.");
    InitMeasurement(headWidth_M, m, g, d, "F04");
    //=================================================================================================================
    m = translate("head_crown_to_neck_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Crown to Neck Back", "Full measurement name.");
    d = translate("Vertical distance from Crown to Neck Back. ('Height: Total' - 'Height: Neck Back').",
                  "Full measurement description.");
    InitMeasurement(headCrownToNeckBack_M, m, g, d, "F05", "(height - height_neck_back)");
    //=================================================================================================================
    m = translate("head_chin_to_neck_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Head: Chin to Neck Back", "Full measurement name.");
    d = translate("Vertical distance from Chin to Neck Back. ('Height' - 'Height: Neck Back' - 'Head: Length')",
                  "Full measurement description.");
    InitMeasurement(headChinToNeckBack_M, m, g, d, "F06", "(height - height_neck_back - head_length)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupG()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("neck_mid_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck circumference, midsection", "Full measurement name.");
    d = translate("Circumference of Neck midsection, about halfway between jaw and torso.",
                  "Full measurement description.");
    InitMeasurement(neckMidCirc_M, m, g, d, "G01");
    //=================================================================================================================
    m = translate("neck_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck circumference", "Full measurement name.");
    d = translate("Neck circumference at base of Neck, touching Neck Back, Neck Sides, and Neck Front.",
                  "Full measurement description.");
    InitMeasurement(neckCirc_M, m, g, d, "G02");
    //=================================================================================================================
    m = translate("highbust_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust circumference", "Full measurement name.");
    d = translate("Circumference at Highbust, following shortest distance between Armfolds across chest, high under "
                  "armpits.", "Full measurement description.");
    InitMeasurement(highbustCirc_M, m, g, d, "G03");
    //=================================================================================================================
    m = translate("bust_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust circumference", "Full measurement name.");
    d = translate("Circumference around Bust, parallel to floor.", "Full measurement description.");
    InitMeasurement(bustCirc_M, m, g, d, "G04");
    //=================================================================================================================
    m = translate("lowbust_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust circumference", "Full measurement name.");
    d = translate("Circumference around LowBust under the breasts, parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(lowbustCirc_M, m, g, d, "G05");
    //=================================================================================================================
    m = translate("rib_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib circumference", "Full measurement name.");
    d = translate("Circumference around Ribs at level of the lowest rib at the side, parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(ribCirc_M, m, g, d, "G06");
    //=================================================================================================================
    m = translate("waist_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist circumference", "Full measurement name.");
    d = translate("Circumference around Waist, following natural contours. Waists are  typically higher in back.",
                  "Full measurement description.");
    InitMeasurement(waistCirc_M, m, g, d, "G07");
    //=================================================================================================================
    m = translate("highhip_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highhip circumference", "Full measurement name.");
    d = translate("Circumference around Highhip, where Abdomen protrusion is  greatest, parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(highhipCirc_M, m, g, d, "G08");
    //=================================================================================================================
    m = translate("hip_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip circumference", "Full measurement name.");
    d = translate("Circumference around Hip where Hip protrusion is greatest, parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(hipCirc_M, m, g, d, "G09");
    //=================================================================================================================
    m = translate("neck_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck arc, front", "Full measurement name.");
    d = translate("From Neck Side to Neck Side through Neck Front.", "Full measurement description.");
    InitMeasurement(neckArcF_M, m, g, d, "G10");
    //=================================================================================================================
    m = translate("highbust_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust arc, front", "Full measurement name.");
    d = translate("From Highbust Side (Armpit) to HIghbust Side (Armpit) across chest.",
                  "Full measurement description.");
    InitMeasurement(highbustArcF_M, m, g, d, "G11");
    //=================================================================================================================
    m = translate("bust_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust arc, front", "Full measurement name.");
    d = translate("From Bust Side to Bust Side across chest.", "Full measurement description.");
    InitMeasurement(bustArcF_M, m, g, d, "G12");
    //=================================================================================================================
    m = translate("lowbust_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust arc, front", "Full measurement name.");
    d = translate("From Lowbust Side to Lowbust Side across front.", "Full measurement description.");
    InitMeasurement(lowbustArcF_M, m, g, d, "G13");
    //=================================================================================================================
    m = translate("rib_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib arc, front", "Full measurement name.");
    d = translate("From Rib Side to Rib Side, across front.", "Full measurement description.");
    InitMeasurement(ribArcF_M, m, g, d, "G14");
    //=================================================================================================================
    m = translate("waist_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist arc, front", "Full measurement name.");
    d = translate("From Waist Side to Waist Side across front.", "Full measurement description.");
    InitMeasurement(waistArcF_M, m, g, d, "G15");
    //=================================================================================================================
    m = translate("highhip_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highhip arc, front", "Full measurement name.");
    d = translate("From Highhip Side to Highhip Side across front.", "Full measurement description.");
    InitMeasurement(highhipArcF_M, m, g, d, "G16");
    //=================================================================================================================
    m = translate("hip_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip arc, front", "Full measurement name.");
    d = translate("From Hip Side to Hip Side across Front.", "Full measurement description.");
    InitMeasurement(hipArcF_M, m, g, d, "G17");
    //=================================================================================================================
    m = translate("neck_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck arc, front, half", "Full measurement name.");
    d = translate("Half of 'Neck arc, front'. ('Neck arc, front' / 2).", "Full measurement description.");
    InitMeasurement(neckArcHalfF_M, m, g, d, "G18", "(neck_arc_f/2)");
    //=================================================================================================================
    m = translate("highbust_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust arc, front, half", "Full measurement name.");
    d = translate("Half of 'Highbust arc, front'. From Highbust Front to Highbust Side. ('Highbust arc,  front' / 2).",
                  "Full measurement description.");
    InitMeasurement(highbustArcHalfF_M, m, g, d, "G19", "(highbust_arc_f/2)");
    //=================================================================================================================
    m = translate("bust_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust arc, front, half", "Full measurement name.");
    d = translate("Half of 'Bust arc, front'. ('Bust arc, front'/2).", "Full measurement description.");
    InitMeasurement(bustArcHalfF_M, m, g, d, "G20", "(bust_arc_f/2)");
    //=================================================================================================================
    m = translate("lowbust_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust arc, front, half", "Full measurement name.");
    d = translate("Half of 'Lowbust arc, front'.  ('Lowbust Arc, front' / 2).", "Full measurement description.");
    InitMeasurement(lowbustArcHalfF_M, m, g, d, "G21", "(lowbust_arc_f/2)");
    //=================================================================================================================
    m = translate("rib_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib arc, front, half", "Full measurement name.");
    d = translate("Half of 'Rib arc, front'.   ('Rib Arc, front' / 2).", "Full measurement description.");
    InitMeasurement(ribArcHalfF_M, m, g, d, "G22", "(rib_arc_f/2)");
    //=================================================================================================================
    m = translate("waist_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist arc, front, half", "Full measurement name.");
    d = translate("Half of 'Waist arc, front'. ('Waist arc, front' / 2).", "Full measurement description.");
    InitMeasurement(waistArcHalfF_M, m, g, d, "G23", "(waist_arc_f/2)");
    //=================================================================================================================
    m = translate("highhip_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highhip arc, front, half", "Full measurement name.");
    d = translate("Half of 'Highhip arc, front'.  ('Highhip arc, front' / 2).", "Full measurement description.");
    InitMeasurement(highhipArcHalfF_M, m, g, d, "G24", "(highhip_arc_f/2)");
    //=================================================================================================================
    m = translate("hip_arc_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip arc, front, half", "Full measurement name.");
    d = translate("Half of 'Hip arc, front'. ('Hip arc, front' / 2).", "Full measurement description.");
    InitMeasurement(hipArcHalfF_M, m, g, d, "G25", "(hip_arc_f/2)");
    //=================================================================================================================
    m = translate("neck_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck arc, back", "Full measurement name.");
    d = translate("From Neck Side to Neck Side across back. ('Neck circumference' - 'Neck arc, front').",
                  "Full measurement description.");
    InitMeasurement(neckArcB_M, m, g, d, "G26", "(neck_circ - neck_arc_f)");
    //=================================================================================================================
    m = translate("highbust_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust arc, back", "Full measurement name.");
    d = translate("From Highbust Side  to Highbust Side across back. ('Highbust circumference' - 'Highbust arc, "
                  "front').", "Full measurement description.");
    InitMeasurement(highbustArcB_M, m, g, d, "G27", "(highbust_circ - highbust_arc_f)");
    //=================================================================================================================
    m = translate("bust_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust arc, back", "Full measurement name.");
    d = translate("From Bust Side to Bust Side across back. ('Bust circumference' - 'Bust arc, front').",
                  "Full measurement description.");
    InitMeasurement(bustArcB_M, m, g, d, "G28", "(bust_circ - bust_arc_f)");
    //=================================================================================================================
    m = translate("lowbust_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust arc, back", "Full measurement name.");
    d = translate("From Lowbust Side to Lowbust Side across back.  ('Lowbust circumference' - 'Lowbust arc, front').",
                  "Full measurement description.");
    InitMeasurement(lowbustArcB_M, m, g, d, "G29", "(lowbust_circ - lowbust_arc_f)");
    //=================================================================================================================
    m = translate("rib_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib arc, back", "Full measurement name.");
    d = translate("From Rib Side to Rib side across back. ('Rib circumference' - 'Rib arc, front').",
                  "Full measurement description.");
    InitMeasurement(ribArcB_M, m, g, d, "G30", "(rib_circ - rib_arc_f)");
    //=================================================================================================================
    m = translate("waist_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist arc, back", "Full measurement name.");
    d = translate("From Waist Side to Waist Side across back. ('Waist circumference' - 'Waist arc, front').",
                  "Full measurement description.");
    InitMeasurement(waistArcB_M, m, g, d, "G31", "(waist_circ - waist_arc_f)");
    //=================================================================================================================
    m = translate("highhip_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highhip arc, back", "Full measurement name.");
    d = translate("From Highhip Side to Highhip Side across back. ('Highhip circumference' - 'Highhip arc, front').",
                  "Full measurement description.");
    InitMeasurement(highhipArcB_M, m, g, d, "G32", "(highhip_circ - highhip_arc_f)");
    //=================================================================================================================
    m = translate("hip_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip arc, back", "Full measurement name.");
    d = translate("From Hip Side to Hip Side across back. ('Hip circumference' - 'Hip arc, front').",
                  "Full measurement description.");
    InitMeasurement(hipArcB_M, m, g, d, "G33", "(hip_circ - hip_arc_f)");
    //=================================================================================================================
    m = translate("neck_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck arc, back, half", "Full measurement name.");
    d = translate("Half of 'Neck arc, back'. ('Neck arc, back' / 2).", "Full measurement description.");
    InitMeasurement(neckArcHalfB_M, m, g, d, "G34", "(neck_arc_b/2)");
    //=================================================================================================================
    m = translate("highbust_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust arc, back, half", "Full measurement name.");
    d = translate("Half of 'Highbust arc, back'. From Highbust Back to Highbust Side. ('Highbust arc, back' / 2).",
                  "Full measurement description.");
    InitMeasurement(highbustArcHalfB_M, m, g, d, "G35", "(highbust_arc_b/2)");
    //=================================================================================================================
    m = translate("bust_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust arc, back, half", "Full measurement name.");
    d = translate("Half of 'Bust arc, back'. ('Bust arc, back' / 2).", "Full measurement description.");
    InitMeasurement(bustArcHalfB_M, m, g, d, "G36", "(bust_arc_b/2)");
    //=================================================================================================================
    m = translate("lowbust_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust arc, back, half", "Full measurement name.");
    d = translate("Half of 'Lowbust Arc, back'. ('Lowbust arc, back' / 2).", "Full measurement description.");
    InitMeasurement(lowbustArcHalfB_M, m, g, d, "G37", "(lowbust_arc_b/2)");
    //=================================================================================================================
    m = translate("rib_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib arc, back, half", "Full measurement name.");
    d = translate("Half of 'Rib arc, back'. ('Rib arc, back' / 2).", "Full measurement description.");
    InitMeasurement(ribArcHalfB_M, m, g, d, "G38", "(rib_arc_b/2)");
    //=================================================================================================================
    m = translate("waist_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist arc, back, half", "Full measurement name.");
    d = translate("Half of 'Waist arc, back'. ('Waist  arc, back' / 2).", "Full measurement description.");
    InitMeasurement(waistArcHalfB_M, m, g, d, "G39", "(waist_arc_b/2)");
    //=================================================================================================================
    m = translate("highhip_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highhip arc, back, half", "Full measurement name.");
    d = translate("Half of 'Highhip arc, back'. From Highhip Back to Highbust Side. ('Highhip arc, back'/ 2).",
           "Full measurement description.");
    InitMeasurement(highhipArcHalfB_M, m, g, d, "G40", "(highhip_arc_b/2)");
    //=================================================================================================================
    m = translate("hip_arc_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip arc, back, half", "Full measurement name.");
    d = translate("Half of 'Hip arc, back'. ('Hip arc, back' / 2).", "Full measurement description.");
    InitMeasurement(hipArcHalfB_M, m, g, d, "G41", "(hip_arc_b/2)");
    //=================================================================================================================
    m = translate("hip_with_abdomen_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Hip arc with Abdomen, front", "Full measurement name.");
    d = translate("Curve stiff paper around front of abdomen, tape at sides. Measure from Hip Side to Hip Side over "
                  "paper across front.", "Full measurement description.");
    InitMeasurement(hipWithAbdomenArcF_M, m, g, d, "G42");
    //=================================================================================================================
    m = translate("body_armfold_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Body circumference at Armfold level", "Full measurement name.");
    d = translate("Measure around arms and torso at Armfold level.", "Full measurement description.");
    InitMeasurement(bodyArmfoldCirc_M, m, g, d, "G43");
    //=================================================================================================================
    m = translate("body_bust_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Body circumference at Bust level", "Full measurement name.");
    d = translate("Measure around arms and torso at Bust level.", "Full measurement description.");
    InitMeasurement(bodyBustCirc_M, m, g, d, "G44");
    //=================================================================================================================
    m = translate("body_torso_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Body circumference of full torso", "Full measurement name.");
    d = translate("Circumference around torso from mid-shoulder around crotch back up to mid-shoulder.",
                  "Full measurement description.");
    InitMeasurement(bodyTorsoCirc_M, m, g, d, "G45");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupH()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("neck_front_to_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Waist Front", "Full measurement name.");
    d = translate("From Neck Front, over tape between Breastpoints, down to Waist Front.",
                  "Full measurement description.");
    InitMeasurement(neckFrontToWaistF_M, m, g, d, "H01");
    //=================================================================================================================
    m = translate("neck_front_to_waist_flat_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Waist Front flat", "Full measurement name.");
    d = translate("From Neck Front down between breasts to Waist Front.", "Full measurement description.");
    InitMeasurement(neckFrontToWaistFlatF_M, m, g, d, "H02");
    //=================================================================================================================
    m = translate("armpit_to_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armpit to Waist Side", "Full measurement name.");
    d = translate("From Armpit down to Waist Side.", "Full measurement description.");
    InitMeasurement(armpitToWaistSide_M, m, g, d, "H03");
    //=================================================================================================================
    m = translate("shoulder_tip_to_waist_side_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Waist Side, front", "Full measurement name.");
    d = translate("From Shoulder Tip, curving around Armscye Front, then down to Waist Side.",
           "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideF_M, m, g, d, "H04");
    //=================================================================================================================
    m = translate("neck_side_to_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist level, front", "Full measurement name.");
    d = translate("From Neck Side straight down front to Waist level.", "Full measurement description.");
    InitMeasurement(neckSideToWaistF_M, m, g, d, "H05");
    //=================================================================================================================
    m = translate("neck_side_to_waist_bustpoint_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist level, through Bustpoint", "Full measurement name.");
    d = translate("From Neck Side over Bustpoint to Waist level, forming a straight line.",
                  "Full measurement description.");
    InitMeasurement(neckSideToWaistBustpointF_M, m, g, d, "H06");
    //=================================================================================================================
    m = translate("neck_front_to_highbust_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Highbust Front", "Full measurement name.");
    d = translate("Neck Front down to Highbust Front.", "Full measurement description.");
    InitMeasurement(neckFrontToHighbustF_M, m, g, d, "H07");
    //=================================================================================================================
    m = translate("highbust_to_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust Front to Waist Front", "Full measurement name.");
    d = translate("From Highbust Front to Waist Front. Use tape to bridge gap between Bustpoints. ('Neck Front to "
                  "Waist Front' - 'Neck Front to Highbust Front').", "Full measurement description.");
    InitMeasurement(highbustToWaistF_M, m, g, d, "H08", "(neck_front_to_waist_f - neck_front_to_highbust_f)");
    //=================================================================================================================
    m = translate("neck_front_to_bust_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Bust Front", "Full measurement name.");
    d = translate("From Neck Front down to Bust Front. Requires tape to cover gap between Bustpoints.",
           "Full measurement description.");
    InitMeasurement(neckFrontToBustF_M, m, g, d, "H09");
    //=================================================================================================================
    m = translate("bust_to_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust Front to Waist Front", "Full measurement name.");
    d = translate("From Bust Front down to Waist level. ('Neck Front to Waist Front' - 'Neck Front to Bust Front').",
                  "Full measurement description.");
    InitMeasurement(bustToWaistF_M, m, g, d, "H10", "(neck_front_to_waist_f - neck_front_to_bust_f)");
    //=================================================================================================================
    m = translate("lowbust_to_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust Front to Waist Front", "Full measurement name.");
    d = translate("From Lowbust Front down to Waist Front.", "Full measurement description.");
    InitMeasurement(lowbustToWaistF_M, m, g, d, "H11");
    //=================================================================================================================
    m = translate("rib_to_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rib Side to Waist Side", "Full measurement name.");
    d = translate("From lowest rib at side down to Waist Side.", "Full measurement description.");
    InitMeasurement(ribToWaistSide_M, m, g, d, "H12");
    //=================================================================================================================
    m = translate("shoulder_tip_to_armfold_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Armfold Front", "Full measurement name.");
    d = translate("From Shoulder Tip around Armscye down to Armfold Front.", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldF_M, m, g, d, "H13");
    //=================================================================================================================
    m = translate("neck_side_to_bust_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Bust level, front", "Full measurement name.");
    d = translate("From Neck Side straight down front to Bust level.", "Full measurement description.");
    InitMeasurement(neckSideToBustF_M, m, g, d, "H14");
    //=================================================================================================================
    m = translate("neck_side_to_highbust_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Highbust level, front", "Full measurement name.");
    d = translate("From Neck Side straight down front to Highbust level.", "Full measurement description.");
    InitMeasurement(neckSideToHighbustF_M, m, g, d, "H15");
    //=================================================================================================================
    m = translate("shoulder_center_to_highbust_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder center to Highbust level, front", "Full measurement name.");
    d = translate("From mid-Shoulder down front to Highbust level, aimed at Bustpoint.",
                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustF_M, m, g, d, "H16");
    //=================================================================================================================
    m = translate("shoulder_tip_to_waist_side_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Waist Side, back", "Full measurement name.");
    d = translate("From Shoulder Tip, curving around Armscye Back, then down to Waist Side.",
                  "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideB_M, m, g, d, "H17");
    //=================================================================================================================
    m = translate("neck_side_to_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist level, back", "Full measurement name.");
    d = translate("From Neck Side straight down back to Waist level.", "Full measurement description.");
    InitMeasurement(neckSideToWaistB_M, m, g, d, "H18");
    //=================================================================================================================
    m = translate("neck_back_to_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Waist Back", "Full measurement name.");
    d = translate("From Neck Back down to Waist Back.", "Full measurement description.");
    InitMeasurement(neckBackToWaistB_M, m, g, d, "H19");
    //=================================================================================================================
    m = translate("neck_side_to_waist_scapula_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist level, through Scapula", "Full measurement name.");
    d = translate("From Neck Side across Scapula down to Waist level, forming a straight line.",
                  "Full measurement description.");
    InitMeasurement(neckSideToWaistScapulaB_M, m, g, d, "H20");
    //=================================================================================================================
    m = translate("neck_back_to_highbust_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Highbust Back", "Full measurement name.");
    d = translate("From Neck Back down to Highbust Back.", "Full measurement description.");
    InitMeasurement(neckBackToHighbustB_M, m, g, d, "H21");
    //=================================================================================================================
    m = translate("highbust_to_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust Back to Waist Back", "Full measurement name.");
    d = translate("From Highbust Back down to Waist Back. ('Neck Back to Waist Back' - 'Neck Back to Highbust Back').",
                  "Full measurement description.");
    InitMeasurement(highbustToWaistB_M, m, g, d, "H22", "(neck_back_to_waist_b - neck_back_to_highbust_b)");
    //=================================================================================================================
    m = translate("neck_back_to_bust_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Bust Back", "Full measurement name.");
    d = translate("From Neck Back down to Bust Back.", "Full measurement description.");
    InitMeasurement(neckBackToBustB_M, m, g, d, "H23");
    //=================================================================================================================
    m = translate("bust_to_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bust Back to Waist Back", "Full measurement name.");
    d = translate("From Bust Back down to Waist level. ('Neck Back to Waist Back' - 'Neck Back to Bust Back').",
                  "Full measurement description.");
    InitMeasurement(bustToWaistB_M, m, g, d, "H24", "(neck_back_to_waist_b - neck_back_to_bust_b)");
    //=================================================================================================================
    m = translate("lowbust_to_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Lowbust Back to Waist Back", "Full measurement name.");
    d = translate("From Lowbust Back down to Waist Back.", "Full measurement description.");
    InitMeasurement(lowbustToWaistB_M, m, g, d, "H25");
    //=================================================================================================================
    m = translate("shoulder_tip_to_armfold_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Armfold Back", "Full measurement name.");
    d = translate("From Shoulder Tip around Armscye down to Armfold Back.", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldB_M, m, g, d, "H26");
    //=================================================================================================================
    m = translate("neck_side_to_bust_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Bust level, back", "Full measurement name.");
    d = translate("From Neck Side straight down back to Bust level.", "Full measurement description.");
    InitMeasurement(neckSideToBustB_M, m, g, d, "H27");
    //=================================================================================================================
    m = translate("neck_side_to_highbust_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Highbust level, back", "Full measurement name.");
    d = translate("From Neck Side straight down back to Highbust level.", "Full measurement description.");
    InitMeasurement(neckSideToHighbustB_M, m, g, d, "H28");
    //=================================================================================================================
    m = translate("shoulder_center_to_highbust_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder center to Highbust level, back", "Full measurement name.");
    d = translate("From mid-Shoulder down back to Highbust level, aimed through Scapula.",
                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustB_M, m, g, d, "H29");
    //=================================================================================================================
    m = translate("waist_to_highhip_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Front to Highhip Front", "Full measurement name.");
    d = translate("From Waist Front to Highhip Front.", "Full measurement description.");
    InitMeasurement(waistToHighhipF_M, m, g, d, "H30");
    //=================================================================================================================
    m = translate("waist_to_hip_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Front to Hip Front", "Full measurement name.");
    d = translate("From Waist Front to Hip Front.", "Full measurement description.");
    InitMeasurement(waistToHipF_M, m, g, d, "H31");
    //=================================================================================================================
    m = translate("waist_to_highhip_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Side to Highhip Side", "Full measurement name.");
    d = translate("From Waist Side to Highhip Side.", "Full measurement description.");
    InitMeasurement(waistToHighhipSide_M, m, g, d, "H32");
    //=================================================================================================================
    m = translate("waist_to_highhip_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Back to Highhip Back", "Full measurement name.");
    d = translate("From Waist Back down to Highhip Back.", "Full measurement description.");
    InitMeasurement(waistToHighhipB_M, m, g, d, "H33");
    //=================================================================================================================
    m = translate("waist_to_hip_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Back to Hip Back", "Full measurement name.");
    d = translate("From Waist Back down to Hip Back. Requires tape to cover the gap between buttocks.",
                  "Full measurement description.");
    InitMeasurement(waistToHipB_M, m, g, d, "H34");
    //=================================================================================================================
    m = translate("waist_to_hip_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Side to Hip Side", "Full measurement name.");
    d = translate("From Waist Side to Hip Side.", "Full measurement description.");
    InitMeasurement(waistToHipSide_M, m, g, d, "H35");
    //=================================================================================================================
    m = translate("shoulder_slope_neck_side_angle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Slope Angle from Neck Side", "Full measurement name.");
    d = translate("Angle formed by line from Neck Side to Shoulder Tip and line from Neck Side parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideAngle_M, m, g, d, "H36");
    //=================================================================================================================
    m = translate("shoulder_slope_neck_side_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Slope length from Neck Side", "Full measurement name.");
    d = translate("Vertical distance between Neck Side and Shoulder Tip.", "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideLength_M, m, g, d, "H37");
    //=================================================================================================================
    m = translate("shoulder_slope_neck_back_angle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Slope Angle from Neck Back", "Full measurement name.");
    d = translate("Angle formed by  line from Neck Back to Shoulder Tip and line from Neck Back parallel to floor.",
           "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackAngle_M, m, g, d, "H38");
    //=================================================================================================================
    m = translate("shoulder_slope_neck_back_height", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Slope length from Neck Back", "Full measurement name.");
    d = translate("Vertical distance between Neck Back and Shoulder Tip.", "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackHeight_M, m, g, d, "H39");
    //=================================================================================================================
    m = translate("shoulder_slope_shoulder_tip_angle",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Slope Angle from Shoulder Tip", "Full measurement name.");
    d = translate("Angle formed by line from Neck Side to Shoulder Tip and vertical line at Shoulder Tip.",
                  "Full measurement description.");
    InitMeasurement(shoulderSlopeShoulderTipAngle_M, m, g, d, "H40");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupI()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("shoulder_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder length", "Full measurement name.");
    d = translate("From Neck Side to Shoulder Tip.", "Full measurement description.");
    InitMeasurement(shoulderLength_M, m, g, d, "I01");
    //=================================================================================================================
    m = translate("shoulder_tip_to_shoulder_tip_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Shoulder Tip, front", "Full measurement name.");
    d = translate("From Shoulder Tip to Shoulder Tip, across front.", "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipF_M, m, g, d, "I02");
    //=================================================================================================================
    m = translate("across_chest_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Across Chest", "Full measurement name.");
    d = translate("From Armscye to Armscye at narrowest width across chest.", "Full measurement description.");
    InitMeasurement(acrossChestF_M, m, g, d, "I03");
    //=================================================================================================================
    m = translate("armfold_to_armfold_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armfold to Armfold, front", "Full measurement name.");
    d = translate("From Armfold to Armfold, shortest distance between Armfolds, not parallel to floor.",
                  "Full measurement description.");
    InitMeasurement(armfoldToArmfoldF_M, m, g, d, "I04");
    //=================================================================================================================
    m = translate("shoulder_tip_to_shoulder_tip_half_f",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Shoulder Tip, front, half", "Full measurement name.");
    d = translate("Half of' Shoulder Tip to Shoulder tip, front'. ('Shoulder Tip to Shoulder Tip, front' / 2).",
                  "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfF_M, m, g, d, "I05", "(shoulder_tip_to_shoulder_tip_f/2)");
    //=================================================================================================================
    m = translate("across_chest_half_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Across Chest, half", "Full measurement name.");
    d = translate("Half of 'Across Chest'. ('Across Chest' / 2).", "Full measurement description.");
    InitMeasurement(acrossChestHalfF_M, m, g, d, "I06", "(across_chest_f/2)");
    //=================================================================================================================
    m = translate("shoulder_tip_to_shoulder_tip_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Shoulder Tip, back", "Full measurement name.");
    d = translate("From Shoulder Tip to Shoulder Tip, across the back.", "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipB_M, m, g, d, "I07");
    //=================================================================================================================
    m = translate("across_back_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Across Back", "Full measurement name.");
    d = translate("From Armscye to Armscye at the narrowest width of the back.", "Full measurement description.");
    InitMeasurement(acrossBackB_M, m, g, d, "I08");
    //=================================================================================================================
    m = translate("armfold_to_armfold_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armfold to Armfold, back", "Full measurement name.");
    d = translate("From Armfold to Armfold across the back.", "Full measurement description.");
    InitMeasurement(armfoldToArmfoldB_M, m, g, d, "I09");
    //=================================================================================================================
    m = translate("shoulder_tip_to_shoulder_tip_half_b",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Shoulder Tip, back, half", "Full measurement name.");
    d = translate("Half of 'Shoulder Tip to Shoulder Tip, back'. ('Shoulder Tip to Shoulder Tip,  back' / 2).",
                  "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfB_M, m, g, d, "I10", "(shoulder_tip_to_shoulder_tip_b/2)");
    //=================================================================================================================
    m = translate("across_back_half_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Across Back, half", "Full measurement name.");
    d = translate("Half of 'Across Back'. ('Across Back' / 2).", "Full measurement description.");
    InitMeasurement(acrossBackHalfB_M, m, g, d, "I11", "(across_back_b/2)");
    //=================================================================================================================
    m = translate("neck_front_to_shoulder_tip_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Shoulder Tip", "Full measurement name.");
    d = translate("From Neck Front to Shoulder Tip.", "Full measurement description.");
    InitMeasurement(neckFrontToShoulderTipF_M, m, g, d, "I12");
    //=================================================================================================================
    m = translate("neck_back_to_shoulder_tip_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Shoulder Tip", "Full measurement name.");
    d = translate("From Neck Back to Shoulder Tip.", "Full measurement description.");
    InitMeasurement(neckBackToShoulderTipB_M, m, g, d, "I13");
    //=================================================================================================================
    m = translate("neck_width", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Width", "Full measurement name.");
    d = translate("Measure between the 'legs' of an unclosed necklace or chain draped around the neck.",
                  "Full measurement description.");
    InitMeasurement(neckWidth_M, m, g, d, "I14");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupJ()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("bustpoint_to_bustpoint", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Bustpoint", "Full measurement name.");
    d = translate("From Bustpoint to Bustpoint.", "Full measurement description.");
    InitMeasurement(bustpointToBustpoint_M, m, g, d, "J01");
    //=================================================================================================================
    m = translate("bustpoint_to_neck_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Neck Side", "Full measurement name.");
    d = translate("From Neck Side to Bustpoint.", "Full measurement description.");
    InitMeasurement(bustpointToNeckSide_M, m, g, d, "J02");
    //=================================================================================================================
    m = translate("bustpoint_to_lowbust", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Lowbust", "Full measurement name.");
    d = translate("From Bustpoint down to Lowbust level, following curve of bust or chest.",
                  "Full measurement description.");
    InitMeasurement(bustpointToLowbust_M, m, g, d, "J03");
    //=================================================================================================================
    m = translate("bustpoint_to_waist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Waist level", "Full measurement name.");
    d = translate("From Bustpoint to straight down to Waist level, forming a straight line (not curving along the "
                  "body).", "Full measurement description.");
    InitMeasurement(bustpointToWaist_M, m, g, d, "J04");
    //=================================================================================================================
    m = translate("bustpoint_to_bustpoint_half", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Bustpoint, half", "Full measurement name.");
    d = translate("Half of 'Bustpoint to Bustpoint'. ('Bustpoint to Bustpoint' / 2).", "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalf_M, m, g, d, "J05", "(bustpoint_to_bustpoint/2)");
    //=================================================================================================================
    m = translate("bustpoint_neck_side_to_waist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint, Neck Side to Waist level", "Full measurement name.");
    d = translate("From Neck Side to Bustpoint, then straight down to Waist level. ('Neck Side to Bustpoint' + "
                  "'Bustpoint to Waist level').", "Full measurement description.");
    InitMeasurement(bustpointNeckSideToWaist_M, m, g, d, "J06", "(bustpoint_to_neck_side + bustpoint_to_waist)");
    //=================================================================================================================
    m = translate("bustpoint_to_shoulder_tip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Shoulder Tip", "Full measurement name.");
    d = translate("From Bustpoint to Shoulder tip.", "Full measurement description.");
    InitMeasurement(bustpointToShoulderTip_M, m, g, d, "J07");
    //=================================================================================================================
    m = translate("bustpoint_to_waist_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Waist Front", "Full measurement name.");
    d = translate("From Bustpoint to Waist Front, in a straight line, not following the curves of the body.",
                  "Full measurement description.");
    InitMeasurement(bustpointToWaistFront_M, m, g, d, "J08");
    //=================================================================================================================
    m = translate("bustpoint_to_bustpoint_halter", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Bustpoint to Bustpoint Halter", "Full measurement name.");
    d = translate("From Bustpoint around Neck Back down to other Bustpoint.", "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalter_M, m, g, d, "J09");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupK()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("shoulder_tip_to_waist_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Waist Front", "Full measurement name.");
    d = translate("From Shoulder Tip diagonal to Waist Front.", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistFront_M, m, g, d, "K01");
    //=================================================================================================================
    m = translate("neck_front_to_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Front to Waist Side", "Full measurement name.");
    d = translate("From Neck Front diagonal to Waist Side.", "Full measurement description.");
    InitMeasurement(neckFrontToWaistSide_M, m, g, d, "K02");
    //=================================================================================================================
    m = translate("neck_side_to_waist_side_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist Side, front", "Full measurement name.");
    d = translate("From Neck Side diagonal across front to Waist Side.", "Full measurement description.");
    InitMeasurement(neckSideToWaistSideF_M, m, g, d, "K03");
    //=================================================================================================================
    m = translate("shoulder_tip_to_waist_back", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Waist Back", "Full measurement name.");
    d = translate("From Shoulder Tip diagonal to Waist Back.", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistBack_M, m, g, d, "K04");
    //=================================================================================================================
    m = translate("shoulder_tip_to_waist_b_1in_offset",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Shoulder Tip to Waist Back, with 1in (2.54cm) offset", "Full measurement name.");
    d = translate("Mark 1in (2.54cm) outward from Waist Back along Waist level. Measure from Shoulder Tip diagonal to "
                  "mark.", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistB_1inOffset_M, m, g, d, "K05");
    //=================================================================================================================
    m = translate("neck_back_to_waist_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Waist Side", "Full measurement name.");
    d = translate("From Neck Back diagonal across back to Waist Side.", "Full measurement description.");
    InitMeasurement(neckBackToWaistSide_M, m, g, d, "K06");
    //=================================================================================================================
    m = translate("neck_side_to_waist_side_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Waist Side, back", "Full measurement name.");
    d = translate("From Neck Side diagonal across back to Waist Side.", "Full measurement description.");
    InitMeasurement(neckSideToWaistSideB_M, m, g, d, "K07");
    //=================================================================================================================
    m = translate("neck_side_to_armfold_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Armfold Front", "Full measurement name.");
    d = translate("From Neck Side diagonal to Armfold Front.", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldF_M, m, g, d, "K08");
    //=================================================================================================================
    m = translate("neck_side_to_armpit_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Highbust Side, front", "Full measurement name.");
    d = translate("From Neck Side diagonal across front to Highbust Side (Armpit).", "Full measurement description.");
    InitMeasurement(neckSideToArmpitF_M, m, g, d, "K09");
    //=================================================================================================================
    m = translate("neck_side_to_bust_side_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Bust Side, front", "Full measurement name.");
    d = translate("Neck Side diagonal across front to Bust Side.", "Full measurement description.");
    InitMeasurement(neckSideToBustSideF_M, m, g, d, "K10");
    //=================================================================================================================
    m = translate("neck_side_to_armfold_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Armfold Back", "Full measurement name.");
    d = translate("From Neck Side diagonal to Armfold Back.", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldB_M, m, g, d, "K11");
    //=================================================================================================================
    m = translate("neck_side_to_armpit_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Highbust Side, back", "Full measurement name.");
    d = translate("From Neck Side diagonal across back to Highbust Side (Armpit).", "Full measurement description.");
    InitMeasurement(neckSideToArmpitB_M, m, g, d, "K12");
    //=================================================================================================================
    m = translate("neck_side_to_bust_side_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Side to Bust Side, back", "Full measurement name.");
    d = translate("Neck Side diagonal across back to Bust Side.", "Full measurement description.");
    InitMeasurement(neckSideToBustSideB_M, m, g, d, "K13");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupL()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("arm_shoulder_tip_to_wrist_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Shoulder Tip to Wrist, bent", "Full measurement name.");
    d = translate("Bend Arm, measure from Shoulder Tip around Elbow to radial Wrist bone.",
                  "Full measurement description.");
    InitMeasurement(armShoulderTipToWristBent_M, m, g, d, "L01");
    //=================================================================================================================
    m = translate("arm_shoulder_tip_to_elbow_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Shoulder Tip to Elbow, bent", "Full measurement name.");
    d = translate("Bend Arm, measure from Shoulder Tip to Elbow Tip.", "Full measurement description.");
    InitMeasurement(armShoulderTipToElbowBent_M, m, g, d, "L02");
    //=================================================================================================================
    m = translate("arm_elbow_to_wrist_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Elbow to Wrist, bent", "Full measurement name.");
    d = translate("Elbow tip to wrist. ('Arm: Shoulder Tip to Wrist, bent' - 'Arm: Shoulder Tip to Elbow, bent').",
                  "Full measurement description.");
    InitMeasurement(armElbowToWristBent_M, m, g, d, "L03",
                    "(arm_shoulder_tip_to_wrist_bent - arm_shoulder_tip_to_elbow_bent)");
    //=================================================================================================================
    m = translate("arm_elbow_circ_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Elbow circumference, bent", "Full measurement name.");
    d = translate("Elbow circumference, arm is bent.", "Full measurement description.");
    InitMeasurement(armElbowCircBent_M, m, g, d, "L04");
    //=================================================================================================================
    m = translate("arm_shoulder_tip_to_wrist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Shoulder Tip to Wrist", "Full measurement name.");
    d = translate("From Shoulder Tip to Wrist bone, arm straight.", "Full measurement description.");
    InitMeasurement(armShoulderTipToWrist_M, m, g, d, "L05");
    //=================================================================================================================
    m = translate("arm_shoulder_tip_to_elbow", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Shoulder Tip to Elbow", "Full measurement name.");
    d = translate("From Shoulder tip to Elbow Tip, arm straight.", "Full measurement description.");
    InitMeasurement(armShoulderTipToElbow_M, m, g, d, "L06");
    //=================================================================================================================
    m = translate("arm_elbow_to_wrist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Elbow to Wrist", "Full measurement name.");
    d = translate("From Elbow to Wrist, arm straight. ('Arm: Shoulder Tip to Wrist' - 'Arm: Shoulder Tip to Elbow').",
                  "Full measurement description.");
    InitMeasurement(armElbowToWrist_M, m, g, d, "L07", "(arm_shoulder_tip_to_wrist - arm_shoulder_tip_to_elbow)");
    //=================================================================================================================
    m = translate("arm_armpit_to_wrist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Armpit to Wrist, inside", "Full measurement name.");
    d = translate("From Armpit to ulna Wrist bone, arm straight.", "Full measurement description.");
    InitMeasurement(armArmpitToWrist_M, m, g, d, "L08");
    //=================================================================================================================
    m = translate("arm_armpit_to_elbow", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Armpit to Elbow, inside", "Full measurement name.");
    d = translate("From Armpit to inner Elbow, arm straight.", "Full measurement description.");
    InitMeasurement(armArmpitToElbow_M, m, g, d, "L09");
    //=================================================================================================================
    m = translate("arm_elbow_to_wrist_inside", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Elbow to Wrist, inside", "Full measurement name.");
    d = translate("From inside Elbow to Wrist. ('Arm: Armpit to Wrist, inside' - 'Arm: Armpit to Elbow, inside').",
                  "Full measurement description.");
    InitMeasurement(armElbowToWristInside_M, m, g, d, "L10", "(arm_armpit_to_wrist - arm_armpit_to_elbow)");
    //=================================================================================================================
    m = translate("arm_upper_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Upper Arm circumference", "Full measurement name.");
    d = translate("Arm circumference at Armpit level.", "Full measurement description.");
    InitMeasurement(armUpperCirc_M, m, g, d, "L11");
    //=================================================================================================================
    m = translate("arm_above_elbow_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Above Elbow circumference", "Full measurement name.");
    d = translate("Arm circumference at Bicep level.", "Full measurement description.");
    InitMeasurement(armAboveElbowCirc_M, m, g, d, "L12");
    //=================================================================================================================
    m = translate("arm_elbow_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Elbow circumference", "Full measurement name.");
    d = translate("Elbow circumference, arm straight.", "Full measurement description.");
    InitMeasurement(armElbowCirc_M, m, g, d, "L13");
    //=================================================================================================================
    m = translate("arm_lower_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Lower Arm circumference", "Full measurement name.");
    d = translate("Arm circumference where lower arm is widest.", "Full measurement description.");
    InitMeasurement(armLowerCirc_M, m, g, d, "L14");
    //=================================================================================================================
    m = translate("arm_wrist_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Wrist circumference", "Full measurement name.");
    d = translate("Wrist circumference.", "Full measurement description.");
    InitMeasurement(armWristCirc_M, m, g, d, "L15");
    //=================================================================================================================
    m = translate("arm_shoulder_tip_to_armfold_line",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Shoulder Tip to Armfold line", "Full measurement name.");
    d = translate("From Shoulder Tip down to Armpit level.", "Full measurement description.");
    InitMeasurement(armShoulderTipToArmfoldLine_M, m, g, d, "L16");
    //=================================================================================================================
    m = translate("arm_neck_side_to_wrist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Side to Wrist", "Full measurement name.");
    d = translate("From Neck Side to Wrist. ('Shoulder Length' + 'Arm: Shoulder Tip to Wrist').",
                  "Full measurement description.");
    InitMeasurement(armNeckSideToWrist_M, m, g, d, "L17", "(shoulder_length + arm_shoulder_tip_to_wrist)");
    //=================================================================================================================
    m = translate("arm_neck_side_to_finger_tip", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Side to Finger Tip", "Full measurement name.");
    d = translate("From Neck Side down arm to tip of middle finger. ('Shoulder Length' + 'Arm: Shoulder Tip to "
                  "Wrist' + 'Hand: Length').", "Full measurement description.");
    InitMeasurement(armNeckSideToFingerTip_M, m, g, d, "L18",
                    "(shoulder_length + arm_shoulder_tip_to_wrist + hand_length)");
    //=================================================================================================================
    m = translate("armscye_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armscye: Circumference", "Full measurement name.");
    d = translate("Let arm hang at side. Measure Armscye circumference through Shoulder Tip and Armpit.",
                  "Full measurement description.");
    InitMeasurement(armscyeCirc_M, m, g, d, "L19");
    //=================================================================================================================
    m = translate("armscye_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armscye: Length", "Full measurement name.");
    d = translate("Vertical distance from Shoulder Tip to Armpit.", "Full measurement description.");
    InitMeasurement(armscyeLength_M, m, g, d, "L20");
    //=================================================================================================================
    m = translate("armscye_width", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armscye: Width", "Full measurement name.");
    d = translate("Horizontal distance between Armscye Front and Armscye Back.", "Full measurement description.");
    InitMeasurement(armscyeWidth_M, m, g, d, "L21");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupM()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("leg_crotch_to_floor", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Crotch to floor", "Full measurement name.");
    d = translate("Stand feet close together. Measure from crotch level (touching body, no extra space) down to floor.",
                  "Full measurement description.");
    InitMeasurement(legCrotchToFloor_M, m, g, d, "M01");
    //=================================================================================================================
    m = translate("leg_waist_side_to_floor", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Waist Side to floor", "Full measurement name.");
    d = translate("From Waist Side along curve to Hip level then straight down to floor.",
                  "Full measurement description.");
    InitMeasurement(legWaistSideToFloor_M, m, g, d, "M02");
    //=================================================================================================================
    m = translate("leg_thigh_upper_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Thigh Upper circumference", "Full measurement name.");
    d = translate("Thigh circumference at the fullest part of the upper Thigh near the Crotch.",
                  "Full measurement description.");
    InitMeasurement(legThighUpperCirc_M, m, g, d, "M03");
    //=================================================================================================================
    m = translate("leg_thigh_mid_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Thigh Middle circumference", "Full measurement name.");
    d = translate("Thigh circumference about halfway between Crotch and Knee.", "Full measurement description.");
    InitMeasurement(legThighMidCirc_M, m, g, d, "M04");
    //=================================================================================================================
    m = translate("leg_knee_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Knee circumference", "Full measurement name.");
    d = translate("Knee circumference with straight leg.", "Full measurement description.");
    InitMeasurement(legKneeCirc_M, m, g, d, "M05");
    //=================================================================================================================
    m = translate("leg_knee_small_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Knee Small circumference", "Full measurement name.");
    d = translate("Leg circumference just below the knee.", "Full measurement description.");
    InitMeasurement(legKneeSmallCirc_M, m, g, d, "M06");
    //=================================================================================================================
    m = translate("leg_calf_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Calf circumference", "Full measurement name.");
    d = translate("Calf circumference at the largest part of lower leg.", "Full measurement description.");
    InitMeasurement(legCalfCirc_M, m, g, d, "M07");
    //=================================================================================================================
    m = translate("leg_ankle_high_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Ankle High circumference", "Full measurement name.");
    d = translate("Ankle circumference where the indentation at the back of the ankle is the deepest.",
                  "Full measurement description.");
    InitMeasurement(legAnkleHighCirc_M, m, g, d, "M08");
    //=================================================================================================================
    m = translate("leg_ankle_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Ankle circumference", "Full measurement name.");
    d = translate("Ankle circumference where front of leg meets the top of the foot.", "Full measurement description.");
    InitMeasurement(legAnkleCirc_M, m, g, d, "M09");
    //=================================================================================================================
    m = translate("leg_knee_circ_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Knee circumference, bent", "Full measurement name.");
    d = translate("Knee circumference with leg bent.", "Full measurement description.");
    InitMeasurement(legKneeCircBent_M, m, g, d, "M10");
    //=================================================================================================================
    m = translate("leg_ankle_diag_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Ankle diagonal circumference", "Full measurement name.");
    d = translate("Ankle circumference diagonal from top of foot to bottom of heel.", "Full measurement description.");
    InitMeasurement(legAnkleDiagCirc_M, m, g, d, "M11");
    //=================================================================================================================
    m = translate("leg_crotch_to_ankle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Crotch to Ankle", "Full measurement name.");
    d = translate("From Crotch to Ankle. ('Leg: Crotch to Floor' - 'Height: Ankle').", "Full measurement description.");
    InitMeasurement(legCrotchToAnkle_M, m, g, d, "M12", "(leg_crotch_to_floor - height_ankle)");
    //=================================================================================================================
    m = translate("leg_waist_side_to_ankle", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Waist Side to Ankle", "Full measurement name.");
    d = translate("From Waist Side to Ankle. ('Leg: Waist Side to Floor' - 'Height: Ankle').",
                  "Full measurement description.");
    InitMeasurement(legWaistSideToAnkle_M, m, g, d, "M13", "(leg_waist_side_to_floor - height_ankle)");
    //=================================================================================================================
    m = translate("leg_waist_side_to_knee", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Leg: Waist Side to Knee", "Full measurement name.");
    d = translate("From Waist Side along curve to Hip level then straight down to  Knee level. ('Leg: Waist Side to "
                  "Floor' - 'Height Knee').", "Full measurement description.");
    InitMeasurement(legWaistSideToKnee_M, m, g, d, "M14", "(leg_waist_side_to_floor - height_knee)");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupN()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("crotch_length", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Crotch length", "Full measurement name.");
    d = translate("Put tape across gap between buttocks at Hip level. Measure from Waist Front down betwen legs and "
                  "up to Waist Back.", "Full measurement description.");
    InitMeasurement(crotchLength_M, m, g, d, "N01");
    //=================================================================================================================
    m = translate("crotch_length_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Crotch length, back", "Full measurement name.");
    d = translate("Put tape across gap between buttocks at Hip level. Measure from Waist Back to mid-Crotch, either "
                  "at the vagina or between testicles and anus).", "Full measurement description.");
    InitMeasurement(crotchLengthB_M, m, g, d, "N02");
    //=================================================================================================================
    m = translate("crotch_length_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Crotch length, front", "Full measurement name.");
    d = translate("From Waist Front to start of vagina or end of testicles. ('Crotch length' - 'Crotch length, back').",
                  "Full measurement description.");
    InitMeasurement(crotchLengthF_M, m, g, d, "N03", "(crotch_length - crotch_length_b)");
    //=================================================================================================================
    m = translate("rise_length_side", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rise length, side", "Full measurement name.");
    d = translate("From Waist Side down to surface, while seated on hard surface.", "Full measurement description.");
    InitMeasurement(riseLengthSide_M, m, g, d, "N04");
    //=================================================================================================================
    m = translate("rise_length_diag", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rise length, diagonal", "Full measurement name.");
    d = translate("Measure from Waist Side diagonally to a string tied at the top of the leg, seated on a hard "
                  "surface.", "Full measurement description.");
    InitMeasurement(riseLengthDiag_M, m, g, d, "N05");
    //=================================================================================================================
    m = translate("rise_length_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rise length, back", "Full measurement name.");
    d = translate("Vertical distance from Waist Back to Crotch level.", "Full measurement description.");
    InitMeasurement(riseLengthB_M, m, g, d, "N06");
    //=================================================================================================================
    m = translate("rise_length_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Rise length, front", "Full measurement name.");
    d = translate("Vertical Distance from Waist Front to Crotch level.", "Full measurement description.");
    InitMeasurement(riseLengthF_M, m, g, d, "N07");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupO()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("neck_back_to_waist_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Waist Front", "Full measurement name.");
    d = translate("From Neck Back around Neck Side down to Waist Front.", "Full measurement description.");
    InitMeasurement(neckBackToWaistFront_M, m, g, d, "O01");
    //=================================================================================================================
    m = translate("waist_to_waist_halter", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist to Waist Halter, around Neck Back", "Full measurement name.");
    d = translate("From Waist level around Neck Back to Waist level.", "Full measurement description.");
    InitMeasurement(waistToWaistHalter_M, m, g, d, "O02");
    //=================================================================================================================
    m = translate("waist_natural_circ", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Natural Waist circumference", "Full measurement name.");
    d = translate("Torso circumference at men's natural side Abdominal Obliques indentation, if Oblique indentation "
                  "isn't found then just below the Navel level.", "Full measurement description.");
    InitMeasurement(waistNaturalCirc_M, m, g, d, "O03");
    //=================================================================================================================
    m = translate("waist_natural_arc_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Natural Waist arc, front", "Full measurement name.");
    d = translate("From Side to Side at the Natural Waist level, across the front.", "Full measurement description.");
    InitMeasurement(waistNaturalArcF_M, m, g, d, "O04");
    //=================================================================================================================
    m = translate("waist_natural_arc_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Natural Waist arc, back", "Full measurement name.");
    d = translate("From Side to Side at Natural Waist level, across the back. Calculate as ( Natural Waist "
                  "circumference  - Natural Waist arc (front) ).", "Full measurement description.");
    InitMeasurement(waistNaturalArcB_M, m, g, d, "O05", "(waist_natural_circ - waist_natural_arc_f)");
    //=================================================================================================================
    m = translate("waist_to_natural_waist_f", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Front to Natural Waist Front", "Full measurement name.");
    d = translate("Length from Waist Front to Natural Waist Front.", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistF_M, m, g, d, "O06");
    //=================================================================================================================
    m = translate("waist_to_natural_waist_b", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Waist Back to Natural Waist Back", "Full measurement name.");
    d = translate("Length from Waist Back to Natural Waist Back.", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistB_M, m, g, d, "O07");
    //=================================================================================================================
    m = translate("arm_neck_back_to_elbow_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Back to Elbow, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Neck Back to Elbow Tip.",
                  "Full measurement description.");
    InitMeasurement(armNeckBackToElbowBent_M, m, g, d, "O08");
    //=================================================================================================================
    m = translate("arm_neck_back_to_wrist_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Back to Wrist, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Neck Back to Elbow Tip to Wrist bone.",
                  "Full measurement description.");
    InitMeasurement(armNeckBackToWristBent_M, m, g, d, "O09");
    //=================================================================================================================
    m = translate("arm_neck_side_to_elbow_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Side to Elbow, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Neck Side to Elbow Tip.",
                  "Full measurement description.");
    InitMeasurement(armNeckSideToElbowBent_M, m, g, d, "O10");
    //=================================================================================================================
    m = translate("arm_neck_side_to_wrist_bent", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Neck Side to Wrist, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Neck Side to Elbow Tip to Wrist bone.",
                  "Full measurement description.");
    InitMeasurement(armNeckSideToWristBent_M, m, g, d, "O11");
    //=================================================================================================================
    m = translate("arm_across_back_center_to_elbow_bent",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Across Back Center to Elbow, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Middle of Back to Elbow Tip.",
                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToElbowBent_M, m, g, d, "O12");
    //=================================================================================================================
    m = translate("arm_across_back_center_to_wrist_bent",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Across Back Center to Wrist, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Middle of Back to Elbow Tip to Wrist bone.",
                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToWristBent_M, m, g, d, "O13");
    //=================================================================================================================
    m = translate("arm_armscye_back_center_to_wrist_bent",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Arm: Armscye Back Center to Wrist, high bend", "Full measurement name.");
    d = translate("Bend Arm with Elbow out, hand in front. Measure from Armscye Back to Elbow Tip.",
                  "Full measurement description.");
    InitMeasurement(armArmscyeBackCenterToWristBent_M, m, g, d, "O14");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupP()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("neck_back_to_bust_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Bust Front", "Full measurement name.");
    d = translate("From Neck Back, over Shoulder, to Bust Front.", "Full measurement description.");
    InitMeasurement(neckBackToBustFront_M, m, g, d, "P01");
    //=================================================================================================================
    m = translate("neck_back_to_armfold_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back to Armfold Front", "Full measurement name.");
    d = translate("From Neck Back over Shoulder to Armfold Front.", "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFront_M, m, g, d, "P02");
    //=================================================================================================================
    m = translate("neck_back_to_armfold_front_to_waist_side",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back, over Shoulder, to Waist Side", "Full measurement name.");
    d = translate("From Neck Back, over Shoulder, down chest to Waist Side.", "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToWaistSide_M, m, g, d, "P03");
    //=================================================================================================================
    m = translate("highbust_back_over_shoulder_to_armfold_front",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust Back, over Shoulder, to Armfold Front", "Full measurement name.");
    d = translate("From Highbust Back over Shoulder to Armfold Front.", "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToArmfoldFront_M, m, g, d, "P04");
    //=================================================================================================================
    m = translate("highbust_back_over_shoulder_to_waist_front",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust Back, over Shoulder, to Waist Front", "Full measurement name.");
    d = translate("From Highbust Back, over Shoulder touching  Neck Side, to Waist Front.",
                  "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToWaistFront_M, m, g, d, "P05");
    //=================================================================================================================
    m = translate("neck_back_to_armfold_front_to_neck_back",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back, to Armfold Front, to Neck Back", "Full measurement name.");
    d = translate("From Neck Back, over Shoulder to Armfold Front, under arm and return to start.",
                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToNeckBack_M, m, g, d, "P06");
    //=================================================================================================================
    m = translate("across_back_center_to_armfold_front_to_across_back_center",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Across Back Center, circled around Shoulder", "Full measurement name.");
    d = translate("From center of Across Back, over Shoulder, under Arm, and return to start.",
                  "Full measurement description.");
    InitMeasurement(acrossBackCenterToArmfoldFrontToAcrossBackCenter_M, m, g, d, "P07");
    //=================================================================================================================
    m = translate("neck_back_to_armfold_front_to_highbust_back",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Neck Back, to Armfold Front, to Highbust Back", "Full measurement name.");
    d = translate("From Neck Back over Shoulder to Armfold Front, under arm to Highbust Back.",
                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToHighbustBack_M, m, g, d, "P08");
    //=================================================================================================================
    m = translate("armfold_to_armfold_bust", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armfold to Armfold, front, curved through Bust Front", "Full measurement name.");
    d = translate("Measure in a curve from Armfold Left Front through Bust Front curved back up to Armfold Right "
                  "Front.", "Full measurement description.");
    InitMeasurement(armfoldToArmfoldBust_M, m, g, d, "P09");
    //=================================================================================================================
    m = translate("armfold_to_bust_front", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armfold to Bust Front", "Full measurement name.");
    d = translate("Measure from Armfold Front to Bust Front, shortest distance between the two, as straight as "
                  "possible.", "Full measurement description.");
    InitMeasurement(armfoldToBustFront_M, m, g, d, "P10");
    //=================================================================================================================
    m = translate("highbust_b_over_shoulder_to_highbust_f",
                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Highbust Back, over Shoulder, to Highbust level", "Full measurement name.");
    d = translate("From Highbust Back, over Shoulder, then aim at Bustpoint, stopping measurement at Highbust level.",
                  "Full measurement description.");
    InitMeasurement(highbustBOverShoulderToHighbustF_M, m, g, d, "P11");
    //=================================================================================================================
    m = translate("armscye_arc", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Armscye: Arc", "Full measurement name.");
    d = translate("From Armscye at Across Chest over ShoulderTip  to Armscye at Across Back.",
                  "Full measurement description.");
    InitMeasurement(armscyeArc_M, m, g, d, "P12");
    //=================================================================================================================
}

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupQ()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = translate("dart_width_shoulder", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Dart Width: Shoulder", "Full measurement name.");
    d = translate("This information is pulled from pattern charts in some patternmaking systems, e.g. Winifred P. "
                  "Aldrich's \"Metric Pattern Cutting\".", "Full measurement description.");
    InitMeasurement(dartWidthShoulder_M, m, g, d, "Q01");
    //=================================================================================================================
    m = translate("dart_width_bust", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Dart Width: Bust", "Full measurement name.");
    d = translate("This information is pulled from pattern charts in some patternmaking systems, e.g. Winifred P. "
                  "Aldrich's \"Metric Pattern Cutting\".", "Full measurement description.");
    InitMeasurement(dartWidthBust_M, m, g, d, "Q02");
    //=================================================================================================================
    m = translate("dart_width_waist", "Name in a formula. Don't use math symbols and space in name!!!!");
    g = translate("Dart Width: Waist", "Full measurement name.");
    d = translate("This information is pulled from pattern charts in some  patternmaking systems, e.g. Winifred P. "
                  "Aldrich's \"Metric Pattern Cutting\".", "Full measurement description.");
    InitMeasurement(dartWidthWaist_M, m, g, d, "Q03");
    //=================================================================================================================
}

#undef translate

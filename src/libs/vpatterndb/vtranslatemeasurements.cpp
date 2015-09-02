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
    return measurements.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::MFromUser(const QString &measurement) const
{
    QMap<QString, QmuTranslation>::const_iterator i = measurements.constBegin();
    while (i != measurements.constEnd())
    {
        if (measurement == i.value().translate())
        {
            return i.key();
        }
        ++i;
    }
    return measurement;
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
    return guiTexts.value(measurement).translate();
}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslateMeasurements::Description(const QString &measurement) const
{
    return descriptions.value(measurement).translate();
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

//---------------------------------------------------------------------------------------------------------------------
void VTranslateMeasurements::InitGroupA()
{
    //Note. We can't use here function and variables because lupdate tool doesn't see string in variables and doesn't
    //mark such string to translation.
    QmuTranslation m;
    QmuTranslation g;
    QmuTranslation d;

    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Total", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from crown of head to floor",
                                  "Full measurement description.");
    InitMeasurement(height_M, m, g, d, "A01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements",
                                  "Vertical distance from the Neck Back (cervicale vertebra) to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightNeckBack_M, m, g, d, "A02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_scapula",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Scapula", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Scapula (Blade point) to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightScapula_M, m, g, d, "A03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_armpit",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Armpit", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Armpit to the floor.",
                                  "Full measurement description.");
    InitMeasurement(heightArmpit_M, m, g, d, "A04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Waist Side to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightWaistSide_M, m, g, d, "A05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Hip level to the floor.",
                                  "Full measurement description.");
    InitMeasurement(heightHip_M, m, g, d, "A06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_gluteal_fold",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Gluteal Fold", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Gluteal fold, where the Gluteal muscle "
                                  "meets the top of the back thigh, to the floor", "Full measurement description.");
    InitMeasurement(heightGlutealFold_M, m, g, d, "A07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the fold at the back of the Knee to "
                                  "the floor", "Full measurement description.");
    InitMeasurement(heightKnee_M, m, g, d, "A08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_calf",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Calf", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the widest point of the calf to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightCalf_M, m, g, d, "A09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_ankle_high",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Ankle High", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the deepest indentation of the back of the "
                                  "ankle to the floor", "Full measurement description.");
    InitMeasurement(heightAnkleHigh_M, m, g, d, "A10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from point where the front leg meets the foot "
                                  "to the floor", "Full measurement description.");
    InitMeasurement(heightAnkle_M, m, g, d, "A11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_highhip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Highhip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Highhip level, where front abdomen is "
                                  "most prominent, to the floor", "Full measurement description.");
    InitMeasurement(heightHighhip_M, m, g, d, "A12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Waist Front to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightWaistFront_M, m, g, d, "A13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_bustpoint",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Bustpoint", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from Bustpoint to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightBustpoint_M, m, g, d, "A14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_shoulder_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Shoulder Tip to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightShoulderTip_M, m, g, d, "A15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Neck Front to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightNeckFront_M, m, g, d, "A16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Neck Side to the floor",
                                  "Full measurement description.");
    InitMeasurement(heightNeckSide_M, m, g, d, "A17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_neck_back_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Neck Back to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Neck Back (cervicale vertebra) to the "
                                  "fold at the back of the knee", "Full measurement description.");
    InitMeasurement(heightNeckBackToKnee_M, m, g, d, "A18", "(height_neck_back - height_knee)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Waist Side to the fold at the back of "
                                  "the knee", "Full measurement description.");
    InitMeasurement(heightWaistSideToKnee_M, m, g, d, "A19", "(height_waist_side - height_knee)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_waist_side_to_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height Waist Side to Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the Waist Side to the Hip level",
                                  "Full measurement description.");
    InitMeasurement(heightWaistSideToHip_M, m, g, d, "A20", "(height_waist_side - height_hip)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "height_knee_to_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Height knee to ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from the fold at the back of the knee to the "
                                  "point where the front leg meets the top of the foot",
                                  "Full measurement description.");
    InitMeasurement(heightKneeToAnkle_M, m, g, d, "A21");
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
    m = QmuTranslation::translate("Measurements", "width_shoulder",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Shoulder", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Shoulder Tip to Shoulder Tip",
                                  "Full measurement description.");
    InitMeasurement(widthShoulder_M, m, g, d, "B01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Bust Side to Bust Side",
                                  "Full measurement description.");
    InitMeasurement(widthBust_M, m, g, d, "B02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Waist Side to Waist Side",
                                  "Full measurement description.");
    InitMeasurement(widthWaist_M, m, g, d, "B03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Hip Side to Hip Side",
                                  "Full measurement description.");
    InitMeasurement(widthHip_M, m, g, d, "B04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "width_abdomen_to_hip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Width Abdomen to Hip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from the greatest abdomen prominence to the "
                                  "greatest hip prominence", "Full measurement description.");
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
    m = QmuTranslation::translate("Measurements", "indent_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Scapula (Blade point) to the Neck Back",
                                  "Full measurement description.");
    InitMeasurement(indentNeckBack_M, m, g, d, "C01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "indent_waist_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance between a flat stick, placed to touch Hip "
                                  "and Scapula, and Waist Back", "Full measurement description.");
    InitMeasurement(indentWaistBack_M, m, g, d, "C02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "indent_ankle_high",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Indent Ankle High", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal Distance betwee a  flat stick, placed perpendicular "
                                  "to Heel, and the greatest indentation of Ankle", "Full measurement description.");
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
    m = QmuTranslation::translate("Measurements", "neck_mid_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Middle Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Middle Neck circumference, roughtly halfway between jaw and torso",
                                  "Full measurement description.");
    InitMeasurement(neckMidCirc_M, m, g, d, "D01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck circumference, at base of neck, from Neck Back through Neck "
                                  "Sides and Neck Front", "Full measurement description.");
    InitMeasurement(neckCirc_M, m, g, d, "D02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference at Highbust, following shortest distance between "
                                  "Armfolds across chest, parallel to floor under arms and across back",
                                  "Full measurement description.");
    InitMeasurement(highbustCirc_M, m, g, d, "D03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around Bust, parallel to floor",
                                  "Full measurement description.");
    InitMeasurement(bustCirc_M, m, g, d, "D04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around LowBust, around the ribs just under the "
                                  "breasts, parallel to floor", "Full measurement description.");
    InitMeasurement(lowbustCirc_M, m, g, d, "D05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around Ribs at level of the lowest rib at the "
                                  "side, parallel to floor", "Full measurement description.");
    InitMeasurement(ribCirc_M, m, g, d, "D06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around Waist, following natural contours. Waists "
                                  "are typically higher in back than in front, not parallel to floor.",
                                  "Full measurement description.");
    InitMeasurement(waistCirc_M, m, g, d, "D07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around Highhip, where Abdomen protrusion is "
                                  "greatest, parallel to floor", "Full measurement description.");
    InitMeasurement(highhipCirc_M, m, g, d, "D08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Circumference around Hip, where Hip protrusion is greatest, "
                                  "parallel to floor", "Full measurement description.");
    InitMeasurement(hipCirc_M, m, g, d, "D09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Neck Side through Neck Front. Calculate as "
                                  "(Neck circumference - Neck arc (back)", "Full measurement description.");
    InitMeasurement(neckArcF_M, m, g, d, "D10", "(neck_circ - neck_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highbust Side (Armpit) to HIghbust Side (Armpit) across "
                                  "chest. Calculate as (Highbust circumference - Highbust arc (back))",
                                  "Full measurement description.");
    InitMeasurement(highbustArcF_M, m, g, d, "D11", "(highbust_circ - highbust_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bust Side to Bust Side across chest. Calculate as "
                                  "(Bust circumference - Bust arc (back))", "Full measurement description.");
    InitMeasurement(bustArcF_M, m, g, d, "D12", "(bust_circ – bust_arc_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Lowbust Side to Lowbust Side across Front",
                                  "Full measurement description.");
    InitMeasurement(lowbustArcF_M, m, g, d, "D13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Rib Side to Rib Side across front",
                                  "Full measurement description.");
    InitMeasurement(ribArcF_M, m, g, d, "D14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist Side to Waist Side across front",
                                  "Full measurement description.");
    InitMeasurement(waistArcF_M, m, g, d, "D15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highhip Side to Highhip Side across front",
                                  "Full measurement description.");
    InitMeasurement(highhipArcF_M, m, g, d, "D16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Hip Side to Hip Side across Front",
                                  "Full measurement description.");
    InitMeasurement(hipArcF_M, m, g, d, "D17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Neck Side across back",
                                  "Full measurement description.");
    InitMeasurement(neckArcB_M, m, g, d, "D18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highbust Side (Armpit) to Highbust Side (Armpit) across back",
                                  "Full measurement description.");
    InitMeasurement(highbustArcB_M, m, g, d, "D19");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bust Side to Bust Side across back",
                                  "Full measurement description.");
    InitMeasurement(bustArcB_M, m, g, d, "D20");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Lowbust Side to Lowbust Side across back. Calculate as "
                                  "(Lowbust circumference - Lowbust arc (front))", "Full measurement description.");
    InitMeasurement(lowbustArcB_M, m, g, d, "D21", "(lowbust_circ - lowbust_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Rib Side to Rib side across back. Calculate as "
                                  "(Rib circumference - Rib arc (front))", "Full measurement description.");
    InitMeasurement(ribArcB_M, m, g, d, "D22", "(rib_circ - rib_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist Side to Waist Side across back. Calculate as "
                                  "(Waist circumference - Waist arc (front))", "Full measurement description.");
    InitMeasurement(waistArcB_M, m, g, d, "D23", "(waist_circ - waist_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highhip Side to Highhip Side across back.  Calculate as "
                                  "(Highhip circumference - Highhip Arc (front))", "Full measurement description.");
    InitMeasurement(highhipArcB_M, m, g, d, "D24", "(highhip_circ - highhip_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Hip Side to Hip Side across back.  Calculate as "
                                  "(Hip circumference - Hip arc (front))", "Full measurement description.");
    InitMeasurement(hipArcB_M, m, g, d, "D25", "(hip_circ - hip_arc_f)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Front to Neck Side. Calculate as (Neck Arc (front)/2)",
                                  "Full measurement description.");
    InitMeasurement(neckArcHalfF_M, m, g, d, "D26", "(neck_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highbust Front to Highbust Side. Calculate as "
                                  "(Highbust Arc (front)/2)", "Full measurement description.");
    InitMeasurement(highbustArcHalfF_M, m, g, d, "D27", "(highbust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bust Front to Bust Side. Calculate as (Bust Arc (front)/2)",
                                  "Full measurement description.");
    InitMeasurement(bustArcHalfF_M, m, g, d, "D28", "(bust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Lowbust Front to Lowbust Side. Calculate as "
                                  "(Lowbust Arc (front)/2)", "Full measurement description.");
    InitMeasurement(lowbustArcHalfF_M, m, g, d, "D29", "(lowbust_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Rib Front to Rib Side. Calculate as (Rib Arc (front) / 2)",
                                  "Full measurement description.");
    InitMeasurement(ribArcHalfF_M, m, g, d, "D30", "(rib_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist Front to Waist Side. Calculate as "
                                  "(Waist Arc (front) / 2)", "Full measurement description.");
    InitMeasurement(waistArcHalfF_M, m, g, d, "D31", "(waist_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highhip Front to Highhip Side. Calculate as "
                                  "(Highhip Arc (front ) / 2)", "Full measurement description.");
    InitMeasurement(highhipArcHalfF_M, m, g, d, "D32", "(highhip_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc, Half (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Hip Front to Hip Side.  Calculate as (Hip Arc (front) / 2)",
                                  "Full measurement description.");
    InitMeasurement(hipArcHalfF_M, m, g, d, "D33", "(hip_arc_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Back to Neck Side. Calculate as (Neck Arc (back) / 2)",
                                  "Full measurement description.");
    InitMeasurement(neckArcHalfB_M, m, g, d, "D34", "(neck_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highbust Back to Highbust Side. Calculate as "
                                  "(Highbust Arc (back) / 2)", "Full measurement description.");
    InitMeasurement(highbustArcHalfB_M, m, g, d, "D35", "(highbust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bust Back to Bust Side. Calculate as (Bust Arc (back) / 2)",
                                  "Full measurement description.");
    InitMeasurement(bustArcHalfB_M, m, g, d, "D36", "(bust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Lowbust Back to Lowbust Side. Calculate as "
                                  "(Lowbust Arc (back) / 2)", "Full measurement description.");
    InitMeasurement(lowbustArcHalfB_M, m, g, d, "D37", "(lowbust_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Rib Back to Highbust Side. Calculate as (Rib Arc (back) / 2)",
                                  "Full measurement description.");
    InitMeasurement(ribArcHalfB_M, m, g, d, "D38", "(rib_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist  Back to Waist  Side. Calculate as "
                                  "(Waist  Arc (back) / 2)", "Full measurement description.");
    InitMeasurement(waistArcHalfB_M, m, g, d, "D39", "(waist_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highhip_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highhip Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Highhip Back to Highbust Side. Calculate as "
                                  "(Highhip Arc (back) / 2)", "Full measurement description.");
    InitMeasurement(highhipArcHalfB_M, m, g, d, "D40", "(highhip_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_arc_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip Arc, Half (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Hip Back to Highbust Side. Calculate as (Hip Arc (back) / 2)",
                                  "Full measurement description.");
    InitMeasurement(hipArcHalfB_M, m, g, d, "D41", "(hip_arc_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hip_with_abdomen_arc",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hip with Abdomen Arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Curve stiff paper around front of abdomen, tape at sides.  "
                                  "Measure from Hip Side to Hip Side over paper across front.",
                                  "Full measurement description.");
    InitMeasurement(hipWithAbdomenArcF_M, m, g, d, "D42");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_armfold_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference at Armfold", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure around arms and torso at Armfold level",
                                  "Full measurement description.");
    InitMeasurement(bodyArmfoldCirc_M, m, g, d, "D43");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_bust_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference at Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure around arms and torso at Bust level",
                                  "Full measurement description.");
    InitMeasurement(bodyBustCirc_M, m, g, d, "D44");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "body_torso_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Body Circumference around Torso", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure from mid-shoulder around  crotch",
                                  "Full measurement description.");
    InitMeasurement(bodyTorsoCirc_M, m, g, d, "D45");
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
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Waist (front)", "Full measurement description.");
    InitMeasurement(neckFrontToWaistF_M, m, g, d, "E01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_flat_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist, Flat (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Waist, Flat (front)", "Full measurement description.");
    InitMeasurement(neckFrontToWaistFlatF_M, m, g, d, "E02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armpit_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armpit to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armpit to Waist Side", "Full measurement description.");
    InitMeasurement(armpitToWaistSide_M, m, g, d, "E03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (front)",
                                  "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideF_M, m, g, d, "E04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist (front)", "Full measurement description.");
    InitMeasurement(neckSideToWaistF_M, m, g, d, "E05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_bustpoint_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bustpoint (front)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bustpoint (front)",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistBustpointF_M, m, g, d, "E06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Side (back)", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistSideB_M, m, g, d, "E07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist (back)", "Full measurement description.");
    InitMeasurement(neckSideToWaistB_M, m, g, d, "E08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Waist Back", "Full measurement description.");
    InitMeasurement(neckBackToWaistB_M, m, g, d, "E09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_bladepoint_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bladepoint (Back)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Waist through Bladepoint (Back)",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistBladepointB_M, m, g, d, "E10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Highbust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Highbust (front)", "Full measurement description.");
    InitMeasurement(neckFrontToHighbustF_M, m, g, d, "E11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_bust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Bust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Front to Bust (front)", "Full measurement description.");
    InitMeasurement(neckFrontToBustF_M, m, g, d, "E12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement description.");
    InitMeasurement(highbustToWaistF_M, m, g, d, "E13", "(neck_front_to_waist - neck_front_to_highbust)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust to Waist (front)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust Center down to Waist line", "Full measurement description.");
    InitMeasurement(bustToWaistF_M, m, g, d, "E14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust to Waist (front)", "Full measurement description.");
    InitMeasurement(lowbustToWaistF_M, m, g, d, "E15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rib_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rib to Waist (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From lowest rib at side down to waist",
                                  "Full measurement description.");
    InitMeasurement(ribToWaistSide_M, m, g, d, "E16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Highbust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Highbust (back)", "Full measurement description.");
    InitMeasurement(neckBackToHighbustB_M, m, g, d, "E17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_bust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Bust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Bust (back)", "Full measurement description.");
    InitMeasurement(neckBackToBustB_M, m, g, d, "E18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust to Waist (back)", "Full measurement description.");
    InitMeasurement(highbustToWaistB_M, m, g, d, "E19", "(neck_back_to_waist - neck_back_to_highbust)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bust_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bust back center to waist line (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Bust back center to waist line (back)",
                                  "Full measurement description.");
    InitMeasurement(bustToWaistB_M, m, g, d, "E20");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "lowbust_to_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Lowbust to Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Lowbust to Waist (back)", "Full measurement description.");
    InitMeasurement(lowbustToWaistB_M, m, g, d, "E21");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (front)", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldF_M, m, g, d, "E22");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust (front)", "Full measurement description.");
    InitMeasurement(neckSideToBustF_M, m, g, d, "E23");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Highbust (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Highbust (front)", "Full measurement description.");
    InitMeasurement(neckSideToHighbustF_M, m, g, d, "E24");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_center_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (front), aimed at Bustpoint",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (front), aimed at Bustpoint",
                                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustF_M, m, g, d, "E25");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Tip to Armfold (back)", "Full measurement description.");
    InitMeasurement(shoulderTipToArmfoldB_M, m, g, d, "E26");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust (back)", "Full measurement description.");
    InitMeasurement(neckSideToBustB_M, m, g, d, "E27");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Highbust (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Highbust (back)", "Full measurement description.");
    InitMeasurement(neckSideToHighbustB_M, m, g, d, "E28");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_center_to_highbust_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (back), aimed through Bladepoint",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Shoulder Center to Highbust (back), aimed through Bladepoint",
                                  "Full measurement description.");
    InitMeasurement(shoulderCenterToHighbustB_M, m, g, d, "E29");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (front)", "Full measurement description.");
    InitMeasurement(waistToHighhipF_M, m, g, d, "E30");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (front)", "Full measurement description.");
    InitMeasurement(waistToHipF_M, m, g, d, "E31");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (side)", "Full measurement description.");
    InitMeasurement(waistToHighhipSide_M, m, g, d, "E32");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_highhip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Highhip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Highhip (back)", "Full measurement description.");
    InitMeasurement(waistToHighhipB_M, m, g, d, "E33");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (back)", "Full measurement description.");
    InitMeasurement(waistToHipB_M, m, g, d, "E34");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_hip_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Hip (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Hip (side)", "Full measurement description.");
    InitMeasurement(waistToHipSide_M, m, g, d, "E35");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_side_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope angle from Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Angle measuredvetween line parallel to floor at Neck Side, and "
                                  "line from Neck Side to Shoulder Tip. Usually measured with special tools.",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideAngle_M, m, g, d, "E36");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_side_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope length from Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance between Neck Side and Shoulder Tip",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckSideLength_M, m, g, d, "E37");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_back_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Angle between line parallel to floor at Neck Back level and line "
                                  "from Neck Back to Shoulder Tip. Usually measured with special tool.",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackAngle_M, m, g, d, "E38");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_neck_back_height",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Height from Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance between Neck Back and Shoulder Tip",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeNeckBackHeight_M, m, g, d, "E39");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_slope_shoulder_tip_angle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Slope Angle from Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Angle between vertical line at Shoulder Tip, and line from Neck "
                                  "Side to Shoulder Tip. Usually measured with special tools.",
                                  "Full measurement description.");
    InitMeasurement(shoulderSlopeShoulderTipAngle_M, m, g, d, "E40");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_to_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust to Waist (front)", "Full measurement description.");
    InitMeasurement(highbustToWaistF_M, m, g, d, "E36", "(neck_front_to_waist - neck_front_to_highbust)");
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
    m = QmuTranslation::translate("Measurements", "shoulder_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Length from Neck Side to Shoulder Tip",
                                  "Full measurement description.");
    InitMeasurement(shoulderLength_M, m, g, d, "F01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_width_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Width (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Shoulder tip, curved around front",
                                  "Full measurement description.");
    InitMeasurement(shoulderWidthF_M, m, g, d, "F02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_chest_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Chest (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armscye to Armscye, at narrowest width across chest",
                                  "Full measurement description.");
    InitMeasurement(acrossChestF_M, m, g, d, "F03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armfold to Armfold, shortest distance between Armfolds, "
                                  "not parallel to floor", "Full measurement description.");
    InitMeasurement(armfoldToArmfoldF_M, m, g, d, "F04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_width_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Shoulder Tip, curved around the back",
                                  "Full measurement description.");
    InitMeasurement(shoulderWidthB_M, m, g, d, "F05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Width (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armscye to Armscye, at the narrowest width of the back",
                                  "Full measurement description.");
    InitMeasurement(acrossBackB_M, m, g, d, "F06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Back (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armfold to Armfold, across the back",
                                  "Full measurement description.");
    InitMeasurement(armfoldToArmfoldB_M, m, g, d, "F07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_shoulder_tip_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Half of the Shoulder Width (front) measurement. Calculate as "
                                  "(Shoulder Width (front) / 2)", "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfF_M, m, g, d, "F08", "(shoulder_tip_to_shoulder_tip_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_chest_half_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Half of the Across Chest (front) measurement. Calculate as "
                                  "(Across Chest (front) / 2)", "Full measurement description.");
    InitMeasurement(acrossChestHalfF_M, m, g, d, "F09", "(across_chest_f/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_shoulder_tip_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Half of the Shoulder Width (back) measurement. Calculate as "
                                  "(Shoulder Width (back) / 2)", "Full measurement description.");
    InitMeasurement(shoulderTipToShoulderTipHalfB_M, m, g, d, "F10", "(shoulder_tip_to_shoulder_tip_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_half_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Half of the Across Back measurement. Calculate as (Across Back / 2)",
                                  "Full measurement description.");
    InitMeasurement(acrossBackHalfB_M, m, g, d, "F11", "(across_back_b/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_shoulder_tip_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Front to Shoulder Tip, across the front",
                                  "Full measurement description.");
    InitMeasurement(neckFrontToShoulderTipF_M, m, g, d, "F12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_shoulder_tip_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Shoulder Tip (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Back to Shoulder tip, across the back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToShoulderTipB_M, m, g, d, "F13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Shoulder Tip (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Place a necklace or chain around neck so that it has 'legs' that "
                                  "hang down. Measure the distance between the 'legs'.",
                                  "Full measurement description.");
    InitMeasurement(neckWidth_M, m, g, d, "F14");
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
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint to Bustpoint", "Full measurement description.");
    InitMeasurement(bustpointToBustpoint_M, m, g, d, "G01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_neck_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Neck Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Bustpoint", "Full measurement description.");
    InitMeasurement(bustpointToNeckSide_M, m, g, d, "G02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_lowbust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Lowbust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint straight down to Lowbust level, following curve "
                                  "of bust or chest", "Full measurement description.");
    InitMeasurement(bustpointToLowbust_M, m, g, d, "G03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint to Waist level, forming a straight line "
                                  "(not curving along the body)", "Full measurement description.");
    InitMeasurement(bustpointToWaist_M, m, g, d, "G04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint_half",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Half of Bustpoint to Bustpoint measurement. Calculate as "
                                  "(Bustpoint to Bustpoint / 2)", "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalf_M, m, g, d, "G05", "(bustpoint_to_bustpoint/2)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_bustpoint_halter",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Bustpoint, Half", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint, around Neck Back, down to Bustpoint",
                                  "Full measurement description.");
    InitMeasurement(bustpointToBustpointHalter_M, m, g, d, "G06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_shoulder_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint to Shoulder tip", "Full measurement description.");
    InitMeasurement(bustpointToShoulderTip_M, m, g, d, "G07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "bustpoint_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Bustpoint to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Bustpoint to Waist Front, in a straight line, not following "
                                  "the curves of the body", "Full measurement description.");
    InitMeasurement(bustpointToWaistFront_M, m, g, d, "G08");
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
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Waist Front", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistFront_M, m, g, d, "H01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_front_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Front to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Front to Waist Side, across front",
                                  "Full measurement description.");
    InitMeasurement(neckFrontToWaistSide_M, m, g, d, "H02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Waist Side, across front",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistSideF_M, m, g, d, "H03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armfold_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armfold (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Armfold front", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldF_M, m, g, d, "H04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armpit_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armpit (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Highbust Side (Armpit), across front",
                                  "Full measurement description.");
    InitMeasurement(neckSideToArmpitF_M, m, g, d, "H05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_side_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust Side, across front",
                                  "Full measurement description.");
    InitMeasurement(neckSideToBustSideF_M, m, g, d, "H06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Waist Back, across back",
                                  "Full measurement description.");
    InitMeasurement(shoulderTipToWaistBack_M, m, g, d, "H07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Back to Waist Side, across back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToWaistSide_M, m, g, d, "H08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_waist_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Waist Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Waist Side, across back",
                                  "Full measurement description.");
    InitMeasurement(neckSideToWaistSideB_M, m, g, d, "H09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armfold_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armfold (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Armfold back", "Full measurement description.");
    InitMeasurement(neckSideToArmfoldB_M, m, g, d, "H10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_armpit_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Armpit (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Highbust Side (Armpit), across back",
                                  "Full measurement description.");
    InitMeasurement(neckSideToArmpitB_M, m, g, d, "H11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_side_to_bust_side_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Side to Bust Side (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Side to Bust Side, across back",
                                  "Full measurement description.");
    InitMeasurement(neckSideToBustSideB_M, m, g, d, "H12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "shoulder_tip_to_waist_b_1in_offset",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Shoulder Tip to Waist Back, with 1in (2.54cm) offset",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Mark 1in (2.54cm) from Waist Back along waistline. Measure "
                                  "from Shoulder Tip to mark.", "Full measurement description.");
    InitMeasurement(shoulderTipToWaistB_1inOffset_M, m, g, d, "H13");
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
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Shoulder Tip to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip around Elbow to radial Wrist bone, Arm is bent",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToWristBent_M, m, g, d, "I01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Shoulder Tip to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Elbow Tip, Arm is bent",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToElbowBent_M, m, g, d, "I02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Elbow to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Elbow Tip to Wrist, calculate as ( Arm: Shoulder Tip to "
                                  "Wrist (bent) -  Arm: Shoulder Tip to Elbow (bent) )",
                                  "Full measurement description.");
    InitMeasurement(armElbowToWristBent_M, m, g, d, "I03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_circ_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Elbow circumference (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Elbow circumference, Arm is bent", "Full measurement description.");
    InitMeasurement(armElbowCircBent_M, m, g, d, "I04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Shoulder Tip to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to radial Wrist bone, Arm is straight",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToWrist_M, m, g, d, "I05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_elbow",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Shoulder Tip to Elbow", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder tip to Elbow Tip, Arm is straight",
                                  "Full measurement description.");
    InitMeasurement(armShoulderTipToElbow_M, m, g, d, "I06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Elbow to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Length from Elbow to Wrist, Arm is straight. Measure as "
                                  "( Arm: Shoulder Tip to Wrist - Arm: Shoulder Tip to Elbow )",
                                  "Full measurement description.");
    InitMeasurement(armElbowToWrist_M, m, g, d, "I07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armpit_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Armpit to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armpit to ulna Wrist bone, Arm is straight",
                                  "Full measurement description.");
    InitMeasurement(armArmpitToWrist_M, m, g, d, "I08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armpit_to_elbow",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Armpit to Elbow", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Armpit to Elbow ulna bone, Arm is straight",
                                  "Full measurement description.");
    InitMeasurement(armArmpitToElbow_M, m, g, d, "I09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_to_wrist_inside",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Elbow to Wrist (inside)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Elbow to Wrist ulna bone, calculate as  "
                                  "( Arm: Armpit to Wrist - Arm: Armpit to Elbow )", "Full measurement description.");
    InitMeasurement(armElbowToWristInside_M, m, g, d, "I10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_upper_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Upper Arm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm circumference at Armpit level", "Full measurement description.");
    InitMeasurement(armUpperCirc_M, m, g, d, "I11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_above_elbow_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Above Elbow circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm circumference at Bicep level", "Full measurement description.");
    InitMeasurement(armAboveElbowCirc_M, m, g, d, "I12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_elbow_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Elbow circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Elbow circumference, Arm is straight",
                                  "Full measurement description.");
    InitMeasurement(armElbowCirc_M, m, g, d, "I13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_lower_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Lower Arm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm circumference where lower arm is widest",
                                  "Full measurement description.");
    InitMeasurement(armLowerCirc_M, m, g, d, "I14");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_wrist_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Wrist circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Wrist circumference", "Full measurement description.");
    InitMeasurement(armWristCirc_M, m, g, d, "I15");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_shoulder_tip_to_armfold_line",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Shoulder Tip to Armfold level", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Shoulder Tip to Armpit level", "Full measurement description.");
    InitMeasurement(armShoulderTipToArmfoldLine_M, m, g, d, "I16");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure Armscye circumference through Shoulder Tip and Armpit, "
                                  "Arm is hanging straight at side", "Full measurement description.");
    InitMeasurement(armscyeCirc_M, m, g, d, "I17");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Vertical distance from Shoulder Tip to Armpit",
                                  "Full measurement description.");
    InitMeasurement(armscyeLength_M, m, g, d, "I18");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Horizontal distance from Armscye front to Armscye back",
                                  "Full measurement description.");
    InitMeasurement(armscyeWidth_M, m, g, d, "I19");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_finger_tip",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Neck Side to Middle Finger Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to end of middle finger, calculate as "
                                  "( Shoulder Length + Arm: Shoulder Tip to Wrist + Hand length )",
                                  "Full measurement description.");
    InitMeasurement(armNeckSideToFingerTip_M, m, g, d, "I20",
                    "(shoulder_length + arm_shoulder_tip_to_wrist + hand_length)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_wrist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm: Neck Side to Wrist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Neck Side to Wrist, calculate as ( Shoulder Length + "
                                  "Arm: Shoulder Tip to Wrist )", "Full measurement description.");
    InitMeasurement(armNeckSideToWrist_M, m, g, d, "I21");
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
    m = QmuTranslation::translate("Measurements", "leg_crotch_to_floor",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Crotch to floor (Inseam)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Stand feet close together. Measure from Crotch level (close to "
                                  "body) straight down to floor.", "Full measurement description.");
    InitMeasurement(legCrotchToFloor_M, m, g, d, "J01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_floor",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Waist Side to floor (Outseam)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure from Waist Side, along hip curve to Hip level, then "
                                  "straight down to floor.", "Full measurement description.");
    InitMeasurement(legWaistSideToFloor_M, m, g, d, "J02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_knee",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Waist Side to Knee", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist side to Knee, calculate as (Leg: Waist Side to Floor - "
                                  "Height: Knee)", "Full measurement description.");
    InitMeasurement(legWaistSideToKnee_M, m, g, d, "J03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_thigh_upper_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Thigh Upper circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Measure thigh circumference at the fullest part of the upper "
                                  "thigh, near the crotch", "Full measurement description.");
    InitMeasurement(legThighUpperCirc_M, m, g, d, "J04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_thigh_mid_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Thigh Middle circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Thigh circumference roughly halfway between crotch and knee",
                                  "Full measurement description.");
    InitMeasurement(legThighMidCirc_M, m, g, d, "J05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Knee circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Knee circumference, Leg is straight",
                                  "Full measurement description.");
    InitMeasurement(legKneeCirc_M, m, g, d, "J06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_small_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Knee Small circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg circumference just below the knee",
                                  "Full measurement description.");
    InitMeasurement(legKneeSmallCirc_M, m, g, d, "J07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_calf_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Calf circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Leg circumference at the widest part of Calf",
                                  "Full measurement description.");
    InitMeasurement(legCalfCirc_M, m, g, d, "J08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_high_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Ankle High circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Ankle circumference where the indentation at the back of the "
                                  "ankle is the deepest", "Full measurement description.");
    InitMeasurement(legAnkleHighCirc_M, m, g, d, "J09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Ankle circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Ankle circumference where front of Leg meets the top of the Foot",
                                  "Full measurement description.");
    InitMeasurement(legAnkleCirc_M, m, g, d, "J10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_knee_circ_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Knee circumference (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Knee circumference, Leg is bent", "Full measurement description.");
    InitMeasurement(legKneeCircBent_M, m, g, d, "J11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_ankle_diag_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Ankle diagonal circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Ankle circumference taken diagonally from corner of Heel through "
                                  "point where Leg meets Foot", "Full measurement description.");
    InitMeasurement(legAnkleDiagCirc_M, m, g, d, "J12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_crotch_to_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Crotch to Ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Crotch to Ankle level, calculate as ( Leg: Crotch to "
                                  "Floor - Height: Ankle )", "Full measurement description.");
    InitMeasurement(legCrotchToAnkle_M, m, g, d, "J13", "(leg_crotch_to_floor - height_ankle)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "leg_waist_side_to_ankle",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Leg: Waist Side to Ankle", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "From Waist Side to Ankle level, calculate as (Leg: Waist Side "
                                  "to Floor  -  Height: Ankle)", "Full measurement description.");
    InitMeasurement(legWaistSideToAnkle_M, m, g, d, "J14", "(leg_waist_side_to_floor - height_ankle)");
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
    m = QmuTranslation::translate("Measurements", "crotch_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length", "Full measurement description.");
    InitMeasurement(crotchLength_M, m, g, d, "K01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length (back)", "Full measurement description.");
    InitMeasurement(crotchLengthB_M, m, g, d, "K02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "crotch_length_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Crotch length (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Crotch length (front)", "Full measurement description.");
    InitMeasurement(crotchLengthF_M, m, g, d, "K03", "(crotch_length - crotch_length_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (side)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (side)", "Full measurement description.");
    InitMeasurement(riseLengthSide_M, m, g, d, "K04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_diag",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (diagonal)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (diagonal)", "Full measurement description.");
    InitMeasurement(riseLengthDiag_M, m, g, d, "K05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (back)", "Full measurement description.");
    InitMeasurement(riseLengthB_M, m, g, d, "K06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "rise_length_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Rise length (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Rise length (front)", "Full measurement description.");
    InitMeasurement(riseLengthF_M, m, g, d, "K07");
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
    m = QmuTranslation::translate("Measurements", "hand_palm_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm length", "Full measurement description.");
    InitMeasurement(handPalmLength_M, m, g, d, "L01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand length", "Full measurement description.");
    InitMeasurement(handLength_M, m, g, d, "L02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_palm_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm width", "Full measurement description.");
    InitMeasurement(handPalmWidth_M, m, g, d, "L03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_palm_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand > Palm circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand > Palm circumference", "Full measurement description.");
    InitMeasurement(handPalmCirc_M, m, g, d, "L04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "hand_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Hand circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Hand circumference", "Full measurement description.");
    InitMeasurement(handCirc_M, m, g, d, "L05");
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
    m = QmuTranslation::translate("Measurements", "foot_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot width", "Full measurement description.");
    InitMeasurement(footWidth_M, m, g, d, "M01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot length", "Full measurement description.");
    InitMeasurement(footLength_M, m, g, d, "M02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot circumference", "Full measurement description.");
    InitMeasurement(footCirc_M, m, g, d, "M03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "foot_instep_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Foot Instep circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Foot Instep circumference", "Full measurement description.");
    InitMeasurement(footInstepCirc_M, m, g, d, "M04");
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
    m = QmuTranslation::translate("Measurements", "head_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head circumference", "Full measurement description.");
    InitMeasurement(headCirc_M, m, g, d, "N01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_length",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head length", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head length", "Full measurement description.");
    InitMeasurement(headLength_M, m, g, d, "N02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_depth",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head depth", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head depth", "Full measurement description.");
    InitMeasurement(headDepth_M, m, g, d, "N03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_width",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head width", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head width", "Full measurement description.");
    InitMeasurement(headWidth_M, m, g, d, "N04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_crown_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head Crown to Neck Back, vertical distance",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head Crown to Neck Back, vertical distance",
                                  "Full measurement description.");
    InitMeasurement(headCrownToNeckBack_M, m, g, d, "N05", "(height - height_neck_back)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "head_chin_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Head > Chin to Neck Back, vertical distance",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Head > Chin to Neck Back, vertical distance",
                                  "Full measurement description.");
    InitMeasurement(headChinToNeckBack_M, m, g, d, "N06", "(height - height_neck_back - head_length)");
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
    m = QmuTranslation::translate("Measurements", "neck_back_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Waist Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Waist Front", "Full measurement description.");
    InitMeasurement(neckBackToWaistFront_M, m, g, d, "O01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_waist_halter",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Waist Halter, around Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Waist Halter, around Neck Back",
                                  "Full measurement description.");
    InitMeasurement(waistToWaistHalter_M, m, g, d, "O02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_circ",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist circumference", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist circumference", "Full measurement description.");
    InitMeasurement(waistNaturalCirc_M, m, g, d, "O03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_arc_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist arc (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist arc (front)", "Full measurement description.");
    InitMeasurement(waistNaturalArcF_M, m, g, d, "O04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_natural_arc_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Natural Waist arc (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Natural Waist arc (back)", "Full measurement description.");
    InitMeasurement(waistNaturalArcB_M, m, g, d, "O05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_natural_waist_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Natural Waist (front)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Natural Waist (front)", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistF_M, m, g, d, "O06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "waist_to_natural_waist_b",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Waist to Natural Waist (back)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Waist to Natural Waist (back)", "Full measurement description.");
    InitMeasurement(waistToNaturalWaistB_M, m, g, d, "O07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_back_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Back to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Back to Elbow (bent)", "Full measurement description.");
    InitMeasurement(armNeckBackToElbowBent_M, m, g, d, "O08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_back_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Back to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Back to Wrist (bent)", "Full measurement description.");
    InitMeasurement(armNeckBackToWristBent_M, m, g, d, "O09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Side to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Side to Elbow (bent)", "Full measurement description.");
    InitMeasurement(armNeckSideToElbowBent_M, m, g, d, "O10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_neck_side_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Neck Side to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Neck Side to Wrist (bent)", "Full measurement description.");
    InitMeasurement(armNeckSideToWristBent_M, m, g, d, "O11");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_across_back_center_to_elbow_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Elbow (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Elbow (bent)",
                                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToElbowBent_M, m, g, d, "O12");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_across_back_center_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Wrist (bent)", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Across Back Center to Wrist (bent)",
                                  "Full measurement description.");
    InitMeasurement(armAcrossBackCenterToWristBent_M, m, g, d, "O13");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "arm_armscye_back_center_to_wrist_bent",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Arm > Armscye Back Center to Wrist (bent)",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Arm > Armscye Back Center to Wrist (bent)",
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
    m = QmuTranslation::translate("Measurements", "armfold_to_armfold_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Armfold curved through Bust Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Armfold curved through Bust Front",
                                  "Full measurement description.");
    InitMeasurement(armfoldToArmfoldBust_M, m, g, d, "P01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armfold_to_bust_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armfold to Bust Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armfold to Bust Front", "Full measurement description.");
    InitMeasurement(armfoldToBustFront_M, m, g, d, "P02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_bust_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Bust Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Bust Front", "Full measurement description.");
    InitMeasurement(neckBackToBustFront_M, m, g, d, "P03");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front", "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFront_M, m, g, d, "P04");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_waist_side",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Waist Side", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Waist Side",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToWaistSide_M, m, g, d, "P05");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_back_over_shoulder_to_armfold_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Armfold Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Armfold Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToArmfoldFront_M, m, g, d, "P06");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_back_over_shoulder_to_waist_front",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Waist Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Waist Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBackOverShoulderToWaistFront_M, m, g, d, "P07");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_neck_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Neck Back", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Neck Back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToNeckBack_M, m, g, d, "P08");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "across_back_center_to_armfold_front_to_across_back_center",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Across Back Center to Armfold Front to Across Back Center",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Across Back Center to Armfold Front to Across Back Center",
                                  "Full measurement description.");
    InitMeasurement(acrossBackCenterToArmfoldFrontToAcrossBackCenter_M, m, g, d, "P09");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "neck_back_to_armfold_front_to_highbust_back",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Highbust Back",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Neck Back to Armfold Front to Highbust Back",
                                  "Full measurement description.");
    InitMeasurement(neckBackToArmfoldFrontToHighbustBack_M, m, g, d, "P10");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "highbust_b_over_shoulder_to_highbust_f",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Highbust Front",
                                  "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Highbust Back over Shoulder to Highbust Front",
                                  "Full measurement description.");
    InitMeasurement(highbustBOverShoulderToHighbustF_M, m, g, d, "P11",
                    "(mid_shoulder_to_highbust_f + mid_shoulder_to_highbust_b)");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "armscye_arc",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Armscye Arc through Shoulder Tip", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Armscye Arc through Shoulder Tip", "Full measurement description.");
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
    m = QmuTranslation::translate("Measurements", "dart_width_shoulder",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Shoulder", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Shoulder", "Full measurement description.");
    InitMeasurement(dartWidthShoulder_M, m, g, d, "Q01");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "dart_width_bust",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Bust", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Bust", "Full measurement description.");
    InitMeasurement(dartWidthBust_M, m, g, d, "Q02");
    //=================================================================================================================
    m = QmuTranslation::translate("Measurements", "dart_width_waist",
                                  "Name in a formula. Don't use math symbols and space in name!!!!");
    g = QmuTranslation::translate("Measurements", "Dart Width > Waist", "Full measurement name.");
    d = QmuTranslation::translate("Measurements", "Dart Width > Waist", "Full measurement description.");
    InitMeasurement(dartWidthWaist_M, m, g, d, "Q03");
    //=================================================================================================================
}

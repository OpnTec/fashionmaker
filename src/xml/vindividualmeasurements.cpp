/************************************************************************
 **
 **  @file   vindividualmeasurements.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vindividualmeasurements.h"

const QString VIndividualMeasurements::AttrIgnore      = QStringLiteral("ignore");
const QString VIndividualMeasurements::AttrName        = QStringLiteral("name");
const QString VIndividualMeasurements::AttrM_number    = QStringLiteral("m_number");
const QString VIndividualMeasurements::AttrGui_text    = QStringLiteral("gui_text");
const QString VIndividualMeasurements::AttrValue       = QStringLiteral("value");
const QString VIndividualMeasurements::AttrDescription = QStringLiteral("description");
const QString VIndividualMeasurements::AttrLang        = QStringLiteral("lang");
const QString VIndividualMeasurements::AttrFamily_name = QStringLiteral("family-name");
const QString VIndividualMeasurements::AttrGiven_name  = QStringLiteral("given-name");
const QString VIndividualMeasurements::AttrBirth_date  = QStringLiteral("birth-date");
const QString VIndividualMeasurements::AttrSex         = QStringLiteral("sex");

VIndividualMeasurements::VIndividualMeasurements(VContainer *data):VDomDocument(data)
{
}

Valentina::Units VIndividualMeasurements::Unit()
{
    const QString unit = UniqueTagText(AttrUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

void VIndividualMeasurements::Measurements()
{
    //head and neck
    Measurement("head_girth");
    Measurement("mid_neck_girth");
    Measurement("neck_base_girth");
    Measurement("head_and_neck_length");
    //torso
    Measurement("center_front_waist_length");
    Measurement("center_back_waist_length");
    Measurement("shoulder_length");
    Measurement("side_waist_length");
    Measurement("trunk_length");
    Measurement("shoulder_girth");
    Measurement("upper_chest_girth");
    Measurement("bust__girth");
    Measurement("under_bust_girth");
    Measurement("waist_girth");
    Measurement("high_hip_girth");
    Measurement("hip_girth");
    Measurement("upper_front_chest_width");
    Measurement("front_chest_width");
    Measurement("across_front_shoulder_width");
    Measurement("across_back_shoulder_width");
    Measurement("upper_back_width");
    Measurement("back_width");
    Measurement("bustpoint_to_bustpoint");
    Measurement("halter_bustpoint_to_bustpoint");
    Measurement("neck_to_bustpoint");
    Measurement("crotch_length");
    Measurement("rise_height");
    Measurement("shoulder_drop");
    Measurement("shoulder_slope_degrees");
    Measurement("front_shoulder_slope_length");
    Measurement("back_shoulder_slope_length");
    Measurement("front_shoulder_to_waist_length");
    Measurement("back_shoulder_to_waist_length");
    Measurement("front_neck_arc");
    Measurement("back_neck_arc");
    Measurement("front_upper-bust_arc");
    Measurement("back_upper-bust_arc");
    Measurement("front_waist_arc");
    Measurement("back_waist_arc");
    Measurement("front_upper-hip_arc");
    Measurement("back_upper-hip_arc");
    Measurement("front_hip_arc");
    Measurement("back_hip_arc");
    Measurement("chest_slope");
    Measurement("back_slope");
    Measurement("front_waist_slope");
    Measurement("back_waist_slope");
    Measurement("front-neck_to_upper-chest_height");
    Measurement("front-neck_to_bust_height");
    //arm
    Measurement("armscye_girth");
    Measurement("elbow_girth");
    Measurement("upper-arm_girth");
    Measurement("wrist_girth");
    Measurement("scye_depth");
    Measurement("shoulder_and_arm_length");
    Measurement("underarm_length");
    Measurement("cervicale_to_wrist_length");
    Measurement("shoulder_to_elbow_length");
    Measurement("arm_length");
    //hand
    Measurement("hand_width");
    Measurement("hand_length");
    Measurement("hand_girth");
    //leg
    Measurement("thigh_girth");
    Measurement("mid_thigh_girth");
    Measurement("knee_girth");
    Measurement("calf_girth");
    Measurement("ankle_girth");
    Measurement("knee_height");
    Measurement("ankle_height");
    //foot
    Measurement("foot_width");
    Measurement("foot_length");
    //heights
    Measurement("height");
    Measurement("cervicale_height");
    Measurement("cervicale_to_knee_height");
    Measurement("waist_height");
    Measurement("high_hip_height");
    Measurement("hip_height");
    Measurement("waist_to_hip_height");
    Measurement("waist_to_knee_height");
    Measurement("crotch_height");
}

void VIndividualMeasurements::Measurement(const QString &tag)
{
    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        qWarning()<<"Measurement" << tag <<"doesn't exist"<<Q_FUNC_INFO;
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
                const bool ignore = QVariant(GetParametrString(domElement, AttrIgnore, "false")).toBool();
                if (ignore)
                {
                    return;
                }
                const QString name = GetParametrString(domElement, AttrName, "");
                if (name.isEmpty())
                {
                    return;
                }
                const QString m_number = GetParametrString(domElement, AttrM_number, "");
                const QString gui_text = GetParametrString(domElement, AttrGui_text, "");
                const qreal value = GetParametrDouble(domElement, AttrValue, "0.0");
                const QString description = GetParametrString(domElement, AttrDescription, "");

                if (Unit() == Valentina::Mm)//Convert to Cm.
                {
                    data->AddMeasurement(name, VMeasurement(value/10.0, gui_text, description, tag));
                    if (m_number.isEmpty())
                    {
                        qWarning()<<"Can't find language-independent measurement name for "<< tag;
                        return;
                    }
                    else
                    {
                        VMeasurement m(value/10.0, gui_text, description, tag);
                        m.setVirtual(true);
                        data->AddMeasurement(m_number, m);
                    }
                }
                else//Cm or inch.
                {
                    data->AddMeasurement(name, VMeasurement(value, gui_text, description, tag));
                    if (m_number.isEmpty())
                    {
                        qWarning()<<"Can't find language-independent measurement name for "<< tag;
                        return;
                    }
                    else
                    {
                        VMeasurement m(value, gui_text, description, tag);
                        m.setVirtual(true);
                        data->AddMeasurement(m_number, m);
                    }
                }
            }
        }
    }
}

QString VIndividualMeasurements::Language()
{
    return UniqueTagText(AttrLang, "en");
}

QString VIndividualMeasurements::FamilyName()
{
    return UniqueTagText(AttrFamily_name, "");
}

QString VIndividualMeasurements::GivenName()
{
    return UniqueTagText(AttrGiven_name, "");
}

QString VIndividualMeasurements::BirthDate()
{
    return UniqueTagText(AttrBirth_date, "");
}

QString VIndividualMeasurements::Sex()
{
    return UniqueTagText(AttrSex, "");
}

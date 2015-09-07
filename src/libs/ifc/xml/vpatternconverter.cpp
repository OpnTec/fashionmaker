/************************************************************************
 **
 **  @file   vpatternconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vpatternconverter.h"
#include "exception/vexception.h"
#include "exception/vexceptionemptyparameter.h"
#include "../qmuparser/qmutokenparser.h"

#include <QDir>
#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VPatternConverter::PatternMinVerStr = QStringLiteral("0.1.0");
const QString VPatternConverter::PatternMaxVerStr = QStringLiteral("0.2.0");
const QString VPatternConverter::CurrentSchema    = QStringLiteral("://schema/pattern/v0.2.0.xsd");

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::VPatternConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::~VPatternConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VPatternConverter::MinVer() const
{
    return GetVersion(PatternMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternConverter::MaxVer() const
{
    return GetVersion(PatternMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::MinVerStr() const
{
    return PatternMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::MaxVerStr() const
{
    return PatternMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000100):
            return QStringLiteral("://schema/pattern/v0.1.0.xsd");
        case (0x000101):
            return QStringLiteral("://schema/pattern/v0.1.1.xsd");
        case (0x000102):
            return QStringLiteral("://schema/pattern/v0.1.2.xsd");
        case (0x000103):
            return QStringLiteral("://schema/pattern/v0.1.3.xsd");
        case (0x000104):
            return QStringLiteral("://schema/pattern/v0.1.4.xsd");
        case (0x000200):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000100):
            {
                ToV0_1_1();
                const QString schema = XSDSchema(0x000101);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000101):
            {
                ToV0_1_2();
                const QString schema = XSDSchema(0x000102);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000102):
            {
                ToV0_1_3();
                const QString schema = XSDSchema(0x000103);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000103):
            {
                ToV0_1_4();
                const QString schema = XSDSchema(0x000104);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000104):
            {
                ToV0_2_0();
                const QString schema = XSDSchema(0x000200);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000200):
                break;
            default:
                break;
        }
    }
    catch (VException &e)
    {
        QString error;
        const QString backupFileName = fileName +".backup";
        if (SafeCopy(backupFileName, fileName, error) == false)
        {
            const QString errorMsg(tr("Error restoring backup file: %1.").arg(error));
            VException excep(errorMsg);
            excep.AddMoreInformation(e.ErrorMessage());
            throw excep;
        }

        QFile file(backupFileName);
        file.remove();

        throw;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_1()
{
    SetVersion(QStringLiteral("0.1.1"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_2()
{
    SetVersion(QStringLiteral("0.1.2"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_3()
{
    SetVersion(QStringLiteral("0.1.3"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_1_4()
{
    SetVersion(QStringLiteral("0.1.4"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_0()
{
    SetVersion(QStringLiteral("0.2.0"));
    TagUnitToV0_2_0();
    TagIncrementToV0_2_0();
    ConvertMeasurementsToV0_2_0();
    TagMeasurementsToV0_2_0();//Alwayse last!!!
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::TagUnitToV0_2_0()
{
    QDomElement unit = createElement("unit");
    QDomText newNodeText = createTextNode(MUnitV0_1_4());
    unit.appendChild(newNodeText);

    QDomElement patternElement = documentElement();
    patternElement.insertAfter(unit, patternElement.firstChildElement("version"));
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::TagIncrementToV0_2_0()
{
    const QSet<QString> names = FixIncrementsToV0_2_0();

    FixPointExpressionsToV0_2_0(names);
    FixArcExpressionsToV0_2_0(names);
    FixPathPointExpressionsToV0_2_0(names);
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ConvertMeasurementsToV0_2_0()
{
    const QMap<QString, QString> names = OldNamesToNewNames_InV0_2_0();
    ConvertPointExpressionsToV0_2_0(names);
    ConvertArcExpressionsToV0_2_0(names);
    ConvertPathPointExpressionsToV0_2_0(names);
}

//---------------------------------------------------------------------------------------------------------------------
QSet<QString> VPatternConverter::FixIncrementsToV0_2_0()
{
    QSet<QString> names;
    const QDomElement incr = TagIncrementsV0_1_4();
    QDomNode domNode = incr.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == "increment")
                {
                    try
                    {
                        const QString name = GetParametrString(domElement, "name");
                        names.insert(name);
                        domElement.setAttribute("name", "#"+name);

                        const QString base = GetParametrString(domElement, "base");
                        domElement.setAttribute("formula", base);
                    }
                    catch (VExceptionEmptyParameter &e)
                    {
                        VException excep("Can't get increment.");
                        excep.AddMoreInformation(e.ErrorMessage());
                        throw excep;
                    }
                    domElement.removeAttribute("id");
                    domElement.removeAttribute("kgrowth");
                    domElement.removeAttribute("ksize");
                    domElement.removeAttribute("base");
                }
            }
        }
        domNode = domNode.nextSibling();
    }
    return names;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::FixPointExpressionsToV0_2_0(const QSet<QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("point");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "length");
            dom.setAttribute("length", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle");
            dom.setAttribute("angle", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
        try
        {
            formula = GetParametrString(dom, "c1Radius");
            dom.setAttribute("c1Radius", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "c2Radius");
            dom.setAttribute("c2Radius", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "cRadius");
            dom.setAttribute("cRadius", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::FixArcExpressionsToV0_2_0(const QSet<QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("arc");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "angle1");
            dom.setAttribute("angle1", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle2");
            dom.setAttribute("angle2", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "radius");
            dom.setAttribute("radius", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "length");
            dom.setAttribute("length", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::FixPathPointExpressionsToV0_2_0(const QSet<QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("pathPoint");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "kAsm1");
            dom.setAttribute("kAsm1", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "kAsm2");
            dom.setAttribute("kAsm2", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle");
            dom.setAttribute("angle", FixIncrementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ConvertPointExpressionsToV0_2_0(const QMap<QString, QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("point");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "length");
            dom.setAttribute("length", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle");
            dom.setAttribute("angle", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
        try
        {
            formula = GetParametrString(dom, "c1Radius");
            dom.setAttribute("c1Radius", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "c2Radius");
            dom.setAttribute("c2Radius", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "cRadius");
            dom.setAttribute("cRadius", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ConvertArcExpressionsToV0_2_0(const QMap<QString, QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("arc");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "angle1");
            dom.setAttribute("angle1", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle2");
            dom.setAttribute("angle2", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "radius");
            dom.setAttribute("radius", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "length");
            dom.setAttribute("length", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ConvertPathPointExpressionsToV0_2_0(const QMap<QString, QString> &names)
{
    QString formula;
    const QDomNodeList list = elementsByTagName("pathPoint");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement dom = list.at(i).toElement();

        try
        {
            formula = GetParametrString(dom, "kAsm1");
            dom.setAttribute("kAsm1", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "kAsm2");
            dom.setAttribute("kAsm2", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            formula = GetParametrString(dom, "angle");
            dom.setAttribute("angle", FixMeasurementInFormulaToV0_2_0(formula, names));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::FixMeasurementInFormulaToV0_2_0(const QString &formula, const QMap<QString, QString> &names)
{
    qmu::QmuTokenParser *cal = new qmu::QmuTokenParser(formula, false, false);// Eval formula
    QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
    delete cal;

    QList<int> tKeys = tokens.keys();// Take all tokens positions
    QList<QString> tValues = tokens.values();

    QString newFormula = formula;// Local copy for making changes
    for (int i = 0; i < tValues.size(); ++i)
    {
        if (not names.contains(tValues.at(i)))
        {
            continue;
        }

        int bias = 0;
        Replace(newFormula, names.value(tValues.at(i)), tKeys.at(i), tValues.at(i), bias);
        if (bias != 0)
        {// Translated token has different length than original. Position next tokens need to be corrected.
            CorrectionsPositions(tKeys.at(i), bias, tokens);
            tKeys = tokens.keys();
            tValues = tokens.values();
        }
    }
    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::FixIncrementInFormulaToV0_2_0(const QString &formula, const QSet<QString> &names)
{
    qmu::QmuTokenParser *cal = new qmu::QmuTokenParser(formula, false, false);// Eval formula
    QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
    delete cal;

    QList<int> tKeys = tokens.keys();// Take all tokens positions
    QList<QString> tValues = tokens.values();

    QString newFormula = formula;// Local copy for making changes
    for (int i = 0; i < tValues.size(); ++i)
    {
        if (not names.contains(tValues.at(i)))
        {
            continue;
        }

        int bias = 0;
        Replace(newFormula, "#"+tValues.at(i), tKeys.at(i), tValues.at(i), bias);
        if (bias != 0)
        {// Translated token has different length than original. Position next tokens need to be corrected.
            CorrectionsPositions(tKeys.at(i), bias, tokens);
            tKeys = tokens.keys();
            tValues = tokens.values();
        }
    }
    return newFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::TagMeasurementsToV0_2_0()
{
    QDomElement ms = TagMeasurementsV0_1_4();
    const QString path = GetParametrString(ms, "path");

    ms.removeAttribute("unit");
    ms.removeAttribute("type");
    ms.removeAttribute("path");

    QDomText newNodeText = createTextNode(QFileInfo(fileName).absoluteDir().relativeFilePath(path));
    ms.appendChild(newNodeText);
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::MUnitV0_1_4() const
{
    const QDomElement element = TagMeasurementsV0_1_4();
    try
    {
        return GetParametrString(element, "unit");
    }
    catch (VExceptionEmptyParameter &e)
    {
        VException excep("Can't get unit.");
        excep.AddMoreInformation(e.ErrorMessage());
        throw excep;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternConverter::TagMeasurementsV0_1_4() const
{
    const QDomNodeList list = elementsByTagName("measurements");
    const QDomElement element = list.at(0).toElement();
    if (not element.isElement())
    {
        VException excep("Can't get tag measurements.");
        throw excep;
    }
    return element;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternConverter::TagIncrementsV0_1_4() const
{
    const QDomNodeList list = elementsByTagName("increments");
    const QDomElement element = list.at(0).toElement();
    if (not element.isElement())
    {
        VException excep("Can't get tag measurements.");
        throw excep;
    }
    return element;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VPatternConverter::ListPathPointExpressionsV0_1_4() const
{
    QStringList expressions;
    const QDomNodeList list = elementsByTagName("pathPoint");
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        try
        {
            expressions.append(GetParametrString(dom, "kAsm1"));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, "kAsm2"));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        try
        {
            expressions.append(GetParametrString(dom, "angle"));
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }
    }

    return expressions;
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> VPatternConverter::OldNamesToNewNames_InV0_2_0()
{
    QMap<QString, QString> names;

    // A02
    names.insert(QStringLiteral("cervicale_height"), QStringLiteral("height_neck_back"));
    // A03
    names.insert(QStringLiteral("height_scapular_point"), QStringLiteral("height_scapula"));
    // A04
    names.insert(QStringLiteral("height_back_angle_axilla"), QStringLiteral("height_armpit"));
    // A05
    names.insert(QStringLiteral("waist_height"), QStringLiteral("height_waist_side"));
    // A06
    names.insert(QStringLiteral("hip_height"), QStringLiteral("height_hip"));
    // A08
    names.insert(QStringLiteral("knee_height"), QStringLiteral("height_knee"));
    // A11
    names.insert(QStringLiteral("ankle_height"), QStringLiteral("height_ankle"));
    // A12
    names.insert(QStringLiteral("high_hip_height"), QStringLiteral("height_highhip"));
    // A13
    names.insert(QStringLiteral("front_waist_to_floor"), QStringLiteral("height_waist_front"));
    // A14
    names.insert(QStringLiteral("height_nipple_point"), QStringLiteral("height_bustpoint"));
    // A15
    names.insert(QStringLiteral("shoulder_height"), QStringLiteral("height_shoulder_tip"));
    names.insert(QStringLiteral("height_shoulder_point"), QStringLiteral("height_shoulder_tip"));
    // A16
    names.insert(QStringLiteral("height_clavicular_point"), QStringLiteral("height_neck_front"));
    names.insert(QStringLiteral("height_front_neck_base_point"), QStringLiteral("height_neck_front"));
    // A17
    names.insert(QStringLiteral("height_base_neck_side_point"), QStringLiteral("height_neck_side"));
    // A18
    names.insert(QStringLiteral("neck_to_knee_point"), QStringLiteral("height_neck_back_to_knee"));
    names.insert(QStringLiteral("cervicale_to_knee_height"), QStringLiteral("height_neck_back_to_knee"));
    // A19
    names.insert(QStringLiteral("waist_to_knee_height"), QStringLiteral("height_waist_side_to_knee"));
    // A20
    names.insert(QStringLiteral("waist_to_hip_height"), QStringLiteral("height_waist_side_to_hip"));
    // C01
    names.insert(QStringLiteral("body_position"), QStringLiteral("indent_neck_back"));
    // D01
    names.insert(QStringLiteral("half_girth_neck_for_shirts"), QStringLiteral("neck_mid_circ"));
    names.insert(QStringLiteral("mid_neck_girth"), QStringLiteral("neck_mid_circ"));
    // D02
    names.insert(QStringLiteral("neck_base_girth"), QStringLiteral("neck_circ"));
    // D03
    names.insert(QStringLiteral("upper_chest_girth"), QStringLiteral("highbust_circ"));
    // D04
    names.insert(QStringLiteral("bust_girth"), QStringLiteral("bust_circ"));
    // D05
    names.insert(QStringLiteral("under_bust_girth"), QStringLiteral("lowbust_circ"));
    // D07
    names.insert(QStringLiteral("waist_girth"), QStringLiteral("waist_circ"));
    // D08
    names.insert(QStringLiteral("high_hip_girth"), QStringLiteral("highhip_circ"));
    // D09
    names.insert(QStringLiteral("hips_excluding_protruding_abdomen"), QStringLiteral("hip_circ"));
    names.insert(QStringLiteral("hip_girth"), QStringLiteral("hip_circ"));
    // D10
    names.insert(QStringLiteral("half_girth_neck"), QStringLiteral("neck_arc_f"));
    names.insert(QStringLiteral("front_neck_arc"), QStringLiteral("neck_arc_f"));
    // D11
    names.insert(QStringLiteral("half_girth_chest_first"), QStringLiteral("highbust_arc_f"));
    names.insert(QStringLiteral("front_upper_chest_arc"), QStringLiteral("highbust_arc_f"));
    // D12
    names.insert(QStringLiteral("half_girth_chest_second"), QStringLiteral("bust_arc_f"));
    // D13
    names.insert(QStringLiteral("half_girth_chest_third"), QStringLiteral("lowbust_arc_f"));
    // D15
    names.insert(QStringLiteral("half_girth_waist"), QStringLiteral("waist_arc_f"));
    names.insert(QStringLiteral("front_waist_arc"), QStringLiteral("waist_arc_f"));
    // D16
    names.insert(QStringLiteral("front_upper_hip_arc"), QStringLiteral("highhip_arc_f"));
    // D17
    names.insert(QStringLiteral("half_girth_hips_excluding_protruding_abdomen"), QStringLiteral("hip_arc_f"));
    names.insert(QStringLiteral("front_hip_arc"), QStringLiteral("hip_arc_f"));
    // D18
    names.insert(QStringLiteral("back_neck_arc"), QStringLiteral("neck_arc_b"));
    // D19
    names.insert(QStringLiteral("back_upper_chest_arc"), QStringLiteral("highbust_arc_b"));
    // D23
    names.insert(QStringLiteral("back_waist_arc"), QStringLiteral("waist_arc_b"));
    // D24
    names.insert(QStringLiteral("back_upper_hip_arc"), QStringLiteral("highhip_arc_b"));
    // D25
    names.insert(QStringLiteral("back_hip_arc"), QStringLiteral("hip_arc_b"));
    // D42
    names.insert(QStringLiteral("half_girth_hips_considering_protruding_abdomen"),
                 QStringLiteral("hip_with_abdomen_arc_f"));
    // D43
    names.insert(QStringLiteral("shoulder_girth"), QStringLiteral("body_armfold_circ"));
    // D45
    names.insert(QStringLiteral("trunk_length"), QStringLiteral("body_torso_circ"));
    // E01
    names.insert(QStringLiteral("front_waist_length"), QStringLiteral("neck_front_to_waist_f"));
    names.insert(QStringLiteral("center_front_waist_length"), QStringLiteral("neck_front_to_waist_f"));
    // E03
    names.insert(QStringLiteral("side_waist_length"), QStringLiteral("armpit_to_waist_side"));
    // E05
    names.insert(QStringLiteral("neck_to_front_waist_line"), QStringLiteral("neck_side_to_waist_f"));
    names.insert(QStringLiteral("waist_to_neck_side"), QStringLiteral("neck_side_to_waist_f"));
    names.insert(QStringLiteral("front_shoulder_to_waist_length"), QStringLiteral("neck_side_to_waist_f"));
    // E08
    names.insert(QStringLiteral("back_shoulder_to_waist_length"), QStringLiteral("neck_side_to_waist_b"));
    // E09
    names.insert(QStringLiteral("center_back_waist_length"), QStringLiteral("neck_back_to_waist_b"));
    // E11
    names.insert(QStringLiteral("neck_to_first_line_chest_circumference"), QStringLiteral("neck_front_to_highbust_f"));
    names.insert(QStringLiteral("front_neck_to_upper_chest_height"), QStringLiteral("neck_front_to_highbust_f"));
    // E12
    names.insert(QStringLiteral("front_neck_to_bust_height"), QStringLiteral("neck_front_to_bust_f"));
    // E13
    names.insert(QStringLiteral("front_waist_to_upper_chest"), QStringLiteral("highbust_to_waist_f"));
    // E15
    names.insert(QStringLiteral("front_waist_to_lower_breast"), QStringLiteral("lowbust_to_waist_f"));
    // E17
    names.insert(QStringLiteral("neck_to_back_line_chest_circumference"), QStringLiteral("neck_back_to_highbust_b"));
    // E30
    names.insert(QStringLiteral("depth_waist_first"), QStringLiteral("waist_to_highhip_f"));
    // E31
    names.insert(QStringLiteral("depth_waist_second"), QStringLiteral("waist_to_hip_f"));
    // E36
    names.insert(QStringLiteral("shoulder_slope_degrees"), QStringLiteral("shoulder_slope_neck_side_angle"));
    // E37
    names.insert(QStringLiteral("shoulder_drop"), QStringLiteral("shoulder_slope_neck_side_length"));
    // F02
    names.insert(QStringLiteral("across_front_shoulder_width"), QStringLiteral("shoulder_width_f"));
    // F03
    names.insert(QStringLiteral("upper_front_chest_width"), QStringLiteral("across_chest_f"));
    // F04
    names.insert(QStringLiteral("chest_width"), QStringLiteral("armfold_to_armfold_f"));
    names.insert(QStringLiteral("front_chest_width"), QStringLiteral("armfold_to_armfold_f"));
    // F05
    names.insert(QStringLiteral("arc_behind_shoulder_girdle"), QStringLiteral("shoulder_width_b"));
    names.insert(QStringLiteral("across_back_shoulder_width"), QStringLiteral("shoulder_width_b"));
    // F06
    names.insert(QStringLiteral("upper_back_width"), QStringLiteral("across_back_b"));
    // F07
    names.insert(QStringLiteral("back_width"), QStringLiteral("armfold_to_armfold_b"));
    // F14
    names.insert(QStringLiteral("neck_transverse_diameter"), QStringLiteral("neck_width"));
    // G01
    names.insert(QStringLiteral("bustpoint_to_bustpoint"), QStringLiteral("bustpoint_to_bustpoint"));
    // G02
    names.insert(QStringLiteral("neck_to_bustpoint"), QStringLiteral("bustpoint_to_neck_side"));
    // G06
    names.insert(QStringLiteral("halter_bustpoint_to_bustpoint"), QStringLiteral("bustpoint_to_bustpoint_halter"));
    // H01
    names.insert(QStringLiteral("front_slash_shoulder_height"), QStringLiteral("shoulder_tip_to_waist_front"));
    names.insert(QStringLiteral("front_shoulder_slope_length"), QStringLiteral("shoulder_tip_to_waist_front"));
    // H02
    names.insert(QStringLiteral("front_waist_slope"), QStringLiteral("neck_front_to_waist_side"));
    // H04
    names.insert(QStringLiteral("height_armhole_slash"), QStringLiteral("neck_side_to_armfold_f"));
    names.insert(QStringLiteral("chest_slope"), QStringLiteral("neck_side_to_armfold_f"));
    // H07
    names.insert(QStringLiteral("slash_shoulder_height"), QStringLiteral("shoulder_tip_to_waist_back"));
    names.insert(QStringLiteral("back_shoulder_slope_length"), QStringLiteral("shoulder_tip_to_waist_back"));
    // H08
    names.insert(QStringLiteral("back_waist_slope"), QStringLiteral("neck_back_to_waist_side"));
    // H10
    names.insert(QStringLiteral("back_slope"), QStringLiteral("neck_side_to_armfold_b"));
    // I05
    names.insert(QStringLiteral("arm_length"), QStringLiteral("arm_shoulder_tip_to_wrist"));
    // I06
    names.insert(QStringLiteral("shoulder_to_elbow_length"), QStringLiteral("arm_shoulder_tip_to_elbow"));
    // I08
    names.insert(QStringLiteral("underarm_length"), QStringLiteral("arm_armpit_to_wrist"));
    // I11
    names.insert(QStringLiteral("upper_arm_girth"), QStringLiteral("arm_upper_circ"));
    // I15
    names.insert(QStringLiteral("wrist_girth"), QStringLiteral("arm_wrist_circ"));
    // I17
    names.insert(QStringLiteral("armscye_girth"), QStringLiteral("armscye_circ"));
    // I19
    names.insert(QStringLiteral("anteroposterior_diameter_hands"), QStringLiteral("armscye_width"));
    // I20
    names.insert(QStringLiteral("neck_to_third_finger"), QStringLiteral("arm_neck_side_to_finger_tip"));
    // I21
    names.insert(QStringLiteral("neck_to_radial_point"), QStringLiteral("arm_neck_side_to_wrist"));
    names.insert(QStringLiteral("shoulder_and_arm_length"), QStringLiteral("arm_neck_side_to_wrist"));
    // J01
    names.insert(QStringLiteral("crotch_height"), QStringLiteral("leg_crotch_to_floor"));
    // J02
    names.insert(QStringLiteral("side_waist_to_floor"), QStringLiteral("leg_waist_side_to_floor"));
    // J03
    names.insert(QStringLiteral("waist_to_knee"), QStringLiteral("leg_waist_side_to_knee"));
    // J04
    names.insert(QStringLiteral("thigh_girth"), QStringLiteral("leg_thigh_upper_circ"));
    // J05
    names.insert(QStringLiteral("mid_thigh_girth"), QStringLiteral("leg_thigh_mid_circ"));
    // J06
    names.insert(QStringLiteral("knee_girth"), QStringLiteral("leg_knee_circ"));
    // J08
    names.insert(QStringLiteral("calf_girth"), QStringLiteral("leg_calf_circ"));
    // J10
    names.insert(QStringLiteral("ankle_girth"), QStringLiteral("leg_ankle_circ"));
    // J11
    names.insert(QStringLiteral("girth_knee_flexed_feet"), QStringLiteral("leg_knee_circ_bent"));
    // K01
    names.insert(QStringLiteral("arc_through_groin_area"), QStringLiteral("crotch_length"));
    // K04
    names.insert(QStringLiteral("waist_to_plane_seat"), QStringLiteral("rise_length_side"));
    names.insert(QStringLiteral("rise_height"), QStringLiteral("rise_length_side"));
    // L02
    names.insert(QStringLiteral("hand_vertical_diameter"), QStringLiteral("hand_length"));
    // L03
    names.insert(QStringLiteral("hand_width"), QStringLiteral("hand_palm_width"));
    // L05
    names.insert(QStringLiteral("hand_girth"), QStringLiteral("hand_circ"));
    // M04
    names.insert(QStringLiteral("girth_foot_instep"), QStringLiteral("foot_instep_circ"));
    // N02
    names.insert(QStringLiteral("head_height"), QStringLiteral("head_length"));
    // N05
    names.insert(QStringLiteral("head_and_neck_length"), QStringLiteral("head_crown_to_neck_back"));
    // N06
    names.insert(QStringLiteral("neck_to_neck_base"), QStringLiteral("head_chin_to_neck_back"));
    // O02
    names.insert(QStringLiteral("arc_length_upper_body"), QStringLiteral("waist_to_waist_halter"));
    // O09
    names.insert(QStringLiteral("cervicale_to_wrist_length"), QStringLiteral("arm_neck_back_to_wrist_bent"));
    // P11
    names.insert(QStringLiteral("strap_length"), QStringLiteral("highbust_b_over_shoulder_to_highbust_f"));
    // P12
    names.insert(QStringLiteral("arc_through_shoulder_joint"), QStringLiteral("armscye_arc"));

    return names;
}

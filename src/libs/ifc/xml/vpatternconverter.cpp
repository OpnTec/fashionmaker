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
#include "../exception/vexception.h"
#include "../exception/vexceptionemptyparameter.h"
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
const QString VPatternConverter::PatternMaxVerStr = QStringLiteral("0.3.3");
const QString VPatternConverter::CurrentSchema    = QStringLiteral("://schema/pattern/v0.3.3.xsd");

//VPatternConverter::PatternMinVer; // <== DON'T FORGET TO UPDATE TOO!!!!
//VPatternConverter::PatternMaxVer; // <== DON'T FORGET TO UPDATE TOO!!!!

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::VPatternConverter(const QString &fileName)
    :VAbstractConverter(fileName)
{
    ValidateInputFile(CurrentSchema);
}

//---------------------------------------------------------------------------------------------------------------------
VPatternConverter::~VPatternConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternConverter::XSDSchema(int ver) const
{
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
            return QStringLiteral("://schema/pattern/v0.2.0.xsd");
        case (0x000201):
            return QStringLiteral("://schema/pattern/v0.2.1.xsd");
        case (0x000202):
            return QStringLiteral("://schema/pattern/v0.2.2.xsd");
        case (0x000203):
            return QStringLiteral("://schema/pattern/v0.2.3.xsd");
        case (0x000204):
            return QStringLiteral("://schema/pattern/v0.2.4.xsd");
        case (0x000205):
            return QStringLiteral("://schema/pattern/v0.2.5.xsd");
        case (0x000206):
            return QStringLiteral("://schema/pattern/v0.2.6.xsd");
        case (0x000207):
            return QStringLiteral("://schema/pattern/v0.2.7.xsd");
        case (0x000300):
            return QStringLiteral("://schema/pattern/v0.3.0.xsd");
        case (0x000301):
            return QStringLiteral("://schema/pattern/v0.3.1.xsd");
        case (0x000302):
            return QStringLiteral("://schema/pattern/v0.3.2.xsd");
        case (0x000303):
            return CurrentSchema;
        default:
            InvalidVersion(ver);
            break;
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
                ToV0_1_1();
                ValidateXML(XSDSchema(0x000101), fileName);
                V_FALLTHROUGH
            case (0x000101):
                ToV0_1_2();
                ValidateXML(XSDSchema(0x000102), fileName);
                V_FALLTHROUGH
            case (0x000102):
                ToV0_1_3();
                ValidateXML(XSDSchema(0x000103), fileName);
                V_FALLTHROUGH
            case (0x000103):
                ToV0_1_4();
                ValidateXML(XSDSchema(0x000104), fileName);
                V_FALLTHROUGH
            case (0x000104):
                ToV0_2_0();
                ValidateXML(XSDSchema(0x000200), fileName);
                V_FALLTHROUGH
            case (0x000200):
                ToV0_2_1();
                ValidateXML(XSDSchema(0x000201), fileName);
                V_FALLTHROUGH
            case (0x000201):
                ToV0_2_2();
                ValidateXML(XSDSchema(0x000202), fileName);
                V_FALLTHROUGH
            case (0x000202):
                ToV0_2_3();
                ValidateXML(XSDSchema(0x000203), fileName);
                V_FALLTHROUGH
            case (0x000203):
                ToV0_2_4();
                ValidateXML(XSDSchema(0x000204), fileName);
                V_FALLTHROUGH
            case (0x000204):
                ToV0_2_5();
                ValidateXML(XSDSchema(0x000205), fileName);
                V_FALLTHROUGH
            case (0x000205):
                ToV0_2_6();
                ValidateXML(XSDSchema(0x000206), fileName);
                V_FALLTHROUGH
            case (0x000206):
                ToV0_2_7();
                ValidateXML(XSDSchema(0x000207), fileName);
                V_FALLTHROUGH
            case (0x000207):
                ToV0_3_0();
                ValidateXML(XSDSchema(0x000300), fileName);
                V_FALLTHROUGH
            case (0x000300):
                ToV0_3_1();
                ValidateXML(XSDSchema(0x000301), fileName);
                V_FALLTHROUGH
            case (0x000301):
                ToV0_3_2();
                ValidateXML(XSDSchema(0x000302), fileName);
                V_FALLTHROUGH
            case (0x000302):
                ToV0_3_3();
                ValidateXML(XSDSchema(0x000303), fileName);
                V_FALLTHROUGH
            case (0x000303):
                break;
            default:
                break;
        }
    }
    catch (VException &e)
    {
        QString error;
        const QString backupFileName = fileName + QLatin1String(".backup");
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
void VPatternConverter::DowngradeToCurrentMaxVersion()
{
    SetVersion(PatternMaxVerStr);
    Save();
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
void VPatternConverter::ToV0_2_1()
{
    SetVersion(QStringLiteral("0.2.1"));
    ConvertMeasurementsToV0_2_1();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_2()
{
    SetVersion(QStringLiteral("0.2.2"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_3()
{
    SetVersion(QStringLiteral("0.2.3"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_4()
{
    FixToolUnionToV0_2_4();
    SetVersion(QStringLiteral("0.2.4"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_5()
{
    SetVersion(QStringLiteral("0.2.5"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_6()
{
    SetVersion(QStringLiteral("0.2.6"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_2_7()
{
    SetVersion(QStringLiteral("0.2.7"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_3_0()
{
    SetVersion(QStringLiteral("0.3.0"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_3_1()
{
    SetVersion(QStringLiteral("0.3.1"));
    RemoveColorToolCutV0_3_1();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_3_2()
{
    SetVersion(QStringLiteral("0.3.2"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ToV0_3_3()
{
    SetVersion(QStringLiteral("0.3.3"));
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
void VPatternConverter::ConvertMeasurementsToV0_2_1()
{
    const QMap<QString, QString> names = OldNamesToNewNames_InV0_2_1();

    // Structure did not change. We can use the same code.
    ConvertPointExpressionsToV0_2_0(names);
    ConvertArcExpressionsToV0_2_0(names);
    ConvertPathPointExpressionsToV0_2_0(names);
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::RemoveColorToolCutV0_3_1()
{
    const QDomNodeList list = elementsByTagName("point");
    for (int i=0; i < list.size(); ++i)
    {
        QDomElement element = list.at(i).toElement();
        if (not element.isNull())
        {
            const QString type = element.attribute(QStringLiteral("type"));
            if (type == QStringLiteral("cutArc") ||
                type == QStringLiteral("cutSpline") ||
                type == QStringLiteral("cutSplinePath"))
            {
                element.removeAttribute(QStringLiteral("color"));
            }
        }
    }
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
void VPatternConverter::FixToolUnionToV0_2_4()
{
    QDomElement root = documentElement();
    const QDomNodeList modelings = root.elementsByTagName(QStringLiteral("modeling"));
    for (int i=0; i<modelings.size(); ++i)
    {
        ParseModelingToV0_2_4(modelings.at(i).toElement());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::ParseModelingToV0_2_4(const QDomElement &modeling)
{
    QDomElement node = modeling.firstChild().toElement();
    while (not node.isNull())
    {
        if (node.tagName() == QLatin1String("tools"))
        {
            const quint32 toolId = node.attribute(QStringLiteral("id")).toUInt();
            QVector<quint32> children;
            QDomElement childNode = node.nextSibling().toElement();
            const QString attrIdTool = QStringLiteral("idTool");
            while (not childNode.isNull())
            {
                if (childNode.hasAttribute(attrIdTool) && childNode.attribute(attrIdTool).toUInt() == toolId)
                {
                    children.append(childNode.attribute(QStringLiteral("idObject")).toUInt());
                }
                else
                {
                    break;
                }
                childNode = childNode.nextSibling().toElement();
            }

            if (not children.isEmpty())
            {
                SaveChildrenToolUnionToV0_2_4(toolId, children);
            }
            node = childNode;
            continue;
        }
        node = node.nextSibling().toElement();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternConverter::SaveChildrenToolUnionToV0_2_4(quint32 id, const QVector<quint32> &children)
{
    QDomElement toolUnion = elementById(id);
    if (toolUnion.isNull())
    {
        return;
    }

    QDomElement tagChildren = createElement(QString("children"));

    for (int i=0; i<children.size(); ++i)
    {
        QDomElement tagChild = createElement(QString("child"));
        tagChild.appendChild(createTextNode(QString().setNum(children.at(i))));
        tagChildren.appendChild(tagChild);
    }

    toolUnion.appendChild(tagChildren);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> VPatternConverter::OldNamesToNewNames_InV0_2_0()
{
    // old name, new name
    QMap<QString, QString> names;

    names.insert(QStringLiteral("cervicale_height"), QStringLiteral("height_neck_back"));
    names.insert(QStringLiteral("height_scapular_point"), QStringLiteral("height_scapula"));
    names.insert(QStringLiteral("height_back_angle_axilla"), QStringLiteral("height_armpit"));
    names.insert(QStringLiteral("waist_height"), QStringLiteral("height_waist_side"));
    names.insert(QStringLiteral("hip_height"), QStringLiteral("height_hip"));
    names.insert(QStringLiteral("knee_height"), QStringLiteral("height_knee"));
    names.insert(QStringLiteral("ankle_height"), QStringLiteral("height_ankle"));
    names.insert(QStringLiteral("high_hip_height"), QStringLiteral("height_highhip"));
    names.insert(QStringLiteral("front_waist_to_floor"), QStringLiteral("height_waist_front"));
    names.insert(QStringLiteral("height_nipple_point"), QStringLiteral("height_bustpoint"));

    QString name = QStringLiteral("height_shoulder_tip");
    names.insert(QStringLiteral("shoulder_height"), name);
    names.insert(QStringLiteral("height_shoulder_point"), name);

    name = QStringLiteral("height_neck_front");
    names.insert(QStringLiteral("height_clavicular_point"), name);
    names.insert(QStringLiteral("height_front_neck_base_point"), name);

    names.insert(QStringLiteral("height_base_neck_side_point"), QStringLiteral("height_neck_side"));

    name = QStringLiteral("height_neck_back_to_knee");
    names.insert(QStringLiteral("neck_to_knee_point"), name);
    names.insert(QStringLiteral("cervicale_to_knee_height"), name);

    names.insert(QStringLiteral("waist_to_knee_height"), QStringLiteral("height_waist_side_to_knee"));
    names.insert(QStringLiteral("waist_to_hip_height"), QStringLiteral("height_waist_side_to_hip"));
    names.insert(QStringLiteral("body_position"), QStringLiteral("indent_neck_back"));

    name = QStringLiteral("neck_mid_circ");
    names.insert(QStringLiteral("half_girth_neck_for_shirts"), name);
    names.insert(QStringLiteral("mid_neck_girth"), name);

    names.insert(QStringLiteral("neck_base_girth"), QStringLiteral("neck_circ"));
    names.insert(QStringLiteral("upper_chest_girth"), QStringLiteral("highbust_circ"));
    names.insert(QStringLiteral("bust_girth"), QStringLiteral("bust_circ"));
    names.insert(QStringLiteral("under_bust_girth"), QStringLiteral("lowbust_circ"));
    names.insert(QStringLiteral("waist_girth"), QStringLiteral("waist_circ"));
    names.insert(QStringLiteral("high_hip_girth"), QStringLiteral("highhip_circ"));
    names.insert(QStringLiteral("hips_excluding_protruding_abdomen"), QStringLiteral("hip_circ"));
    names.insert(QStringLiteral("hip_girth"), QStringLiteral("hip_circ_with_abdomen"));

    name = QStringLiteral("neck_arc_f");
    names.insert(QStringLiteral("half_girth_neck"), name);
    names.insert(QStringLiteral("front_neck_arc"), name);

    name = QStringLiteral("highbust_arc_f");
    names.insert(QStringLiteral("half_girth_chest_first"), name);
    names.insert(QStringLiteral("front_upper_chest_arc"), name);

    names.insert(QStringLiteral("half_girth_chest_second"), QStringLiteral("bust_arc_f"));
    names.insert(QStringLiteral("half_girth_chest_third"), QStringLiteral("lowbust_arc_f"));

    name = QStringLiteral("waist_arc_f");
    names.insert(QStringLiteral("half_girth_waist"), name);
    names.insert(QStringLiteral("front_waist_arc"), name);

    names.insert(QStringLiteral("front_upper_hip_arc"), QStringLiteral("highhip_arc_f"));

    name = QStringLiteral("hip_arc_f");
    names.insert(QStringLiteral("half_girth_hips_excluding_protruding_abdomen"), name);
    names.insert(QStringLiteral("front_hip_arc"), name);

    names.insert(QStringLiteral("back_neck_arc"), QStringLiteral("neck_arc_b"));
    names.insert(QStringLiteral("back_upper_chest_arc"), QStringLiteral("highbust_arc_b"));
    names.insert(QStringLiteral("back_waist_arc"), QStringLiteral("waist_arc_b"));
    names.insert(QStringLiteral("back_upper_hip_arc"), QStringLiteral("highhip_arc_b"));
    names.insert(QStringLiteral("back_hip_arc"), QStringLiteral("hip_arc_b"));
    names.insert(QStringLiteral("half_girth_hips_considering_protruding_abdomen"),
                 QStringLiteral("hip_with_abdomen_arc_f"));
    names.insert(QStringLiteral("shoulder_girth"), QStringLiteral("body_armfold_circ"));
    names.insert(QStringLiteral("trunk_length"), QStringLiteral("body_torso_circ"));
    names.insert(QStringLiteral("front_waist_length"), QStringLiteral("neck_front_to_waist_f"));
    names.insert(QStringLiteral("center_front_waist_length"), QStringLiteral("neck_front_to_waist_flat_f"));
    names.insert(QStringLiteral("side_waist_length"), QStringLiteral("armpit_to_waist_side"));
    names.insert(QStringLiteral("waist_to_neck_side"), QStringLiteral("neck_side_to_waist_b"));

    name = QStringLiteral("neck_side_to_waist_f");
    names.insert(QStringLiteral("neck_to_front_waist_line"), name);
    names.insert(QStringLiteral("front_shoulder_to_waist_length"), name);

    names.insert(QStringLiteral("back_shoulder_to_waist_length"), QStringLiteral("neck_side_to_waist_b"));
    names.insert(QStringLiteral("center_back_waist_length"), QStringLiteral("neck_back_to_waist_b"));

    name = QStringLiteral("neck_front_to_highbust_f");
    names.insert(QStringLiteral("neck_to_first_line_chest_circumference"), name);
    names.insert(QStringLiteral("front_neck_to_upper_chest_height"), name);

    names.insert(QStringLiteral("front_neck_to_bust_height"), QStringLiteral("neck_front_to_bust_f"));
    names.insert(QStringLiteral("front_waist_to_upper_chest"), QStringLiteral("highbust_to_waist_f"));
    names.insert(QStringLiteral("front_waist_to_lower_breast"), QStringLiteral("lowbust_to_waist_f"));
    names.insert(QStringLiteral("neck_to_back_line_chest_circumference"), QStringLiteral("neck_back_to_highbust_b"));
    names.insert(QStringLiteral("depth_waist_first"), QStringLiteral("waist_to_highhip_f"));
    names.insert(QStringLiteral("depth_waist_second"), QStringLiteral("waist_to_hip_f"));
    names.insert(QStringLiteral("shoulder_slope_degrees"), QStringLiteral("shoulder_slope_neck_side_angle"));
    names.insert(QStringLiteral("shoulder_drop"), QStringLiteral("shoulder_slope_neck_side_length"));
    names.insert(QStringLiteral("across_front_shoulder_width"), QStringLiteral("shoulder_tip_to_shoulder_tip_f"));
    names.insert(QStringLiteral("upper_front_chest_width"), QStringLiteral("across_chest_f"));
    names.insert(QStringLiteral("chest_width"), QStringLiteral("across_chest_f"));
    names.insert(QStringLiteral("front_chest_width"), QStringLiteral("armfold_to_armfold_f"));

    name = QStringLiteral("shoulder_tip_to_shoulder_tip_b");
    names.insert(QStringLiteral("arc_behind_shoulder_girdle"), name);
    names.insert(QStringLiteral("across_back_shoulder_width"), name);

    names.insert(QStringLiteral("upper_back_width"), QStringLiteral("across_back_b"));
    names.insert(QStringLiteral("back_width"), QStringLiteral("armfold_to_armfold_b"));
    names.insert(QStringLiteral("neck_transverse_diameter"), QStringLiteral("neck_width"));
    names.insert(QStringLiteral("bustpoint_to_bustpoint"), QStringLiteral("bustpoint_to_bustpoint"));
    names.insert(QStringLiteral("neck_to_bustpoint"), QStringLiteral("bustpoint_to_neck_side"));
    names.insert(QStringLiteral("halter_bustpoint_to_bustpoint"), QStringLiteral("bustpoint_to_bustpoint_halter"));

    name = QStringLiteral("shoulder_tip_to_waist_front");
    names.insert(QStringLiteral("front_slash_shoulder_height"), name);
    names.insert(QStringLiteral("front_shoulder_slope_length"), name);

    names.insert(QStringLiteral("front_waist_slope"), QStringLiteral("neck_front_to_waist_side"));

    name = QStringLiteral("neck_side_to_armfold_f");
    names.insert(QStringLiteral("height_armhole_slash"), name);
    names.insert(QStringLiteral("chest_slope"), name);

    name = QStringLiteral("shoulder_tip_to_waist_back");
    names.insert(QStringLiteral("slash_shoulder_height"), name);
    names.insert(QStringLiteral("back_shoulder_slope_length"), name);

    names.insert(QStringLiteral("back_waist_slope"), QStringLiteral("neck_back_to_waist_side"));
    names.insert(QStringLiteral("back_slope"), QStringLiteral("neck_side_to_armfold_b"));
    names.insert(QStringLiteral("arm_length"), QStringLiteral("arm_shoulder_tip_to_wrist"));
    names.insert(QStringLiteral("shoulder_to_elbow_length"), QStringLiteral("arm_shoulder_tip_to_elbow"));
    names.insert(QStringLiteral("underarm_length"), QStringLiteral("arm_armpit_to_wrist"));
    names.insert(QStringLiteral("upper_arm_girth"), QStringLiteral("arm_upper_circ"));
    names.insert(QStringLiteral("wrist_girth"), QStringLiteral("arm_wrist_circ"));
    names.insert(QStringLiteral("armscye_girth"), QStringLiteral("armscye_circ"));
    names.insert(QStringLiteral("anteroposterior_diameter_hands"), QStringLiteral("armscye_width"));
    names.insert(QStringLiteral("neck_to_third_finger"), QStringLiteral("arm_neck_side_to_finger_tip"));
    names.insert(QStringLiteral("neck_to_radial_point"), QStringLiteral("arm_neck_side_to_outer_elbow"));
    names.insert(QStringLiteral("shoulder_and_arm_length"), QStringLiteral("arm_neck_side_to_wrist"));
    names.insert(QStringLiteral("crotch_height"), QStringLiteral("leg_crotch_to_floor"));
    names.insert(QStringLiteral("side_waist_to_floor"), QStringLiteral("leg_waist_side_to_floor"));
    names.insert(QStringLiteral("waist_to_knee"), QStringLiteral("leg_waist_side_to_knee"));
    names.insert(QStringLiteral("thigh_girth"), QStringLiteral("leg_thigh_upper_circ"));
    names.insert(QStringLiteral("mid_thigh_girth"), QStringLiteral("leg_thigh_mid_circ"));
    names.insert(QStringLiteral("knee_girth"), QStringLiteral("leg_knee_circ"));
    names.insert(QStringLiteral("calf_girth"), QStringLiteral("leg_calf_circ"));
    names.insert(QStringLiteral("ankle_girth"), QStringLiteral("leg_ankle_circ"));
    names.insert(QStringLiteral("girth_knee_flexed_feet"), QStringLiteral("leg_knee_circ_bent"));
    names.insert(QStringLiteral("arc_through_groin_area"), QStringLiteral("crotch_length"));
    names.insert(QStringLiteral("waist_to_plane_seat"), QStringLiteral("rise_length_side_sitting"));
    names.insert(QStringLiteral("rise_height"), QStringLiteral("rise_length_diag"));
    names.insert(QStringLiteral("hand_vertical_diameter"), QStringLiteral("hand_length"));
    names.insert(QStringLiteral("hand_width"), QStringLiteral("hand_palm_width"));
    names.insert(QStringLiteral("hand_girth"), QStringLiteral("hand_circ"));
    names.insert(QStringLiteral("girth_foot_instep"), QStringLiteral("foot_instep_circ"));
    names.insert(QStringLiteral("head_height"), QStringLiteral("head_length"));
    names.insert(QStringLiteral("head_and_neck_length"), QStringLiteral("head_crown_to_neck_back"));
    names.insert(QStringLiteral("neck_to_neck_base"), QStringLiteral("head_chin_to_neck_back"));
    names.insert(QStringLiteral("arc_length_upper_body"), QStringLiteral("waist_to_waist_halter"));
    names.insert(QStringLiteral("cervicale_to_wrist_length"), QStringLiteral("arm_neck_back_to_wrist_bent"));
    names.insert(QStringLiteral("strap_length"), QStringLiteral("highbust_b_over_shoulder_to_highbust_f"));
    names.insert(QStringLiteral("arc_through_shoulder_joint"), QStringLiteral("armscye_arc"));
    names.insert(QStringLiteral("head_girth"), QStringLiteral("head_circ"));
    names.insert(QStringLiteral("elbow_girth"), QStringLiteral("arm_elbow_circ"));
    names.insert(QStringLiteral("height_under_buttock_folds"), QStringLiteral("height_gluteal_fold"));
    names.insert(QStringLiteral("scye_depth"), QStringLiteral("neck_back_to_highbust_b"));
    names.insert(QStringLiteral("back_waist_to_upper_chest"), QStringLiteral("across_back_to_waist_b"));

    return names;
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> VPatternConverter::OldNamesToNewNames_InV0_2_1()
{
    // old name, new name
    QMap<QString, QString> names;

    names.insert(QStringLiteral("rise_length_side"), QStringLiteral("rise_length_side_sitting"));
    names.insert(QStringLiteral("size"), QStringLiteral("bust_arc_f"));

    return names;
}

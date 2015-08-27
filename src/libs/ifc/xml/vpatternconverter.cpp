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

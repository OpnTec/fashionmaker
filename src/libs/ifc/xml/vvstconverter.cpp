/************************************************************************
 **
 **  @file   VVSTConverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
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

#include "vvstconverter.h"

#include <QDomNode>
#include <QDomNodeList>
#include <QDomText>
#include <QFile>
#include <QLatin1String>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>

#include "../exception/vexception.h"
#include "../vmisc/def.h"
#include "vabstractmconverter.h"

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VVSTConverter::MeasurementMinVerStr = QStringLiteral("0.3.0");
const QString VVSTConverter::MeasurementMaxVerStr = QStringLiteral("0.4.4");
const QString VVSTConverter::CurrentSchema        = QStringLiteral("://schema/standard_measurements/v0.4.4.xsd");

//VVSTConverter::MeasurementMinVer; // <== DON'T FORGET TO UPDATE TOO!!!!
//VVSTConverter::MeasurementMaxVer; // <== DON'T FORGET TO UPDATE TOO!!!!

static const QString strTagRead_Only = QStringLiteral("read-only");

//---------------------------------------------------------------------------------------------------------------------
VVSTConverter::VVSTConverter(const QString &fileName)
    :VAbstractMConverter(fileName)
{
    ValidateInputFile(CurrentSchema);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::XSDSchema(int ver) const
{
    switch (ver)
    {
        case (0x000300):
            return QStringLiteral("://schema/standard_measurements/v0.3.0.xsd");
        case (0x000400):
            return QStringLiteral("://schema/standard_measurements/v0.4.0.xsd");
        case (0x000401):
            return QStringLiteral("://schema/standard_measurements/v0.4.1.xsd");
        case (0x000402):
            return QStringLiteral("://schema/standard_measurements/v0.4.2.xsd");
        case (0x000403):
            return QStringLiteral("://schema/standard_measurements/v0.4.3.xsd");
        case (0x000404):
            return CurrentSchema;
        default:
            InvalidVersion(ver);
            break;
    }
    return QString();//unreachable code
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ApplyPatches()
{
    switch (m_ver)
    {
        case (0x000300):
            ToV0_4_0();
            ValidateXML(XSDSchema(0x000400), m_convertedFileName);
            V_FALLTHROUGH
        case (0x000400):
            ToV0_4_1();
            ValidateXML(XSDSchema(0x000401), m_convertedFileName);
            V_FALLTHROUGH
        case (0x000401):
            ToV0_4_2();
            ValidateXML(XSDSchema(0x000402), m_convertedFileName);
            V_FALLTHROUGH
        case (0x000402):
            ToV0_4_3();
            ValidateXML(XSDSchema(0x000403), m_convertedFileName);
            V_FALLTHROUGH
        case (0x000403):
            ToV0_4_4();
            ValidateXML(XSDSchema(0x000404), m_convertedFileName);
            V_FALLTHROUGH
        case (0x000404):
            break;
        default:
            InvalidVersion(m_ver);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::DowngradeToCurrentMaxVersion()
{
    SetVersion(MeasurementMaxVerStr);
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
bool VVSTConverter::IsReadOnly() const
{
    // Check if attribute read-only was not changed in file format
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMaxVer == CONVERTER_VERSION_CHECK(0, 4, 4),
                      "Check attribute read-only.");

    // Possibly in future attribute read-only will change position etc.
    // For now position is the same for all supported format versions.
    // But don't forget to keep all versions of attribute until we support that format versions

    return UniqueTagText(strTagRead_Only, falseStr) == trueStr;
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::AddNewTagsForV0_4_0()
{
    // TODO. Delete if minimal supported version is 0.4.0
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                      "Time to refactor the code.");

    QDomElement rootElement = this->documentElement();
    QDomNode refChild = rootElement.firstChildElement("version");

    {
        QDomElement ro = createElement(QStringLiteral("read-only"));
        const QDomText roNodeText = createTextNode("false");
        ro.appendChild(roNodeText);
        refChild = rootElement.insertAfter(ro, refChild);
    }

    {
        QDomElement notes = createElement(QStringLiteral("notes"));
        const QDomText nodeText = createTextNode(UniqueTagText(QStringLiteral("description")));
        notes.appendChild(nodeText);
        rootElement.insertAfter(notes, refChild);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::RemoveTagsForV0_4_0()
{
    // TODO. Delete if minimal supported version is 0.4.0
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                      "Time to refactor the code.");

    QDomElement rootElement = this->documentElement();

    {
        const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("description"));
        if (not nodeList.isEmpty())
        {
            rootElement.removeChild(nodeList.at(0));
        }
    }

    {
        const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("id"));
        if (not nodeList.isEmpty())
        {
            rootElement.removeChild(nodeList.at(0));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ConvertMeasurementsToV0_4_0()
{
    // TODO. Delete if minimal supported version is 0.4.0
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                      "Time to refactor the code.");

    const QString tagBM = QStringLiteral("body-measurements");

    QDomElement bm = createElement(tagBM);

    const QMultiMap<QString, QString> names = OldNamesToNewNames_InV0_3_0();
    const QList<QString> keys = names.uniqueKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        qreal resValue = 0;
        qreal resSizeIncrease = 0;
        qreal resHeightIncrease = 0;

        // This has the same effect as a .values(), just isn't as elegant
        const QList<QString> list = names.values( keys.at(i) );
        foreach(const QString &val, list )
        {
            const QDomNodeList nodeList = this->elementsByTagName(val);
            if (nodeList.isEmpty())
            {
                continue;
            }

            QDomElement m = nodeList.at(0).toElement();
            const qreal value = GetParametrDouble(m, QStringLiteral("value"), "0.0");
            const qreal size_increase = GetParametrDouble(m, QStringLiteral("size_increase"), "0.0");
            const qreal height_increase = GetParametrDouble(m, QStringLiteral("height_increase"), "0.0");

            if (not qFuzzyIsNull(value))
            {
                resValue = value;
                resSizeIncrease = size_increase;
                resHeightIncrease = height_increase;
            }
        }

        bm.appendChild(AddMV0_4_0(keys.at(i), resValue, resSizeIncrease, resHeightIncrease));
    }

    QDomElement rootElement = this->documentElement();
    const QDomNodeList listBM = elementsByTagName(tagBM);
    rootElement.replaceChild(bm, listBM.at(0));
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VVSTConverter::AddMV0_4_0(const QString &name, qreal value, qreal sizeIncrease, qreal heightIncrease)
{
    // TODO. Delete if minimal supported version is 0.4.0
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                      "Time to refactor the code.");

    QDomElement element = createElement(QStringLiteral("m"));

    SetAttribute(element, QStringLiteral("name"), name);
    SetAttribute(element, QStringLiteral("base"), QString().setNum(value));
    SetAttribute(element, QStringLiteral("size_increase"), QString().setNum(sizeIncrease));
    SetAttribute(element, QStringLiteral("height_increase"), QString().setNum(heightIncrease));
    SetAttribute(element, QStringLiteral("description"), QString(""));
    SetAttribute(element, QStringLiteral("full_name"), QString(""));

    return element;
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::PM_SystemV0_4_1()
{
    // TODO. Delete if minimal supported version is 0.4.1
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 1),
                      "Time to refactor the code.");

    QDomElement pm_system = createElement(QStringLiteral("pm_system"));
    pm_system.appendChild(createTextNode(QStringLiteral("998")));

    const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("size"));
    QDomElement personal = nodeList.at(0).toElement();

    QDomElement parent = personal.parentNode().toElement();
    parent.insertBefore(pm_system, personal);
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ConvertMeasurementsToV0_4_2()
{
    // TODO. Delete if minimal supported version is 0.4.2
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 2),
                      "Time to refactor the code.");

    const QMap<QString, QString> names = OldNamesToNewNames_InV0_3_3();
    auto i = names.constBegin();
    while (i != names.constEnd())
    {
        const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("m"));
        if (nodeList.isEmpty())
        {
            ++i;
            continue;
        }

        for (int ii = 0; ii < nodeList.size(); ++ii)
        {
            const QString attrName = QStringLiteral("name");
            QDomElement element = nodeList.at(ii).toElement();
            const QString name = GetParametrString(element, attrName);
            if (name == i.value())
            {
                SetAttribute(element, attrName, i.key());
            }
        }

        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_0()
{
    // TODO. Delete if minimal supported version is 0.4.0
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 0),
                      "Time to refactor the code.");

    AddRootComment();
    SetVersion(QStringLiteral("0.4.0"));
    AddNewTagsForV0_4_0();
    RemoveTagsForV0_4_0();
    ConvertMeasurementsToV0_4_0();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_1()
{
    // TODO. Delete if minimal supported version is 0.4.1
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 1),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.4.1"));
    PM_SystemV0_4_1();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_2()
{
    // TODO. Delete if minimal supported version is 0.4.2
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 2),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.4.2"));
    ConvertMeasurementsToV0_4_2();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_3()
{
    // TODO. Delete if minimal supported version is 0.4.3
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 3),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.4.3"));
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ToV0_4_4()
{
    // TODO. Delete if minimal supported version is 0.4.4
    Q_STATIC_ASSERT_X(VVSTConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 4, 4),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.4.4"));
    Save();
}

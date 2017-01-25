/************************************************************************
 **
 **  @file   vvitconverter.cpp
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

#include "vvitconverter.h"

#include <QDomNode>
#include <QDomNodeList>
#include <QDomText>
#include <QFile>
#include <QForeachContainer>
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

const QString VVITConverter::MeasurementMinVerStr = QStringLiteral("0.2.0");
const QString VVITConverter::MeasurementMaxVerStr = QStringLiteral("0.3.3");
const QString VVITConverter::CurrentSchema        = QStringLiteral("://schema/individual_measurements/v0.3.3.xsd");

//VVITConverter::MeasurementMinVer; // <== DON'T FORGET TO UPDATE TOO!!!!
//VVITConverter::MeasurementMaxVer; // <== DON'T FORGET TO UPDATE TOO!!!!

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::VVITConverter(const QString &fileName)
    :VAbstractMConverter(fileName)
{
    ValidateInputFile(CurrentSchema);
}

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::~VVITConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::XSDSchema(int ver) const
{
    switch (ver)
    {
        case (0x000200):
            return QStringLiteral("://schema/individual_measurements/v0.2.0.xsd");
        case (0x000300):
            return QStringLiteral("://schema/individual_measurements/v0.3.0.xsd");
        case (0x000301):
            return QStringLiteral("://schema/individual_measurements/v0.3.1.xsd");
        case (0x000302):
            return QStringLiteral("://schema/individual_measurements/v0.3.2.xsd");
        case (0x000303):
            return CurrentSchema;
        default:
            InvalidVersion(ver);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000200):
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
void VVITConverter::DowngradeToCurrentMaxVersion()
{
    SetVersion(MeasurementMaxVerStr);
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::AddNewTagsForV0_3_0()
{
    // TODO. Delete if minimal supported version is 0.3.0
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 0),
                      "Time to refactor the code.");

    QDomElement rootElement = this->documentElement();
    QDomNode refChild = rootElement.firstChildElement("version");

    QDomElement ro = createElement(QStringLiteral("read-only"));
    const QDomText roNodeText = createTextNode("false");
    ro.appendChild(roNodeText);
    refChild = rootElement.insertAfter(ro, refChild);

    refChild = rootElement.insertAfter(createElement(QStringLiteral("notes")), refChild);

    QDomElement unit = createElement("unit");
    unit.appendChild(createTextNode(MUnitV0_2_0()));
    rootElement.insertAfter(unit, refChild);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::MUnitV0_2_0()
{
    // TODO. Delete if minimal supported version is 0.3.0
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 0),
                      "Time to refactor the code.");

    return UniqueTagText(QStringLiteral("unit"), QStringLiteral("cm"));
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ConvertMeasurementsToV0_3_0()
{
    // TODO. Delete if minimal supported version is 0.3.0
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 0),
                      "Time to refactor the code.");

    const QString tagBM = QStringLiteral("body-measurements");

    QDomElement bm = createElement(tagBM);

    const QMultiMap<QString, QString> names = OldNamesToNewNames_InV0_3_0();
    const QList<QString> keys = names.uniqueKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        qreal resValue = 0;

        // This has the same effect as a .values(), just isn't as elegant
        const QList<QString> list = names.values( keys.at(i) );
        foreach(const QString &val, list )
        {
            const QDomNodeList nodeList = this->elementsByTagName(val);
            if (nodeList.isEmpty())
            {
                continue;
            }

            const qreal value = GetParametrDouble(nodeList.at(0).toElement(), QStringLiteral("value"), "0.0");

            if (not qFuzzyIsNull(value))
            {
                resValue = value;
            }
        }

        bm.appendChild(AddMV0_3_0(keys.at(i), resValue));
    }

    QDomElement rootElement = this->documentElement();
    const QDomNodeList listBM = elementsByTagName(tagBM);
    rootElement.replaceChild(bm, listBM.at(0));
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VVITConverter::AddMV0_3_0(const QString &name, qreal value)
{
    // TODO. Delete if minimal supported version is 0.3.0
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 0),
                      "Time to refactor the code.");

    QDomElement element = createElement(QStringLiteral("m"));

    SetAttribute(element, QStringLiteral("name"), name);
    SetAttribute(element, QStringLiteral("value"), QString().setNum(value));
    SetAttribute(element, QStringLiteral("description"), QString(""));
    SetAttribute(element, QStringLiteral("full_name"), QString(""));

    return element;
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::GenderV0_3_1()
{
    // TODO. Delete if minimal supported version is 0.3.1
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 1),
                      "Time to refactor the code.");

    const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("sex"));
    QDomElement sex = nodeList.at(0).toElement();

    QDomElement gender = createElement(QStringLiteral("gender"));
    gender.appendChild(createTextNode(sex.text()));

    QDomElement parent = sex.parentNode().toElement();
    parent.replaceChild(gender, sex);
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::PM_SystemV0_3_2()
{
    // TODO. Delete if minimal supported version is 0.3.2
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 2),
                      "Time to refactor the code.");

    QDomElement pm_system = createElement(QStringLiteral("pm_system"));
    pm_system.appendChild(createTextNode(QStringLiteral("998")));

    const QDomNodeList nodeList = this->elementsByTagName(QStringLiteral("personal"));
    QDomElement personal = nodeList.at(0).toElement();

    QDomElement parent = personal.parentNode().toElement();
    parent.insertBefore(pm_system, personal);
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ConvertMeasurementsToV0_3_3()
{
    // TODO. Delete if minimal supported version is 0.3.3
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 3),
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
void VVITConverter::ToV0_3_0()
{
    // TODO. Delete if minimal supported version is 0.3.0
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 0),
                      "Time to refactor the code.");

    AddRootComment();
    SetVersion(QStringLiteral("0.3.0"));
    AddNewTagsForV0_3_0();
    ConvertMeasurementsToV0_3_0();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ToV0_3_1()
{
    // TODO. Delete if minimal supported version is 0.3.1
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 1),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.3.1"));
    GenderV0_3_1();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ToV0_3_2()
{
    // TODO. Delete if minimal supported version is 0.3.2
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 2),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.3.2"));
    PM_SystemV0_3_2();
    Save();
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ToV0_3_3()
{
    // TODO. Delete if minimal supported version is 0.3.3
    Q_STATIC_ASSERT_X(VVITConverter::MeasurementMinVer < CONVERTER_VERSION_CHECK(0, 3, 3),
                      "Time to refactor the code.");

    SetVersion(QStringLiteral("0.3.3"));
    ConvertMeasurementsToV0_3_3();
    Save();
}

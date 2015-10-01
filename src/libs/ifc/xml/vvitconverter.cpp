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
#include "exception/vexception.h"

#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VVITConverter::MeasurementMinVerStr = QStringLiteral("0.2.0");
const QString VVITConverter::MeasurementMaxVerStr = QStringLiteral("0.3.0");
const QString VVITConverter::CurrentSchema        = QStringLiteral("://schema/individual_measurements/v0.3.0.xsd");

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::VVITConverter(const QString &fileName)
    :VAbstractMConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VVITConverter::~VVITConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VVITConverter::MinVer() const
{
    return GetVersion(MeasurementMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VVITConverter::MaxVer() const
{
    return GetVersion(MeasurementMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVITConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000200):
            return QStringLiteral("://schema/individual_measurements/v0.2.0.xsd");
        case (0x000300):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
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
            {
                ToV0_3_0();
                const QString schema = XSDSchema(0x000300);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000300):
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
void VVITConverter::AddNewTagsForV0_3_0()
{
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
    return UniqueTagText(QStringLiteral("unit"), QStringLiteral("cm"));
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ConvertMeasurementsToV0_3_0()
{
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
    QDomElement element = createElement(QStringLiteral("m"));

    SetAttribute(element, QStringLiteral("name"), name);
    SetAttribute(element, QStringLiteral("value"), QString().setNum(value));
    SetAttribute(element, QStringLiteral("description"), QString(""));
    SetAttribute(element, QStringLiteral("full_name"), QString(""));

    return element;
}

//---------------------------------------------------------------------------------------------------------------------
void VVITConverter::ToV0_3_0()
{
    AddRootComment();
    SetVersion(QStringLiteral("0.3.0"));
    AddNewTagsForV0_3_0();
    ConvertMeasurementsToV0_3_0();
    Save();
}

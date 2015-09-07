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
#include "exception/vexception.h"

#include <QFile>

/*
 * Version rules:
 * 1. Version have three parts "major.minor.patch";
 * 2. major part only for stable releases;
 * 3. minor - 10 or more patch changes, or one big change;
 * 4. patch - little change.
 */

const QString VVSTConverter::MeasurementMinVerStr = QStringLiteral("0.3.0");
const QString VVSTConverter::MeasurementMaxVerStr = QStringLiteral("0.4.0");
const QString VVSTConverter::CurrentSchema        = QStringLiteral("://schema/standard_measurements/v0.4.0.xsd");

//---------------------------------------------------------------------------------------------------------------------
VVSTConverter::VVSTConverter(const QString &fileName)
    :VAbstractMConverter(fileName)
{
    const QString schema = XSDSchema(ver);
    ValidateXML(schema, fileName);
}

//---------------------------------------------------------------------------------------------------------------------
VVSTConverter::~VVSTConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
int VVSTConverter::MinVer() const
{
    return GetVersion(MeasurementMinVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
int VVSTConverter::MaxVer() const
{
    return GetVersion(MeasurementMaxVerStr);
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVSTConverter::XSDSchema(int ver) const
{
    CheckVersion(ver);

    switch (ver)
    {
        case (0x000300):
            return QStringLiteral("://schema/standard_measurements/v0.3.0.xsd");
        case (0x000400):
            return CurrentSchema;
        default:
        {
            const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVSTConverter::ApplyPatches()
{
    try
    {
        switch (ver)
        {
            case (0x000300):
            {
                ToV0_4_0();
                const QString schema = XSDSchema(0x000400);
                ValidateXML(schema, fileName);
                V_FALLTHROUGH
            }
            case (0x000400):
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
void VVSTConverter::AddNewTagsForV0_4_0()
{
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
    const QString tagBM = QStringLiteral("body-measurements");

    QDomElement bm = createElement(tagBM);

    QMultiMap<QString, QString> names = OldNamesToNewNames_InV0_3_0();

    QMutableMapIterator<QString, QString> iter( names );
    while( iter.hasNext() )
    {
        iter.next();

        qreal resValue = 0;
        qreal resSizeIncrease = 0;
        qreal resHeightIncrease = 0;

        // This has the same effect as a .values(), just isn't as elegant
        const QList<QString> list = names.values( iter.key() );
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

        bm.appendChild(AddMV0_4_0(iter.key(), resValue, resSizeIncrease, resHeightIncrease));
    }

    QDomElement rootElement = this->documentElement();
    const QDomNodeList listBM = elementsByTagName(tagBM);
    rootElement.replaceChild(bm, listBM.at(0));
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VVSTConverter::AddMV0_4_0(const QString &name, qreal value, qreal sizeIncrease, qreal heightIncrease)
{
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
void VVSTConverter::ToV0_4_0()
{
    AddRootComment();
    SetVersion(QStringLiteral("0.4.0"));
    AddNewTagsForV0_4_0();
    RemoveTagsForV0_4_0();
    ConvertMeasurementsToV0_4_0();
    Save();
}

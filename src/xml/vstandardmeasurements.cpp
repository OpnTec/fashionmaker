/************************************************************************
 **
 **  @file   vstandardmeasurements.cpp
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

#include "vstandardmeasurements.h"
#include <QDebug>

const QString VStandardMeasurements::TagMeasurement      = QStringLiteral("measurement");
const QString VStandardMeasurements::TagDescription      = QStringLiteral("description");
const QString VStandardMeasurements::TagSize             = QStringLiteral("size");
const QString VStandardMeasurements::TagHeight           = QStringLiteral("height");
const QString VStandardMeasurements::AttrName            = QStringLiteral("name");
const QString VStandardMeasurements::AttrGui_text        = QStringLiteral("gui_text");
const QString VStandardMeasurements::AttrBase            = QStringLiteral("base");
const QString VStandardMeasurements::AttrSize_increace   = QStringLiteral("size_increace");
const QString VStandardMeasurements::AttrHeight_increase = QStringLiteral("height_increase");
const QString VStandardMeasurements::AttrNumber          = QStringLiteral("number");

VStandardMeasurements::VStandardMeasurements(VContainer *data):VDomDocument(data)
{
}

Valentina::Units VStandardMeasurements::Unit()
{
    const QString unit = UniqueTagText(AttrUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

QString VStandardMeasurements::Description()
{
    const QString desc = UniqueTagText(TagDescription, "");
    if (desc.isEmpty())
    {
        qDebug()<<"Empty description in standard table."<<Q_FUNC_INFO;
    }
    return desc;
}

void VStandardMeasurements::Measurements()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagMeasurement);
    if (nodeList.isEmpty())
    {
        qDebug()<<"Measurement list is empty"<<Q_FUNC_INFO;
        return;
    }
    else
    {
        for(qint32 i = 0; i < nodeList.size(); ++i)
        {
            const QDomNode domNode = nodeList.at(i);
            if (domNode.isNull() == false && domNode.isElement())
            {
                const QDomElement domElement = domNode.toElement();
                if (domElement.isNull() == false)
                {
                    const QString name = GetParametrString(domElement, AttrName, "");
                    if (name.isEmpty())
                    {
                        continue;
                    }
                    const QString gui_text = GetParametrString(domElement, AttrGui_text, "");
                    const qreal base = GetParametrDouble(domElement, AttrBase, "0.0");
                    const qreal size_increace = GetParametrDouble(domElement, AttrSize_increace, "0.0");
                    const qreal height_increase = GetParametrDouble(domElement, AttrHeight_increase, "0.0");
                    const QString number = GetParametrString(domElement, AttrNumber, "");

                    if (Unit() == Valentina::Mm)// Convert to Cm.
                    {
                        data->AddMeasurement(name, VMeasurement(base/10.0, size_increace/10.0, height_increase/10.0,
                                                                gui_text, number));
                    }
                    else// Cm or inch.
                    {
                        data->AddMeasurement(name, VMeasurement(base, size_increace, height_increase,
                                                                          gui_text, number));
                    }
                }
            }
        }
    }
}

void VStandardMeasurements::SetSize()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagSize);
    if (nodeList.isEmpty())
    {
        data->SetSize(50);
        data->SetSizeName("Сг");
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                const QString name = GetParametrString(domElement, AttrName, "Сг");
                const qreal base = GetParametrDouble(domElement, AttrBase, "50.0");
                if (Unit() == Valentina::Mm)// Convert to Cm.
                {
                    data->SetSize(base/10.0);
                    data->SetSizeName(name);
                }
                else// Cm or inch.
                {
                    data->SetSize(base);
                    data->SetSizeName(name);
                }
            }
        }
    }
}

void VStandardMeasurements::SetHeight()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagHeight);
    if (nodeList.isEmpty())
    {
        data->SetHeight(176);
        data->SetHeightName("P");
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                const QString name = GetParametrString(domElement, AttrName, "Р");
                const qreal base = GetParametrDouble(domElement, AttrBase, "176.0");
                if (Unit() == Valentina::Mm)// Convert to Cm.
                {
                    data->SetHeight(base/10.0);
                    data->SetHeightName(name);
                }
                else// Cm or inch.
                {
                    data->SetHeight(base);
                    data->SetHeightName(name);
                }
            }
        }
    }
}

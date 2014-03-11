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

VStandardMeasurements::VStandardMeasurements(VContainer *data):VDomDocument(data)
{
}

Valentina::Units VStandardMeasurements::Unit()
{
    const QString unit = UniqueTagText("unit", "cm");
    QStringList units;
    units << "mm" << "cm" << "in";
    Valentina::Units result = Valentina::Cm;
    switch (units.indexOf(unit))
    {
        case 0:// mm
            result = Valentina::Mm;
            break;
        case 1:// cm
            result = Valentina::Cm;
            break;
        case 2:// in
            result = Valentina::In;
            break;
        default:
            result = Valentina::Cm;
            break;
    }
    return result;
}

QString VStandardMeasurements::Description()
{
    const QString desc = UniqueTagText("description", "");
    if (desc.isEmpty())
    {
        qWarning()<<"Empty description in standard table."<<Q_FUNC_INFO;
    }
    return desc;
}

void VStandardMeasurements::Measurements()
{
    const QDomNodeList nodeList = this->elementsByTagName("measurement");
    if (nodeList.isEmpty())
    {
        qWarning()<<"Measurement list is empty"<<Q_FUNC_INFO;
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
                    const QString name = GetParametrString(domElement, "name", "");
                    if (name.isEmpty())
                    {
                        continue;
                    }
                    const QString gui_text = GetParametrString(domElement, "gui_text", "");
                    const qreal base = GetParametrDouble(domElement, "base", "0.0");
                    const qreal size_increace = GetParametrDouble(domElement, "size_increace", "0.0");
                    const qreal height_increase = GetParametrDouble(domElement, "height_increase", "0.0");
                    const QString number = GetParametrString(domElement, "number", "");

                    if (Unit() == Valentina::Mm)//Convert to Cm.
                    {
                        data->AddStandardTableRow(name, VStandardTableRow(base/10.0, size_increace/10.0,
                                                                          height_increase/10.0, gui_text, number));
                    }
                    else//Cm or inch.
                    {
                        data->AddStandardTableRow(name, VStandardTableRow(base, size_increace, height_increase,
                                                                          gui_text, number));
                    }
                }
            }
        }
    }
}

void VStandardMeasurements::SetSize()
{
    const QDomNodeList nodeList = this->elementsByTagName("size");
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
                const QString name = GetParametrString(domElement, "name", "Сг");
                const qreal base = GetParametrDouble(domElement, "base", "50.0");
                if (Unit() == Valentina::Mm)//Convert to Cm.
                {
                    data->SetSize(base/10.0);
                    data->SetSizeName(name);
                }
                else//Cm or inch.
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
    const QDomNodeList nodeList = this->elementsByTagName("height");
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
                const QString name = GetParametrString(domElement, "name", "Р");
                const qreal base = GetParametrDouble(domElement, "base", "176.0");
                if (Unit() == Valentina::Mm)//Convert to Cm.
                {
                    data->SetHeight(base/10.0);
                    data->SetHeightName(name);
                }
                else//Cm or inch.
                {
                    data->SetHeight(base);
                    data->SetHeightName(name);
                }
            }
        }
    }
}

void VStandardMeasurements::Update()
{
    Measurements();
}

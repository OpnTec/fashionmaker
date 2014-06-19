/************************************************************************
 **
 **  @file   vstandardmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../widgets/vapplication.h"

const QString VStandardMeasurements::TagDescription      = QStringLiteral("description");
const QString VStandardMeasurements::TagSize             = QStringLiteral("size");
const QString VStandardMeasurements::TagHeight           = QStringLiteral("height");
const QString VStandardMeasurements::AttrSize_increase   = QStringLiteral("size_increase");
const QString VStandardMeasurements::AttrHeight_increase = QStringLiteral("height_increase");

//---------------------------------------------------------------------------------------------------------------------
VStandardMeasurements::VStandardMeasurements(VContainer *data)
    :VAbstractMeasurements(data)
{
    SCASSERT(data != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
VStandardMeasurements::~VStandardMeasurements()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VStandardMeasurements::Description()
{
    const QString desc = UniqueTagText(TagDescription, "");
    if (desc.isEmpty())
    {
        qDebug()<<"Empty description in standard table."<<Q_FUNC_INFO;
    }
    return desc;
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::ReadMeasurement(const QDomElement &domElement, const QString &tag)
{
    const qreal value = GetParametrDouble(domElement, AttrValue, "0.0");
    const qreal size_increase = GetParametrDouble(domElement, AttrSize_increase, "0.0");
    const qreal height_increase = GetParametrDouble(domElement, AttrHeight_increase, "0.0");

    if (MUnit() == Unit::Mm)// Convert to Cm.
    {
        data->AddMeasurement(tag, VMeasurement(value/10.0, size_increase/10.0, height_increase/10.0,
                                               qApp->GuiText(tag), qApp->Description(tag), tag));
    }
    else// Cm or inch.
    {
        data->AddMeasurement(tag, VMeasurement(value, size_increase, height_increase, qApp->GuiText(tag),
                                               qApp->Description(tag), tag));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetSize()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagSize);
    if (nodeList.isEmpty())
    {
        data->SetSize(50);
        data->SetSizeName(size_M);
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, AttrValue, "50.0");
                if (MUnit() == Unit::Mm)// Convert to Cm.
                {
                    value = value/10.0;
                }
                data->SetSize(value);
                data->SetSizeName(size_M);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetHeight()
{
    const QDomNodeList nodeList = this->elementsByTagName(TagHeight);
    if (nodeList.isEmpty())
    {
        data->SetHeight(176);
        data->SetHeightName(height_M);
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, AttrValue, "176.0");
                if (MUnit() == Unit::Mm)// Convert to Cm.
                {
                    value = value / 10.0;
                }
                data->SetHeight(value);
                data->SetHeightName(height_M);
            }
        }
    }
}

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

#include "vstandardmeasurements.h"
#include <QDebug>
#include "../core/vapplication.h"

const QString VStandardMeasurements::TagDescription      = QStringLiteral("description");
const QString VStandardMeasurements::TagId               = QStringLiteral("id");
const QString VStandardMeasurements::TagSize             = QStringLiteral("size");
const QString VStandardMeasurements::TagHeight           = QStringLiteral("height");

const QString VStandardMeasurements::AttrSize_increase   = QStringLiteral("size_increase");
const QString VStandardMeasurements::AttrHeight_increase = QStringLiteral("height_increase");
const QString VStandardMeasurements::AttrBase            = QStringLiteral("base");

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
QString VStandardMeasurements::OrigDescription ()
{
    const QString desc = UniqueTagText(TagDescription, "");
    if (desc.isEmpty())
    {
        qWarning()<<"Empty description in standard table."<<Q_FUNC_INFO;
    }
    return desc;
}

//---------------------------------------------------------------------------------------------------------------------
QString VStandardMeasurements::TrDescription()
{
    const QString trDesc = qApp->STDescription(Id());
    if (trDesc.isEmpty() == false)
    {
        return trDesc;
    }
    else
    {
        return OrigDescription ();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VStandardMeasurements::Id()
{
    const QString id = UniqueTagText(TagId, "");
    if (id.isEmpty())
    {
        qWarning()<<"Empty id value in standard table."<<Q_FUNC_INFO;
    }
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VStandardMeasurements::Size() const
{
    return TakeParametr(TagSize, AttrBase, 50);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VStandardMeasurements::Height() const
{
    return TakeParametr(TagHeight, AttrBase, 176);
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::ReadMeasurement(const QDomElement &domElement, const QString &tag)
{
    qreal value = GetParametrDouble(domElement, AttrValue, "0.0");
    value = UnitConvertor(value, MUnit(), qApp->patternUnit());

    qreal size_increase = GetParametrDouble(domElement, AttrSize_increase, "0.0");
    size_increase = UnitConvertor(size_increase, MUnit(), qApp->patternUnit());

    qreal height_increase = GetParametrDouble(domElement, AttrHeight_increase, "0.0");
    height_increase = UnitConvertor(height_increase, MUnit(), qApp->patternUnit());

    if (MUnit() == Unit::Inch)
    {
        qWarning()<<"Standard table can't use inch unit.";
    }

    data->AddVariable(tag, new VMeasurement(tag, value, size_increase, height_increase, qApp->GuiText(tag),
                                            qApp->Description(tag), tag));
}


//---------------------------------------------------------------------------------------------------------------------
qreal VStandardMeasurements::TakeParametr(const QString &tag, const QString &attr, qreal defValue) const
{
    const qreal defVal = UnitConvertor(defValue, Unit::Cm, qApp->patternUnit());

    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        return defVal;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                qreal value = GetParametrDouble(domElement, attr, QString("%1").arg(defVal));
                value = UnitConvertor(value, MUnit(), qApp->patternUnit());
                return value;
            }
        }
    }
    return defVal;
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetSize()
{
    data->SetSize(Size());
    data->SetSizeName(size_M);
}

//---------------------------------------------------------------------------------------------------------------------
void VStandardMeasurements::SetHeight()
{
    data->SetHeight(Height());
    data->SetHeightName(height_M);
}

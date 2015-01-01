/************************************************************************
 **
 **  @file   vstandardtablecell.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vmeasurement.h"
#include "vmeasurement_p.h"
#include "../core/vapplication.h"
#include "../xml/vabstractmeasurements.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMeasurement create empty measurement
 */
VMeasurement::VMeasurement()
    :VVariable(), d(new VMeasurementData)
{
    SetType(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMeasurement create measurement for standard table
 * @param name measurement's name
 * @param base value in base size and height
 * @param ksize increment in sizes
 * @param kheight increment in heights
 * @param gui_text shor tooltip for user
 * @param description measurement full description
 * @param tagName measurement's tag name in file
 */
VMeasurement::VMeasurement(const QString &name, const qreal &base, const qreal &ksize, const qreal &kheight,
                           const QString &gui_text, const QString &description, const QString &tagName)
    :VVariable(name, base, ksize, kheight, description), d(new VMeasurementData(gui_text, tagName))
{
    SetType(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VMeasurement create measurement for individual table
 * @param name measurement's name
 * @param base value in base size and height
 * @param gui_text shor tooltip for user
 * @param description measurement full description
 * @param tagName measurement's tag name in file
 */
VMeasurement::VMeasurement(const QString &name, const qreal &base, const QString &gui_text, const QString &description,
                           const QString &tagName)
    :VVariable(name, base, description), d(new VMeasurementData(gui_text, tagName))
{
    SetType(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::VMeasurement(const VMeasurement &m)
    :VVariable(m), d(m.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement &VMeasurement::operator=(const VMeasurement &m)
{
    if ( &m == this )
    {
        return *this;
    }
    VVariable::operator=(m);
    d = m.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurement::~VMeasurement()
{}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurement::ListHeights(QMap<GHeights, bool> heights)
{
    QStringList list;
    if (qApp->patternUnit() == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    QMap<GHeights, bool>::const_iterator i = heights.constBegin();
    while (i != heights.constEnd())
    {
        if (i.value() && i.key() != GHeights::ALL)
        {
            ListValue(list, static_cast<int>(i.key()));
        }
        ++i;
    }

    if (list.isEmpty())
    {
        // from 92 cm to 194 cm
        for (int i = 92; i<= 194; i = i+6)
        {
            ListValue(list, i);
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurement::ListSizes(QMap<GSizes, bool> sizes)
{
    QStringList list;
    if (qApp->patternUnit() == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    QMap<GSizes, bool>::const_iterator i = sizes.constBegin();
    while (i != sizes.constEnd())
    {
        if (i.value() && i.key() != GSizes::ALL)
        {
            ListValue(list, static_cast<int>(i.key()));
        }
        ++i;
    }

    if (list.isEmpty())
    {
        // from 22 cm to 56 cm
        for (int i = 22; i<= 56; i = i+2)
        {
           ListValue(list, i);
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurement::ListValue(QStringList &list, qreal value)
{
    qreal val = VAbstractMeasurements::UnitConvertor(value, Unit::Cm, qApp->patternUnit());
    QString strVal = QString("%1").arg(val);
    list.append(strVal);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetGuiText measurement name for tooltip
 * @return measurement name
 */
QString VMeasurement::GetGuiText() const
{
    return d->gui_text;
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurement::TagName() const
{
    return d->_tagName;
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurement::setTagName(const QString &tagName)
{
    d->_tagName = tagName;
}

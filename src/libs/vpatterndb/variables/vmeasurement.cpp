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

#include "vmeasurement.h"

#include <QMap>
#include <QMessageLogger>
#include <QtDebug>

#include "../ifc/ifcdef.h"
#include "vvariable.h"
#include "vmeasurement_p.h"

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
VMeasurement::VMeasurement(quint32 index, const QString &name, qreal baseSize, qreal baseHeight, const qreal &base,
                           const qreal &ksize, const qreal &kheight, const QString &gui_text,
                           const QString &description, const QString &tagName)
    :VVariable(name, baseSize, baseHeight, base, ksize, kheight, description),
      d(new VMeasurementData(index, gui_text, tagName))
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
VMeasurement::VMeasurement(VContainer *data, quint32 index, const QString &name, const qreal &base,
                           const QString &formula, bool ok, const QString &gui_text, const QString &description,
                           const QString &tagName)
    :VVariable(name, base, description), d(new VMeasurementData(data, index, formula, ok, gui_text, tagName))
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
QStringList VMeasurement::ListHeights(QMap<GHeights, bool> heights, Unit patternUnit)
{
    QStringList list;
    if (patternUnit == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    QMap<GHeights, bool>::const_iterator i = heights.constBegin();
    while (i != heights.constEnd())
    {
        if (i.value() && i.key() != GHeights::ALL)
        {
            ListValue(list, static_cast<int>(i.key()), patternUnit);
        }
        ++i;
    }

    if (list.isEmpty())
    {
        list = VMeasurement::WholeListHeights(patternUnit);
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurement::ListSizes(QMap<GSizes, bool> sizes, Unit patternUnit)
{
    QStringList list;
    if (patternUnit == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    QMap<GSizes, bool>::const_iterator i = sizes.constBegin();
    while (i != sizes.constEnd())
    {
        if (i.value() && i.key() != GSizes::ALL)
        {
            ListValue(list, static_cast<int>(i.key()), patternUnit);
        }
        ++i;
    }

    if (list.isEmpty())
    {
        list = VMeasurement::WholeListSizes(patternUnit);
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurement::WholeListHeights(Unit patternUnit)
{
    QStringList list;
    if (patternUnit == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    for (int i = static_cast<int>(GHeights::H50); i<= static_cast<int>(GHeights::H200); i = i+heightStep)
    {
        ListValue(list, i, patternUnit);
    }

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurement::WholeListSizes(Unit patternUnit)
{
    QStringList list;
    if (patternUnit == Unit::Inch)
    {
        qWarning()<<"Standard table doesn't support inches.";
        return list;
    }

    for (int i = static_cast<int>(GSizes::S22); i<= static_cast<int>(GSizes::S72); i = i+sizeStep)
    {
       ListValue(list, i, patternUnit);
    }

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurement::IsGradationSizeValid(const QString &size)
{
    if (not size.isEmpty())
    {
        const QStringList sizes = VMeasurement::WholeListSizes(Unit::Cm);
        if (sizes.contains(size))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurement::IsGradationHeightValid(const QString &height)
{
    if (not height.isEmpty())
    {
        const QStringList heights = VMeasurement::WholeListHeights(Unit::Cm);
        if (heights.contains(height))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurement::ListValue(QStringList &list, qreal value, Unit patternUnit)
{
    const qreal val = UnitConvertor(value, Unit::Cm, patternUnit);
    const QString strVal = QString("%1").arg(val);
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

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurement::GetFormula() const
{
    return d->formula;
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurement::IsCustom() const
{
    if (GetName().indexOf(CustomMSign) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VMeasurement::Index() const
{
    return static_cast<int>(d->index);
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurement::IsFormulaOk() const
{
    return d->formulaOk;
}

//---------------------------------------------------------------------------------------------------------------------
VContainer *VMeasurement::GetData()
{
    return &d->data;
}

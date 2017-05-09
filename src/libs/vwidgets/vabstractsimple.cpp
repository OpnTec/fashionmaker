/************************************************************************
 **
 **  @file   vabstractsimple.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
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

#include "vabstractsimple.h"

const qreal VAbstractSimple::m_defFactor = 1;

//---------------------------------------------------------------------------------------------------------------------
VAbstractSimple::VAbstractSimple(quint32 id, const QColor &currentColor, Unit patternUnit, qreal *factor,
                                 QObject *parent)
    : QObject(parent),
      id (id),
      factor(factor),
      currentColor(currentColor),
      enabled(true),
      patternUnit(patternUnit),
      selectionType(SelectionType::ByMouseRelease),
      type(GOType::Unknown)
{
    if (this->factor == nullptr)
    {
        this->factor = const_cast<qreal *>(&m_defFactor);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractSimple::~VAbstractSimple()
{}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSimple::ToolSelectionType(const SelectionType &type)
{
    selectionType = type;
}

//---------------------------------------------------------------------------------------------------------------------
QColor VAbstractSimple::CorrectColor(const QColor &color) const
{
    if (enabled)
    {
        return color;
    }
    else
    {
        return Qt::gray;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QColor VAbstractSimple::GetCurrentColor() const
{
    return currentColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSimple::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSimple::ContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
GOType VAbstractSimple::GetType() const
{
    return type;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractSimple::SetType(const GOType &value)
{
    type = value;
}

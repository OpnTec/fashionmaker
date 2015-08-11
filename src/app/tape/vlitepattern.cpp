/************************************************************************
 **
 **  @file   vlitepattern.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 8, 2015
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

#include "vlitepattern.h"

//---------------------------------------------------------------------------------------------------------------------
VLitePattern::VLitePattern(QObject *parent)
    : VAbstractPattern(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void VLitePattern::CreateEmptyFile(const QString &tablePath)
{
    Q_UNUSED(tablePath)
}

//---------------------------------------------------------------------------------------------------------------------
void VLitePattern::IncrementReferens(quint32 id) const
{
    Q_UNUSED(id)
}

//---------------------------------------------------------------------------------------------------------------------
void VLitePattern::DecrementReferens(quint32 id) const
{
    Q_UNUSED(id)
}

//---------------------------------------------------------------------------------------------------------------------
QString VLitePattern::GenerateLabel(const LabelType &type, const QString &reservedName) const
{
    Q_UNUSED(type)
    Q_UNUSED(reservedName)
    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
void VLitePattern::UpdateToolData(const quint32 &id, VContainer *data)
{
    Q_UNUSED(id)
    Q_UNUSED(data)
}

//---------------------------------------------------------------------------------------------------------------------
void VLitePattern::LiteParseTree(const Document &parse)
{
    Q_UNUSED(parse)
}

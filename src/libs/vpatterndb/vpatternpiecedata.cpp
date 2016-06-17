/************************************************************************
 **
 **  @file   vpatternpiecedata.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
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

#include "vpatternpiecedata.h"

//---------------------------------------------------------------------------------------------------------------------

VPatternPieceData::VPatternPieceData()
{}

//---------------------------------------------------------------------------------------------------------------------

VPatternPieceData::~VPatternPieceData()
{}

//---------------------------------------------------------------------------------------------------------------------

void VPatternPieceData::Append(const MaterialCutPlacement& rMCP)
{
    m_conMCP.append(rMCP);
}

//---------------------------------------------------------------------------------------------------------------------

void VPatternPieceData::Insert(int i, const MaterialCutPlacement& rMCP)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i <= GetMCPCount());
    m_conMCP.insert(i, rMCP);
}

//---------------------------------------------------------------------------------------------------------------------

int VPatternPieceData::GetMCPCount() const
{
    return m_conMCP.count();
}

//---------------------------------------------------------------------------------------------------------------------

const MaterialCutPlacement& VPatternPieceData::GetMCP(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    return m_conMCP[i];
}

//---------------------------------------------------------------------------------------------------------------------

void VPatternPieceData::RemoveMCP(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    m_conMCP.removeAt(i);
}

//---------------------------------------------------------------------------------------------------------------------

QString VPatternPieceData::GetLetter() const
{
    return m_qsLetter;
}

//---------------------------------------------------------------------------------------------------------------------

void VPatternPieceData::SetLetter(QString qsLetter)
{
    m_qsLetter = qsLetter.left(3);
}

//---------------------------------------------------------------------------------------------------------------------

QString VPatternPieceData::GetName() const
{
    return m_qsName;
}

//---------------------------------------------------------------------------------------------------------------------

void VPatternPieceData::SetName(QString qsName)
{
    m_qsName = qsName;
}

//---------------------------------------------------------------------------------------------------------------------


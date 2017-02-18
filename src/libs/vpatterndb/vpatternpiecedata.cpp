/************************************************************************
 **
 **  @file   vpatternpiecedata.cpp
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
MaterialCutPlacement::MaterialCutPlacement()
    :m_eMaterial(MaterialType::mtFabric), m_qsMaterialUserDef(), m_iCutNumber(0), m_ePlacement(PlacementType::ptNone)
{}

//---------------------------------------------------------------------------------------------------------------------
VPatternPieceData::VPatternPieceData()
    :m_qsLetter(), m_conMCP(), m_ptPos(0, 0), m_dLabelWidth(0), m_dLabelHeight(0),
    m_iFontSize(0), m_dRotation(0), m_bVisible(true)
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
void VPatternPieceData::Set(int i, const MaterialCutPlacement& rMCP)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    m_conMCP[i] = rMCP;
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
    return m_conMCP.at(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::RemoveMCP(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    m_conMCP.removeAt(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::Clear()
{
    m_qsLetter.clear();
    m_conMCP.clear();
}

//---------------------------------------------------------------------------------------------------------------------
const QString& VPatternPieceData::GetLetter() const
{
    return m_qsLetter;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetLetter(QString qsLetter)
{
    m_qsLetter = qsLetter.left(3);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPatternPieceData::GetPos() const
{
    return m_ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetPos(const QPointF& ptPos)
{
    m_ptPos = ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternPieceData::GetLabelWidth() const
{
    return m_dLabelWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetLabelWidth(qreal dLabelW)
{
    m_dLabelWidth = dLabelW;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternPieceData::GetLabelHeight() const
{
    return m_dLabelHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetLabelHeight(qreal dLabelH)
{
    m_dLabelHeight = dLabelH;
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternPieceData::GetFontSize() const
{
    return m_iFontSize;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetFontSize(int iSize)
{
    m_iFontSize = iSize;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternPieceData::GetRotation() const
{
    return m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetRotation(qreal dRot)
{
    m_dRotation = dRot;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPatternPieceData::IsVisible() const
{
    return m_bVisible;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternPieceData::SetVisible(bool bVal)
{
    m_bVisible = bVal;
}

//---------------------------------------------------------------------------------------------------------------------


/************************************************************************
 **
 **  @file   vpatternlabeldata.cpp
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

#include "vpatternlabeldata.h"

//---------------------------------------------------------------------------------------------------------------------
VPatternLabelData::VPatternLabelData()
    :m_ptPos(0, 0), m_dLabelWidth(0), m_dLabelHeight(0), m_iFontSize(0),
      m_dRotation(0), m_bVisible(true)
{}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPatternLabelData::GetPos() const
{
    return m_ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetPos(const QPointF& ptPos)
{
    m_ptPos = ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternLabelData::GetLabelWidth() const
{
    return m_dLabelWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetLabelWidth(qreal dLabelW)
{
    m_dLabelWidth = dLabelW;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternLabelData::GetLabelHeight() const
{
    return m_dLabelHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetLabelHeight(qreal dLabelH)
{
    m_dLabelHeight = dLabelH;
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternLabelData::GetFontSize() const
{
    return m_iFontSize;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetFontSize(int iSize)
{
    m_iFontSize = iSize;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternLabelData::GetRotation() const
{
    return m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetRotation(qreal dRot)
{
    m_dRotation = dRot;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPatternLabelData::IsVisible() const
{
    return m_bVisible;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetVisible(bool bVal)
{
    m_bVisible = bVal;
}

//---------------------------------------------------------------------------------------------------------------------

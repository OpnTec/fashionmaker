/************************************************************************
 **
 **  @file   vpatterninfogeometry.cpp
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

#include "vpatterninfogeometry.h"

//---------------------------------------------------------------------------------------------------------------------
VPatternInfoGeometry::VPatternInfoGeometry()
    :m_ptPos(0, 0), m_dLabelWidth(0), m_dLabelHeight(0), m_iFontSize(MIN_FONT_SIZE),
      m_dRotation(0), m_bVisible(true)
{
    m_iFontSize = MIN_FONT_SIZE;
    // 0 means unknown width
    m_dLabelWidth = 0;
    m_dLabelHeight = 0;
    m_dRotation = 0;
}

//---------------------------------------------------------------------------------------------------------------------
VPatternInfoGeometry::~VPatternInfoGeometry()
{}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPatternInfoGeometry::GetPos() const
{
    return m_ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetPos(const QPointF& ptPos)
{
    m_ptPos = ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternInfoGeometry::GetLabelWidth() const
{
    return m_dLabelWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetLabelWidth(qreal dLabelW)
{
    m_dLabelWidth = dLabelW;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternInfoGeometry::GetLabelHeight() const
{
    return m_dLabelHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetLabelHeight(qreal dLabelH)
{
    m_dLabelHeight = dLabelH;
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternInfoGeometry::GetFontSize() const
{
    return m_iFontSize;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetFontSize(int iSize)
{
    if (iSize >= MIN_FONT_SIZE)
    {
        m_iFontSize = iSize;
    }
    else
    {
        m_iFontSize = MIN_FONT_SIZE;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPatternInfoGeometry::GetRotation() const
{
    return m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetRotation(qreal dRot)
{
    m_dRotation = dRot;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPatternInfoGeometry::IsVisible() const
{
    return m_bVisible;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternInfoGeometry::SetVisible(bool bVal)
{
    m_bVisible = bVal;
}

//---------------------------------------------------------------------------------------------------------------------

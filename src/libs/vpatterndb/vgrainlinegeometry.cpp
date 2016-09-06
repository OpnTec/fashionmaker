/************************************************************************
 **
 **  @file   vgrainlinegeometry.cpp
 **  @author Bojan Kverh
 **  @date   September 06, 2016
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

#include <QPointF>

#include "vgrainlinegeometry.h"

//---------------------------------------------------------------------------------------------------------------------

VGrainlineGeometry::VGrainlineGeometry()
    :m_ptPos(0, 0), m_dLength(0), m_dRotation(0), m_bVisible(true)
{}

//---------------------------------------------------------------------------------------------------------------------

VGrainlineGeometry::~VGrainlineGeometry()
{}

//---------------------------------------------------------------------------------------------------------------------

QPointF VGrainlineGeometry::GetPos() const
{
    return m_ptPos;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetPos(const QPointF &ptPos)
{
    m_ptPos = ptPos;
}

//---------------------------------------------------------------------------------------------------------------------

qreal VGrainlineGeometry::GetLength() const
{
    return m_dLength;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetLength(qreal dLen)
{
    m_dLength = dLen;
}

//---------------------------------------------------------------------------------------------------------------------

qreal VGrainlineGeometry::GetRotation() const
{
    return m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetRotation(qreal dRot)
{
    m_dRotation = dRot;
}

//---------------------------------------------------------------------------------------------------------------------

bool VGrainlineGeometry::IsVisible() const
{
    return m_bVisible;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

//---------------------------------------------------------------------------------------------------------------------


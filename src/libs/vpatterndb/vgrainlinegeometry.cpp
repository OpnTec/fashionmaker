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
    :m_ptPos(0, 0), m_qsLength(), m_qsRotation(), m_bVisible(false), m_eArrowType(atBoth)
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

QString VGrainlineGeometry::GetLength() const
{
    return m_qsLength;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetLength(const QString& qsLen)
{
    m_qsLength = qsLen;
}

//---------------------------------------------------------------------------------------------------------------------

QString VGrainlineGeometry::GetRotation() const
{
    return m_qsRotation;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetRotation(const QString& qsRot)
{
    m_qsRotation = qsRot;
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

VGrainlineGeometry::ArrowType VGrainlineGeometry::GetArrowType() const
{
    return m_eArrowType;
}

//---------------------------------------------------------------------------------------------------------------------

void VGrainlineGeometry::SetArrowType(ArrowType eAT)
{
    m_eArrowType = eAT;
}

//---------------------------------------------------------------------------------------------------------------------


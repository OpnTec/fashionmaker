/************************************************************************
 **
 **  @file   vgrainlinedata.cpp
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

#include "vgrainlinedata.h"
#include "../ifc/ifcdef.h"

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::VGrainlineData()
    : VAbstractFloatItemData(),
      m_qsLength(),
      m_dRotation(),
      m_eArrowType(atBoth),
      m_topPin(NULL_ID),
      m_bottomPin(NULL_ID)
{}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::~VGrainlineData()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VGrainlineData::GetLength() const
{
    return m_qsLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetLength(const QString& qsLen)
{
    m_qsLength = qsLen;
}

//---------------------------------------------------------------------------------------------------------------------
QString VGrainlineData::GetRotation() const
{
    return m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetRotation(const QString& qsRot)
{
    m_dRotation = qsRot;
}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::ArrowType VGrainlineData::GetArrowType() const
{
    return m_eArrowType;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetArrowType(ArrowType eAT)
{
    m_eArrowType = eAT;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VGrainlineData::TopPin() const
{
    return m_topPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetTopPin(const quint32 &topPin)
{
    m_topPin = topPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VGrainlineData::BottomPin() const
{
    return m_bottomPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetBottomPin(const quint32 &bottomPin)
{
    m_bottomPin = bottomPin;
}

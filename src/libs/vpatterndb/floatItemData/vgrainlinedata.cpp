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
#include "vgrainlinedata_p.h"

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::VGrainlineData()
    : VAbstractFloatItemData(),
      d(new VGrainlineDataPrivate())
{}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::VGrainlineData(const VGrainlineData &data)
    : VAbstractFloatItemData(data),
      d (data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData &VGrainlineData::operator=(const VGrainlineData &data)
{
    if ( &data == this )
    {
        return *this;
    }
    VAbstractFloatItemData::operator=(data);
    d = data.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineData::~VGrainlineData()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VGrainlineData::GetLength() const
{
    return d->m_qsLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetLength(const QString& qsLen)
{
    d->m_qsLength = qsLen;
}

//---------------------------------------------------------------------------------------------------------------------
QString VGrainlineData::GetRotation() const
{
    return d->m_dRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetRotation(const QString& qsRot)
{
    d->m_dRotation = qsRot;
}

//---------------------------------------------------------------------------------------------------------------------
ArrowType VGrainlineData::GetArrowType() const
{
    return d->m_eArrowType;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetArrowType(ArrowType eAT)
{
    d->m_eArrowType = eAT;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VGrainlineData::CenterPin() const
{
    return d->m_centerPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetCenterPin(quint32 centerPin)
{
    d->m_centerPin = centerPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VGrainlineData::TopPin() const
{
    return d->m_topPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetTopPin(quint32 topPin)
{
    d->m_topPin = topPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VGrainlineData::BottomPin() const
{
    return d->m_bottomPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineData::SetBottomPin(quint32 bottomPin)
{
    d->m_bottomPin = bottomPin;
}

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
#include "vpatternlabeldata_p.h"
#include "../ifc/ifcdef.h"

//---------------------------------------------------------------------------------------------------------------------
VPatternLabelData::VPatternLabelData()
    : VAbstractFloatItemData(),
      d(new VPatternLabelDataPrivate())
{}

//---------------------------------------------------------------------------------------------------------------------
VPatternLabelData::VPatternLabelData(const VPatternLabelData &data)
    : VAbstractFloatItemData(data),
      d (data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPatternLabelData &VPatternLabelData::operator=(const VPatternLabelData &data)
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
VPatternLabelData::~VPatternLabelData()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternLabelData::GetLabelWidth() const
{
    return d->m_dLabelWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetLabelWidth(const QString &dLabelW)
{
    d->m_dLabelWidth = dLabelW;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternLabelData::GetLabelHeight() const
{
    return d->m_dLabelHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetLabelHeight(const QString &dLabelH)
{
    d->m_dLabelHeight = dLabelH;
}

//---------------------------------------------------------------------------------------------------------------------
int VPatternLabelData::GetFontSize() const
{
    return d->m_iFontSize;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetFontSize(int iSize)
{
    d->m_iFontSize = iSize;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPatternLabelData::GetRotation() const
{
    return d->m_dLabelAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetRotation(const QString &dRot)
{
    d->m_dLabelAngle = dRot;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPatternLabelData::CenterPin() const
{
    return d->m_centerPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetCenterPin(const quint32 &centerPin)
{
    d->m_centerPin = centerPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPatternLabelData::TopLeftPin() const
{
    return d->m_topLeftPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetTopLeftPin(const quint32 &topLeftPin)
{
    d->m_topLeftPin = topLeftPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPatternLabelData::BottomRightPin() const
{
    return d->m_bottomRightPin;
}

//---------------------------------------------------------------------------------------------------------------------
void VPatternLabelData::SetBottomRightPin(const quint32 &bottomRightPin)
{
    d->m_bottomRightPin = bottomRightPin;
}

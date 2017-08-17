/************************************************************************
 **
 **  @file   vpiecelabeldata.cpp
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

#include "vpiecelabeldata.h"
#include "vpiecelabeldata_p.h"

#include <QList>

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::VPieceLabelData()
    : VPatternLabelData(),
      d(new VPieceLabelDataPrivate())
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::VPieceLabelData(const VPieceLabelData &data)
    : VPatternLabelData(data),
      d (data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData &VPieceLabelData::operator=(const VPieceLabelData &data)
{
    if ( &data == this )
    {
        return *this;
    }
    VPatternLabelData::operator=(data);
    d = data.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::~VPieceLabelData()
{}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::Append(const MaterialCutPlacement& rMCP)
{
    d->m_conMCP.append(rMCP);
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::Insert(int i, const MaterialCutPlacement& rMCP)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i <= GetMCPCount());
    d->m_conMCP.insert(i, rMCP);
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::Set(int i, const MaterialCutPlacement& rMCP)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    d->m_conMCP[i] = rMCP;
}

//---------------------------------------------------------------------------------------------------------------------
int VPieceLabelData::GetMCPCount() const
{
    return d->m_conMCP.count();
}

//---------------------------------------------------------------------------------------------------------------------
const MaterialCutPlacement& VPieceLabelData::GetMCP(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    return d->m_conMCP.at(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::RemoveMCP(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < GetMCPCount());
    d->m_conMCP.removeAt(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::Clear()
{
    d->m_qsLetter.clear();
    d->m_conMCP.clear();
}

//---------------------------------------------------------------------------------------------------------------------
const QString& VPieceLabelData::GetLetter() const
{
    return d->m_qsLetter;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetLetter(QString qsLetter)
{
    d->m_qsLetter = qsLetter.left(3);
}

//---------------------------------------------------------------------------------------------------------------------
int VPieceLabelData::GetQuantity() const
{
    return d->m_quantity;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetQuantity(int val)
{
    if (val >= 1)
    {
        d->m_quantity = val;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceLabelData::IsOnFold() const
{
    return d->m_onFold;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetOnFold(bool onFold)
{
    d->m_onFold = onFold;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLabelTemplateLine> VPieceLabelData::GetLabelTemplate() const
{
    return d->m_lines;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetLabelTemplate(const QVector<VLabelTemplateLine> &lines)
{
    d->m_lines = lines;
}

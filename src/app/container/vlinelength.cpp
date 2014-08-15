/************************************************************************
 **
 **  @file   vlinelength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vlinelength.h"
#include "../geometry/vpointf.h"
#include "../widgets/vapplication.h"

#include <QLineF>

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine()
    :VInternalVariable(), p1Id(0), p2Id(0)
{
    type = VarType::LineLength;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine(const VPointF *p1, const quint32 &p1Id, const VPointF *p2, const quint32 &p2Id)
    :VInternalVariable(), p1Id(p1Id), p2Id(p2Id)
{
    SCASSERT(p1 != nullptr);
    SCASSERT(p2 != nullptr);

    type = VarType::LineLength;
    name = QString(line_+"%1_%2").arg(p1->name(), p2->name());
    SetValue(p1, p2);
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine(const VLengthLine &var)
    :VInternalVariable(var), p1Id(var.GetP1Id()), p2Id(var.GetP2Id())
{}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine &VLengthLine::operator=(const VLengthLine &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
    this->p1Id = var.GetP1Id();
    this->p2Id = var.GetP2Id();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::~VLengthLine()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VLengthLine::Filter(quint32 id)
{
    return id == p1Id || id == p2Id;
}

//---------------------------------------------------------------------------------------------------------------------
void VLengthLine::SetValue(const VPointF *p1, const VPointF *p2)
{
    SCASSERT(p1 != nullptr);
    SCASSERT(p2 != nullptr);

    value = qApp->fromPixel(QLineF(p1->toQPointF(), p2->toQPointF()).length());
}

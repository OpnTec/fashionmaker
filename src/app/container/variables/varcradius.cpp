/************************************************************************
 **
 **  @file   varcradius.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "varcradius.h"
#include "varcradius_p.h"
#include "../libs/vgeometry/varc.h"
#include "../core/vapplication.h"

#include <QLineF>

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius()
    :VInternalVariable(), d(new VArcRadiusData)
{
    SetType(VarType::ArcRadius);
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius(const VArc *arc, quint32 arcId)
    :VInternalVariable(), d(new VArcRadiusData(arcId))
{
    SCASSERT(arc != nullptr);

    SetType(VarType::ArcRadius);
    SetName(QString(radiusArc_+"%1").arg(arc->name()));
    SetValue(arc);
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius(const VArcRadius &var)
    :VInternalVariable(var), d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius &VArcRadius::operator=(const VArcRadius &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::~VArcRadius()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VArcRadius::Filter(quint32 id)
{
    return id == d->arcId;
}

//---------------------------------------------------------------------------------------------------------------------
void VArcRadius::SetValue(const VArc *arc)
{
    SCASSERT(arc != nullptr);

    VInternalVariable::SetValue(qApp->fromPixel(arc->GetRadius()));
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VArcRadius::GetArcId() const
{
    return d->arcId;
}

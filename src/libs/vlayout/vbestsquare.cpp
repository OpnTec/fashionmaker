/************************************************************************
 **
 **  @file   vbestsquare.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
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

#include "vbestsquare.h"

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare()
    :resI(0), resJ(0), resMatrix(QMatrix()), resSquare(LLONG_MAX), valideResult(false), resMirror(false),
      type (BestFrom::Rotation)
{}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(qint64 square, int i, int j, const QTransform &matrix, bool mirror, BestFrom type)
{
    if (square <= resSquare && square > 0 && type >= this->type)
    {
        resI = i;
        resJ = j;
        resMatrix = matrix;
        resSquare = square;
        valideResult = true;
        resMirror = mirror;
        this->type = type;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquare &best)
{
    if (best.ValideResult())
    {
        NewResult(best.BestSquare(), best.GContourEdge(), best.DetailEdge(), best.Matrix(), best.Mirror(), best.Type());
    }
}

//---------------------------------------------------------------------------------------------------------------------
qint64 VBestSquare::BestSquare() const
{
    return resSquare;
}

//---------------------------------------------------------------------------------------------------------------------
int VBestSquare::GContourEdge() const
{
    return resI;
}

//---------------------------------------------------------------------------------------------------------------------
int VBestSquare::DetailEdge() const
{
    return resJ;
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VBestSquare::Matrix() const
{
    return resMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::ValideResult() const
{
    return valideResult;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::Mirror() const
{
    return resMirror;
}

//---------------------------------------------------------------------------------------------------------------------
BestFrom VBestSquare::Type() const
{
    return type;
}

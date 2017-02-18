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

#include "vbestsquare.h"

#include <QMatrix>

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(const QSizeF &sheetSize, bool saveLength)
    :resI(0), resJ(0), resMatrix(QMatrix()), bestSize(QSizeF(sheetSize.width()+10, sheetSize.height()+10)),
      sheetWidth(sheetSize.width()), valideResult(false), resMirror(false), type (BestFrom::Rotation),
      saveLength(saveLength)
{}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const QSizeF &candidate, int i, int j, const QTransform &matrix, bool mirror, BestFrom type)
{
    if (saveLength)
    {
        const QSizeF saveLengthSize(sheetWidth, candidate.height());
        if (Square(saveLengthSize) <= Square(bestSize) && Square(saveLengthSize) > 0 && type >= this->type)
        {
            bestSize = saveLengthSize;
        }
        else
        {
            return;
        }
    }
    else
    {
        if (Square(candidate) <= Square(bestSize) && Square(candidate) > 0 && type >= this->type)
        {
            bestSize = candidate;
        }
        else
        {
            return;
        }
    }

    resI = i;
    resJ = j;
    resMatrix = matrix;
    valideResult = true;
    resMirror = mirror;
    this->type = type;
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquare &best)
{
    if (best.ValidResult() && saveLength == best.IsSaveLength())
    {
        NewResult(best.BestSize(), best.GContourEdge(), best.DetailEdge(), best.Matrix(), best.Mirror(), best.Type());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF VBestSquare::BestSize() const
{
    return bestSize;
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
bool VBestSquare::ValidResult() const
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

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsSaveLength() const
{
    return saveLength;
}

//---------------------------------------------------------------------------------------------------------------------
qint64 VBestSquare::Square(const QSizeF &size)
{
    return static_cast<qint64>(size.width()*size.height());
}

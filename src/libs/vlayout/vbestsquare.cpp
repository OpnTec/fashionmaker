/************************************************************************
 **
 **  @file   vbestsquare.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

namespace
{
//---------------------------------------------------------------------------------------------------------------------
qint64 Square(const QSizeF &size)
{
    return static_cast<qint64>(size.width()*size.height());
}
} // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(const QSizeF &sheetSize, bool saveLength)
    : resI(0),
      resJ(0),
      resMatrix(),
      bestSize(QSizeF(sheetSize.width()+10, sheetSize.height()+10)),
      sheetSize(sheetSize),
      valideResult(false),
      resMirror(false),
      type(BestFrom::Rotation),
      saveLength(saveLength),
      position(INT_MAX)
{}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const QSizeF &candidate, int i, int j, const QTransform &matrix, bool mirror,
                            qreal position, BestFrom type)
{
    auto SaveResult = [this, candidate, i, j, matrix, mirror, type, position]()
    {
        bestSize = candidate;
        resI = i;
        resJ = j;
        resMatrix = matrix;
        valideResult = true;
        resMirror = mirror;
        this->type = type;
        this->position = position;
    };

    if (saveLength)
    {
        const bool isPortrait = sheetSize.height() >= sheetSize.width();
        const QSizeF saveSpaceSize = isPortrait ? QSizeF(sheetSize.width(), candidate.height()) :
                                                  QSizeF(candidate.width(), sheetSize.height());

        const QSizeF saveSpaceBestSize = isPortrait ? QSizeF(sheetSize.width(), bestSize.height()) :
                                                      QSizeF(bestSize.width(), sheetSize.height());

        if (Square(saveSpaceSize) <= Square(saveSpaceBestSize) && Square(candidate) <= Square(bestSize)
                && position <= this->position && Square(saveSpaceSize) > 0 && Square(saveSpaceBestSize) > 0
                && type >= this->type)
        {
            SaveResult();
        }
    }
    else
    {
        if (Square(candidate) <= Square(bestSize) && Square(candidate) > 0 && position <= this->position
                && type >= this->type)
        {
            SaveResult();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquare &best)
{
    if (best.ValidResult() && saveLength == best.IsSaveLength())
    {
        NewResult(best.BestSize(), best.GContourEdge(), best.DetailEdge(), best.Matrix(), best.Mirror(),
                  best.Position(), best.Type());
    }
}

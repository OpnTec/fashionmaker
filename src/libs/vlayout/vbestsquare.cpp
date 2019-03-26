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
#include "vbestsquare_p.h"

#include <QMatrix>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR inline qint64 Square(const QSizeF &size)
{
    return static_cast<qint64>(size.width()*size.height());
}
} // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(const QSizeF &sheetSize, bool saveLength)
    : d(new VBestSquareData(sheetSize, saveLength))
{}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(const VBestSquare &res)
    : d(res.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::~VBestSquare()
{}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const QSizeF &candidate, int i, int j, const QTransform &matrix, bool mirror,
                            qreal position, BestFrom type)
{
    auto SaveResult = [this, candidate, i, j, matrix, mirror, type, position]()
    {
        d->bestSize = candidate;
        d->resI = i;
        d->resJ = j;
        d->resMatrix = matrix;
        d->valideResult = true;
        d->resMirror = mirror;
        d->type = type;
        d->position = position;
    };

    if (d->saveLength)
    {
        const bool isPortrait = d->sheetSize.height() >= d->sheetSize.width();
        const QSizeF saveSpaceSize = isPortrait ? QSizeF(d->sheetSize.width(), candidate.height()) :
                                                  QSizeF(candidate.width(), d->sheetSize.height());

        const QSizeF saveSpaceBestSize = isPortrait ? QSizeF(d->sheetSize.width(), d->bestSize.height()) :
                                                      QSizeF(d->bestSize.width(), d->sheetSize.height());

        if (Square(saveSpaceSize) <= Square(saveSpaceBestSize) && Square(candidate) <= Square(d->bestSize)
                && position <= d->position && Square(saveSpaceSize) > 0 && Square(saveSpaceBestSize) > 0
                && type >= d->type)
        {
            SaveResult();
        }
    }
    else
    {
        if (Square(candidate) <= Square(d->bestSize) && Square(candidate) > 0 && position <= d->position
                && type >= d->type)
        {
            SaveResult();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquare &best)
{
    if (best.IsValidResult() && d->saveLength == best.IsSaveLength())
    {
        NewResult(best.BestSize(), best.GContourEdge(), best.DetailEdge(), best.Matrix(), best.Mirror(),
                  best.Position(), best.Type());
    }
}

//---------------------------------------------------------------------------------------------------------------------
inline QSizeF VBestSquare::BestSize() const
{
    return d->bestSize;
}

//---------------------------------------------------------------------------------------------------------------------
inline int VBestSquare::GContourEdge() const
{
    return d->resI;
}

//---------------------------------------------------------------------------------------------------------------------
int VBestSquare::DetailEdge() const
{
    return d->resJ;
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VBestSquare::Matrix() const
{
    return d->resMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsValidResult() const
{
    return d->valideResult;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::Mirror() const
{
    return d->resMirror;
}

//---------------------------------------------------------------------------------------------------------------------
BestFrom VBestSquare::Type() const
{
    return d->type;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VBestSquare::Position() const
{
    return d->position;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsSaveLength() const
{
    return d->saveLength;
}

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
Q_DECL_CONSTEXPR inline qint64 Square(QSizeF size)
{
    return static_cast<qint64>(size.width()*size.height());
}
} // anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(QSizeF sheetSize, bool saveLength)
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
void VBestSquare::NewResult(const VBestSquareResData &data)
{
    auto SaveResult = [this, data]()
    {
        d->valideResult = true;
        d->data = data;
    };

    const qint64 candidateSquare = Square(data.bestSize);

    if (candidateSquare > 0)
    {
        if (data.type >= d->data.type && candidateSquare <= Square(d->data.bestSize)
                && data.depthPosition <= d->data.depthPosition)
        {
            if (d->saveLength)
            {
                if (data.depthPosition <= d->data.depthPosition)
                {
                    SaveResult();
                }
            }
            else
            {
                SaveResult();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquare &best)
{
    if (best.HasValidResult() && d->saveLength == best.IsSaveLength())
    {
        NewResult(best.BestResultData());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF VBestSquare::BestSize() const
{
    return d->data.bestSize;
}

//---------------------------------------------------------------------------------------------------------------------
int VBestSquare::GContourEdge() const
{
    return d->data.globalI;
}

//---------------------------------------------------------------------------------------------------------------------
int VBestSquare::DetailEdge() const
{
    return d->data.detJ;
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VBestSquare::Matrix() const
{
    return d->data.resMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::HasValidResult() const
{
    return d->valideResult;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::Mirror() const
{
    return d->data.resMirror;
}

//---------------------------------------------------------------------------------------------------------------------
BestFrom VBestSquare::Type() const
{
    return d->data.type;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VBestSquare::Position() const
{
    return d->depthPosition;
}

//---------------------------------------------------------------------------------------------------------------------
VBestSquareResData VBestSquare::BestResultData() const
{
    return d->data;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsSaveLength() const
{
    return d->saveLength;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsPortrait() const
{
    return d->sheetSize.height() >= d->sheetSize.width();
}

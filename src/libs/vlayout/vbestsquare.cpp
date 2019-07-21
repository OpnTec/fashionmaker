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
#include "../vmisc/def.h"

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
VBestSquare::VBestSquare(QSizeF sheetSize, bool saveLength, bool isPortrait)
    : d(new VBestSquareData(sheetSize, saveLength, isPortrait))
{}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::VBestSquare(const VBestSquare &res)
    : d(res.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare::~VBestSquare()
{}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare &VBestSquare::operator=(const VBestSquare &res)
{
    if ( &res == this )
    {
        return *this;
    }
    d = res.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::NewResult(const VBestSquareResData &data)
{
    auto SaveResult = [this, data]()
    {
        d->valideResult = true;
        d->data = data;
    };

    const qint64 candidateSquare = Square(data.bestSize);

    if (candidateSquare > 0 && data.type >= d->data.type && candidateSquare <= Square(d->data.bestSize))
    {
        if (not HasValidResult())
        {
            SaveResult(); // First result
        }
        else
        {
            if (d->saveLength)
            {
                if (qAbs(data.depthPosition - d->data.depthPosition) <= accuracyPointOnLine
                        && IsImprovedSidePosition(data.sidePosition))
                {
                    SaveResult();
                }
                else if (data.depthPosition < d->data.depthPosition)
                {
                    SaveResult();
                }
            }
            else
            {
                if (IsImprovedSidePosition(data.sidePosition)
                        || qAbs(data.sidePosition - d->data.sidePosition) <= accuracyPointOnLine)
                {
                    SaveResult();
                }
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
bool VBestSquare::IsTerminatedByException() const
{
    return d->terminatedByException;
}

//---------------------------------------------------------------------------------------------------------------------
QString VBestSquare::ReasonTerminatedByException() const
{
    return d->exceptionReason;
}

//---------------------------------------------------------------------------------------------------------------------
void VBestSquare::TerminatedByException(const QString &reason)
{
    d->valideResult = false;
    d->terminatedByException = true;
    d->exceptionReason = reason;
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
bool VBestSquare::IsImprovedSidePosition(qreal sidePosition) const
{
    const bool lessThan = d->data.sidePosition < sidePosition;
    const bool greaterThan = d->data.sidePosition > sidePosition;

    return IsPortrait() ?  greaterThan : lessThan;
}

//---------------------------------------------------------------------------------------------------------------------
bool VBestSquare::IsPortrait() const
{
    return d->isPortrait;
}

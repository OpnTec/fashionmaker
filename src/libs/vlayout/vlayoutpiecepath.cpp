/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "vlayoutpiecepath.h"
#include "vlayoutpiecepath_p.h"
#include "vlayoutdef.h"

#include <QPainterPath>

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath()
    : d(new VLayoutPiecePathData)
{
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath(const QVector<QPointF> &points, Qt::PenStyle penStyle)
    : d(new VLayoutPiecePathData(points, penStyle))
{
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath(const VLayoutPiecePath &path)
    : d(path.d)
{
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath &VLayoutPiecePath::operator=(const VLayoutPiecePath &path)
{
    if ( &path == this )
    {
        return *this;
    }
    d = path.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::~VLayoutPiecePath()
{
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiecePath::GetPainterPath() const
{
    QPainterPath path;
    if (not d->m_points.isEmpty())
    {
        path.moveTo(d->m_points.at(0));
        for (qint32 j = 1; j < d->m_points.count(); ++j)
        {
            path.lineTo(d->m_points.at(j));
        }
        path.setFillRule(Qt::WindingFill);
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiecePath::Points() const
{
    return d->m_points;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetPoints(const QVector<QPointF> &points)
{
    d->m_points = points;
}

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle VLayoutPiecePath::PenStyle() const
{
    return d->m_penStyle;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetPenStyle(const Qt::PenStyle &penStyle)
{
    d->m_penStyle = penStyle;
}

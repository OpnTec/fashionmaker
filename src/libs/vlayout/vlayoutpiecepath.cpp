/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

const quint32 VLayoutPiecePathData::streamHeader = 0xA53F0225; // CRC-32Q string "VLayoutPiecePathData"
const quint16 VLayoutPiecePathData::classVersion = 1;

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &dataStream, const VLayoutPiecePath &path)
{
    dataStream << *path.d;
    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &dataStream, VLayoutPiecePath &path)
{
    dataStream >> *path.d;
    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath()
    : d(new VLayoutPiecePathData)
{
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath(const QVector<QPointF> &points)
    : d(new VLayoutPiecePathData(points))
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

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath(const VLayoutPiecePath &&path) Q_DECL_NOTHROW
    : d(path.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath &VLayoutPiecePath::operator=(VLayoutPiecePath &&path) Q_DECL_NOTHROW
{
    std::swap(d, path.d);
    return *this;
}
#endif

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
        path.addPolygon(QPolygonF(d->m_points));
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

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiecePath::IsCutPath() const
{
    return d->m_cut;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetCutPath(bool cut)
{
    d->m_cut = cut;
}

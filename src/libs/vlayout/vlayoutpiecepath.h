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

#ifndef VLAYOUTPIECEPATH_H
#define VLAYOUTPIECEPATH_H

#include <QPointF>
#include <QSharedDataPointer>

class VLayoutPiecePathData;
class QPainterPath;

class VLayoutPiecePath
{
public:
    VLayoutPiecePath();
    VLayoutPiecePath(const QVector<QPointF> &points, Qt::PenStyle penStyle = Qt::SolidLine);
    VLayoutPiecePath(const VLayoutPiecePath &path);

    virtual ~VLayoutPiecePath();

    VLayoutPiecePath &operator=(const VLayoutPiecePath &path);
#ifdef Q_COMPILER_RVALUE_REFS
    VLayoutPiecePath &operator=(VLayoutPiecePath &&path) Q_DECL_NOTHROW { Swap(path); return *this; }
#endif

    void Swap(VLayoutPiecePath &path) Q_DECL_NOTHROW
    { std::swap(d, path.d); }

    QPainterPath GetPainterPath() const;

    QVector<QPointF> Points() const;
    void             SetPoints(const QVector<QPointF> &points);

    Qt::PenStyle PenStyle() const;
    void         SetPenStyle(const Qt::PenStyle &penStyle);

private:
    QSharedDataPointer<VLayoutPiecePathData> d;
};

Q_DECLARE_TYPEINFO(VLayoutPiecePath, Q_MOVABLE_TYPE);

#endif // VLAYOUTPIECEPATH_H

/************************************************************************
 **
 **  @file   vcontour.h
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

#ifndef VCONTOUR_H
#define VCONTOUR_H

#include "vlayoutdef.h"

#include <QVector>
#include <QSharedDataPointer>
#include <QSizeF>

class VContourData;
class QPointF;
class VLayoutDetail;
class QLineF;
class QRectF;

class VContour
{
public:
    VContour();
    VContour(int height, int width);
    VContour(const VContour &contour);
    VContour &operator=(const VContour &contour);
    ~VContour();

    void             SetContour(const QVector<QPointF> &contour);
    QVector<QPointF> GetContour() const;

    quint32 GetShift() const;
    void    SetShift(quint32 shift);

    int  GetHeight() const;
    void SetHeight(int height);

    int  GetWidth() const;
    void SetWidth(int width);

    QSizeF GetSize() const;

    QVector<QPointF> UniteWithContour(const VLayoutDetail &detail, int globalI, int detJ, BestFrom type) const;

    QLineF EmptySheetEdge() const;
    int    GlobalEdgesCount() const;
    QLineF GlobalEdge(int i) const;
    QVector<QPointF> CutEdge(const QLineF &edge) const;
    QVector<QPointF> CutEmptySheetEdge() const;

    const QPointF &	at(int i) const;

    QRectF BoundingRect() const;

private:
    QSharedDataPointer<VContourData> d;

    void AppendWhole(QVector<QPointF> &contour, const VLayoutDetail &detail, int detJ) const;
};

Q_DECLARE_TYPEINFO(VContour, Q_MOVABLE_TYPE);

#endif // VCONTOUR_H

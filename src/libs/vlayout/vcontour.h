/************************************************************************
 **
 **  @file   vcontour.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VCONTOUR_H
#define VCONTOUR_H

#include <QSharedDataPointer>
#include <QSizeF>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>

#include "vlayoutdef.h"

class VContourData;
class QPointF;
class QLineF;
class QRectF;
class QPainterPath;
class VLayoutPiece;

class VContour
{
public:
    VContour();
    VContour(int height, int width, qreal layoutWidth);
    VContour(const VContour &contour);

    ~VContour();

    VContour &operator=(const VContour &contour);
#ifdef Q_COMPILER_RVALUE_REFS
    VContour(const VContour &&contour) Q_DECL_NOTHROW;
    VContour &operator=(VContour &&contour) Q_DECL_NOTHROW;
#endif

    void CeateEmptySheetContour();

    void             SetContour(const QVector<QPointF> &contour);
    QVector<QPointF> GetContour() const;

    qreal GetShift() const;
    void  SetShift(qreal shift);

    int  GetHeight() const;
    void SetHeight(int height);

    int  GetWidth() const;
    void SetWidth(int width);

    bool IsPortrait() const;

    QSizeF GetSize() const;

    QVector<QPointF> UniteWithContour(const VLayoutPiece &detail, int globalI, int detJ, BestFrom type) const;

    QLineF EmptySheetEdge() const;
    int    GlobalEdgesCount() const;
    QLineF GlobalEdge(int i) const;
    QVector<QPointF> CutEdge(const QLineF &edge) const;
    QVector<QPointF> CutEmptySheetEdge() const;

    const QPointF &	at(int i) const;

private:
    QSharedDataPointer<VContourData> d;

    void AppendWhole(QVector<QPointF> &contour, const VLayoutPiece &detail, int detJ) const;
    void InsertDetail(QVector<QPointF> &contour, const VLayoutPiece &detail, int detJ) const;

    void ResetAttributes();

    int EmptySheetEdgesCount() const;
};

Q_DECLARE_TYPEINFO(VContour, Q_MOVABLE_TYPE);

#endif // VCONTOUR_H

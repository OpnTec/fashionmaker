/************************************************************************
 **
 **  @file   vlayoutpaper.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VLAYOUTPAPER_H
#define VLAYOUTPAPER_H

#include <QSharedDataPointer>

class VLayoutPaperData;
class VLayoutDetail;
class QPointF;
class QLineF;
class QPolygonF;
class BestResult;
class QPainterPath;
class QGraphicsItem;

class VLayoutPaper
{
public:
    VLayoutPaper();
    VLayoutPaper(int height, int width);
    VLayoutPaper(const VLayoutPaper &paper);
    VLayoutPaper &operator=(const VLayoutPaper &paper);
    virtual ~VLayoutPaper();

    int  GetHeight() const;
    void SetHeight(int height);

    int  GetWidth() const;
    void SetWidth(int width);

    unsigned int GetShift() const;
    void         SetShift(unsigned int shift);

    void SetPaperIndex(quint32 index);

    bool ArrangeDetail(const VLayoutDetail &detail);
    int  Count() const;
    QGraphicsItem *GetItem() const;

private:
    QSharedDataPointer<VLayoutPaperData> d;

    enum class CrossingType : char
    {
        NoIntersection = 0,
        Intersection = 1,
        EdgeError = 2
    };

    enum class InsideType : char
    {
        Outside = 0,
        Inside = 1,
        EdgeError = 2
    };

    bool AddToBlankSheet(const VLayoutDetail &detail);
    bool AddToSheet(const VLayoutDetail &detail);

    bool CheckCombineEdges(VLayoutDetail &detail, int j, int &dEdge) const;
    bool CheckRotationEdges(VLayoutDetail &detail, int j, int dEdge, int angle) const;

    CrossingType Crossing(const VLayoutDetail &detail, int globalI, int detailI) const;
    InsideType   InsideContour(const VLayoutDetail &detail, int detailI) const;
    qreal        CheckSide(const QLineF &edge, const QPointF &p) const;
    bool         SheetContains(const QRectF &rect) const;

    void CombineEdges(VLayoutDetail &detail, const QLineF &globalEdge, int dEdge) const;
    void RotateEdges(VLayoutDetail &detail, const QLineF &globalEdge, int dEdge, int angle) const;

    QVector<QPointF> UniteWithContour(const VLayoutDetail &detail, int globalI, int detJ) const;

    QLineF GlobalEdge(int i) const;
    int    EdgesCount() const;

    QPolygonF GlobalPolygon() const;
    QVector<QPointF> CutEdge(const QLineF &edge) const;

    bool SaveResult(const BestResult &bestResult, const VLayoutDetail &detail);

    void DrawDebug(const VLayoutDetail &detail) const;
    QPainterPath ShowDirection(const QLineF &edge) const;
    QPainterPath DrawContour(const QVector<QPointF> &points) const;
    QPainterPath DrawDetails() const;
};

#endif // VLAYOUTPAPER_H

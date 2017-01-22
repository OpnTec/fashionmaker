/************************************************************************
 **
 **  @file   vposition.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 1, 2015
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

#ifndef VPOSITION_H
#define VPOSITION_H

#include <qcompilerdetection.h>
#include <QRunnable>
#include <QVector>
#include <QtGlobal>

#include "vbestsquare.h"
#include "vcontour.h"
#include "vlayoutdef.h"
#include "vlayoutpiece.h"

class QLineF;
class QPainterPath;
class QPointF;
class QPolygonF;
class QRectF;

class VPosition : public QRunnable
{
public:
    VPosition(const VContour &gContour, int j, const VLayoutPiece &detail, int i, volatile bool *stop, bool rotate,
              int rotationIncrease, bool saveLength);
    virtual ~VPosition() Q_DECL_OVERRIDE{}

    quint32 getPaperIndex() const;
    void setPaperIndex(const quint32 &value);

    quint32 getFrame() const;
    void setFrame(const quint32 &value);

    quint32 getDetailsCount() const;
    void setDetailsCount(const quint32 &value);

    void setDetails(const QVector<VLayoutPiece> &details);

    VBestSquare getBestResult() const;

    static void DrawDebug(const VContour &contour, const VLayoutPiece &detail, int frame, quint32 paperIndex,
                          int detailsCount, const QVector<VLayoutPiece> &details = QVector<VLayoutPiece>());

    static int Bias(int length, int maxLength);

private:
    Q_DISABLE_COPY(VPosition)
    VBestSquare bestResult;
    const VContour gContour;
    const VLayoutPiece detail;
    int i;
    int j;
    quint32 paperIndex;
    quint32 frame;
    quint32 detailsCount;
    QVector<VLayoutPiece> details;
    volatile bool *stop;
    bool rotate;
    int rotationIncrease;
    /**
     * @brief angle_between keep angle between global edge and detail edge. Need for optimization rotation.
     */
    qreal angle_between;

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

    virtual void run() Q_DECL_OVERRIDE;

    void SaveCandidate(VBestSquare &bestResult, const VLayoutPiece &detail, int globalI, int detJ, BestFrom type);

    bool CheckCombineEdges(VLayoutPiece &detail, int j, int &dEdge);
    bool CheckRotationEdges(VLayoutPiece &detail, int j, int dEdge, int angle) const;

    CrossingType Crossing(const VLayoutPiece &detail) const;
    bool         SheetContains(const QRectF &rect) const;

    void CombineEdges(VLayoutPiece &detail, const QLineF &globalEdge, const int &dEdge);
    void RotateEdges(VLayoutPiece &detail, const QLineF &globalEdge, int dEdge, int angle) const;

    static QPainterPath ShowDirection(const QLineF &edge);
    static QPainterPath DrawContour(const QVector<QPointF> &points);
    static QPainterPath DrawDetails(const QVector<VLayoutPiece> &details);

    void Rotate(int increase);
};

#endif // VPOSITION_H

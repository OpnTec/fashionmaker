/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include "vsplinepoint.h"
#include <QVector>
#include "vspline.h"
#include "options.h"

namespace SplinePoint{
    enum Position
    {
        FirstPoint,
        LastPoint
    };
}

/**
 * @brief The VSplinePath клас, що розраховує шлях сплайнів.
 */
class VSplinePath{
public:
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                          VSplinePath();
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                          VSplinePath(const QMap<qint64, VPointF> *points, qreal kCurve = 1,
                                      Draw::Mode mode = Draw::Calculation, qint64 idObject = 0);
                          VSplinePath(const VSplinePath& splPath);
    /**
     * @brief append додає точку сплайну до шляху.
     * @param point точка.
     */
    void                  append(VSplinePoint point);
    qint32                Count() const;
    qint32                CountPoint() const;
    VSpline               GetSpline(qint32 index) const;
    QPainterPath          GetPath() const;
    QVector<QPointF>      GetPathPoints() const;
    QVector<VSplinePoint> GetSplinePath() const;
    qreal                 GetLength() const;
    const QMap<qint64, VPointF> *GetDataPoints() const;
    void                  UpdatePoint(qint32 indexSpline, SplinePoint::Position pos, VSplinePoint point);
    VSplinePoint          GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos) const;
    /**
     * @brief Clear очищає шлях сплайнів.
     */
    void                  Clear();
    qreal getKCurve() const;
    void setKCurve(const qreal &value);
    const QVector<VSplinePoint> *GetPoint() const;
    VSplinePath& operator=(const VSplinePath &path);
    VSplinePoint & operator[](int indx);
    qint32 referens() const;
    void incrementReferens();
    void decrementReferens();
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);

    qint64 getIdObject() const;
    void setIdObject(const qint64 &value);

protected:
    /**
     * @brief path вектор з точок сплайна.
     */
    QVector<VSplinePoint> path;
    qreal kCurve;
    Draw::Mode mode;
    const QMap<qint64, VPointF> *points;
    qint32 _referens;
    qint64 idObject;
};

#endif // VSPLINEPATH_H

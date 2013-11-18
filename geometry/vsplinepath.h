/************************************************************************
 **
 **  @file   vsplinepath.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include "vsplinepoint.h"
#include "../container/vpointf.h"
#include "vspline.h"
#include <QApplication>

namespace SplinePoint
{
    enum Position { FirstPoint, LastPoint };
    Q_DECLARE_FLAGS(Positions, Position)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( SplinePoint::Positions )

/**
 * @brief The VSplinePath клас, що розраховує шлях сплайнів.
 */
class VSplinePath
{
    Q_DECLARE_TR_FUNCTIONS(VSplinePath)
public:
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                  VSplinePath();
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                  VSplinePath(const QHash<qint64, VPointF> *points, qreal kCurve = 1,
                              Draw::Draws mode = Draw::Calculation, qint64 idObject = 0);
                  VSplinePath(const VSplinePath& splPath);
    /**
     * @brief append додає точку сплайну до шляху.
     * @param point точка.
     */
    void          append(const VSplinePoint &point);
    qint32        Count() const;
    inline qint32 CountPoint() const {return path.size();}
    VSpline       GetSpline(qint32 index) const;
    QPainterPath  GetPath() const;
    QVector<QPointF> GetPathPoints() const;
    inline QVector<VSplinePoint> GetSplinePath() const {return path;}
    qreal         GetLength() const;
    inline QHash<qint64, VPointF> GetDataPoints() const {return points;}
    void          UpdatePoint(qint32 indexSpline, const SplinePoint::Position &pos, const VSplinePoint &point);
    VSplinePoint  GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos) const;
    /**
     * @brief Clear очищає шлях сплайнів.
     */
    inline void   Clear() {path.clear();}
    inline qreal  getKCurve() const {return kCurve;}
    inline void   setKCurve(const qreal &value) {kCurve = value;}
    inline const  QVector<VSplinePoint> *GetPoint() const {return &path;}
    VSplinePath   &operator=(const VSplinePath &path);
    VSplinePoint  &operator[](ptrdiff_t indx);
    inline Draw::Draws getMode() const {return mode;}
    inline void   setMode(const Draw::Draws &value) {mode = value;}

    inline qint64 getIdObject() const {return idObject;}
    inline void   setIdObject(const qint64 &value) {idObject = value;}
    QString       name() const {return _name;}
    void          setName(const QString &name) {_name = name;}
protected:
    /**
     * @brief path вектор з точок сплайна.
     */
    QVector<VSplinePoint> path;
    qreal         kCurve;
    Draw::Draws   mode;
    QHash<qint64, VPointF> points;
    qint64        idObject;
    QString       _name;
};

#endif // VSPLINEPATH_H

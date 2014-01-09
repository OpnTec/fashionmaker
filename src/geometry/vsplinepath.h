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
#include "vpointf.h"
#include "vspline.h"
#include "vgobject.h"
#include <QApplication>
#include "../container/vcontainer.h"

namespace SplinePoint
{
    /**
     * @brief The Position enum
     */
    enum Position { FirstPoint, LastPoint };
    Q_DECLARE_FLAGS(Positions, Position)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( SplinePoint::Positions )

/**
 * @brief The VSplinePath клас, що розраховує шлях сплайнів.
 */
class VSplinePath :public VGObject
{
    Q_DECLARE_TR_FUNCTIONS(VSplinePath)
public:
                  /**
                   * @brief VSplinePath конструктор по замовчуванню.
                   */
                  VSplinePath(qreal kCurve = 1, qint64 idObject = 0, Draw::Draws mode = Draw::Calculation);
                  /**
                   * @brief VSplinePath
                   * @param splPath
                   */
                  VSplinePath(const VSplinePath& splPath);
    /**
     * @brief append додає точку сплайну до шляху.
     * @param point точка.
     */
    void          append(const VSplinePoint &point);
    /**
     * @brief Count
     * @return
     */
    qint32        Count() const;
    /**
     * @brief CountPoint
     * @return
     */
    inline qint32 CountPoint() const {return path.size();}
    /**
     * @brief GetSpline
     * @param index
     * @return
     */
    VSpline       GetSpline(qint32 index) const;
    /**
     * @brief GetPath
     * @return
     */
    QPainterPath  GetPath() const;
    /**
     * @brief GetPathPoints
     * @return
     */
    QVector<QPointF> GetPathPoints() const;
    /**
     * @brief GetSplinePath
     * @return
     */
    inline QVector<VSplinePoint> GetSplinePath() const {return path;}
    /**
     * @brief GetLength
     * @return
     */
    qreal         GetLength() const;
    /**
     * @brief UpdatePoint
     * @param indexSpline
     * @param pos
     * @param point
     */
    void          UpdatePoint(qint32 indexSpline, const SplinePoint::Position &pos, const VSplinePoint &point);
    /**
     * @brief GetSplinePoint
     * @param indexSpline
     * @param pos
     * @return
     */
    VSplinePoint  GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos) const;
    /**
     * @brief Clear очищає шлях сплайнів.
     */
    inline void   Clear() {path.clear();}
    /**
     * @brief getKCurve
     * @return
     */
    inline qreal  getKCurve() const {return kCurve;}
    /**
     * @brief setKCurve
     * @param value
     */
    inline void   setKCurve(const qreal &value) {kCurve = value;}
    /**
     * @brief GetPoint
     * @return
     */
    inline const  QVector<VSplinePoint> *GetPoint() const {return &path;}
    /**
     * @brief operator =
     * @param path
     * @return
     */
    VSplinePath   &operator=(const VSplinePath &path);
    /**
     * @brief operator []
     * @param indx
     * @return
     */
    VSplinePoint  &operator[](ptrdiff_t indx);
    const VSplinePoint &at(ptrdiff_t indx) const;
    /**
     * @brief CutSplinePath
     * @param length
     * @param p1
     * @param p2
     * @param spl1p3
     * @param spl2p2
     * @return
     */
    QPointF       CutSplinePath(qreal length, qint32 &p1, qint32 &p2, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2,
                                QPointF &spl2p3) const;
    virtual QString name() const{return _name;}
    qint32 getMaxCountPoints() const;
    void setMaxCountPoints(const qint32 &value);

protected:
    /**
     * @brief path вектор з точок сплайна.
     */
    QVector<VSplinePoint> path;
    /**
     * @brief kCurve
     */
    qreal         kCurve;
    qint32        maxCountPoints;
};

#endif // VSPLINEPATH_H

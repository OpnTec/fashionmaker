/************************************************************************
 **
 **  @file   vspline.h
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

#ifndef VSPLINE_H
#define VSPLINE_H

#include "vpointf.h"
#include "vgobject.h"

#include <QHash>
#include <QLineF>
#include <QPainterPath>

class QString;

#define M_2PI   6.28318530717958647692528676655900576

/**
 * @brief VSpline class that implements the spline.
 */
class VSpline :public VGObject
{
    Q_OBJECT
public:
    /**
     * @brief VSpline default constructor
     */
    VSpline();
    /**
     * @brief VSpline constructor.
     * @param spline spline from which the copy.
     */
    VSpline (const VSpline &spline );
    /**
     * @brief VSpline constructor.
     * @param p1 початкова точка сплайна.
     * @param p4 кінцева точка сплайна.
     * @param angle1 кут в градусах першої напрямної.
     * @param angle2 кут в градусах другої напрямної.
     * @param kCurve коефіцієнт кривизни сплайна.
     * @param kAsm1 коефіцієнт довжини першої напрямної.
     * @param kAsm2 коефіцієнт довжини другої напрямної.
     */
    VSpline (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve,
             qint64 idObject = 0, Draw::Draws mode = Draw::Calculation);
    /**
     * @brief VSpline конструктор.
     * @param p1 початкова точка сплайну.
     * @param p2 перша контролююча точка сплайну.
     * @param p3 друга контролююча точка сплайну.
     * @param p4 кінцева точка сплайну.
     */
    VSpline (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve, qint64 idObject = 0,
             Draw::Draws mode = Draw::Calculation);
    /**
     * @brief GetP1 повертає першу точку сплайну.
     * @return перша точка сплайну.
     */
    VPointF        GetP1 () const {return p1;}
    /**
     * @brief GetP2 повертує першу контрольну точку сплайну.
     * @return перша контрольна точка сплайну.
     */
    inline QPointF GetP2 () const {return p2;}
    /**
     * @brief GetP3 повертає другу контрольну точку сплайну.
     * @return друга контрольна точка сплайну.
     */
    inline QPointF GetP3 () const {return p3;}
    /**
     * @brief GetP4 повертає останню точку сплайну.
     * @return остання точка сплайну.
     */
    inline VPointF GetP4 () const {return p4;}
    /**
     * @brief GetAngle1 повертає кут першої напрямної.
     * @return кут в градусах.
     */
    inline qreal   GetAngle1 () const {return angle1;}
    /**
     * @brief GetAngle2 повертає кут другої напрямної.
     * @return кут в градусах.
     */
    inline qreal   GetAngle2() const {return angle2;}
    /**
     * @brief GetLength повертає довжину сплайну.
     * @return довжина сплайну.
     */
    qreal          GetLength () const;
    /**
     * @brief name
     * @return
     */
    QString        name () const;
    /**
     * @brief GetKasm1
     * @return
     */
    inline qreal   GetKasm1() const {return kAsm1;}
    /**
     * @brief GetKasm2
     * @return
     */
    inline qreal   GetKasm2() const {return kAsm2;}
    /**
     * @brief GetKcurve
     * @return
     */
    inline qreal   GetKcurve() const {return kCurve;}
    /**
     * @brief CrossingSplLine перевіряє перетин сплайну з лінією.
     * @param line лінія з якою перевіряється перетин.
     * @param intersectionPoint точка перетину.
     * @return результат перевірки.
     */
    QLineF::IntersectType CrossingSplLine(const QLineF &line, QPointF *intersectionPoint ) const;
    qreal           LengthT(qreal t) const;
    /**
     * @brief CutSpline cut spline. GetPointP1() of base spline will return first point for first spline, GetPointP4()
     * of base spline will return forth point of second spline.
     * @param length length first spline
     * @param spl1p2 second point of first spline
     * @param spl1p3 third point of first spline
     * @param spl2p2 second point of second spline
     * @param spl2p3 third point of second spline
     * @return point of cutting. This point is forth point of first spline and first point of second spline.
     */
    QPointF         CutSpline ( qreal length, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2, QPointF &spl2p3) const;
    /**
     * @brief GetPoints повертає точки з яких складається сплайн.
     * @return список точок.
     */
    QVector<QPointF> GetPoints () const;
    /**
     * @brief GetPath повертає шлях сплайну.
     * @return шлях.
     */
    QPainterPath   GetPath() const;
    /**
     * @brief SplinePoints
     * @param p1
     * @param p4
     * @param angle1
     * @param angle2
     * @param kAsm1
     * @param kAsm2
     * @param kCurve
     * @return
     */
    static QVector<QPointF> SplinePoints(const QPointF &p1, const QPointF &p4, qreal angle1, qreal angle2, qreal kAsm1,
                                         qreal kAsm2, qreal kCurve);
    /**
     * @brief operator =
     * @param spl
     * @return
     */
    VSpline        &operator=(const VSpline &spl);
protected:
    /**
     * @brief GetPoints повертає точки з яких складається сплайн.
     * @param p1 початкова точка сплайну.
     * @param p2 перша контролююча точка сплайну.
     * @param p3 друга контролююча точка сплайну.
     * @param p4 кінцева точка сплайну.
     * @return список точок.
     */
    static QVector<QPointF> GetPoints (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 );
private:
    /**
     * @brief p1 початкова точка сплайну
     */
    VPointF        p1;          // перша точка
    /**
     * @brief p2 перша контрольна точка сплайну.
     */
    QPointF        p2;          // друга точка
    /**
     * @brief p3 друга контрольна точка сплайну.
     */
    QPointF        p3;          // третя точка
    /**
     * @brief p4 кінцеві точка сплайну.
     */
    VPointF        p4;          // четверта точка
    /**
     * @brief angle1 кут в градусах першої напрямної.
     */
    qreal          angle1;      // кут нахилу дотичної в першій точці
    /**
     * @brief angle2 кут в градусах другої напрямної.
     */
    qreal          angle2;      // кут нахилу дотичної в другій точці
    /**
     * @brief kAsm1
     */
    qreal          kAsm1;
    /**
     * @brief kAsm2
     */
    qreal          kAsm2;
    /**
     * @brief kCurve
     */
    qreal          kCurve;
    /**
     * @brief LengthBezier повертає дожину сплайну за його чотирьма точками.
     * @param p1 початкова точка сплайну.
     * @param p2 перша контролююча точка сплайну.
     * @param p3 друга контролююча точка сплайну.
     * @param p4 кінцева точка сплайну.
     * @return дожина сплайну.
     */
    qreal          LengthBezier (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 ) const;
    /**
     * @brief PointBezier_r знаходить точки сплайну по його чотирьом точках.
     * @param x1 х координата першої точки сплайну.
     * @param y1 у координата другої точки сплайну.
     * @param x2 х координата першої контрольної точки сплайну.
     * @param y2 у координата першої контрольної точки сплайну.
     * @param x3 х координата другої контрольної точки сплайну.
     * @param y3 у координата другої контрольної точки сплайну.
     * @param x4 х координата кінцевої точки сплайну.
     * @param y4 у координата кінцевої точки сплайну.
     * @param level рівень рекурсії. Напочатку повинен дорівнювати 0.
     * @param px список х координат точок сплайну.
     * @param py список у коодринат сплайну.
     */
    static void    PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4,
                                  qint16 level, QVector<qreal> &px, QVector<qreal> &py);
    /**
     * @brief CalcSqDistance розраховує довжину між точками.
     * @param x1 х координата першої точки.
     * @param y1 у координата другої точки.
     * @param x2 х координата першої точки.
     * @param y2 у координата другої точки.
     * @return довжину.
     */
    static qreal   CalcSqDistance ( qreal x1, qreal y1, qreal x2, qreal y2);
    void           CreateName();
};

#endif // VSPLINE_H

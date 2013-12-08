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

#include "../container/vpointf.h"

#include <QHash>
#include <QLineF>
#include <QPainterPath>

class QString;

#define M_2PI   6.28318530717958647692528676655900576

/**
 * @brief VSpline клас, що реалізує сплайн.
 */
class VSpline
{
public:
                   VSpline();
                     /**
                      * @brief VSpline конструктор.
                      * @param spline сплайн з якого копіюємо.
                      */
                   VSpline (const VSpline &spline );
                     /**
                      * @brief VSpline конструктор.
                      * @param p1 початкова точка сплайна.
                      * @param p4 кінцева точка сплайна.
                      * @param angle1 кут в градусах першої напрямної.
                      * @param angle2 кут в градусах другої напрямної.
                      * @param kCurve коефіцієнт кривизни сплайна.
                      * @param kAsm1 коефіцієнт довжини першої напрямної.
                      * @param kAsm2 коефіцієнт довжини другої напрямної.
                      */
                   VSpline (const QHash<qint64, VPointF> *points, qint64 p1, qint64 p4, qreal angle1, qreal angle2,
                            qreal kAsm1, qreal kAsm2, qreal kCurve, Draw::Draws mode = Draw::Calculation,
                            qint64 idObject = 0);
                     /**
                      * @brief VSpline конструктор.
                      * @param p1 початкова точка сплайну.
                      * @param p2 перша контролююча точка сплайну.
                      * @param p3 друга контролююча точка сплайну.
                      * @param p4 кінцева точка сплайну.
                      */
                   VSpline (const QHash<qint64, VPointF> *points, qint64 p1, QPointF p2, QPointF p3, qint64 p4,
                            qreal kCurve, Draw::Draws mode = Draw::Calculation, qint64 idObject = 0);
    /**
     * @brief ModifiSpl модифікує сплайн.
     * @param p1 початкова точка сплайну.
     * @param p4 кінцева точка сплайну.
     * @param angle1 кут в градусах першої напрямної.
     * @param angle2 кут в градусах другої напрямної.
     * @param kCurve коефіцієнт кривизни сплайну.
     * @param kAsm1 коефіцієнт довжини першої напрямної.
     * @param kAsm2 коефіцієнт довжини другої напрямної.
     */
    void           ModifiSpl ( qint64 p1, qint64 p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2,
                                 qreal kCurve);
    /**
     * @brief ModifiSpl модифікує сплайн.
     * @param p1 початкова точка сплайну.
     * @param p2 перша контролююча точка сплайну.
     * @param p3 друга контролююча точка сплайну.
     * @param p4 кінцева точка сплайну.
     */
    void           ModifiSpl (const qint64 &p1, const QPointF &p2, const QPointF &p3, const qint64 &p4,
                              const qreal &kCurve);
    /**
     * @brief RotationSpl поворот сплайна навколо точки на кут в градусах проти годиникової стрілки.
     * @param pRotate точка навколо якої повертаємо.
     * @param angle кут в градусах.
     */
//    void         RotationSpl ( QPointF pRotate, qreal angle );
    /**
     * @brief BiasSpl зміщує сплайн.
     * @param mx зміщення по х координаті.
     * @param my зміщення по у координаті.
     */
//    void         BiasSpl ( qreal mx, qreal my );
    /**
     * @brief GetP1 повертає першу точку сплайну.
     * @return перша точка сплайну.
     */
    qint64         GetP1 () const {return p1;}
    /**
     * @brief GetPointP1
     * @return
     */
    VPointF        GetPointP1() const;
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
    inline qint64  GetP4 () const {return p4;}
    /**
     * @brief GetPointP4
     * @return
     */
    VPointF        GetPointP4 () const;
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
     * @brief GetName
     * @return
     */
    QString        GetName () const;
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
     * @brief GetDataPoints
     * @return
     */
    inline const QHash<qint64, VPointF> GetDataPoints() const {return points;}
    /**
     * @brief CrossingSplLine перевіряє перетин сплайну з лінією.
     * @param line лінія з якою перевіряється перетин.
     * @param intersectionPoint точка перетину.
     * @return результат перевірки.
     */
    QLineF::IntersectType CrossingSplLine(const QLineF &line, QPointF *intersectionPoint ) const;
    /**
     * @brief CutSpline розрізає сплайн.
     * @param length дожина першого сплайну.
     * @param curFir перший сплайн.
     * @param curSec другий сплайн.
     */
//    void         CutSpline ( qreal length, VSpline* curFir, VSpline* curSec ) const;
    /**
     * @brief CutSpline розрізає сплайн.
     * @param point точка що ділить сплайн.
     * @param curFir перший сплайн.
     * @param curSec другий сплайн.
     */
//    void         CutSpline (QPointF point, VSpline* curFir, VSpline* curSec ) const;
    /**
     * @brief PutAlongSpl розміщає точку на сплайні.
     * @param moveP точка яка розміщується на сплайні.
     * @param move довжина від початку сплайну.
     */
//    void         PutAlongSpl ( QPointF &moveP, qreal move ) const;
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
     * @brief Mirror вертикальне дзеркалення сплайну відносно точки.
     * @param Pmirror точка відносно якої відбувається вертикальне дзеркалення сплайну.
     */
//    void         Mirror(const QPointF Pmirror);
    /**
     * @brief getMode
     * @return
     */
    inline Draw::Draws getMode() const {return mode;}
    /**
     * @brief setMode
     * @param value
     */
    inline void    setMode(const Draw::Draws &value) {mode = value;}
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
     * @brief getIdObject
     * @return
     */
    inline qint64  getIdObject() const {return idObject;}
    /**
     * @brief setIdObject
     * @param value
     */
    inline void    setIdObject(const qint64 &value) {idObject = value;}
    /**
     * @brief operator =
     * @param spl
     * @return
     */
    VSpline        &operator=(const VSpline &spl);
    /**
     * @brief name
     * @return
     */
    QString        name() const {return _name;}
    /**
     * @brief setName
     * @param name
     */
    void           setName(const QString &name) {_name = name;}
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
    qint64         p1;          // перша точка
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
    qint64         p4;          // четверта точка
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
     * @brief points
     */
    QHash<qint64, VPointF> points;
    /**
     * @brief mode
     */
    Draw::Draws    mode;
    /**
     * @brief idObject
     */
    qint64         idObject;
    /**
     * @brief _name
     */
    QString        _name;
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
//    /**
//     * @brief Cubic знаходить розв'язок кубічного рівняння.
//     * @param x коефіцієнт.
//     * @param a коефіцієнт.
//     * @param b коефіцієнт.
//     * @param c коефіцієнт.
//     * @return повертає корені рівняння.
//     */
//    static qint32  Cubic(qreal *x, qreal a, qreal b, qreal c);
    /**
     * @brief calc_t знаходить параметр t якому відповідає точка на сплайні.
     * @param curve_coord1 координата Х або У кривої.
     * @param curve_coord2 координата Х або У кривої.
     * @param curve_coord3 координата Х або У кривої.
     * @param curve_coord4 координата Х або У кривої.
     * @param point_coord координата Х або У точки на кривій.
     * @return
     */
//    static qreal calc_t (qreal curve_coord1, qreal curve_coord2, qreal curve_coord3, qreal curve_coord4,
//                         qreal point_coord)const;
    /**
     * @brief param_t  знаходить підходяще значення параметра t якому відповідає точка на сплайні
     * @param pBt точка для якої шукається значення параметра t.
     * @return підходяще значення t.
     */
//    qreal        param_t (QPointF pBt)const;
};

#endif // VSPLINE_H

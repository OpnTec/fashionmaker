/************************************************************************
 **
 **  @file   varc.h
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

#ifndef VARC_H
#define VARC_H

#include "vspline.h"
class QString;
class QLineF;
class QPainterPath;
class QPointF;

/**
 * @brief VArc клас, що реалізує дугу. Дуга розраховується за годиниковою стрілкою.
 */
class VArc
{
    Q_DECLARE_TR_FUNCTIONS(VArc)
public:
                       /**
                        * @brief VArc конструктор по замовчуванню.
                        */
                       VArc ();
                       /**
                        * @brief VArc конструктор.
                        * @param center точка центру.
                        * @param radius радіус.
                        * @param f1 початковий кут в градусах.
                        * @param f2 кінцевий кут в градусах.
                        */
                       VArc (const QHash<qint64, VPointF> *points, qint64 center, qreal radius, QString formulaRadius,
                             qreal f1, QString formulaF1, qreal f2, QString formulaF2,
                             Draw::Draws mode = Draw::Calculation, qint64 idObject = 0);
                       /**
                        * @brief VArc
                        * @param arc
                        */
                       VArc(const VArc &arc);
                       /**
                        * @brief operator =
                        * @param arc
                        * @return
                        */
                       VArc& operator= (const VArc &arc);
    /**
     * @brief GetF1 повертає початковий кут дуги.
     * @return повертає кут в градусах.
     */
    inline QString     GetFormulaF1 () const {return formulaF1;}
    /**
     * @brief GetF1
     * @return
     */
    inline qreal       GetF1 () const {return f1;}
    /**
     * @brief GetF2 повертає кінцевий кут дуги.
     * @return повертає кут в градусах.
     */
    inline QString     GetFormulaF2 () const {return formulaF2;}
    /**
     * @brief GetF2
     * @return
     */
    inline qreal       GetF2 () const {return f2;}
    /**
     * @brief GetLength повертає довжину дуги.
     * @return повертає довжину дуги.
     */
    inline qreal       GetLength () const {return M_PI * radius/180 * (f2-f1);}
    /**
     * @brief GetRadius повертає радіус дуги.
     * @return повертає радіус дуги.
     */
    inline QString     GetFormulaRadius () const {return formulaRadius;}
    /**
     * @brief GetRadius
     * @return
     */
    inline qreal       GetRadius () const {return radius;}
    /**
     * @brief GetCenter повертає точку центра дуги.
     * @return повертає точку центра дуги.
     */
    inline qint64      GetCenter () const {return center;}
    /**
     * @brief GetCenterPoint
     * @return
     */
    QPointF            GetCenterPoint() const;
    /**
     * @brief GetCenterVPoint
     * @return
     */
    VPointF            GetCenterVPoint() const;
    /**
     * @brief GetP1 повертає першу точку з якої починається дуга.
     * @return точку початку дуги.
     */
    QPointF            GetP1 () const;
    /**
     * @brief GetP2 повертає другу точку в якій закінчується дуга.
     * @return точку кінця дуги.
     */
    QPointF            GetP2 () const;
    /**
     * @brief GetDataPoints
     * @return
     */
    const QHash<qint64, VPointF> GetDataPoints() const;
    /**
     * @brief GetPath будує шлях по даній дузі.
     * @return повертає шлях.
     */
    QPainterPath       GetPath() const;
    /**
     * @brief AngleArc
     * @return
     */
    qreal              AngleArc() const;
    /**
     * @brief NumberSplOfArc
     * @return
     */
    qint32             NumberSplOfArc () const;
    /**
     * @brief GetPoints
     * @return
     */
    QVector<QPointF>   GetPoints () const;
    /**
     * @brief SplOfArc
     * @param number
     * @return
     */
    QVector<QPointF>   SplOfArc( qint32 number ) const;
    /**
     * @brief getMode
     * @return
     */
    inline Draw::Draws getMode() const {return mode;}
    /**
     * @brief setMode
     * @param value
     */
    inline void        setMode(const Draw::Draws &value) {mode = value;}
    /**
     * @brief getIdObject
     * @return
     */
    inline qint64      getIdObject() const {return idObject;}
    /**
     * @brief setIdObject
     * @param value
     */
    inline void        setIdObject(const qint64 &value) {idObject = value;}
    /**
     * @brief name
     * @return
     */
    QString            name() const {return _name;}
    /**
     * @brief setName
     * @param name
     */
    void               setName(const QString &name) {_name = name;}
private:
    /**
     * @brief f1 початковий кут в градусах
     */
    qreal              f1;          // початковий кут нахилу дуги (градуси)
    /**
     * @brief formulaF1
     */
    QString            formulaF1;
    /**
     * @brief f2 кінцевий кут в градусах
     */
    qreal              f2;          // кінцевий кут нахилу дуги (градуси)
    /**
     * @brief formulaF2
     */
    QString            formulaF2;
    /**
     * @brief radius радіус дуги.
     */
    qreal              radius;
    /**
     * @brief formulaRadius
     */
    QString            formulaRadius;
    /**
     * @brief center центральна точка дуги.
     */
    qint64             center;
    /**
     * @brief points
     */
    QHash<qint64, VPointF> points;
    /**
     * @brief mode
     */
    Draw::Draws        mode;
    /**
     * @brief idObject
     */
    qint64             idObject;
    /**
     * @brief _name
     */
    QString            _name;
};

#endif // VARC_H

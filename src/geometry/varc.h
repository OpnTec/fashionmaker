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

#include "vgobject.h"
#include <QCoreApplication>
#include "../options.h"
#include "vpointf.h"
class QString;
class QLineF;
class QPainterPath;

/**
 * @brief VArc class for anticlockwise arc.
 */
class VArc: public VGObject
{
    Q_DECLARE_TR_FUNCTIONS(VArc)
public:
                       /**
                        * @brief VArc default constructor.
                        */
                       VArc ();
                       /**
                        * @brief VArc constructor.
                        * @param center center point.
                        * @param radius arc radius.
                        * @param f1 start angle (degree).
                        * @param f2 end angle (degree).
                        */
                       VArc (VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1, qreal f2,
                             QString formulaF2, quint32 idObject = 0, Draw::Draws mode = Draw::Calculation);
                       /**
                        * @brief VArc copy constructor
                        * @param arc arc
                        */
                       VArc(const VArc &arc);
                       /**
                        * @brief operator = assignment operator
                        * @param arc arc
                        * @return arc
                        */
                       VArc& operator= (const VArc &arc);
    /**
     * @brief GetF1 return start angle.
     * @return angle in degree.
     */
    inline QString     GetFormulaF1 () const {return formulaF1;}
    /**
     * @brief GetF1 return formula for start angle.
     * @return string with formula.
     */
    inline qreal       GetF1 () const {return f1;}
    /**
     * @brief GetF2 return end angle.
     * @return angle in degree.
     */
    inline QString     GetFormulaF2 () const {return formulaF2;}
    /**
     * @brief GetF2 return formula for end angle.
     * @return string with formula.
     */
    inline qreal       GetF2 () const {return f2;}
    /**
     * @brief GetLength return arc length.
     * @return length.
     */
    qreal              GetLength () const;
    /**
     * @brief GetRadius return arc radius.
     * @return radius.
     */
    inline QString     GetFormulaRadius () const {return formulaRadius;}
    /**
     * @brief GetRadius return formula for radius.
     * @return string with formula.
     */
    inline qreal       GetRadius () const {return radius;}
    /**
     * @brief GetCenter return center point.
     * @return center point.
     */
    inline VPointF     GetCenter () const {return center;}
    /**
     * @brief GetP1 return point associated with start angle.
     * @return point.
     */
    QPointF GetP1() const;
    /**
     * @brief GetP2 return point associated with end angle.
     * @return точку point.
     */
    QPointF            GetP2 () const;
    /**
     * @brief GetPath return QPainterPath for this arc.
     * @return path.
     */
    QPainterPath       GetPath() const;
    /**
     * @brief AngleArc calculate arc angle.
     * @return angle in degree.
     */
    qreal              AngleArc() const;
    /**
     * @brief GetPoints return list of points needed for drawing arc.
     * @return list of points
     */
    QVector<QPointF>   GetPoints () const;
    /**
     * @brief name return arc name. This name used in variables.
     * @return name
     */
    virtual QString    name() const;
    /**
     * @brief CutArc cut arc into two arcs.
     * @param length length first arc.
     * @param arc1 first arc.
     * @param arc2 second arc.
     * @return point cutting
     */
    QPointF            CutArc (const qreal &length, VArc &arc1, VArc &arc2) const;
    /**
     * @brief setId keep id arc in data.
     * @param id id arc in data.
     */
    virtual void       setId(const quint32 &id);
private:
    /**
     * @brief f1 start angle in degree.
     */
    qreal              f1;
    /**
     * @brief formulaF1 formula for start angle.
     */
    QString            formulaF1;
    /**
     * @brief f2 end angle in degree.
     */
    qreal              f2;
    /**
     * @brief formulaF2 formula for end angle.
     */
    QString            formulaF2;
    /**
     * @brief radius arc radius.
     */
    qreal              radius;
    /**
     * @brief formulaRadius formula for arc radius.
     */
    QString            formulaRadius;
    /**
     * @brief center center point of arc.
     */
    VPointF            center;
};

#endif // VARC_H

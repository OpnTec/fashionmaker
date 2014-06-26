/************************************************************************
 **
 **  @file   varc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "vabstractcurve.h"
#include "vpointf.h"
#include <QCoreApplication>

class QPainterPath;

/**
 * @brief VArc class for anticlockwise arc.
 */
class VArc: public VAbstractCurve
{
    Q_DECLARE_TR_FUNCTIONS(VArc)
public:
    VArc ();
    VArc (VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1, qreal f2,
         QString formulaF2, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VArc(const VArc &arc);
    VArc& operator= (const VArc &arc);
    QString            GetFormulaF1 () const;
    qreal              GetF1 () const;
    QString            GetFormulaF2 () const;
    qreal              GetF2 () const;
    qreal              GetLength () const;
    QString            GetFormulaRadius () const;
    qreal              GetRadius () const;
    VPointF            GetCenter () const;
    QPointF            GetP1() const;
    QPointF            GetP2 () const;
    qreal              AngleArc() const;
    QVector<QPointF>   GetPoints () const;
    QPointF            CutArc (const qreal &length, VArc &arc1, VArc &arc2) const;
    virtual void       setId(const quint32 &id);
private:
    /** @brief f1 start angle in degree. */
    qreal              f1;

    /** @brief formulaF1 formula for start angle. */
    QString            formulaF1;

    /** @brief f2 end angle in degree. */
    qreal              f2;

    /** @brief formulaF2 formula for end angle. */
    QString            formulaF2;

    /** @brief radius arc radius. */
    qreal              radius;

    /** @brief formulaRadius formula for arc radius. */
    QString            formulaRadius;

    /** @brief center center point of arc. */
    VPointF            center;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return start angle.
 * @return angle in degree.
 */
inline QString VArc::GetFormulaF1() const
{
    return formulaF1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula for start angle.
 * @return string with formula.
 */
inline qreal VArc::GetF1() const
{
    return f1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return end angle.
 * @return angle in degree.
 */
inline QString VArc::GetFormulaF2() const
{
    return formulaF2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula for end angle.
 * @return string with formula.
 */
inline qreal VArc::GetF2() const
{
    return f2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return arc radius.
 * @return radius.
 */
inline QString VArc::GetFormulaRadius() const
{
    return formulaRadius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula for radius.
 * @return string with formula.
 */
inline qreal VArc::GetRadius() const
{
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return center point.
 * @return center point.
 */
inline VPointF VArc::GetCenter() const
{
    return center;
}

#endif // VARC_H

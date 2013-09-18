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

#ifndef VSPLINEPOINT_H
#define VSPLINEPOINT_H

#include "QtGlobal"
#include <QMetaType>

/**
 * @brief The VSplinePoint клас, що містить у собі інформацію про точки сплайну.
 */
class VSplinePoint{
public:
            /**
             * @brief VSplinePoint конструктор по замповчуванню.
             */
            VSplinePoint();
            /**
             * @brief VSplinePoint конструктор.
             * @param pSpline точка сплайну.
             * @param angle кут дотичної сплайна.
             * @param factor коефіцієнт довжини дотичної.
             */
            VSplinePoint(qint64 pSpline, qreal kAsm1, qreal angle, qreal kAsm2);
            VSplinePoint(const VSplinePoint &point);
            ~VSplinePoint();
    /**
     * @brief P повертає точку.
     * @return точка.
     */
    qint64 P() const;
    void SetP(const qint64 &value);
    /**
     * @brief Angle1 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    qreal   Angle1() const;
    void    SetAngle(const qreal &value);
    /**
     * @brief Angle2 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    qreal   Angle2() const;
    /**
     * @brief KAsm1 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    qreal   KAsm1() const;
    void    SetKAsm1(const qreal &value);
    /**
     * @brief KAsm2 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    qreal   KAsm2() const;
    void    SetKAsm2(const qreal &value);
protected:
    /**
     * @brief pSpline точка сплайну.
     */
    qint64 pSpline;
    /**
     * @brief angle кут дотичної сплайну.
     */
    qreal   angle;
    /**
     * @brief kAsm1 коефіцієнт довжини дотичної сплайну.
     */
    qreal   kAsm1;
    /**
     * @brief kAsm2 коефіцієнт довжини дотичної сплайну.
     */
    qreal   kAsm2;
};

Q_DECLARE_METATYPE(VSplinePoint)

#endif // VSPLINEPOINT_H

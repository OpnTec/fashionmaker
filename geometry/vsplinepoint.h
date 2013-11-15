/************************************************************************
 **
 **  @file vsplinepoint.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#ifndef VSPLINEPOINT_H
#define VSPLINEPOINT_H

#include <QMetaType>

/**
 * @brief The VSplinePoint клас, що містить у собі інформацію про точки сплайну.
 */
class VSplinePoint
{
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
                  ~VSplinePoint() {}
    /**
     * @brief P повертає точку.
     * @return точка.
     */
    inline qint64 P() const {return pSpline;}
    inline void   SetP(const qint64 &value) {pSpline = value;}
    /**
     * @brief Angle1 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    inline qreal  Angle1() const {return angle+180;}
    inline void   SetAngle(const qreal &value) {angle = value;}
    /**
     * @brief Angle2 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    inline qreal  Angle2() const {return angle;}
    /**
     * @brief KAsm1 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    inline qreal  KAsm1() const {return kAsm1;}
    inline void   SetKAsm1(const qreal &value) {kAsm1 = value;}
    /**
     * @brief KAsm2 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    inline qreal  KAsm2() const {return kAsm2;}
    inline void   SetKAsm2(const qreal &value) {kAsm2 = value;}
protected:
    /**
     * @brief pSpline точка сплайну.
     */
    qint64        pSpline;
    /**
     * @brief angle кут дотичної сплайну.
     */
    qreal         angle;
    /**
     * @brief kAsm1 коефіцієнт довжини дотичної сплайну.
     */
    qreal         kAsm1;
    /**
     * @brief kAsm2 коефіцієнт довжини дотичної сплайну.
     */
    qreal         kAsm2;
};

Q_DECLARE_METATYPE(VSplinePoint)

#endif // VSPLINEPOINT_H

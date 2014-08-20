/************************************************************************
 **
 **  @file   vspline_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VSPLINE_P_H
#define VSPLINE_P_H

#include <QSharedData>
#include "../options.h"
#include "vpointf.h"
#include <QLineF>

class VSplineData : public QSharedData
{
public:
    VSplineData()
        :p1(VPointF()), p2(QPointF()), p3(QPointF()), p4(VPointF()), angle1(0), angle2(0), kAsm1(1), kAsm2(1), kCurve(1)
    {}

    VSplineData ( const VSplineData &spline )
        :QSharedData(spline), p1(spline.p1), p2(spline.p2), p3(spline.p3), p4(spline.p4), angle1(spline.angle1),
          angle2(spline.angle2), kAsm1(spline.angle2), kAsm2(spline.kAsm1), kCurve(spline.kCurve)
    {}

    VSplineData (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve)
        :p1(p1), p2(QPointF()), p3(QPointF()), p4(p4), angle1(angle1), angle2(angle2), kAsm1(kAsm1), kAsm2(kAsm2),
          kCurve(kCurve)
    {
        qreal L = 0, radius = 0, angle = 90;
        QPointF point1 = this->p1.toQPointF();
        QPointF point4 = this->p4.toQPointF();
        radius = QLineF(point1, point4).length()/1.414213;//1.414213=sqrt(2);
        L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );
        QLineF p1p2(this->p1.x(), this->p1.y(), this->p1.x() + L * kAsm1, this->p1.y());
        p1p2.setAngle(angle1);
        QLineF p4p3(this->p4.x(), this->p4.y(), this->p4.x() + L * kAsm2, this->p4.y());
        p4p3.setAngle(angle2);
        this->p2 = p1p2.p2();
        this->p3 = p4p3.p2();
    }

    VSplineData (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve)
        :p1(p1), p2(p2), p3(p3), p4(p4), angle1(0), angle2(0), kAsm1(1), kAsm2(1), kCurve(1)
    {
        this->angle1 = QLineF ( this->p1.toQPointF(), this->p2 ).angle();
        this->angle2 = QLineF ( this->p4.toQPointF(), this->p3 ).angle();

        qreal L = 0, radius = 0, angle = 90;
        QPointF point1 = this->p1.toQPointF();
        QPointF point4 = this->p4.toQPointF();
        radius = QLineF(point1, point4).length()/1.414213;//1.414213=sqrt(2);
        L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );

        this->kCurve = kCurve;
        this->kAsm1 = QLineF ( this->p1.toQPointF(), this->p2 ).length()/L;
        this->kAsm2 = QLineF ( this->p4.toQPointF(), this->p3 ).length()/L;
    }

    virtual ~VSplineData() {}

    /** @brief p1 first spline point. */
    VPointF        p1;

    /** @brief p2 first control point. */
    QPointF        p2;

    /** @brief p3 second control point. */
    QPointF        p3;

    /** @brief p4 last spline point. */
    VPointF        p4;

    /** @brief angle1 first angle control line. */
    qreal          angle1;

    /** @brief angle2 second angle control line. */
    qreal          angle2;

    /** @brief kAsm1 coefficient of length first control line. */
    qreal          kAsm1;

    /** @brief kAsm2 coefficient of length second control line. */
    qreal          kAsm2;

    /** @brief kCurve coefficient of curvature spline. */
    qreal          kCurve;
};


#endif // VSPLINE_P_H

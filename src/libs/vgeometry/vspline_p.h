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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "vpointf.h"
#include <QLineF>
#include <QtCore/qmath.h>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VSplineData : public QSharedData
{
public:
    VSplineData();
    VSplineData(const VSplineData &spline);
    VSplineData(VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve);
    VSplineData(VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve);
    VSplineData(VPointF p1, VPointF p4, qreal angle1, const QString &angle1F, qreal angle2, const QString &angle2F,
                qreal c1Length, const QString &c1LengthF, qreal c2Length, const QString &c2LengthF, qreal kCurve);
    virtual ~VSplineData();

    static qreal GetL(const QPointF &p1, const QPointF &p4, qreal kCurve);

    /** @brief p1 first spline point. */
    VPointF p1;

    /** @brief p4 fourth spline point. */
    VPointF p4;

    /** @brief angle1 first angle control line. */
    qreal   angle1;

    /** @brief angle1F the first control point angle formula*/
    QString angle1F;

    /** @brief angle2 second angle control line. */
    qreal   angle2;

    /** @brief angle2F the second control point angle formula*/
    QString angle2F;

    /** @brief c1Length the length from the first spline point to the first control point. */
    qreal   c1Length;

    /** @brief c1LengthF the formula from the first spline point to the first control point. */
    QString c1LengthF;

    /** @brief c2Length the length from the fourth spline point to the second control point. */
    qreal   c2Length;

    /** @brief c2LengthF the formula length from the fourth spline point to the second control point. */
    QString c2LengthF;

    /** @brief kCurve coefficient of curvature spline. */
    qreal   kCurve;

private:
    VSplineData &operator=(const VSplineData &) Q_DECL_EQ_DELETE;
};

//---------------------------------------------------------------------------------------------------------------------
VSplineData::VSplineData()
    : p1(),
      p4(),
      angle1(0),
      angle1F("0"),
      angle2(0),
      angle2F("0"),
      c1Length(0),
      c1LengthF("0"),
      c2Length(0),
      c2LengthF("0"),
      kCurve(1)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplineData::VSplineData(const VSplineData &spline)
    : QSharedData(spline),
      p1(spline.p1),
      p4(spline.p4),
      angle1(spline.angle1),
      angle1F(spline.angle1F),
      angle2(spline.angle2),
      angle2F(spline.angle2F),
      c1Length(spline.c1Length),
      c1LengthF(spline.c1LengthF),
      c2Length(spline.c2Length),
      c2LengthF(spline.c2LengthF),
      kCurve(spline.kCurve)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplineData::VSplineData(VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve)
    : p1(p1),
      p4(p4),
      angle1(angle1),
      angle1F(QString().number(angle1)),
      angle2(angle2),
      angle2F(QString().number(angle2)),
      c1Length(0),
      c1LengthF("0"),
      c2Length(0),
      c2LengthF("0"),
      kCurve(kCurve)
{
    const qreal L = GetL(p1.toQPointF(), p4.toQPointF(), kCurve);

    QLineF p1p2(p1.x(), p1.y(), p1.x() + L * kAsm1, p1.y());
    p1p2.setAngle(angle1);
    c1Length = p1p2.length();
    c1LengthF = QString().number(c1Length);

    QLineF p4p3(p4.x(), p4.y(), p4.x() + L * kAsm2, p4.y());
    p4p3.setAngle(angle2);
    c2Length = p4p3.length();
    c2LengthF = QString().number(c2Length);
}

//---------------------------------------------------------------------------------------------------------------------
VSplineData::VSplineData(VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve)
    : p1(p1),
      p4(p4),
      angle1(0),
      angle1F("0"),
      angle2(0),
      angle2F("0"),
      c1Length(0),
      c1LengthF("0"),
      c2Length(0),
      c2LengthF("0"),
      kCurve(kCurve)
{
    QLineF p1p2(p1.toQPointF(), p2);

    angle1 = p1p2.angle();
    angle1F = QString().number(angle1);

    c1Length = p1p2.length();
    c1LengthF = QString().number(c1Length);

    QLineF p4p3(p4.toQPointF(), p3);

    angle2 = p4p3.angle();
    angle2F = QString().number(angle2);

    c2Length = p4p3.length();
    c2LengthF = QString().number(c2Length);
}

//---------------------------------------------------------------------------------------------------------------------
VSplineData::VSplineData(VPointF p1, VPointF p4, qreal angle1, const QString &angle1F, qreal angle2,
                         const QString &angle2F, qreal c1Length, const QString &c1LengthF,
                         qreal c2Length, const QString &c2LengthF, qreal kCurve)
    : p1(p1),
      p4(p4),
      angle1(angle1),
      angle1F(angle1F),
      angle2(angle2),
      angle2F(angle2F),
      c1Length(c1Length),
      c1LengthF(c1LengthF),
      c2Length(c2Length),
      c2LengthF(c2LengthF),
      kCurve(kCurve)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplineData::~VSplineData()
{}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplineData::GetL(const QPointF &p1, const QPointF &p4, qreal kCurve)
{
    static const qreal angle = 90;
    const qreal radius = QLineF(p1, p4).length()/M_SQRT2;
    return kCurve * radius * 4 / 3 * qTan( angle * M_PI_4 / 180.0 );
}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VSPLINE_P_H

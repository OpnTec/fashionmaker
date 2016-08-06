/************************************************************************
 **
 **  @file   vsplinepoint_p.h
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

#ifndef VSPLINEPOINT_P_H
#define VSPLINEPOINT_P_H

#include <QSharedData>
#include <QDebug>

#include "vpointf.h"
#include "../vmisc/def.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VFSplinePointData : public QSharedData
{
public:

    VFSplinePointData()
        : pSpline(VPointF()),
          angle1(0),
          angle2(180),
          kAsm1(1),
          kAsm2(1)
    {}

    VFSplinePointData(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
        : pSpline(pSpline),
          angle1(angle1),
          angle2(angle2),
          kAsm1(kAsm1),
          kAsm2(kAsm2)
    {
        if (VFuzzyComparePossibleNulls(angle1, angle2) || not qFuzzyCompare(qAbs(angle1-angle2), 180) )
        {
            qDebug()<<"Make angle1 and angle2 correct.";
            this->angle1 = angle1;
            this->angle2 = angle1 + 180;
        }
    }

    VFSplinePointData(const VFSplinePointData &point)
        : QSharedData(point),
          pSpline(point.pSpline),
          angle1(point.angle1),
          angle2(point.angle2),
          kAsm1(point.kAsm1),
          kAsm2(point.kAsm2)
    {}

    virtual ~VFSplinePointData();

    /** @brief pSpline point. */
    VPointF pSpline;

    /** @brief angle1 first angle spline. */
    qreal   angle1;

    /** @brief angle2 second angle spline. */
    qreal   angle2;

    /** @brief kAsm1 coefficient of length first control line. */
    qreal   kAsm1;

    /** @brief kAsm2 coefficient of length second control line. */
    qreal   kAsm2;

private:
    VFSplinePointData &operator=(const VFSplinePointData &) Q_DECL_EQ_DELETE;
};

VFSplinePointData::~VFSplinePointData()
{}

//--------------------------------------VSplinePointData---------------------------------------------------------------

class VSplinePointData : public QSharedData
{
public:
    VSplinePointData()
        : pSpline(),
          angle1(0),
          angle1F("0"),
          angle2(180),
          angle2F("180"),
          length1(0),
          length1F("0"),
          length2(0),
          length2F("0")
    {}

    VSplinePointData(VPointF pSpline, qreal angle1, const QString &angle1F, qreal angle2, const QString &angle2F,
                     qreal length1, const QString &length1F, qreal length2, const QString &length2F)
        : pSpline(pSpline),
          angle1(angle1),
          angle1F(angle1F),
          angle2(angle2),
          angle2F(angle2F),
          length1(length1),
          length1F(length1F),
          length2(length2),
          length2F(length2F)
    {
        if (VFuzzyComparePossibleNulls(angle1, angle2) || not qFuzzyCompare(qAbs(angle1-angle2), 180))
        {
            qDebug()<<"Make angle1 and angle2 correct.";
            this->angle2 = angle1 + 180;
            this->angle2F = QString().number(angle2);
        }
    }

    VSplinePointData(const VSplinePointData &point)
        : QSharedData(point),
          pSpline(point.pSpline),
          angle1(point.angle1),
          angle1F(point.angle1F),
          angle2(point.angle2),
          angle2F(point.angle2F),
          length1(point.length1),
          length1F(point.length1F),
          length2(point.length2),
          length2F(point.length2F)
    {}

    virtual ~VSplinePointData();

    /** @brief pSpline point. */
    VPointF pSpline;

    /** @brief angle1 first angle spline. */
    qreal   angle1;
    QString angle1F;

    /** @brief angle2 second angle spline. */
    qreal   angle2;
    QString angle2F;

    /** @brief length1 length a first control line. */
    qreal   length1;
    QString length1F;

    /** @brief length2 length a second control line. */
    qreal   length2;
    QString length2F;

private:
    VSplinePointData &operator=(const VSplinePointData &) Q_DECL_EQ_DELETE;
};

VSplinePointData::~VSplinePointData()
{}

QT_WARNING_POP

#endif // VSPLINEPOINT_P_H

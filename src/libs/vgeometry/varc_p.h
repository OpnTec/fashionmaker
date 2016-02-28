/************************************************************************
 **
 **  @file   varc_p.h
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

#ifndef VARC_P_H
#define VARC_P_H

#include <QSharedData>
#include "vgeometrydef.h"
#include <../vmisc/vabstractapplication.h>
#include "vpointf.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VArcData : public QSharedData
{
public:

    VArcData ()
        : f1(0), formulaF1(QString()), f2(0), formulaF2(QString()), radius(0), formulaRadius(QString()),
          center(VPointF()), isFlipped(false), formulaLength()
    {}

    VArcData (VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1, qreal f2,
                QString formulaF2)
        : f1(f1), formulaF1(formulaF1), f2(f2), formulaF2(formulaF2), radius(radius), formulaRadius(formulaRadius),
          center(center), isFlipped(false), formulaLength()
    {}

    VArcData(VPointF center, qreal radius, qreal f1, qreal f2)
        : f1(f1), formulaF1(QString().number(f1)),
          f2(f2), formulaF2(QString().number(f2)),
          radius(radius), formulaRadius(QString().number(qApp->fromPixel(radius))),
          center(center), isFlipped(false), formulaLength()
    {}

    VArcData (QString formulaLength, VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1)
        : f1(f1), formulaF1(formulaF1), f2(0), formulaF2("0"), radius(radius), formulaRadius(formulaRadius),
          center(center), isFlipped(false), formulaLength(formulaLength)
    {}

    VArcData(VPointF center, qreal radius, qreal f1)
        : f1(f1), formulaF1(QString().number(f1)), f2(0), formulaF2("0"), radius(radius),
          formulaRadius(QString().number(qApp->fromPixel(radius))), center(center), isFlipped(false), formulaLength()
    {}

    VArcData(const VArcData &arc)
        : QSharedData(arc), f1(arc.f1), formulaF1(arc.formulaF1), f2(arc.f2), formulaF2(arc.formulaF2),
          radius(arc.radius), formulaRadius(arc.formulaRadius), center(arc.center), isFlipped(arc.isFlipped),
          formulaLength(arc.formulaLength)
    {}

    virtual ~VArcData();

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

    bool               isFlipped;

    QString            formulaLength;

private:
    VArcData &operator=(const VArcData &) Q_DECL_EQ_DELETE;
};

VArcData::~VArcData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VARC_P_H

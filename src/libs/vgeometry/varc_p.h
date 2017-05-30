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
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VArcData : public QSharedData
{
public:
    VArcData();
    VArcData(qreal radius, const QString &formulaRadius);
    explicit VArcData(qreal radius);
    VArcData(const VArcData &arc);
    virtual ~VArcData();

    /** @brief radius arc radius. */
    qreal              radius;

    /** @brief formulaRadius formula for arc radius. */
    QString            formulaRadius;

private:
    VArcData &operator=(const VArcData &) Q_DECL_EQ_DELETE;
};

//---------------------------------------------------------------------------------------------------------------------
VArcData::VArcData()
    : radius(0),
      formulaRadius(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
VArcData::VArcData(qreal radius, const QString &formulaRadius)
    : radius(radius),
      formulaRadius(formulaRadius)
{}

//---------------------------------------------------------------------------------------------------------------------
VArcData::VArcData(qreal radius)
    : radius(radius),
      formulaRadius(QString().number(qApp->fromPixel(radius)))
{}

//---------------------------------------------------------------------------------------------------------------------
VArcData::VArcData(const VArcData &arc)
    : QSharedData(arc),
      radius(arc.radius),
      formulaRadius(arc.formulaRadius)
{}

//---------------------------------------------------------------------------------------------------------------------
VArcData::~VArcData()
{}

QT_WARNING_POP

#endif // VARC_P_H

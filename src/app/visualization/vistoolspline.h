/************************************************************************
 **
 **  @file   vistoolspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 8, 2014
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

#ifndef VISTOOLSPLINE_H
#define VISTOOLSPLINE_H

#include "vispath.h"

class VisToolSpline : public VisPath
{
    Q_OBJECT
public:
    VisToolSpline(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolSpline();

    virtual void RefreshGeometry();

    void         setPoint4Id(const quint32 &value);
    void         setAngle1(const qreal &value);
    void         setAngle2(const qreal &value);
    void         setKAsm1(const qreal &value);
    void         setKAsm2(const qreal &value);
    void         setKCurve(const qreal &value);
protected:
    Q_DISABLE_COPY(VisToolSpline)
    quint32              point4Id;
    QGraphicsEllipseItem *lineP1;
    QGraphicsEllipseItem *lineP4;
    QGraphicsLineItem    *line;
    qreal                angle1;
    qreal                angle2;
    qreal                kAsm1;
    qreal                kAsm2;
    qreal                kCurve;
};

#endif // VISTOOLSPLINE_H

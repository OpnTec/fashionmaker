/************************************************************************
 **
 **  @file   vistoolendline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
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

#include "vistoolendline.h"
#include "../geometry/vpointf.h"
#include "../container/vcontainer.h"
#include "../widgets/vapplication.h"
#include "../tools/vabstracttool.h"

#include <QGraphicsScene>
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::VisToolEndLine(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), length(0), angle(0), point(nullptr)
{
    this->mainColor = Qt::red;

    point = InitPoint(mainColor);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::~VisToolEndLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::RefreshGeometry()
{
    const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
    QLineF line;
    if (qFuzzyCompare(1 + length, 1 + 0))
    {
        QPointF second;
        if(this->scene() == nullptr)
        {
            second = scenePos;
        }
        else
        {
            second = Ray(first->toQPointF());
        }
        line = QLineF(first->toQPointF(), second);
    }
    else
    {
        line = Line(first->toQPointF(), length, angle);
        DrawPoint(point, line.p2(), mainColor);
    }
    DrawLine(this, line, mainColor, lineStyle);
    toolTip = QString(tr("<b>Point at distance and angle</b>: angle = %1°; <b>Shift</b> - sticking angle, "
                         "<b>Enter</b> - finish creation")).arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisToolEndLine::Angle() const
{
    return this->line().angle();
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::setAngle(const qreal &value)
{
    angle = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::setLength(const QString &expression)
{
    length = FindLength(expression);
}

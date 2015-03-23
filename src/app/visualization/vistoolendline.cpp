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

#include "vistoolendline.h"
#include "../geometry/vpointf.h"
#include "../container/vcontainer.h"
#include "../tools/vabstracttool.h"

#include <QGraphicsScene>

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::VisToolEndLine(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), length(0), angle(0), point(nullptr)
{
    this->mainColor = Qt::red;

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::~VisToolEndLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::RefreshGeometry()
{
    const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(point1Id);
    QLineF line;
    if (qFuzzyCompare(1 + length, 1 + 0))
    {
        line = QLineF(first->toQPointF(), Ray(first->toQPointF()));
    }
    else
    {
        line = VGObject::BuildLine(first->toQPointF(), length, angle);
        DrawPoint(point, line.p2(), mainColor);
    }
    DrawLine(this, line, mainColor, lineStyle);
    Visualization::toolTip = QString(tr("<b>Point at distance and angle</b>: angle = %1°; <b>Shift</b> - "
                                        "sticking angle, <b>Enter</b> - finish creation")).arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolEndLine::Angle() const
{
    return QString("%1").arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::SetAngle(const QString &expression)
{
    angle = FindVal(expression);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::setLength(const QString &expression)
{
    length = FindLength(expression);
}

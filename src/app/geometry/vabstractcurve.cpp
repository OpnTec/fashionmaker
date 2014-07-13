/************************************************************************
 **
 **  @file   vabstractcurve.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
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

#include "vabstractcurve.h"

#include <QPainterPath>

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve::VAbstractCurve(const GOType &type, const quint32 &idObject, const Draw &mode)
    :VGObject(type, idObject, mode)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve::VAbstractCurve(const VAbstractCurve &curve)
    :VGObject(curve)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve &VAbstractCurve::operator=(const VAbstractCurve &curve)
{
    VGObject::operator=(curve);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractCurve::GetPath() const
{
    QPainterPath path;

    QVector<QPointF> points = GetPoints();
    if (points.count() >= 2)
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            path.moveTo(points.at(i));
            path.lineTo(points.at(i+1));
        }
    }
    else
    {
        qDebug()<<"points.count() < 2"<<Q_FUNC_INFO;
    }
    return path;
}

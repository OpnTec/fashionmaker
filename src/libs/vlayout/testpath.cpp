/************************************************************************
 **
 **  @file   testpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#include "testpath.h"

#include <QVector>
#include <QJsonArray>
#include <QTemporaryFile>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>
#include <QPointF>

#include "vsapoint.h"

//---------------------------------------------------------------------------------------------------------------------
#if !defined(V_NO_ASSERT)
// Use for writing tests
QJsonObject PointToJson(const QPointF &point)
{
    QJsonObject pointObject
    {
        {"type", "QPointF"},
        {"x", point.x()},
        {"y", point.y()},
    };
    return pointObject;
}

//---------------------------------------------------------------------------------------------------------------------
void VectorToJson(const QVector<QPointF> &points, QJsonObject &json)
{
    QJsonArray pointsArray;
    for (auto point: points)
    {
        pointsArray.append(PointToJson(point));
    }
    json[QLatin1String("vector")] = pointsArray;
}

//---------------------------------------------------------------------------------------------------------------------
void VectorToJson(const QVector<VSAPoint> &points, QJsonObject &json)
{
    QJsonArray pointsArray;
    for (auto point: points)
    {
        pointsArray.append(point.toJson());
    }
    json[QLatin1String("vector")] = pointsArray;
}

#endif // !defined(V_NO_ASSERT)

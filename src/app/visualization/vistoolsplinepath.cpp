/************************************************************************
 **
 **  @file   vistoolsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 9, 2014
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

#include "vistoolsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolSplinePath::VisToolSplinePath(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent), points(QVector<QGraphicsEllipseItem *>()), line(nullptr), path(VSplinePath()),
      mode(Mode::Creation)
{
    line = InitItem<QGraphicsLineItem>(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolSplinePath::~VisToolSplinePath()
{
    qDeleteAll(points.begin(), points.end());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::RefreshGeometry()
{
    if (path.CountPoint() > 0)
    {
        QVector<VSplinePoint> pathPoints = path.GetSplinePath();
        if (path.CountPoint() == 1)
        {
            QGraphicsEllipseItem *point = this->getPoint(0);
            DrawPoint(point, pathPoints.at(0).P().toQPointF(), supportColor);

            if (mode == Mode::Creation)
            {
                QLineF sceneLine = QLineF(pathPoints.at(0).P().toQPointF(), Visualization::scenePos);
                DrawLine(line, sceneLine, mainColor, lineStyle);

                path[0].SetAngle2(sceneLine.angle());
                emit PathChanged(path);
            }
        }
        else
        {
            for (int i = 0; i < pathPoints.size(); ++i)
            {
                QGraphicsEllipseItem *point = this->getPoint(static_cast<unsigned>(i));
                DrawPoint(point, pathPoints.at(i).P().toQPointF(), supportColor);
            }

            if (mode == Mode::Creation)
            {
                QLineF sceneLine = QLineF(pathPoints.at(pathPoints.size() - 1).P().toQPointF(),
                                          Visualization::scenePos);
                DrawLine(line, sceneLine, mainColor, lineStyle);

                path[pathPoints.size() - 1].SetAngle2(sceneLine.angle());
                emit PathChanged(path);
            }

            DrawPath(this, path.GetPath(PathDirection::Show), mainColor, Qt::SolidLine, Qt::RoundCap);
        }
        if (path.CountPoint() < 3)
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select three or more points");
        }
        else
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select three or more points, "
                                        "<b>Enter</b> - finish creation");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::setPath(const VSplinePath &value)
{
    path = value;
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePath VisToolSplinePath::getPath()
{
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
Mode VisToolSplinePath::getMode() const
{
    return mode;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::setMode(const Mode &value)
{
    mode = value;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisToolSplinePath::getPoint(unsigned int i)
{
    if (static_cast<unsigned int>(points.size() - 1) >= i && points.isEmpty() == false)
    {
        return points.at(static_cast<int>(i));
    }
    else
    {
        QGraphicsEllipseItem * point = InitPoint(supportColor, this);
        points.append(point);
        return point;
    }
    return nullptr;
}

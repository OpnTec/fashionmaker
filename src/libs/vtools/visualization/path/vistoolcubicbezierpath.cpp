/************************************************************************
 **
 **  @file   vistoolcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vistoolcubicbezierpath.h"
#include "../vgeometry/vspline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCubicBezierPath::VisToolCubicBezierPath(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      mainPoints(),
      ctrlPoints(),
      lines(),
      newCurveSegment(nullptr),
      path(),
      helpLine1(nullptr),
      helpLine2(nullptr)
{
    helpLine1 = InitItem<QGraphicsLineItem>(mainColor, this);
    helpLine2 = InitItem<QGraphicsLineItem>(mainColor, this);

    newCurveSegment = InitItem<QGraphicsPathItem>(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolCubicBezierPath::~VisToolCubicBezierPath()
{
    qDeleteAll(mainPoints);
    qDeleteAll(lines);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezierPath::RefreshGeometry()
{
    const QVector<VPointF> pathPoints = path.GetCubicPath();
    const int size = pathPoints.size();
    if (size > 0)
    {
        const int countSubSpl = VCubicBezierPath::CountSubSpl(size);

        for (int i = 0; i < size; ++i)
        {
            QGraphicsEllipseItem *point = this->getPoint(mainPoints, static_cast<unsigned>(i), 1/*zValue*/);
            DrawPoint(point, pathPoints.at(i), supportColor);
        }

        if (mode == Mode::Creation)
        {
            if (countSubSpl < 1)
            {
                Creating(pathPoints, size-1);
            }
            else
            {
                const qint32 last = VCubicBezierPath::SubSplOffset(countSubSpl) + 3;
                Creating(pathPoints, size-1-last);
            }
        }

        if (countSubSpl >= 1)
        {
            DrawPath(this, path.GetPath(PathDirection::Show), mainColor, Qt::SolidLine, Qt::RoundCap);

            for (qint32 i = 1; i<=countSubSpl; ++i)
            {
                const int preLastPoint = (countSubSpl - 1) * 2;
                const int lastPoint = preLastPoint + 1;

                const VSpline spl = path.GetSpline(i);

                QGraphicsLineItem *ctrlLine1 = this->getLine(static_cast<unsigned>(preLastPoint));
                DrawLine(ctrlLine1, QLineF(spl.GetP1(), spl.GetP2()), mainColor, Qt::DashLine);

                QGraphicsEllipseItem *p2 = this->getPoint(ctrlPoints, static_cast<unsigned>(preLastPoint));
                DrawPoint(p2, spl.GetP2(), Qt::green);

                QGraphicsLineItem *ctrlLine2 = this->getLine(static_cast<unsigned>(lastPoint));
                DrawLine(ctrlLine2, QLineF(spl.GetP4(), spl.GetP3()), mainColor, Qt::DashLine);

                QGraphicsEllipseItem *p3 = this->getPoint(ctrlPoints, static_cast<unsigned>(lastPoint));
                DrawPoint(p3, spl.GetP3(), Qt::green);
            }
        }

        RefreshToolTip();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezierPath::setPath(const VCubicBezierPath &value)
{
    path = value;

    RefreshToolTip();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
VCubicBezierPath VisToolCubicBezierPath::getPath()
{
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisToolCubicBezierPath::getPoint(QVector<QGraphicsEllipseItem *> &points, quint32 i, qreal z)
{
    if (not points.isEmpty() && static_cast<quint32>(points.size() - 1) >= i)
    {
        return points.at(static_cast<int>(i));
    }
    else
    {
        auto point = InitPoint(supportColor, this, z);
        points.append(point);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsLineItem *VisToolCubicBezierPath::getLine(quint32 i)
{
    if (static_cast<quint32>(lines.size() - 1) >= i && lines.isEmpty() == false)
    {
        return lines.at(static_cast<int>(i));
    }
    else
    {
        auto line = InitItem<QGraphicsLineItem>(mainColor, this);
        lines.append(line);
        return line;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezierPath::Creating(const QVector<VPointF> &pathPoints, int pointsLeft)
{
    const int size = pathPoints.size();
    if (pathPoints.isEmpty() || size+1 < pointsLeft)
    {
        return;
    }

    int subSplPoints = 0;
    const int subSplCount = VCubicBezierPath::CountSubSpl(size);
    if (subSplCount >= 1)
    {
        subSplPoints = VCubicBezierPath::SubSplPointsCount(subSplCount)-1;
    }

    switch(pointsLeft)
    {
        case 0:
        {
            const VPointF p1 = pathPoints.last();
            if (pathPoints.size() >= 4)
            {
                QLineF p1p2(p1, Visualization::scenePos);
                QLineF prP3p1(pathPoints.at(size-2), p1);
                p1p2.setAngle(prP3p1.angle());

                const QPointF p2 = p1p2.p2();

                VSpline spline(p1, p2, Visualization::scenePos, VPointF(Visualization::scenePos));
                DrawPath(newCurveSegment, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);

                DrawLine(helpLine1, p1p2, mainColor, Qt::DashLine);

                const int preLastPoint = subSplCount * 2;
                QGraphicsEllipseItem *p2Ctrl = this->getPoint(ctrlPoints, static_cast<unsigned>(preLastPoint));
                DrawPoint(p2Ctrl, p2, Qt::green);
            }
            else
            {
                DrawLine(helpLine1, QLineF(p1, Visualization::scenePos), mainColor, Qt::DashLine);
            }
            break;
        }
        case 1:
        {
            const VPointF p1 = pathPoints.at(subSplPoints + pointsLeft-1);
            QPointF p2 = pathPoints.at(subSplPoints + pointsLeft);

            if (subSplCount >= 1)
            {
                QLineF p1p2(p1, p2);
                QLineF prP3p1(pathPoints.at(subSplPoints + pointsLeft-2), p1);
                p1p2.setAngle(prP3p1.angle());
                p2 = p1p2.p2();
            }

            DrawLine(helpLine1, QLineF(p1, p2), mainColor, Qt::DashLine);

            VSpline spline(p1, p2, Visualization::scenePos, VPointF(Visualization::scenePos));
            DrawPath(newCurveSegment, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);

            const int preLastPoint = subSplCount * 2;
            QGraphicsEllipseItem *p2Ctrl = this->getPoint(ctrlPoints, static_cast<unsigned>(preLastPoint));
            DrawPoint(p2Ctrl, p2, Qt::green);
            break;
        }
        case 2:
        {
            const VPointF p1 = pathPoints.at(subSplPoints + pointsLeft-2);
            QPointF p2 = pathPoints.at(subSplPoints + pointsLeft-1);
            const QPointF p3 = pathPoints.at(subSplPoints + pointsLeft);

            if (subSplCount >= 1)
            {
                QLineF p1p2(p1, p2);
                QLineF prP3p1(pathPoints.at(subSplPoints + pointsLeft-3), p1);
                p1p2.setAngle(prP3p1.angle());
                p2 = p1p2.p2();
            }

            DrawLine(helpLine1, QLineF(p1, p2), mainColor, Qt::DashLine);
            DrawLine(helpLine2, QLineF(p3, Visualization::scenePos), mainColor, Qt::DashLine);

            VSpline spline(p1, p2, p3, VPointF(Visualization::scenePos));
            DrawPath(newCurveSegment, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);

            const int preLastPoint = subSplCount * 2;
            QGraphicsEllipseItem *p2Ctrl = this->getPoint(ctrlPoints, static_cast<unsigned>(preLastPoint));
            DrawPoint(p2Ctrl, p2, Qt::green);
            break;
        }
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezierPath::RefreshToolTip()
{
    const int size = path.CountPoints();
    if (size > 0)
    {
        const int countSubSpl = VCubicBezierPath::CountSubSpl(size);

        if (size < 7)
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select seven or more points");
        }
        else if (size >= 7 && size - VCubicBezierPath::SubSplPointsCount(countSubSpl) == 0)
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select seven or more points, "
                                        "<b>Enter</b> - finish creation");
        }
        else
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select more points for complete segment");
        }
        emit ToolTip(Visualization::toolTip);
    }
}

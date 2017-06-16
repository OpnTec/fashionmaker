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

#include <QGraphicsPathItem>
#include <QLineF>
#include <QPainterPath>
#include <QPoint>
#include <Qt>
#include <QtAlgorithms>

#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepoint.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "../visualization.h"
#include "vispath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolSplinePath::VisToolSplinePath(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      points(QVector<QGraphicsEllipseItem *>()),
      ctrlPoints(QVector<VControlPointSpline *>()),
      newCurveSegment(nullptr),
      path(VSplinePath()),
      isLeftMousePressed(false),
      pointSelected(false),
      ctrlPoint()
{
    newCurveSegment = InitItem<VCurvePathItem>(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolSplinePath::~VisToolSplinePath()
{
    qDeleteAll(ctrlPoints);
    qDeleteAll(points);
    emit ToolTip("");
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::RefreshGeometry()
{
    if (path.CountPoints() > 0)
    {
        const QVector<VSplinePoint> pathPoints = path.GetSplinePath();
        const int size = pathPoints.size();

        for (int i = 0; i < size; ++i)
        {
            QGraphicsEllipseItem *point = this->getPoint(static_cast<unsigned>(i));
            DrawPoint(point, static_cast<QPointF>(pathPoints.at(i).P()), supportColor);
        }

        if (mode == Mode::Creation)
        {
            if (size > 1)
            {
                for (qint32 i = 1; i<=path.CountSubSpl(); ++i)
                {
                    const int preLastPoint = (path.CountSubSpl() - 1) * 2;
                    const int lastPoint = preLastPoint + 1;

                    VSpline spl = path.GetSpline(i);

                    ctrlPoints[preLastPoint]->RefreshCtrlPoint(i, SplinePointPosition::FirstPoint,
                                                               static_cast<QPointF>(spl.GetP2()),
                                                               static_cast<QPointF>(spl.GetP1()));
                    ctrlPoints[lastPoint]->RefreshCtrlPoint(i, SplinePointPosition::LastPoint,
                                                            static_cast<QPointF>(spl.GetP3()),
                                                            static_cast<QPointF>(spl.GetP4()));
                }
            }

            Creating(static_cast<QPointF>(pathPoints.at(size-1).P()), size);
        }

        if (size > 1)
        {
            DrawPath(this, path.GetPath(), path.GetDirectionPath(), mainColor, lineStyle, Qt::RoundCap);
        }

        if (path.CountPoints() < 3)
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select three or more points");
        }
        else
        {
            Visualization::toolTip = tr("<b>Curved path</b>: select three or more points, "
                                        "<b>Enter</b> - finish creation");
        }
        if (mode == Mode::Show)
        {
            Visualization::toolTip = tr("Use <b>Shift</b> for sticking angle!");
            emit ToolTip(Visualization::toolTip);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::setPath(const VSplinePath &value)
{
    path = value;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
VSplinePath VisToolSplinePath::getPath()
{
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    for (int i=0; i < points.size(); ++i)
    {
        ScalePoint(points[i], scale);
    }

    VisPath::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::MouseLeftPressed()
{
    if (mode == Mode::Creation)
    {
        isLeftMousePressed = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::MouseLeftReleased()
{
    if (mode == Mode::Creation)
    {
        isLeftMousePressed = false;
        RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisToolSplinePath::getPoint(quint32 i)
{
    if (static_cast<quint32>(points.size() - 1) >= i && points.isEmpty() == false)
    {
        return points.at(static_cast<int>(i));
    }
    else
    {
        pointSelected = false;

        auto point = InitPoint(supportColor, this);
        points.append(point);

        if (points.size() == 1)
        {
            auto *controlPoint1 = new VControlPointSpline(points.size(), SplinePointPosition::FirstPoint, this);
            controlPoint1->hide();
            ctrlPoints.append(controlPoint1);
        }
        else
        {
            auto *controlPoint1 = new VControlPointSpline(points.size()-1, SplinePointPosition::LastPoint, this);
            controlPoint1->hide();
            ctrlPoints.append(controlPoint1);

            auto *controlPoint2 = new VControlPointSpline(points.size(), SplinePointPosition::FirstPoint, this);
            controlPoint2->hide();
            ctrlPoints.append(controlPoint2);
        }

        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::Creating(const QPointF &pSpl, int size)
{
    int lastPoint = 0;
    int preLastPoint = 0;

    if (size > 1)
    {
        lastPoint = (size - 1) * 2;
        preLastPoint = lastPoint - 1;
    }

    if (isLeftMousePressed && not pointSelected)
    {
        newCurveSegment->hide();

        ctrlPoint = Visualization::scenePos;

        if (not ctrlPoints[lastPoint]->isVisible())
        {
            //Radius of point circle, but little bigger. Need handle with hover sizes.
            if (QLineF(pSpl, ctrlPoint).length() > defPointRadiusPixel*1.5)
            {
                if (size == 1)
                {
                    ctrlPoints[lastPoint]->show();
                }
                else
                {
                    ctrlPoints[preLastPoint]->show();
                    ctrlPoints[lastPoint]->show();
                }
            }
            else
            {
                ctrlPoint = pSpl;
            }
        }

        QLineF ctrlLine (pSpl, Visualization::scenePos);
        ctrlLine.setAngle(ctrlLine.angle()+180);

        if (size == 1)
        {
            ctrlPoints[lastPoint]->RefreshCtrlPoint(size, SplinePointPosition::FirstPoint, ctrlPoint, pSpl);
        }
        else
        {
            ctrlPoints[preLastPoint]->RefreshCtrlPoint(size-1, SplinePointPosition::LastPoint, ctrlLine.p2(), pSpl);
            ctrlPoints[lastPoint]->RefreshCtrlPoint(size, SplinePointPosition::FirstPoint, ctrlPoint, pSpl);
        }

        VSpline spline(VPointF(pSpl), ctrlPoint, Visualization::scenePos, VPointF(Visualization::scenePos));

        if (size == 1)
        {
            path[size-1].SetAngle2(spline.GetStartAngle(), spline.GetStartAngleFormula());
            if (ctrlPoint != pSpl)
            {
                path[size-1].SetLength2(spline.GetC1Length(), spline.GetC1LengthFormula());
            }
            else
            {
                path[size-1].SetLength2(0, "0");
            }
            emit PathChanged(path);
        }
        else
        {
            const VSpline spl = path.GetSpline(size - 1);
            VSpline preSpl(spl.GetP1(), static_cast<QPointF>(spl.GetP2()), ctrlLine.p2(), VPointF(pSpl));

            path[size-1].SetAngle2(spline.GetStartAngle(), spline.GetStartAngleFormula());
            if (ctrlPoint != pSpl)
            {
                path[size-1].SetLength1(preSpl.GetC2Length(), preSpl.GetC2LengthFormula());
                path[size-1].SetLength2(spline.GetC1Length(), spline.GetC1LengthFormula());
            }
            else
            {
                path[size-1].SetLength1(0, "0");
                path[size-1].SetLength2(0, "0");
            }
            emit PathChanged(path);
        }
    }
    else
    {
        pointSelected = true;

        VSpline spline(VPointF(pSpl), ctrlPoint, Visualization::scenePos, VPointF(Visualization::scenePos));

        path[size-1].SetAngle2(spline.GetStartAngle(), spline.GetStartAngleFormula());

        if (ctrlPoint != pSpl)
        {
            path[size-1].SetLength2(spline.GetC1Length(), spline.GetC1LengthFormula());
        }
        else
        {
            path[size-1].SetLength2(0, "0");
        }
        emit PathChanged(path);

        DrawPath(newCurveSegment, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);
    }
}

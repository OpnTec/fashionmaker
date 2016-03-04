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
#include "../vwidgets/vcontrolpointspline.h"

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
    newCurveSegment = InitItem<QGraphicsPathItem>(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolSplinePath::~VisToolSplinePath()
{
    qDeleteAll(ctrlPoints);
    qDeleteAll(points);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSplinePath::RefreshGeometry()
{
    if (path.CountPoint() > 0)
    {
        const QVector<VSplinePoint> pathPoints = path.GetSplinePath();
        const int size = pathPoints.size();

        for (int i = 0; i < size; ++i)
        {
            QGraphicsEllipseItem *point = this->getPoint(static_cast<unsigned>(i));
            DrawPoint(point, pathPoints.at(i).P().toQPointF(), supportColor);
        }

        if (mode == Mode::Creation)
        {
            if (size > 1)
            {
                for (qint32 i = 1; i<=path.Count(); ++i)
                {
                    const int preLastPoint = (path.Count() - 1) * 2;
                    const int lastPoint = preLastPoint + 1;

                    VSpline spl = path.GetSpline(i);

                    ctrlPoints[preLastPoint]->RefreshCtrlPoint(i, SplinePointPosition::FirstPoint, spl.GetP2(),
                                                               spl.GetP1().toQPointF());
                    ctrlPoints[lastPoint]->RefreshCtrlPoint(i, SplinePointPosition::LastPoint, spl.GetP3(),
                                                            spl.GetP4().toQPointF());
                }
            }

            Creating(pathPoints.at(size-1).P().toQPointF(), size);
        }

        if (size > 1)
        {
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
// cppcheck-suppress unusedFunction
VSplinePath VisToolSplinePath::getPath()
{
    return path;
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
            auto *controlPoint1 = new VControlPointSpline(points.size(), SplinePointPosition::FirstPoint,
                                                          *Visualization::data->GetPatternUnit(),
                                                          this);
            controlPoint1->hide();
            ctrlPoints.append(controlPoint1);
        }
        else
        {
            auto *controlPoint1 = new VControlPointSpline(points.size()-1, SplinePointPosition::LastPoint,
                                                          *Visualization::data->GetPatternUnit(),
                                                          this);
            controlPoint1->hide();
            ctrlPoints.append(controlPoint1);

            auto *controlPoint2 = new VControlPointSpline(points.size(), SplinePointPosition::FirstPoint,
                                                          *Visualization::data->GetPatternUnit(),
                                                          this);
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
    //Radius of point circle, but little bigger. Need handle with hover sizes.
    const static qreal radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;

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
            if (QLineF(pSpl, ctrlPoint).length() > radius)
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
            VSpline preSpl(spl.GetP1(), spl.GetP2(), ctrlLine.p2(), VPointF(pSpl));

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

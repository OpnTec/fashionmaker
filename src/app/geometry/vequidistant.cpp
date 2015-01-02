/************************************************************************
 **
 **  @file   vequidistant.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 1, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "varc.h"
#include "vequidistant.h"
#include "vpointf.h"
#include "vspline.h"
#include "vsplinepath.h"
#include "../core/vapplication.h"
#include <QDebug>
#include <QPainterPath>
#include "../container/vcontainer.h"
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
VEquidistant::VEquidistant(const VContainer *data)
    :data(data)
{
    SCASSERT(data != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VEquidistant::ContourPath(const quint32 &idDetail) const
{
    SCASSERT(data != nullptr);
    VDetail detail = data->GetDetail(idDetail);
    QVector<QPointF> points;
    QVector<QPointF> pointsEkv;
    for (int i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(detail.at(i).getId());
                points.append(point->toQPointF());
                if (detail.getSeamAllowance() == true)
                {
                    QPointF pEkv = point->toQPointF();
                    pEkv.setX(pEkv.x()+detail.at(i).getMx());
                    pEkv.setY(pEkv.y()+detail.at(i).getMy());
                    pointsEkv.append(pEkv);
                }
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve=data->GeometricObject<VAbstractCurve>(detail.at(i).getId());

                const QPointF begin = StartSegment(detail, i);
                const QPointF end = EndSegment(detail, i);

                QVector<QPointF> nodePoints = curve->GetSegmentPoints(begin, end, detail.at(i).getReverse());
                points << nodePoints;
                if (detail.getSeamAllowance() == true)
                {
                    pointsEkv << biasPoints(nodePoints, detail.at(i).getMx(), detail.at(i).getMy());
                }
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(detail.at(i).getTypeTool());
                break;
        }
    }

    QPainterPath path;
    path.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    pointsEkv = CorrectEquidistantPoints(pointsEkv);
    pointsEkv = CheckLoops(pointsEkv);

    if (detail.getSeamAllowance() == true)
    {
        QPainterPath ekv;
        if (detail.getClosed() == true)
        {
            ekv = Equidistant(pointsEkv, EquidistantType::CloseEquidistant, qApp->toPixel(detail.getWidth()));
        }
        else
        {
            ekv = Equidistant(pointsEkv, EquidistantType::OpenEquidistant, qApp->toPixel(detail.getWidth()));
        }
        path.addPath(ekv);
        path.setFillRule(Qt::WindingFill);
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VEquidistant::StartSegment(const VDetail &detail, const int &i) const
{
    QPointF begin;
    if (detail.CountNode() > 1)
    {
        if (i == 0)
        {
            if (detail.at(detail.CountNode()-1).getTypeTool() == Tool::NodePoint)
            {
                begin = data->GeometricObject<VPointF>(detail.at(detail.CountNode()-1).getId())->toQPointF();
            }
        }
        else
        {
            if (detail.at(i-1).getTypeTool() == Tool::NodePoint)
            {
                begin = data->GeometricObject<VPointF>(detail.at(i-1).getId())->toQPointF();
            }
        }
    }
    return begin;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VEquidistant::EndSegment(const VDetail &detail, const int &i) const
{
    QPointF end;
    if (detail.CountNode() > 2)
    {
        if (i == detail.CountNode() - 1)
        {
            if (detail.at(0).getTypeTool() == Tool::NodePoint)
            {
                end = data->GeometricObject<VPointF>(detail.at(0).getId())->toQPointF();
            }
        }
        else
        {
            if (detail.at(i+1).getTypeTool() == Tool::NodePoint)
            {
                end = data->GeometricObject<VPointF>(detail.at(i+1).getId())->toQPointF();
            }
        }
    }
    return end;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VEquidistant::biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my)
{
    QVector<QPointF> p;
    for (qint32 i = 0; i < points.size(); ++i)
    {
        QPointF point = points.at(i);
        point.setX(point.x() + mx);
        point.setY(point.y() + my);
        p.append(point);
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VEquidistant::CorrectEquidistantPoints(const QVector<QPointF> &points)
{
    QVector<QPointF> correctPoints;
    if (points.size()<4)//Better don't check if only three points. We can destroy equidistant.
    {
        qDebug()<<"Only three points.";
        return points;
    }
    //Clear equivalent points
    for (qint32 i = 0; i <points.size(); ++i)
    {
        if (i == points.size()-1)
        {
            correctPoints.append(points.at(i));
            continue;
        }
        if (points.at(i) == points.at(i+1))
        {
            correctPoints.append(points.at(i));
            ++i;
        }
        else
        {
            correctPoints.append(points.at(i));
        }
    }
    if (correctPoints.size()<3)
    {
        return correctPoints;
    }
    //Remove point on line
    QPointF point;
    for (qint32 i = 1; i <correctPoints.size()-1; ++i)
    {
        QLineF l1(correctPoints.at(i-1), correctPoints.at(i));
        QLineF l2(correctPoints.at(i), correctPoints.at(i+1));
        QLineF::IntersectType intersect = l1.intersect(l2, &point);
        if (intersect == QLineF::NoIntersection)
        {
            correctPoints.remove(i);
        }
    }
    return correctPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VEquidistant::Equidistant(QVector<QPointF> points, const EquidistantType &eqv, const qreal &width)
{
    QPainterPath ekv;
    QVector<QPointF> ekvPoints;
    if ( points.size() < 3 )
    {
        qDebug()<<"Not enough points for building the equidistant.";
        return ekv;
    }
    if (width <= 0)
    {
        qDebug()<<"Width <= 0.";
        return ekv;
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if (i != points.size()-1)
        {
            if (points.at(i) == points.at(i+1))
            {
                points.remove(i+1);
            }
        }
        else
        {
            if (points.at(i) == points.at(0))
            {
                points.remove(i);
            }
        }
    }
    if (eqv == EquidistantType::CloseEquidistant)
    {
        points.append(points.at(0));
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if ( i == 0 && eqv == EquidistantType::CloseEquidistant)
        {//first point, polyline closed
            ekvPoints<<EkvPoint(QLineF(points.at(points.size()-2), points.at(points.size()-1)),
                                QLineF(points.at(1), points.at(0)), width);
            continue;
        }
        else if (i == 0 && eqv == EquidistantType::OpenEquidistant)
        {//first point, polyline doesn't closed
            ekvPoints.append(SingleParallelPoint(QLineF(points.at(0), points.at(1)), 90, width));
            continue;
        }
        if (i == points.size()-1 && eqv == EquidistantType::CloseEquidistant)
        {//last point, polyline closed
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == points.size()-1 && eqv == EquidistantType::OpenEquidistant)
        {//last point, polyline doesn't closed
                ekvPoints.append(SingleParallelPoint(QLineF(points.at(points.size()-1), points.at(points.size()-2)),
                                                     -90, width));
                continue;
        }
        //points in the middle of polyline
        ekvPoints<<EkvPoint(QLineF(points.at(i-1), points.at(i)), QLineF(points.at(i+1), points.at(i)), width);
    }
    ekvPoints = CheckLoops(ekvPoints);
    ekv.moveTo(ekvPoints.at(0));
    for (qint32 i = 1; i < ekvPoints.count(); ++i)
    {
        ekv.lineTo(ekvPoints.at(i));
    }
    return ekv;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VEquidistant::CheckLoops(const QVector<QPointF> &points)
{
    QVector<QPointF> ekvPoints;
    /*If we got less than 4 points no need seek loops.*/
    if (points.size() < 4)
    {
        qDebug()<<"Less then 4 points. Doesn't need check for loops.";
        return points;
    }
    bool closed = false;
    if (points.at(0) == points.at(points.size()-1))
    {
        closed = true;
    }
    qint32 i, j;
    for (i = 0; i < points.size(); ++i)
    {
        /*Last three points no need check.*/
        if (i >= points.size()-3)
        {
            ekvPoints.append(points.at(i));
            continue;
        }
        QPointF crosPoint;
        QLineF::IntersectType intersect = QLineF::NoIntersection;
        QLineF line1(points.at(i), points.at(i+1));
        for (j = i+2; j < points.size()-1; ++j)
        {
            QLineF line2(points.at(j), points.at(j+1));
            intersect = line1.intersect(line2, &crosPoint);
            if (intersect == QLineF::BoundedIntersection)
            {
                break;
            }
        }
        if (intersect == QLineF::BoundedIntersection)
        {
            if (i == 0 && j+1 == points.size()-1 && closed)
            {
                /*We got closed contour.*/
                ekvPoints.append(points.at(i));
            }
            else
            {
                /*We found loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(crosPoint);
                i = j;
            }
        }
        else
        {
            /*We did not found loop.*/
            ekvPoints.append(points.at(i));
        }
    }
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VEquidistant::EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width)
{
    SCASSERT(width > 0);
    QVector<QPointF> points;
    if (line1.p2() != line2.p2())
    {
        qDebug()<<"Last points of two lines must be equal.";
        return QVector<QPointF>();
    }
    QPointF CrosPoint;
    QLineF bigLine1 = ParallelLine(line1, width );
    QLineF bigLine2 = ParallelLine(QLineF(line2.p2(), line2.p1()), width );
    QLineF::IntersectType type = bigLine1.intersect( bigLine2, &CrosPoint );
    switch (type)
    {
        case (QLineF::BoundedIntersection):
            points.append(CrosPoint);
            return points;
            break;
        case (QLineF::UnboundedIntersection):
        {
                QLineF line( line1.p2(), CrosPoint );
                const qreal length = line.length();
                if (length > width*2.4)
                { // Cutting too long an acute angle
                    line.setLength(width); // Not sure about width value here
                    QLineF cutLine(line.p2(), CrosPoint); // Cut line is a perpendicular
                    cutLine.setLength(length); // Decided take this length

                    // We do not check intersection type because intersection must alwayse exist
                    QPointF px;
                    cutLine.setAngle(cutLine.angle()+90);
                    QLineF::IntersectType type = bigLine1.intersect( cutLine, &px );
                    if (type == QLineF::NoIntersection)
                    {
                        qDebug()<<"Couldn't find intersection with cut line.";
                    }
                    points.append(px);

                    cutLine.setAngle(cutLine.angle()-180);
                    type = bigLine2.intersect( cutLine, &px );
                    if (type == QLineF::NoIntersection)
                    {
                        qDebug()<<"Couldn't find intersection with cut line.";
                    }
                    points.append(px);
                }
                else
                {
                    points.append(CrosPoint);
                    return points;
                }
            break;
        }
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line.*/
            points.append(bigLine1.p2());
            return points;
            break;
        default:
            break;
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VEquidistant::ParallelLine(const QLineF &line, qreal width)
{
    SCASSERT(width > 0);
    QLineF paralel = QLineF (SingleParallelPoint(line, 90, width), SingleParallelPoint(QLineF(line.p2(), line.p1()),
                                                                                       -90, width));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VEquidistant::SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width)
{
    SCASSERT(width > 0);
    QLineF pLine = line;
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

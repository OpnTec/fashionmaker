/************************************************************************
 **
 **  @file   vequidistant.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vequidistant.h"

QPainterPath VEquidistant::ContourPath(const quint32 &idDetail, const VContainer *data) const
{
    Q_CHECK_PTR(data);
    VDetail detail = data->GetDetail(idDetail);
    QVector<QPointF> points;
    QVector<QPointF> pointsEkv;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const VPointF *point = data->GeometricObject<const VPointF*>(detail.at(i).getId());
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
            {
                const VArc *arc = data->GeometricObject<const VArc *>(detail.at(i).getId());
                qreal len1 = GetLengthContour(points, arc->GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(arc->GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << arc->GetPoints();
                    if (detail.getSeamAllowance() == true)
                    {
                        pointsEkv << biasPoints(arc->GetPoints(), detail.at(i).getMx(), detail.at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(arc->GetPoints());
                    if (detail.getSeamAllowance() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(arc->GetPoints()), detail.at(i).getMx(),
                                                detail.at(i).getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSpline):
            {
                const VSpline *spline = data->GeometricObject<const VSpline *>(detail.at(i).getId());
                qreal len1 = GetLengthContour(points, spline->GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(spline->GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << spline->GetPoints();
                    if (detail.getSeamAllowance() == true)
                    {
                        pointsEkv << biasPoints(spline->GetPoints(), detail.at(i).getMx(), detail.at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(spline->GetPoints());
                    if (detail.getSeamAllowance() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(spline->GetPoints()), detail.at(i).getMx(),
                                                detail.at(i).getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSplinePath):
            {
                const VSplinePath *splinePath = data->GeometricObject<const VSplinePath *>(detail.at(i).getId());
                qreal len1 = GetLengthContour(points, splinePath->GetPathPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(splinePath->GetPathPoints()));
                if (len1 <= lenReverse)
                {
                    points << splinePath->GetPathPoints();
                    if (detail.getSeamAllowance() == true)
                    {
                     pointsEkv << biasPoints(splinePath->GetPathPoints(), detail.at(i).getMx(), detail.at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(splinePath->GetPathPoints());
                    if (detail.getSeamAllowance() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(splinePath->GetPathPoints()), detail.at(i).getMx(),
                                                detail.at(i).getMy());
                    }
                }
            }
            break;
            default:
                qWarning()<<"Get wrong tool type. Ignore."<<detail.at(i).getTypeTool();
                break;
        }
    }

    QPainterPath path;
    path.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points[i]);
    }
    path.lineTo(points[0]);

    pointsEkv = CorrectEquidistantPoints(pointsEkv);
    pointsEkv = CheckLoops(pointsEkv);

    if (detail.getSeamAllowance() == true)
    {
        QPainterPath ekv;
        if (detail.getClosed() == true)
        {
            ekv = Equidistant(pointsEkv, Detail::CloseEquidistant, toPixel(detail.getWidth()));
        }
        else
        {
            ekv = Equidistant(pointsEkv, Detail::OpenEquidistant, toPixel(detail.getWidth()));
        }
        path.addPath(ekv);
        path.setFillRule(Qt::WindingFill);
    }
    return path;
}

qreal VEquidistant::GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints)
{
    qreal length = 0;
    QVector<QPointF> points;
    points << contour << newPoints;
    for (qint32 i = 0; i < points.size()-1; ++i)
    {
        QLineF line(points.at(i), points.at(i+1));
        length += line.length();
    }
    return length;
}

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

QVector<QPointF> VEquidistant::CorrectEquidistantPoints(const QVector<QPointF> &points)
{
    QVector<QPointF> correctPoints;
    if (points.size()<4)//Better don't check if only three points. We can destroy equidistant.
    {
        qWarning()<<"Only three points.";
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
        QLineF l1(correctPoints[i-1], correctPoints[i]);
        QLineF l2(correctPoints[i], correctPoints[i+1]);
        QLineF::IntersectType intersect = l1.intersect(l2, &point);
        if (intersect == QLineF::NoIntersection)
        {
            correctPoints.remove(i);
        }
    }
    return correctPoints;
}

QPainterPath VEquidistant::Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv,
                                       const qreal &width) const
{
    QPainterPath ekv;
    QVector<QPointF> ekvPoints;
    if ( points.size() < 3 )
    {
        qDebug()<<"Not enough points for building the equidistant.\n";
        return ekv;
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if (i != points.size()-1)
        {
            if (points[i] == points[i+1])
            {
                points.remove(i+1);
            }
        }
        else
        {
            if (points[i] == points[0])
            {
                points.remove(i);
            }
        }
    }
    if (eqv == Detail::CloseEquidistant)
    {
        points.append(points.at(0));
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if ( i == 0 && eqv == Detail::CloseEquidistant)
        {//first point, polyline closed
            ekvPoints<<EkvPoint(QLineF(points[points.size()-2], points[points.size()-1]), QLineF(points[1], points[0]),
                    width);
            continue;
        }
        else if (i == 0 && eqv == Detail::OpenEquidistant)
        {//first point, polyline doesn't closed
            ekvPoints.append(SingleParallelPoint(QLineF(points[0], points[1]), 90, width));
            continue;
        }
        if (i == points.size()-1 && eqv == Detail::CloseEquidistant)
        {//last point, polyline closed
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == points.size()-1 && eqv == Detail::OpenEquidistant)
        {//last point, polyline doesn't closed
                ekvPoints.append(SingleParallelPoint(QLineF(points[points.size()-1], points[points.size()-2]), -90,
                        width));
                continue;
        }
        //points in the middle of polyline
        ekvPoints<<EkvPoint(QLineF(points[i-1], points[i]), QLineF(points[i+1], points[i]), width);
    }
    ekvPoints = CheckLoops(ekvPoints);
    ekv.moveTo(ekvPoints[0]);
    for (qint32 i = 1; i < ekvPoints.count(); ++i)
    {
        ekv.lineTo(ekvPoints[i]);
    }
    return ekv;
}

QVector<QPointF> VEquidistant::CheckLoops(const QVector<QPointF> &points)
{
    QVector<QPointF> ekvPoints;
    /*If we got less than 4 points no need seek loops.*/
    if (points.size() < 4)
    {
       return ekvPoints;
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

QVector<QPointF> VEquidistant::GetReversePoint(const QVector<QPointF> &points)
{
    Q_ASSERT(points.size() > 0);
    QVector<QPointF> reversePoints;
    for (qint32 i = points.size() - 1; i >= 0; --i)
    {
        reversePoints.append(points.at(i));
    }
    return reversePoints;
}

QVector<QPointF> VEquidistant::EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width) const
{
    Q_ASSERT(width > 0);
    QVector<QPointF> points;
    if (line1.p2() != line2.p2())
    {
        qWarning()<<"Last point of two lines must be equal.";
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
                if (line.length() > width + toPixel(8))
                {
                    QLineF lineL = QLineF(bigLine1.p2(), CrosPoint);
                    lineL.setLength(width);
                    points.append(lineL.p2());

                    lineL = QLineF(bigLine2.p1(), CrosPoint);
                    lineL.setLength(width);
                    points.append(lineL.p2());
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

QLineF VEquidistant::ParallelLine(const QLineF &line, qreal width) const
{
    Q_ASSERT(width > 0);
    QLineF paralel = QLineF (SingleParallelPoint(line, 90, width), SingleParallelPoint(QLineF(line.p2(), line.p1()),
                                                                                       -90, width));
    return paralel;
}

QPointF VEquidistant::SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width)
{
    Q_ASSERT(width > 0);
    QLineF pLine = line;
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

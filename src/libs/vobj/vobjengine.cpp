/************************************************************************
 **
 **  @file   vobjengine.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 12, 2014
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

#include "vobjengine.h"

#include <QTextStream>
#include <QDebug>
#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
static inline QPaintEngine::PaintEngineFeatures svgEngineFeatures()
{
#ifdef Q_CC_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

    return QPaintEngine::PaintEngineFeatures(
        QPaintEngine::AllFeatures
        & ~QPaintEngine::PatternBrush
        & ~QPaintEngine::PerspectiveTransform
        & ~QPaintEngine::ConicalGradientFill
        & ~QPaintEngine::PorterDuff);

#ifdef Q_CC_CLANG
#pragma clang diagnostic pop
#endif
}

//---------------------------------------------------------------------------------------------------------------------
VObjEngine::VObjEngine()
    :QPaintEngine(svgEngineFeatures()), stream(nullptr), globalPointsCount(0), outputDevice(nullptr), planeCount(0),
      size(), resolution(96), matrix()
{
    for (int i=0; i < MAX_POINTS; i++)
    {
        points[i].x = 0;
        points[i].y = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VObjEngine::~VObjEngine()
{
    outputDevice = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
bool VObjEngine::begin(QPaintDevice *pdev)
{
    Q_UNUSED(pdev)
    if (outputDevice == nullptr)
    {
        qWarning("VObjEngine::begin(), no output device");
        return false;
    }
    if (outputDevice->isOpen() == false)
    {
        if (outputDevice->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false)
        {
            qWarning("VObjEngine::begin(), could not open output device: '%s'",
                     qPrintable(outputDevice->errorString()));
            return false;
        }
    }
    else if (outputDevice->isWritable() == false)
    {
        qWarning("VObjEngine::begin(), could not write to read-only output device: '%s'",
                 qPrintable(outputDevice->errorString()));
        return false;
    }

    if (size.isValid() == false)
    {
        qWarning()<<"VObjEngine::begin(), size is not valid";
        return false;
    }

    stream = new QTextStream(outputDevice);
    *stream << "# Valentina OBJ File" <<  endl;
    *stream << "# www.valentina-project.org/" <<  endl;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VObjEngine::end()
{
    delete stream;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VObjEngine::updateState(const QPaintEngineState &state)
{
    QPaintEngine::DirtyFlags flags = state.state();

    // always stream full gstate, which is not required, but...
    flags |= QPaintEngine::AllDirty;


    if (flags & QPaintEngine::DirtyTransform)
    {
        matrix = state.matrix(); // Save new matrix for moving paths
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::drawPath(const QPainterPath &path)
{
    QPolygonF polygon = path.toFillPolygon(matrix);
    polygon = MakePointsUnique(polygon);// Points must be unique
    if (polygon.size() < 3)
    {
        return;
    }

    qint64 sq = Square(polygon);

    ++planeCount;
    *stream << "o Plane." << QString("%1").arg(planeCount, 3, 10, QLatin1Char('0')) << endl;

    unsigned int num_points = 0;

    for (int i=0; i < polygon.count(); i++)
    {
        if ( num_points < MAX_POINTS )
        {
            points[num_points].x = polygon.at(i).x();
            points[num_points].y = polygon.at(i).y();
            num_points++;
        }
    }

    int offset = 0;
    delaunay2d_t *res = delaunay2d_from(points, num_points);//Calculate faces

    QPointF pf[MAX_POINTS];
    bool skipFace=false;//Need skip first face

    for (unsigned int i = 0; i < res->num_faces; i++ )
    {
        if (offset == 0)
        {
            skipFace=true;
        }
        else
        {
            skipFace=false;
        }
        int num_verts = static_cast<int>(res->faces[offset]);
        offset++;
        for ( int j = 0; j < num_verts; j++ )
        {
            int p0 = static_cast<int>(res->faces[offset + j]);
            pf[j] = QPointF(points[p0].x, points[p0].y);
        }
        if (skipFace == false )
        {
            QPolygonF face;
            for ( int i = 0; i < num_verts; i++ )
            {
                face << QPointF(pf[i]);
            }
            QPolygonF united = polygon.united(face);
            qint64 sqUnited = Square(united);
            if (sqUnited <= sq)
            {// This face incide our base polygon.
                drawPolygon(pf, num_verts, QPaintEngine::OddEvenMode);
            }
        }
        offset += num_verts;
    }

    delaunay2d_release(res);//Don't forget release data
    *stream << "s off" << endl;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    Q_UNUSED(mode)

    drawPoints(points, pointCount);
    *stream << "f";

    for (int i = 0; i < pointCount; ++i)
    {
        *stream << QString(" %1").arg(globalPointsCount - static_cast<unsigned int>(pointCount) + i + 1);
    }
    *stream << endl;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::drawPolygon(const QPoint *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    QPaintEngine::drawPolygon(points, pointCount, mode);
}

//---------------------------------------------------------------------------------------------------------------------
QPaintEngine::Type VObjEngine::type() const
{
    return QPaintEngine::User;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::drawPoints(const QPointF *points, int pointCount)
{
    for (int i = 0; i < pointCount; ++i)
    {
        qreal x = ((points[i].x() - 0)/qFloor(size.width()/2.0)) - 1.0;
        qreal y = (((points[i].y() - 0)/qFloor(size.width()/2.0)) - 1.0)*-1;

        *stream << "v" << " " << QString::number(x, 'f', 6 ) << " " << QString::number(y, 'f', 6 ) << " "
             << "0.000000" << endl;
        ++globalPointsCount;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::drawPoints(const QPoint *points, int pointCount)
{
    QPaintEngine::drawPoints(points, pointCount);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VObjEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    Q_UNUSED(r)
    Q_UNUSED(pm)
    Q_UNUSED(sr)
}

//---------------------------------------------------------------------------------------------------------------------
QSize VObjEngine::getSize() const
{
    return size;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::setSize(const QSize &value)
{
    Q_ASSERT(!isActive());
    size = value;
}

//---------------------------------------------------------------------------------------------------------------------
QIODevice *VObjEngine::getOutputDevice() const
{
    return outputDevice;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::setOutputDevice(QIODevice *value)
{
    Q_ASSERT(!isActive());
    outputDevice = value;
}

//---------------------------------------------------------------------------------------------------------------------
int VObjEngine::getResolution() const
{
    return resolution;
}

//---------------------------------------------------------------------------------------------------------------------
void VObjEngine::setResolution(int value)
{
    Q_ASSERT(!isActive());
    resolution = value;
}

//---------------------------------------------------------------------------------------------------------------------
QPolygonF VObjEngine::MakePointsUnique(const QPolygonF &polygon) const
{
    QVector<QPointF> set;
    QPolygonF uniquePolygon;
    for (int i=0; i < polygon.count(); i++)
    {
        if (set.contains(polygon.at(i)) == false)
        {
            set.append(polygon.at(i));
            uniquePolygon.append(polygon.at(i));
        }
    }
    return uniquePolygon;
}

//---------------------------------------------------------------------------------------------------------------------
qint64 VObjEngine::Square(const QPolygonF &poly) const
{
    QVector<qreal> x;
    QVector<qreal> y;

    int n = poly.count();
    qreal s, res = 0;
    qint64 sq = 0;

    for (int i=0; i < n; i++)
    {
        x.append(poly.at(i).x());
        y.append(poly.at(i).y());
    }

    // Calculation a polygon area through the sum of the areas of trapezoids
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            s = x.at(i)*(y.at(n-1) - y.at(i+1)); //if i == 0, then y[i-1] replace on y[n-1]
            res += s;
        }
        else
        {
            if (i == n-1)
            {
                s = x.at(i)*(y.at(i-1) - y.at(0)); // if i == n-1, then y[i+1] replace on y[0]
                res += s;
            }
            else
            {
                s = x.at(i)*(y.at(i-1) - y.at(i+1));
                res += s;
            }
        }
    }
    sq = qFloor(qAbs(res/2.0));
    return sq;
}

/************************************************************************
 **
 **  @file   vobjengine.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VOBJENGINE_H
#define VOBJENGINE_H

#include <qcompilerdetection.h>
#include <QMatrix>
#include <QPaintEngine>
#include <QPolygonF>
#include <QRectF>
#include <QSharedPointer>
#include <QSize>
#include <QtGlobal>

#include "delaunay.h"

class QTextStream;

#define MAX_POINTS      512

class VObjEngine : public QPaintEngine
{
public:
    VObjEngine();
    virtual ~VObjEngine() override;

    virtual bool begin(QPaintDevice *pdev) override;
    virtual bool end() override;
    virtual void updateState(const QPaintEngineState &state) override;
    virtual void drawPath(const QPainterPath &path) override;
    virtual Type type() const override;
    virtual void drawPoints(const QPointF *points, int pointCount) override;
    virtual void drawPoints(const QPoint *points, int pointCount) override;
    virtual void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override;
    virtual void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode) override;
    virtual void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode) override;

    QSize getSize() const;
    void setSize(const QSize &value);

    QIODevice *getOutputDevice() const;
    void setOutputDevice(QIODevice *value);

    int getResolution() const;
    void setResolution(int value);

private:
    Q_DISABLE_COPY(VObjEngine)
    QSharedPointer<QTextStream> stream;
    quint32     globalPointsCount;
    QSharedPointer<QIODevice> outputDevice;
    del_point2d_t    points[MAX_POINTS];
    quint32     planeCount;
    QSize            size;
    int              resolution;
    QTransform       matrix;

    QPolygonF  MakePointsUnique(const QPolygonF &polygon)const;
    qint64     Square(const QPolygonF &poly)const;
};

#endif // VOBJENGINE_H

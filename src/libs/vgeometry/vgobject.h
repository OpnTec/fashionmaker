/************************************************************************
 **
 **  @file   vgobject.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 12, 2013
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

#ifndef VGOBJECT_H
#define VGOBJECT_H

#include "vgeometrydef.h"
#include <QString>
#include <QtGlobal>
#include <QSharedDataPointer>
#include <QVector>

class VGObjectData;
class QLineF;
class QPointF;
class QRectF;

/**
 * @brief The VGObject class keep information graphical objects.
 */
class VGObject
{
public:
    VGObject();
    VGObject(const GOType &type, const quint32 &idObject = 0, const Draw &mode = Draw::Calculation);
    VGObject(const VGObject &obj);
    VGObject& operator= (const VGObject &obj);
    virtual ~VGObject();

    quint32         getIdObject() const;
    void            setIdObject(const quint32 &value);

    virtual QString name() const;
    void            setName(const QString &name);

    Draw            getMode() const;
    void            setMode(const Draw &value);

    GOType          getType() const;
    void            setType(const GOType &type);

    quint32         id() const;
    virtual void    setId(const quint32 &id);

    static QLineF  BuildLine(const QPointF &p1, const qreal& length, const qreal &angle);
    static QPointF BuildRay(const QPointF &firstPoint, const qreal &angle, const QRectF &scRect);
    static QLineF  BuildAxis(const QPointF &p, const qreal &angle, const QRectF &scRect);
    static QLineF  BuildAxis(const QPointF &p1, const QPointF &p2, const QRectF &scRect);

    static QPointF LineIntersectRect(const QRectF &rec, const QLineF &line);
    static int     IntersectionCircles(const QPointF &c1, double r1, const QPointF &c2, double r2, QPointF &p1,
                                       QPointF &p2);
    static qint32  LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                       QPointF &p2);
    static QPointF ClosestPoint(const QLineF &line, const QPointF &point);
    static QPointF addVector (const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k);
    static void    LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c);
    static bool    IsPointOnLineSegment (const QPointF &t, const QPointF &p1, const QPointF &p2);

    static QVector<QPointF> GetReversePoints(const QVector<QPointF> &points);
    static int GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints);
private:
    QSharedDataPointer<VGObjectData> d;

    static bool    IsPointOnLineviaPDP(const QPointF &t, const QPointF &p1, const QPointF &p2);
    static double  PerpDotProduct(const QPointF &t, const QPointF &p1, const QPointF &p2);
    static double  GetEpsilon(const QPointF &p1, const QPointF &p2);
    static QLineF  CreateSegment(double a, double b, double c);
};

#endif // VGOBJECT_H

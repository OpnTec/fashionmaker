/************************************************************************
 **
 **  @file   vabstractcurve.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VABSTRACTCURVE_H
#define VABSTRACTCURVE_H

#include "vgobject.h"
#include <QPointF>

enum class PathDirection : char { Hide, Show };

class QPainterPath;
class QLineF;

class VAbstractCurve :public VGObject
{
public:
    VAbstractCurve(const GOType &type, const quint32 &idObject = NULL_ID, const Draw &mode = Draw::Calculation);
    VAbstractCurve(const VAbstractCurve &curve);
    VAbstractCurve& operator= (const VAbstractCurve &curve);

    virtual QVector<QPointF> GetPoints() const =0;
    QVector<QPointF>         GetSegmentPoints(const QPointF &begin, const QPointF &end, bool reverse = false) const;

    virtual QPainterPath     GetPath(PathDirection direction = PathDirection::Hide) const;
    virtual qreal            GetLength() const =0;
    virtual QVector<QPointF> IntersectLine(const QLineF &line) const;
protected:
    QPainterPath             ShowDirection(const QVector<QPointF> &points) const;
private:
    static QVector<QPointF>  FromBegin(const QVector<QPointF> &points, const QPointF &begin);
    QVector<QPointF>         ToEnd(const QVector<QPointF> &points, const QPointF &end) const;
};

#endif // VABSTRACTCURVE_H

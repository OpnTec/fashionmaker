/************************************************************************
 **
 **  @file   vabstractcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
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

#ifndef VABSTRACTCUBICBEZIERPATH_H
#define VABSTRACTCUBICBEZIERPATH_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "vabstractbezier.h"
#include "vgeometrydef.h"

class VPointF;
class VSpline;
class VSplinePoint;

class VAbstractCubicBezierPath : public VAbstractBezier
{
    Q_DECLARE_TR_FUNCTIONS(VAbstractCubicBezierPath)
public:
    VAbstractCubicBezierPath(const GOType &type, const quint32 &idObject = NULL_ID,
                             const Draw &mode = Draw::Calculation);
    VAbstractCubicBezierPath(const VAbstractCubicBezierPath &curve);
    VAbstractCubicBezierPath& operator= (const VAbstractCubicBezierPath &curve);
    virtual ~VAbstractCubicBezierPath();

    virtual qint32  CountSubSpl() const =0;
    virtual qint32  CountPoints() const =0;
    virtual void    Clear() =0;
    virtual VSpline GetSpline(qint32 index) const =0;
    virtual QVector<VSplinePoint> GetSplinePath() const =0;

    virtual QPainterPath     GetPath(PathDirection direction = PathDirection::Hide) const Q_DECL_OVERRIDE;
    virtual QVector<QPointF> GetPoints() const Q_DECL_OVERRIDE;
    virtual qreal            GetLength() const Q_DECL_OVERRIDE;

    int Segment(const QPointF &p) const;

    QPointF CutSplinePath(qreal length, qint32 &p1, qint32 &p2, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2,
                          QPointF &spl2p3) const;

    virtual QString NameForHistory(const QString &toolName) const Q_DECL_OVERRIDE;

protected:
    virtual void CreateName() Q_DECL_OVERRIDE;

    virtual VPointF FirstPoint() const =0;
    virtual VPointF LastPoint() const =0;
};

#endif // VABSTRACTCUBICBEZIERPATH_H

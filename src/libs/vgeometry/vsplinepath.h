/************************************************************************
 **
 **  @file   vsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QPainterPath>
#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>

#include "vabstractcubicbezierpath.h"
#include "vgeometrydef.h"
#include "vpointf.h"
#include "vspline.h"
#include "vsplinepoint.h"

class QPointF;
class VSplinePathData;

/**
 * @brief The VSplinePath class keep information about splinePath.
 */
class VSplinePath :public VAbstractCubicBezierPath
{
    Q_DECLARE_TR_FUNCTIONS(VSplinePath)
public:
    explicit VSplinePath(quint32 idObject = 0, Draw mode = Draw::Calculation);
    VSplinePath(const QVector<VFSplinePoint> &points, qreal kCurve = 1, quint32 idObject = 0,
                Draw mode = Draw::Calculation);
    VSplinePath(const QVector<VSplinePoint> &points, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VSplinePath(const VSplinePath& splPath);
    VSplinePath Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix = QString()) const;
    virtual ~VSplinePath() Q_DECL_OVERRIDE;

    VSplinePath  &operator=(const VSplinePath &path);
    VSplinePoint &operator[](int indx);

    void   append(const VSplinePoint &point);

    virtual qint32  CountSubSpl() const Q_DECL_OVERRIDE;
    virtual qint32  CountPoints() const Q_DECL_OVERRIDE;
    virtual void    Clear() Q_DECL_OVERRIDE;
    virtual VSpline GetSpline(qint32 index) const Q_DECL_OVERRIDE;

    virtual QVector<VSplinePoint> GetSplinePath() const Q_DECL_OVERRIDE;
    QVector<VFSplinePoint> GetFSplinePath() const;

    virtual qreal GetStartAngle () const Q_DECL_OVERRIDE;
    virtual qreal GetEndAngle () const Q_DECL_OVERRIDE;

    void         UpdatePoint(qint32 indexSpline, const SplinePointPosition &pos, const VSplinePoint &point);
    VSplinePoint GetSplinePoint(qint32 indexSpline, SplinePointPosition pos) const;

    const VSplinePoint &at(int indx) const;
protected:
    virtual VPointF FirstPoint() const  Q_DECL_OVERRIDE;
    virtual VPointF LastPoint() const  Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VSplinePathData> d;
};

Q_DECLARE_TYPEINFO(VSplinePath, Q_MOVABLE_TYPE);

#endif // VSPLINEPATH_H

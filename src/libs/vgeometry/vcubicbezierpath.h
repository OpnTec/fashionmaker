/************************************************************************
 **
 **  @file   vcubicbezierpath.h
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

#ifndef VCUBICBEZIERPATH_H
#define VCUBICBEZIERPATH_H

#include "vabstractcubicbezierpath.h"
#include "vpointf.h"

#include <QCoreApplication>

class VCubicBezierPathData;
class VSpline;

class VCubicBezierPath : public VAbstractCubicBezierPath
{
    Q_DECLARE_TR_FUNCTIONS(VCubicBezierPath)
public:
    explicit VCubicBezierPath(quint32 idObject = 0, Draw mode = Draw::Calculation);
    VCubicBezierPath(const VCubicBezierPath &curve);
    VCubicBezierPath(const QVector<VPointF> &points, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VCubicBezierPath &operator=(const VCubicBezierPath &curve);
    virtual ~VCubicBezierPath();

    VPointF &operator[](int indx);

    const VPointF &at(int indx) const;

    void   append(const VPointF &point);

    virtual qint32  CountSubSpl() const Q_DECL_OVERRIDE;
    virtual qint32  CountPoints() const Q_DECL_OVERRIDE;
    virtual void    Clear() Q_DECL_OVERRIDE;
    virtual VSpline GetSpline(qint32 index) const Q_DECL_OVERRIDE;
    virtual qreal   GetStartAngle () const Q_DECL_OVERRIDE;
    virtual qreal   GetEndAngle () const Q_DECL_OVERRIDE;

    virtual QVector<VSplinePoint> GetSplinePath() const Q_DECL_OVERRIDE;
    QVector<VPointF> GetCubicPath() const;

    static qint32 CountSubSpl(qint32 size);
    static qint32 SubSplOffset(qint32 subSplIndex);
    static qint32 SubSplPointsCount(qint32 countSubSpl);
protected:
    virtual VPointF FirstPoint() const  Q_DECL_OVERRIDE;
    virtual VPointF LastPoint() const  Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VCubicBezierPathData> d;
};

Q_DECLARE_TYPEINFO(VCubicBezierPath, Q_MOVABLE_TYPE);

#endif // VCUBICBEZIERPATH_H

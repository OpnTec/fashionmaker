/************************************************************************
 **
 **  @file   vcubicbezier.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 3, 2016
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

#ifndef VCUBICBEZIER_H
#define VCUBICBEZIER_H

#include <qcompilerdetection.h>
#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>

#include "vabstractcubicbezier.h"
#include "vgeometrydef.h"
#include "vpointf.h"

class VCubicBezierData;

class VCubicBezier : public VAbstractCubicBezier
{
public:
    VCubicBezier();
    VCubicBezier(const VCubicBezier &curve);
    VCubicBezier(const VPointF &p1, const VPointF &p2, const VPointF &p3, const VPointF &p4, quint32 idObject = 0,
                 Draw mode = Draw::Calculation);
    VCubicBezier Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix = QString()) const;
    VCubicBezier Flip(const QLineF &axis, const QString &prefix = QString()) const;
    VCubicBezier Move(qreal length, qreal angle, const QString &prefix = QString()) const;
    virtual ~VCubicBezier();

    VCubicBezier &operator=(const VCubicBezier &curve);
#ifdef Q_COMPILER_RVALUE_REFS
    VCubicBezier &operator=(VCubicBezier &&curve) Q_DECL_NOTHROW { Swap(curve); return *this; }
#endif

    void Swap(VCubicBezier &curve) Q_DECL_NOTHROW
    { std::swap(d, curve.d); }

    virtual VPointF GetP1() const Q_DECL_OVERRIDE;
    void            SetP1(const VPointF &p);

    virtual VPointF GetP2() const Q_DECL_OVERRIDE;
    void    SetP2(const VPointF &p);

    virtual VPointF GetP3() const Q_DECL_OVERRIDE;
    void    SetP3(const VPointF &p);

    virtual VPointF GetP4() const Q_DECL_OVERRIDE;
    void            SetP4(const VPointF &p);

    virtual qreal            GetStartAngle() const Q_DECL_OVERRIDE;
    virtual qreal            GetEndAngle() const Q_DECL_OVERRIDE;
    virtual qreal            GetLength() const Q_DECL_OVERRIDE;
    virtual QVector<QPointF> GetPoints() const Q_DECL_OVERRIDE;

    virtual qreal GetC1Length() const Q_DECL_OVERRIDE;
    virtual qreal GetC2Length() const Q_DECL_OVERRIDE;

protected:
    virtual QPointF GetControlPoint1() const Q_DECL_OVERRIDE;
    virtual QPointF GetControlPoint2() const Q_DECL_OVERRIDE;

private:
    QSharedDataPointer<VCubicBezierData> d;
};

Q_DECLARE_TYPEINFO(VCubicBezier, Q_MOVABLE_TYPE);

#endif // VCUBICBEZIER_H

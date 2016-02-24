/************************************************************************
 **
 **  @file   vspline.h
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

#ifndef VSPLINE_H
#define VSPLINE_H

#include "vabstractcurve.h"
#include "vpointf.h"
#include <QLineF>
#include <QPointF>

class QPainterPath;
class VSplineData;

#define M_2PI   6.28318530717958647692528676655900576

/**
 * @brief VSpline class that implements the spline.
 */
class VSpline :public VAbstractCurve
{
public:
    VSpline();
    VSpline (const VSpline &spline );
    VSpline (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve,
             quint32 idObject = 0, Draw mode = Draw::Calculation);
    VSpline (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve, quint32 idObject = 0,
             Draw mode = Draw::Calculation);
    VSpline (VPointF p1, VPointF p4, qreal angle1, const QString &angle1Formula, qreal angle2,
             const QString &angle2Formula, qreal c1Length, const QString &c1LengthFormula, qreal c2Length,
             const QString &c2LengthFormula, qreal kCurve, quint32 idObject = 0, Draw mode = Draw::Calculation);
    virtual ~VSpline() Q_DECL_OVERRIDE;
    VSpline &operator=(const VSpline &spl);
    VPointF GetP1 () const;
    QPointF GetP2 () const;
    QPointF GetP3 () const;
    VPointF GetP4 () const;

    virtual qreal GetStartAngle () const Q_DECL_OVERRIDE;
    virtual qreal GetEndAngle() const Q_DECL_OVERRIDE;

    QString GetStartAngleFormula () const;
    QString GetEndAngleFormula() const;

    qreal   GetC1Length() const;
    qreal   GetC2Length() const;

    QString C1LengthFormula() const;
    QString C2LengthFormula() const;

    qreal   GetLength () const;
    qreal   GetKasm1() const;
    qreal   GetKasm2() const;
    qreal   GetKcurve() const;
    void    SetKcurve(qreal factor);
    qreal   LengthT(qreal t) const;
    QPointF CutSpline ( qreal length, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2, QPointF &spl2p3) const;
    QPointF CutSpline ( qreal length, VSpline &spl1, VSpline &spl2) const;
    QVector<QPointF> GetPoints () const;
    // cppcheck-suppress unusedFunction
    static QVector<QPointF> SplinePoints(const QPointF &p1, const QPointF &p4, qreal angle1, qreal angle2, qreal kAsm1,
                                         qreal kAsm2, qreal kCurve);
    qreal   ParamT(const QPointF &pBt) const;
protected:
    static QVector<QPointF> GetPoints (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 );
    virtual void CreateName() Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VSplineData> d;
    static qreal   LengthBezier (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 );
    static void    PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4,
                                  qint16 level, QVector<qreal> &px, QVector<qreal> &py);
    static qreal   CalcSqDistance ( qreal x1, qreal y1, qreal x2, qreal y2);
    QVector<qreal> CalcT(qreal curveCoord1, qreal curveCoord2, qreal curveCoord3, qreal curveCoord4,
                         qreal pointCoord) const;
    static qint32  Cubic(QVector<qreal> &x, qreal a, qreal b, qreal c);
    static int     Sign(long double ld);
};

Q_DECLARE_TYPEINFO(VSpline, Q_MOVABLE_TYPE);

#endif // VSPLINE_H

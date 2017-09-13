/************************************************************************
 **
 **  @file   vellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   February 1, 2016
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

#ifndef VELLIPTICALARC_H
#define VELLIPTICALARC_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>

#include "vabstractarc.h"
#include "vgeometrydef.h"
#include "vpointf.h"

class VEllipticalArcData;

class VEllipticalArc : public VAbstractArc
{
    Q_DECLARE_TR_FUNCTIONS(VEllipticalArc)
public:
    VEllipticalArc();
    VEllipticalArc (const VPointF &center, qreal radius1, qreal radius2, const QString &formulaRadius1,
                    const QString &formulaRadius2, qreal f1, const QString &formulaF1, qreal f2,
                    const QString &formulaF2, qreal rotationAngle, const QString &formulaRotationAngle,
                    quint32 idObject = 0, Draw mode = Draw::Calculation);
    VEllipticalArc (const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal f2, qreal rotationAngle);
    VEllipticalArc (qreal length, const QString &formulaLength, const VPointF &center, qreal radius1, qreal radius2,
                    const QString &formulaRadius1, const QString &formulaRadius2, qreal f1, const QString &formulaF1,
                    qreal rotationAngle, const QString &formulaRotationAngle, quint32 idObject = 0,
                    Draw mode = Draw::Calculation);
    VEllipticalArc (qreal length, const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal rotationAngle);
    VEllipticalArc(const VEllipticalArc &arc);

    VEllipticalArc Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix = QString()) const;
    VEllipticalArc Flip(const QLineF &axis, const QString &prefix = QString()) const;
    VEllipticalArc Move(qreal length, qreal angle, const QString &prefix = QString()) const;

    virtual ~VEllipticalArc() Q_DECL_OVERRIDE;

    VEllipticalArc& operator= (const VEllipticalArc &arc);
#ifdef Q_COMPILER_RVALUE_REFS
    VEllipticalArc &operator=(VEllipticalArc &&arc) Q_DECL_NOTHROW { Swap(arc); return *this; }
#endif

    void Swap(VEllipticalArc &arc) Q_DECL_NOTHROW
    { VAbstractArc::Swap(arc); std::swap(d, arc.d); }

    QString GetFormulaRotationAngle () const;
    void    SetFormulaRotationAngle (const QString &formula, qreal value);
    qreal   GetRotationAngle() const;

    QString GetFormulaRadius1 () const;
    void    SetFormulaRadius1 (const QString &formula, qreal value);
    qreal   GetRadius1 () const;

    QString GetFormulaRadius2 () const;
    void    SetFormulaRadius2 (const QString &formula, qreal value);
    qreal   GetRadius2 () const;

    virtual qreal GetLength () const Q_DECL_OVERRIDE;

    QPointF GetP1() const;
    QPointF GetP2() const;

    virtual QVector<QPointF> GetPoints () const Q_DECL_OVERRIDE;

    QPointF CutArc (const qreal &length, VEllipticalArc &arc1, VEllipticalArc &arc2) const;
    QPointF CutArc (const qreal &length) const;
protected:
    virtual void CreateName() Q_DECL_OVERRIDE;
    virtual void FindF2(qreal length) Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VEllipticalArcData> d;

    qreal MaxLength() const;
};

Q_DECLARE_METATYPE(VEllipticalArc)
Q_DECLARE_TYPEINFO(VEllipticalArc, Q_MOVABLE_TYPE);

#endif // VELLIPTICALARC_H

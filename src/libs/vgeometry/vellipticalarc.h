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

#include "vabstractcurve.h"
#include "vpointf.h"
#include <QCoreApplication>

class VEllipticalArcData;

class VEllipticalArc : public VAbstractCurve
{
    Q_DECLARE_TR_FUNCTIONS(VEllipticalArc)
public:
    VEllipticalArc();
    VEllipticalArc (VPointF center, qreal radius1, qreal radius2, QString formulaRadius1, QString formulaRadius2, qreal f1,
          QString formulaF1, qreal f2, QString formulaF2, qreal rotationAngle, quint32 idObject = 0, Draw mode = Draw::Calculation);

    VEllipticalArc (VPointF center, qreal radius1, qreal radius2, qreal f1, qreal f2, qreal rotationAngle);

    VEllipticalArc (qreal length, QString formulaLength, VPointF center, qreal radius1, qreal radius2,
          QString formulaRadius1, QString formulaRadius2, qreal f1,
          QString formulaF1,  quint32 idObject = 0, Draw mode = Draw::Calculation);

    VEllipticalArc (qreal length, VPointF center, qreal radius1, qreal radius2, qreal f1);

    VEllipticalArc(const VEllipticalArc &arc);

    VEllipticalArc& operator= (const VEllipticalArc &arc);

    virtual ~VEllipticalArc() Q_DECL_OVERRIDE;

    QString            GetFormulaF1 () const;
    void               SetFormulaF1 (const QString &formula, qreal value);
    virtual qreal      GetStartAngle () const Q_DECL_OVERRIDE;

    QString            GetFormulaF2 () const;
    void               SetFormulaF2 (const QString &formula, qreal value);
    virtual qreal      GetEndAngle () const Q_DECL_OVERRIDE;

    qreal              GetRotationAngle() const;

    QString            GetFormulaRadius1 () const;
    void               SetFormulaRadius1 (const QString &formula, qreal value);
    qreal              GetRadius1 () const;

    QString            GetFormulaRadius2 () const;
    void               SetFormulaRadius2 (const QString &formula, qreal value);
    qreal              GetRadius2 () const;

    VPointF            GetCenter () const;
    void               SetCenter (const VPointF &value);

    QString            GetFormulaLength () const;
    void               SetFormulaLength (const QString &formula, qreal value);
    qreal              GetLength () const;

    QPointF            GetP1() const;
    QPointF            GetP2 () const;

    qreal              AngleArc() const;
    QVector<qreal>     GetAngles () const;
    QVector<QPointF>   GetPoints () const;
    QPointF            CutArc (const qreal &length, VEllipticalArc &arc1, VEllipticalArc &arc2) const;
    QPointF            CutArc (const qreal &length) const;
    virtual void       setId(const quint32 &id) Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VEllipticalArcData> d;

    void EllipticalArcName();
    void FindF2(qreal length);

    qreal MaxLength() const;
    QPointF GetPoint (qreal angle) const;
};

Q_DECLARE_TYPEINFO(VEllipticalArc, Q_MOVABLE_TYPE);

#endif // VELLIPTICALARC_H

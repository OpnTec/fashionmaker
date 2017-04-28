/************************************************************************
 **
 **  @file   varc.h
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

#ifndef VARC_H
#define VARC_H

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

class VArcData;

/**
 * @brief VArc class for anticlockwise arc.
 */
class VArc: public VAbstractArc
{
    Q_DECLARE_TR_FUNCTIONS(VArc)
public:
    VArc ();
    VArc (const VPointF &center, qreal radius, const QString &formulaRadius, qreal f1, const QString &formulaF1,
          qreal f2, const QString &formulaF2, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VArc (const VPointF &center, qreal radius, qreal f1, qreal f2);
    VArc (qreal length, const QString &formulaLength, const VPointF &center, qreal radius, const QString &formulaRadius,
          qreal f1, const QString &formulaF1,  quint32 idObject = 0, Draw mode = Draw::Calculation);
    VArc (qreal length, const VPointF &center, qreal radius, qreal f1);
    VArc(const VArc &arc);
    VArc Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix = QString()) const;
    VArc Flip(const QLineF &axis, const QString &prefix = QString()) const;
    VArc Move(qreal length, qreal angle, const QString &prefix = QString()) const;
    virtual ~VArc() Q_DECL_OVERRIDE;

    VArc& operator= (const VArc &arc);
#ifdef Q_COMPILER_RVALUE_REFS
    VArc &operator=(VArc &&arc) Q_DECL_NOTHROW { Swap(arc); return *this; }
#endif

    void Swap(VArc &arc) Q_DECL_NOTHROW
    { VAbstractArc::Swap(arc); std::swap(d, arc.d); }

    QString GetFormulaRadius () const;
    void    SetFormulaRadius (const QString &formula, qreal value);
    qreal   GetRadius () const;

    virtual qreal GetLength () const Q_DECL_OVERRIDE;

    QPointF GetP1() const;
    QPointF GetP2 () const;

    virtual QVector<QPointF> GetPoints () const Q_DECL_OVERRIDE;

    QPointF CutArc (const qreal &length, VArc &arc1, VArc &arc2) const;
    QPointF CutArc (const qreal &length) const;
protected:
    virtual void CreateName() Q_DECL_OVERRIDE;
    virtual void FindF2(qreal length) Q_DECL_OVERRIDE;
private:
    QSharedDataPointer<VArcData> d;

    qreal MaxLength() const;
};

Q_DECLARE_TYPEINFO(VArc, Q_MOVABLE_TYPE);

#endif // VARC_H

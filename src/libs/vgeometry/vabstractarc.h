/************************************************************************
 **
 **  @file   vabstractarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
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

#ifndef VABSTRACTARC_H
#define VABSTRACTARC_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QString>
#include <QtGlobal>

#include "../ifc/ifcdef.h"
#include "vabstractcurve.h"
#include "vgeometrydef.h"

class VAbstractArcData;
class VPointF;

class VAbstractArc : public VAbstractCurve
{
public:
    explicit VAbstractArc(const GOType &type, const quint32 &idObject = NULL_ID, const Draw &mode = Draw::Calculation);
    VAbstractArc (const GOType &type, const VPointF &center, qreal f1, const QString &formulaF1, qreal f2,
                  const QString &formulaF2, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VAbstractArc (const GOType &type, const VPointF &center, qreal f1, qreal f2, quint32 idObject = 0,
                  Draw mode = Draw::Calculation);
    VAbstractArc (const GOType &type, const QString &formulaLength, const VPointF &center, qreal f1,
                  const QString &formulaF1, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VAbstractArc (const GOType &type, const VPointF &center, qreal f1, quint32 idObject = 0,
                  Draw mode = Draw::Calculation);
    explicit VAbstractArc(const VAbstractArc &arc);
    VAbstractArc& operator= (const VAbstractArc &arc);
    virtual ~VAbstractArc();

    QString       GetFormulaF1 () const;
    void          SetFormulaF1 (const QString &formula, qreal value);
    virtual qreal GetStartAngle () const Q_DECL_OVERRIDE;

    QString       GetFormulaF2 () const;
    void          SetFormulaF2 (const QString &formula, qreal value);
    virtual qreal GetEndAngle () const Q_DECL_OVERRIDE;

    VPointF GetCenter () const;
    void    SetCenter (const VPointF &point);

    QString GetFormulaLength () const;
    void    SetFormulaLength (const QString &formula, qreal value);

    virtual void    setId(const quint32 &id) Q_DECL_OVERRIDE;
    virtual QString NameForHistory(const QString &toolName) const Q_DECL_OVERRIDE;

    bool IsFlipped() const;
    qreal AngleArc() const;
protected:
    void SetFlipped(bool value);
    virtual void FindF2(qreal length)=0;
    void SetFormulaLength(const QString &formula);
private:
    QSharedDataPointer<VAbstractArcData> d;
};

#endif // VABSTRACTARC_H

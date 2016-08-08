/************************************************************************
 **
 **  @file   vincrementtablerow.h
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

#ifndef VINCREMENTTABLEROW_H
#define VINCREMENTTABLEROW_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QtGlobal>

#include "vvariable.h"

class VContainer;
class VIncrementData;

/**
 * @brief The VIncrement class keep data row of increment table
 */
class VIncrement :public VVariable
{
public:
    VIncrement();
    VIncrement(VContainer *data, const QString &name, quint32 index, qreal base, const QString &formula, bool ok,
               const QString &description = QString());
    VIncrement(const VIncrement &incr);
    VIncrement &operator=(const VIncrement &incr);
    virtual ~VIncrement() Q_DECL_OVERRIDE;

    quint32     getIndex() const;
    QString     GetFormula() const;
    bool        IsFormulaOk() const;
    VContainer *GetData();

private:
    QSharedDataPointer<VIncrementData> d;
};

Q_DECLARE_TYPEINFO(VIncrement, Q_MOVABLE_TYPE);

#endif // VINCREMENTTABLEROW_H

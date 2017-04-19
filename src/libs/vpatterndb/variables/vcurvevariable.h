/************************************************************************
 **
 **  @file   vcurvelength.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#ifndef VCURVEVARIABLE_H
#define VCURVEVARIABLE_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>

#include "vinternalvariable.h"

class VCurveVariableData;

class VCurveVariable : public VInternalVariable
{
public:
    VCurveVariable();
    VCurveVariable(const quint32 &id, const quint32 &parentId);
    VCurveVariable(const VCurveVariable &var);

    virtual ~VCurveVariable() Q_DECL_OVERRIDE;

    VCurveVariable &operator=(const VCurveVariable &var);
#ifdef Q_COMPILER_RVALUE_REFS
    VCurveVariable &operator=(VCurveVariable &&var) Q_DECL_NOTHROW { Swap(var); return *this; }
#endif

    void Swap(VCurveVariable &var) Q_DECL_NOTHROW
    { std::swap(d, var.d); }

    virtual bool Filter(quint32 id) Q_DECL_OVERRIDE;

    quint32      GetId() const;
    void         SetId(const quint32 &id);

    quint32      GetParentId() const;
    void         SetParentId(const quint32 &value);
private:
    QSharedDataPointer<VCurveVariableData> d;
};

Q_DECLARE_TYPEINFO(VCurveVariable, Q_MOVABLE_TYPE);

#endif // VCURVEVARIABLE_H

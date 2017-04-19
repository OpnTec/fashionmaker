/************************************************************************
 **
 **  @file   vlinelength.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
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

#ifndef VLINELENGTH_H
#define VLINELENGTH_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"
#include "vinternalvariable.h"

class VLengthLineData;
class VPointF;

class VLengthLine :public VInternalVariable
{
public:
    VLengthLine();
    VLengthLine(const VPointF *p1, const quint32 &p1Id, const VPointF *p2, const quint32 &p2Id, Unit patternUnit);
    VLengthLine(const VLengthLine &var);

    virtual ~VLengthLine() Q_DECL_OVERRIDE;

    VLengthLine &operator=(const VLengthLine &var);
#ifdef Q_COMPILER_RVALUE_REFS
    VLengthLine &operator=(VLengthLine &&var) Q_DECL_NOTHROW { Swap(var); return *this; }
#endif

    void Swap(VLengthLine &var) Q_DECL_NOTHROW
    { std::swap(d, var.d); }

    virtual bool Filter(quint32 id) Q_DECL_OVERRIDE;
    void         SetValue(const VPointF *p1, const VPointF *p2);
    quint32      GetP1Id() const;
    quint32      GetP2Id() const;
private:
    QSharedDataPointer<VLengthLineData> d;
};

Q_DECLARE_TYPEINFO(VLengthLine, Q_MOVABLE_TYPE);

#endif // VLINELENGTH_H

/************************************************************************
 **
 **  @file   vbestsquare.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VBESTSQUARE_H
#define VBESTSQUARE_H

#include <QSizeF>
#include <QTransform>
#include <QtGlobal>
#include <QSharedDataPointer>
#include <QTypeInfo>

#include "vlayoutdef.h"

class VBestSquareData;

class VBestSquare
{
public:
    VBestSquare();
    VBestSquare(QSizeF sheetSize, bool saveLength, bool isPortrait);
    VBestSquare(const VBestSquare &res);
    virtual ~VBestSquare();

    VBestSquare &operator=(const VBestSquare &res);
#ifdef Q_COMPILER_RVALUE_REFS
    VBestSquare(const VBestSquare &&res) Q_DECL_NOTHROW;
    VBestSquare &operator=(VBestSquare &&res) Q_DECL_NOTHROW;
#endif

    void NewResult(const VBestSquareResData &data);
    void NewResult(const VBestSquare &best);

    QSizeF     BestSize() const;
    int        GContourEdge() const;
    int        DetailEdge() const;
    QTransform Matrix() const;
    bool       HasValidResult() const;
    bool       Mirror() const;
    BestFrom   Type() const;
    bool       IsTerminatedByException() const;
    QString    ReasonTerminatedByException() const;
    void       TerminatedByException(const QString &reason);

    VBestSquareResData BestResultData() const;

    bool IsSaveLength() const;
    bool IsImprovedSidePosition(qreal sidePosition) const;
    bool IsPortrait() const;

private:
    QSharedDataPointer<VBestSquareData> d;

};

Q_DECLARE_TYPEINFO(VBestSquare, Q_MOVABLE_TYPE);

#endif // VBESTSQUARE_H

/************************************************************************
 **
 **  @file   vbank.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VBANK_H
#define VBANK_H

#include <QHash>
#include <QMap>
#include <QRectF>
#include <QVector>
#include <QtGlobal>
#include <QLoggingCategory>

#include "../vmisc/typedef.h"
#include "vlayoutpiece.h"

// An annoying char define, from the Windows team in <rpcndr.h>
// #define small char
// http://stuartjames.info/Journal/c--visual-studio-2012-vs2012--win8--converting-projects-up-some-conflicts-i-found.aspx
#if defined (Q_OS_WIN) && defined (Q_CC_MSVC)
#pragma push_macro("small")
#undef small
#endif

Q_DECLARE_LOGGING_CATEGORY(lBank)

enum class Cases : qint8 { CaseThreeGroup = 0, CaseTwoGroup, CaseDesc, UnknownCase};

class VBank
{
    Q_DECLARE_TR_FUNCTIONS(VBank)
public:
    VBank();

    qreal GetLayoutWidth() const;
    void SetLayoutWidth(qreal value);

    bool GetManualPriority() const;
    void SetManualPriority(bool value);

    bool IsNestQuantity() const;
    void SetNestQuantity(bool value);

    void SetDetails(const QVector<VLayoutPiece> &details);
    int  GetNext();
    VLayoutPiece GetDetail(int i) const;

    void Arranged(int i);
    void NotArranged(int i);

    bool PrepareUnsorted();
    bool PrepareDetails();
    void Reset();
    void SetCaseType(Cases caseType);

    int AllDetailsCount() const;
    int LeftToArrange() const;
    int ArrangedCount() const;

    qreal GetBiggestDiagonal() const;

    bool IsRotationNeeded() const;

private:
    Q_DISABLE_COPY(VBank)
    QVector<VLayoutPiece> details{};

    QMap<uint, QHash<int, qint64>> unsorted{};
    QMap<uint, QHash<int, qint64>> big{};
    QMap<uint, QHash<int, qint64>> middle{};
    QMap<uint, QHash<int, qint64>> small{};
    QMap<uint, QMultiMap<qint64, int>> desc{};

    QVector<uint> groups{};
    QVector<vidtype> arranged{};

    qreal layoutWidth{0};

    Cases caseType{Cases::CaseDesc};
    bool prepare{false};
    qreal diagonal{0};
    bool m_nestQuantity{false};
    bool m_manualPriority{false};

    void PrepareGroup();

    void PrepareThreeGroups(uint priority);
    void PrepareTwoGroups(uint priority);
    void PrepareDescGroup(uint priority);

    int GetNextThreeGroups(uint priority) const;
    int GetNextTwoGroups(uint priority) const;
    int GetNextDescGroup(uint priority) const;

    void SqMaxMin(qint64 &sMax, qint64 &sMin, uint priority) const;

    bool ArrangedDetail(QMap<uint, QHash<int, qint64>> &container, int i);
    bool ArrangedDetail(QMap<uint, QMultiMap<qint64, int>> &container, int i);
};

#if defined (Q_OS_WIN) && defined (Q_CC_MSVC)
#pragma pop_macro("small")
#endif

#endif // VBANK_H

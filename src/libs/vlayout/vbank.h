/************************************************************************
 **
 **  @file   vbank.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VBANK_H
#define VBANK_H

#include <QVector>
#include <QHash>
#include <QRectF>

class QPointF;
class VLayoutDetail;

enum class Cases : char { CaseThreeGroup, CaseTwoGroup, CaseDesc};

class VBank
{
public:
    VBank();

    qreal GetLayoutWidth() const;
    void SetLayoutWidth(const qreal &value);

    void SetDetails(const QVector<VLayoutDetail> &details);
    int  GetTiket();
    VLayoutDetail GetDetail(int i) const;

    void Arranged(int i);
    void NotArranged(int i);

    bool Prepare();
    void Reset();
    void SetCaseType(Cases caseType);

    int AllDetailsCount() const;
    int LeftArrange() const;
    int ArrangedCount() const;

    QRectF GetBiggestBoundingRect() const;

private:
    Q_DISABLE_COPY(VBank)
    QVector<VLayoutDetail> details;
    QHash<int, qint64> unsorted;

    QHash<int, qint64> big;
    QHash<int, qint64> middle;
    QHash<int, qint64> small;

    qreal layoutWidth;

    Cases caseType;
    bool prepare;
    QRectF boundingRect;

    void PrepareGroup();

    void PrepareThreeGroups();
    void PrepareTwoGroups();
    void PrepareDescGroup();

    int GetNextThreeGroups() const;
    int GetNextTwoGroups() const;
    int GetNextDescGroup() const;

    void SqMaxMin(qint64 &sMax, qint64 &sMin) const;
    void BiggestBoundingRect();
};

#endif // VBANK_H

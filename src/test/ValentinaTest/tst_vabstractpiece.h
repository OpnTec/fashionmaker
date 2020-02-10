/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef TST_VABSTRACTPIECE_H
#define TST_VABSTRACTPIECE_H

#include "../vtest/abstracttest.h"

class VSAPoint;

class TST_VAbstractPiece : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_VAbstractPiece(QObject *parent = nullptr);

private slots:
    void EquidistantRemoveLoop_data();
    void EquidistantRemoveLoop() const;
    void SumTrapezoids() const;
    void PathRemoveLoop_data() const;
    void PathRemoveLoop() const;
    void PathLoopsCase_data() const;
    void PathLoopsCase() const;
    void BrokenDetailEquidistant_data();
    void BrokenDetailEquidistant() const;
    void EquidistantAngleType_data();
    void EquidistantAngleType() const;
    void CorrectEquidistantPoints_data();
    void CorrectEquidistantPoints() const;
    void TestCorrectEquidistantPoints_data();
    void TestCorrectEquidistantPoints() const;
#ifndef Q_OS_WIN // Disabled due to "undefined behavior" problem
    void PossibleInfiniteClearLoops_data() const;
    void PossibleInfiniteClearLoops() const;
#endif

private:
    QVector<VSAPoint> InputPointsCase3() const;
    QVector<QPointF>  OutputPointsCase3() const;

    void Case3() const;
    void Case4() const;
    void Case5() const;

    QVector<VSAPoint> InputPointsIssue548Case2() const;
    QVector<QPointF> OutputPointsIssue548Case2() const;

    QVector<VSAPoint> InputPointsIssue548Case3() const;
    QVector<QPointF> OutputPointsIssue548Case3() const;

    QVector<QPointF> InputPointsCase3a() const;
    QVector<QPointF> InputPointsCase4a() const;
    QVector<QPointF> InputPointsCase5a() const;

    QVector<VSAPoint> InputLoopByIntersectionTest();
    QVector<QPointF>  OutputLoopByIntersectionTest();
};

#endif // TST_VABSTRACTPIECE_H

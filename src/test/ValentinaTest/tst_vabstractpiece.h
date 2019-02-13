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
    void BrokenDetailEquidistant_data() const;
    void BrokenDetailEquidistant() const;
    void EquidistantAngleType_data() const;
    void EquidistantAngleType() const;
    void CorrectEquidistantPoints_data() const;
    void CorrectEquidistantPoints() const;
    void TestCorrectEquidistantPoints_data();
    void TestCorrectEquidistantPoints() const;
#ifndef Q_OS_WIN // Disabled due to "undefined behavior" problem
    void PossibleInfiniteClearLoops_data() const;
    void PossibleInfiniteClearLoops() const;
#endif

private:
    QVector<VSAPoint> InputPointsSeamTest1PieceByAngle() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceByAngle() const;

    QVector<VSAPoint> InputPointsSeamTest1PieceByIntersection() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceByIntersection() const;

    QVector<VSAPoint> InputPointsSeamTest1PieceByFirstEdgeSymmerty() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceByFirstEdgeSymmerty() const;

    QVector<VSAPoint> InputPointsSeamTest1PieceBySecondEdgeSymmerty() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceBySecondEdgeSymmerty() const;

    QVector<VSAPoint> InputPointsSeamTest1PieceByFirstRightAngle() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceByFirstRightAngle() const;

    QVector<VSAPoint> InputPointsSeamTest1PieceBySecondRightAngle() const;
    QVector<QPointF>  OutputPointsSeamTest1PieceBySecondRightAngle() const;

    QVector<VSAPoint> InputPointsCase2() const;
    QVector<QPointF>  OutputPointsCase2() const;

    QVector<VSAPoint> InputPointsCase3() const;
    QVector<QPointF>  OutputPointsCase3() const;

    QVector<VSAPoint> InputPointsIssue627() const;
    QVector<QPointF>  OutputPointsIssue627() const;

    QVector<VSAPoint> InputPointsIssue767_Fabric_TopCollar() const;
    QVector<QPointF>  OutputPointsIssue767_Fabric_TopCollar() const;

    QVector<VSAPoint> InputPointsIssue767_SkinFusing_TopCollar_Notch() const;
    QVector<QPointF>  OutputPointsIssue767_SkinFusing_TopCollar_Notch() const;

    QVector<VSAPoint> InputPointsIssue880_Piece_Detail() const;
    QVector<QPointF>  OutputPointsIssue880_Piece_Detail() const;

    QVector<VSAPoint> InputPointsIssue880_Piece_Detail_1() const;
    QVector<QPointF>  OutputPointsIssue880_Piece_Detail_1() const;

    void Case3() const;
    void Case4() const;
    void Case5() const;

    QVector<VSAPoint> InputPointsIssue298Case1() const;
    QVector<QPointF> OutputPointsIssue298Case1() const;

    QVector<VSAPoint> InputPointsIssue298Case2() const;
    QVector<QPointF> OutputPointsIssue298Case2() const;

    QVector<VSAPoint> InputPointsIssue548Case1() const;
    QVector<QPointF> OutputPointsIssue548Case1() const;

    QVector<VSAPoint> InputPointsIssue548Case2() const;
    QVector<QPointF> OutputPointsIssue548Case2() const;

    QVector<VSAPoint> InputPointsIssue548Case3() const;
    QVector<QPointF> OutputPointsIssue548Case3() const;

    QVector<QPointF> InputPointsCase3a() const;
    QVector<QPointF> InputPointsCase4a() const;
    QVector<QPointF> InputPointsCase5a() const;

    QVector<VSAPoint> InputPointsIssue646() const;
    QVector<QPointF>  OutputPointsIssue646() const;

    QVector<VSAPoint> InputPointsIssue923Test1();
    QVector<QPointF>  OutputPointsIssue923Test1();

    QVector<VSAPoint> InputPointsIssue923Test2();
    QVector<QPointF>  OutputPointsIssue923Test2();

    QVector<VSAPoint> InputPointsIssue923Test3();
    QVector<QPointF>  OutputPointsIssue923Test3();

    QVector<VSAPoint> InputPointsIssue923Test4();
    QVector<QPointF>  OutputPointsIssue923Test4();

    QVector<VSAPoint> InputPointsIssue923Test5();
    QVector<QPointF>  OutputPointsIssue923Test5();

    QVector<VSAPoint> InputPointsIssue923Test6();
    QVector<QPointF>  OutputPointsIssue923Test6();

    QVector<VSAPoint> InputPointsIssue923Test1_1();
    QVector<QPointF>  OutputPointsIssue923Test1_1();

    QVector<VSAPoint> InputPointsIssue923Test2_2();
    QVector<QPointF>  OutputPointsIssue923Test2_2();

    QVector<VSAPoint> InputPointsIssue923Test3_3();
    QVector<QPointF>  OutputPointsIssue923Test3_3();

    QVector<VSAPoint> InputPointsIssue923Test4_4();
    QVector<QPointF>  OutputPointsIssue923Test4_4();

    QVector<VSAPoint> InputPointsIssue923Test5_5();
    QVector<QPointF>  OutputPointsIssue923Test5_5();

    QVector<VSAPoint> InputPointsIssue923Test6_6();
    QVector<QPointF>  OutputPointsIssue923Test6_6();

    QVector<VSAPoint> InputPointsIssue937Case1() const;
    QVector<QPointF>  OutputPointsIssue937Case1() const;

    QVector<VSAPoint> InputPointsIssue937Case2() const;
    QVector<QPointF>  OutputPointsIssue937Case2() const;

    QVector<VSAPoint> InputPointsIssue937Case3() const;
    QVector<QPointF>  OutputPointsIssue937Case3() const;

    QVector<VSAPoint> InputPointsIssue937Case4() const;
    QVector<QPointF>  OutputPointsIssue937Case4() const;
};

#endif // TST_VABSTRACTPIECE_H

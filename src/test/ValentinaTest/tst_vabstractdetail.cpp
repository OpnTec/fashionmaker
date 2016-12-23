/************************************************************************
 **
 **  @file   tst_vabstractdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 4, 2015
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

#include "tst_vabstractdetail.h"
#include "../vlayout/vabstractdetail.h"

#include <QPointF>
#include <QVector>

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VAbstractDetail::TST_VAbstractDetail(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::EquidistantRemoveLoop_data()
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<int>("eqv");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<QVector<QPointF>>("ekvOrig");

    // These are two real cases where equdistant has loop.
    // See issue #298. Segmented Curve isn't selected in Seam Allowance tool.
    // https://bitbucket.org/dismine/valentina/issue/298/segmented-curve-isnt-selected-in-seam
    // Code should clean loops in path.
    QTest::newRow("Issue 298. Case1") << InputPointsIssue298Case1()
                                      << static_cast<int>(EquidistantType::CloseEquidistant)
                                      << 75.5906 // seam allowance width
                                      << OutputPointsIssue298Case1();

    QTest::newRow("Issue 298. Case2") << InputPointsIssue298Case2()
                                      << static_cast<int>(EquidistantType::CloseEquidistant)
                                      << 37.7953 // seam allowance width
                                      << OutputPointsIssue298Case2();

    // See issue #548. Bug Detail tool. Case when seam allowance is wrong.
    // https://bitbucket.org/dismine/valentina/issues/548/bug-detail-tool-case-when-seam-allowance
    // Files: Steampunk_trousers.val and marie.vit
    // Actually buggy detail see in file src/app/share/collection/bugs/Steampunk_trousers_issue_#548.val
    // Code should clean loops in path.
    QTest::newRow("Issue 548. Case1") << InputPointsIssue548Case1()
                                      << static_cast<int>(EquidistantType::CloseEquidistant)
                                      << 11.338582677165354 // seam allowance width (0.3 cm)
                                      << OutputPointsIssue548Case1();

    QTest::newRow("Issue 548. Case2") << InputPointsIssue548Case2()
                                      << static_cast<int>(EquidistantType::CloseEquidistant)
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue548Case2();

    QTest::newRow("Issue 548. Case3") << InputPointsIssue548Case3()
                                      << static_cast<int>(EquidistantType::CloseEquidistant)
                                      << 75.59055118110237 // seam allowance width (2.0 cm)
                                      << OutputPointsIssue548Case3();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VAbstractDetail::EquidistantRemoveLoop() const
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(int, eqv);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, static_cast<EquidistantType>(eqv), width);

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::SumTrapezoids() const
{
    // Case3 checks that the method 'SumTrapezoids' returns negative value for three clockwise allocated points
    // Case4 checks that the method 'SumTrapezoids' returns positive value for three counterclock-wise allocated points
    // Case5 checks that the method 'SumTrapezoids' returns 0 for one point
    Case3();
    Case4();
    Case5();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::PathRemoveLoop_data() const
{
    QTest::addColumn<QVector<QPointF>>("path");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> path;
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(10, 10);
    QTest::newRow("Correct closed a path (four unique points)") << path << path;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
    QTest::newRow("Correct unclosed a path (four unique points)") << path << path;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    QTest::newRow("Correct closed a path (six unique points)") << path << path;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
    QTest::newRow("Correct unclosed a path (six unique points)") << path << path;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 20);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    QVector<QPointF> res;
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, closed a path (four unique points)") << path << res;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("One loop, unclosed a path (four unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 10);
    path << QPointF(0, 20);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("Two loops, closed a path (six unique points)") << path << res;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("Two loops, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the first loop, closed a path (six unique points)") << path << res;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("One loop, the first loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(10, 10);
    res << QPointF(5, 15);
    res << QPointF(10, 20);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the second loop, closed a path (six unique points)") << path << res;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("One loop, the second loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 15);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(20, 10);
    QTest::newRow("Correct closed a path, point on line (four unique points)") << path << path;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
    QTest::newRow("Corect unclosed a path, point on line (four unique points)") << path << path;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(0, 10);
    path << QPointF(10, 15);
    path << QPointF(0, 20);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(10, 15);
    path << QPointF(20, 10);
    QTest::newRow("Correct closed a path, point on line (six unique points)") << path << path;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
    QTest::newRow("Corect unclosed a path, point on line (six unique points)") << path << path;

    path.clear();
    path << QPointF(100.96979100571033, 1797.6153764073072);
    path << QPointF(168.3888427659865, 1807.2395034187866);
    path << QPointF(206.78076137364403, 1812.2910842036706);
    path << QPointF(239.1630793382262, 1815.951361623424);
    path << QPointF(267.5320085054171, 1818.4827543754482);
    path << QPointF(293.9502505847841, 1820.144031725603);
    path << QPointF(320.48133946750147, 1821.175819320443);
    path << QPointF(364.5960626489172, 1822.0507669842166);
    path << QPointF(400.66867742260206, 1822.488188976378);
    path << QPointF(623.3126833308274, 1822.488188976378);
    path << QPointF(653.5489038032683, 2162.6456692913384);
    path << QPointF(570.545584385708, 2162.6456692913384);
    path << QPointF(600.7818048581489, 1822.488188976378);
    path << QPointF(1001.3385826771654, 1822.488188976378);
    path << QPointF(1001.3385826771654, 2680.44094488189);
    path << QPointF(-22.11646613738226, 2680.44094488189);
    path << QPointF(100.96979100571033, 1797.6153764073072);

    res.clear();
    res << QPointF(100.96979100571033, 1797.6153764073072);
    res << QPointF(168.3888427659865, 1807.2395034187866);
    res << QPointF(206.78076137364403, 1812.2910842036706);
    res << QPointF(239.1630793382262, 1815.951361623424);
    res << QPointF(267.5320085054171, 1818.4827543754482);
    res << QPointF(293.9502505847841, 1820.144031725603);
    res << QPointF(320.48133946750147, 1821.175819320443);
    res << QPointF(364.5960626489172, 1822.0507669842166);
    res << QPointF(400.66867742260206, 1822.488188976378);
    res << QPointF(1001.3385826771654, 1822.488188976378);
    res << QPointF(1001.3385826771654, 1822.488188976378);
    res << QPointF(1001.3385826771654, 2680.44094488189);
    res << QPointF(-22.11646613738226, 2680.44094488189);
    res << QPointF(100.96979100571033, 1797.6153764073072);

    // See the file "collection/bugs/Issue_#493.val"
    QTest::newRow("Test case issue #493") << path << res;

    path.clear();
    path << QPointF(-656.9465284876832, -3606.6183279948636);
    path << QPointF(-656.5996104603414, -3606.6000783462687);
    path << QPointF(-655.7439133016985, -3607.1236310612317);
    path << QPointF(-654.129780081666, -3609.0558393168476);
    path << QPointF(-651.3154902471701, -3613.939306009108);
    path << QPointF(-647.8207651830382, -3621.2084054506768);
    path << QPointF(-641.4701586077349, -3636.0289997859454);
    path << QPointF(-630.9244502073004, -3663.23035747934);
    path << QPointF(-618.4465305467888, -3697.4982896415795);
    path << QPointF(-604.3873016966293, -3737.732371148936);
    path << QPointF(-581.1891087215608, -3806.460957656939);
    path << QPointF(-547.7936207285052, -3909.520915257629);
    path << QPointF(-514.3891332445846, -4016.6378180116963);
    path << QPointF(-491.17181635142833, -4093.9874129706236);
    path << QPointF(-477.094588519539, -4142.335384784734);
    path << QPointF(-464.5941701318652, -4186.745679830414);
    path << QPointF(-454.0214632588362, -4226.117872983938);
    path << QPointF(-449.6699112298347, -4243.2921010175705);
    path << QPointF(-576.966638263205, -3606.6183279948636);

    res.clear();
    res << QPointF(-656.9465284876832, -3606.6183279948636);
    res << QPointF(-656.5996104603414, -3606.6000783462687);
    res << QPointF(-656.5697831440032, -3606.6183279948636);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a countur path.
    // The curve that causes the issue is the first in the list.
    QTest::newRow("Test case issue #515. Big loop in countur path.") << path << res;

    path.clear();
    path << QPointF(-449.6699112298347, -4243.2921010175705);
    path << QPointF(-576.966638263205, -3606.6183279948636);
    path << QPointF(-656.9465284876832, -3606.6183279948636);
    path << QPointF(-656.5996104603414, -3606.6000783462687);
    path << QPointF(-655.7439133016985, -3607.1236310612317);
    path << QPointF(-654.129780081666, -3609.0558393168476);
    path << QPointF(-651.3154902471701, -3613.939306009108);
    path << QPointF(-647.8207651830382, -3621.2084054506768);
    path << QPointF(-641.4701586077349, -3636.0289997859454);
    path << QPointF(-630.9244502073004, -3663.23035747934);
    path << QPointF(-618.4465305467888, -3697.4982896415795);
    path << QPointF(-604.3873016966293, -3737.732371148936);
    path << QPointF(-581.1891087215608, -3806.460957656939);
    path << QPointF(-547.7936207285052, -3909.520915257629);
    path << QPointF(-514.3891332445846, -4016.6378180116963);
    path << QPointF(-491.17181635142833, -4093.9874129706236);
    path << QPointF(-477.094588519539, -4142.335384784734);
    path << QPointF(-464.5941701318652, -4186.745679830414);
    path << QPointF(-454.0214632588362, -4226.117872983938);

    res.clear();
    res << QPointF(-449.6699112298347, -4243.2921010175705);
    res << QPointF(-576.966638263205, -3606.6183279948636);
    res << QPointF(-656.5697831440032, -3606.6183279948636);
    res << QPointF(-655.7439133016985, -3607.1236310612317);
    res << QPointF(-654.129780081666, -3609.0558393168476);
    res << QPointF(-651.3154902471701, -3613.939306009108);
    res << QPointF(-647.8207651830382, -3621.2084054506768);
    res << QPointF(-641.4701586077349, -3636.0289997859454);
    res << QPointF(-630.9244502073004, -3663.23035747934);
    res << QPointF(-618.4465305467888, -3697.4982896415795);
    res << QPointF(-604.3873016966293, -3737.732371148936);
    res << QPointF(-581.1891087215608, -3806.460957656939);
    res << QPointF(-547.7936207285052, -3909.520915257629);
    res << QPointF(-514.3891332445846, -4016.6378180116963);
    res << QPointF(-491.17181635142833, -4093.9874129706236);
    res << QPointF(-477.094588519539, -4142.335384784734);
    res << QPointF(-464.5941701318652, -4186.745679830414);
    res << QPointF(-454.0214632588362, -4226.117872983938);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a countur path.
    // The curve that causes the issue is the last in the list.
    QTest::newRow("Test case issue #515. Small loop in countur path.") << path << res;

    path.clear();
    path << QPointF(-656.9465284876832, -3606.6183279948636);
    path << QPointF(-656.5996104603414, -3606.6000783462687);
    path << QPointF(-655.7439133016985, -3607.1236310612317);
    path << QPointF(-654.129780081666, -3609.0558393168476);
    path << QPointF(-651.3154902471701, -3613.939306009108);
    path << QPointF(-647.8207651830382, -3621.2084054506768);
    path << QPointF(-641.4701586077349, -3636.0289997859454);
    path << QPointF(-630.9244502073004, -3663.23035747934);
    path << QPointF(-618.4465305467888, -3697.4982896415795);
    path << QPointF(-604.3873016966293, -3737.732371148936);
    path << QPointF(-581.1891087215608, -3806.460957656939);
    path << QPointF(-547.7936207285052, -3909.520915257629);
    path << QPointF(-514.3891332445846, -4016.6378180116963);
    path << QPointF(-491.17181635142833, -4093.9874129706236);
    path << QPointF(-477.094588519539, -4142.335384784734);
    path << QPointF(-464.5941701318652, -4186.745679830414);
    path << QPointF(-454.0214632588362, -4226.117872983938);
    path << QPointF(-449.6699112298347, -4243.2921010175705);
    path << QPointF(-576.966638263205, -3606.6183279948636);

    res.clear();
    res << QPointF(-656.9465284876832, -3606.6183279948636);
    res << QPointF(-656.5996104603414, -3606.6000783462687);
    res << QPointF(-656.5697831440032, -3606.6183279948636);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a seam allowance path.
    // The curve that causes the issue is the first in the list.
    QTest::newRow("Test case issue #515. Big loop in seam allowance path.") << path << res;

    path.clear();
    path << QPointF(-449.6699112298347, -4243.2921010175705);
    path << QPointF(-576.966638263205, -3606.6183279948636);
    path << QPointF(-656.9465284876832, -3606.6183279948636);
    path << QPointF(-656.5996104603414, -3606.6000783462687);
    path << QPointF(-655.7439133016985, -3607.1236310612317);
    path << QPointF(-654.129780081666, -3609.0558393168476);
    path << QPointF(-651.3154902471701, -3613.939306009108);
    path << QPointF(-647.8207651830382, -3621.2084054506768);
    path << QPointF(-641.4701586077349, -3636.0289997859454);
    path << QPointF(-630.9244502073004, -3663.23035747934);
    path << QPointF(-618.4465305467888, -3697.4982896415795);
    path << QPointF(-604.3873016966293, -3737.732371148936);
    path << QPointF(-581.1891087215608, -3806.460957656939);
    path << QPointF(-547.7936207285052, -3909.520915257629);
    path << QPointF(-514.3891332445846, -4016.6378180116963);
    path << QPointF(-491.17181635142833, -4093.9874129706236);
    path << QPointF(-477.094588519539, -4142.335384784734);
    path << QPointF(-464.5941701318652, -4186.745679830414);
    path << QPointF(-454.0214632588362, -4226.117872983938);

    res.clear();
    res << QPointF(-449.6699112298347, -4243.2921010175705);
    res << QPointF(-576.966638263205, -3606.6183279948636);
    res << QPointF(-656.5697831440032, -3606.6183279948636);
    res << QPointF(-655.7439133016985, -3607.1236310612317);
    res << QPointF(-654.129780081666, -3609.0558393168476);
    res << QPointF(-651.3154902471701, -3613.939306009108);
    res << QPointF(-647.8207651830382, -3621.2084054506768);
    res << QPointF(-641.4701586077349, -3636.0289997859454);
    res << QPointF(-630.9244502073004, -3663.23035747934);
    res << QPointF(-618.4465305467888, -3697.4982896415795);
    res << QPointF(-604.3873016966293, -3737.732371148936);
    res << QPointF(-581.1891087215608, -3806.460957656939);
    res << QPointF(-547.7936207285052, -3909.520915257629);
    res << QPointF(-514.3891332445846, -4016.6378180116963);
    res << QPointF(-491.17181635142833, -4093.9874129706236);
    res << QPointF(-477.094588519539, -4142.335384784734);
    res << QPointF(-464.5941701318652, -4186.745679830414);
    res << QPointF(-454.0214632588362, -4226.117872983938);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a seam allowance path.
    // The curve that causes the issue is the last in the list.
    QTest::newRow("Test case issue #515. Small loop in seam allowance path.") << path << res;

    path.clear();
    path << QPointF(1229.6503937007876, 937.6667716535435);
    path << QPointF(203.08931117793543, 937.6667716535435);
    path << QPointF(459.7677349767701, -2166.704563141019);
    path << QPointF(1229.6503937007876, -1990.077167189857);
    path << QPointF(1229.6503937007876, -555.2466141732282);
    path << QPointF(920.1053824527112, -555.2466141732282);
    path << QPointF(887.034516310979, -63.90803149606281);
    path << QPointF(816.3607592795726, -63.908031496062826);
    path << QPointF(780.7580397937137, -592.8627210002539);
    path << QPointF(816.0241340748559, -1202.917917917055);
    path << QPointF(887.3711415156957, -1202.917917917055);
    path << QPointF(920.4420076574283, -630.8371653543306);
    path << QPointF(1229.6503937007876, -630.8371653543306);
    path << QPointF(1229.6503937007876, 937.6667716535435);

    res.clear();
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(203.08931117793543, 937.6667716535435);
    res << QPointF(459.7677349767702, -2166.704563141019);
    res << QPointF(1229.6503937007876, -1990.077167189857);
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(1229.6503937007876, 937.6667716535435);

    // See the file "collection/bugs/Issue_#603.val"
    // Point H1 is first in the list
    QTest::newRow("Test issue 603. Case 1.") << path << res;

    path.clear();
    path << QPointF(1229.6503937007876, -630.8371653543306);
    path << QPointF(1229.6503937007876, 937.6667716535435);
    path << QPointF(203.08931117793543, 937.6667716535435);
    path << QPointF(459.7677349767702, -2166.704563141019);
    path << QPointF(1229.6503937007876, -1990.077167189857);
    path << QPointF(1229.6503937007876, -555.2466141732282);
    path << QPointF(920.1053824527112, -555.2466141732282);
    path << QPointF(887.034516310979, -63.90803149606281);
    path << QPointF(816.3607592795726, -63.908031496062826);
    path << QPointF(780.7580397937137, -592.8627210002539);
    path << QPointF(816.0241340748559, -1202.917917917055);
    path << QPointF(887.3711415156957, -1202.917917917055);
    path << QPointF(920.4420076574283, -630.8371653543306);
    path << QPointF(1229.6503937007876, -630.8371653543306);

    res.clear();
    res << QPointF(1229.6503937007876, -1990.077167189857);
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(203.08931117793543, 937.6667716535435);
    res << QPointF(459.7677349767702, -2166.704563141019);
    res << QPointF(1229.6503937007876, -1990.077167189857);

    // See the file "collection/bugs/Issue_#603.val"
    // Point T1 is first in the list
    QTest::newRow("Test issue 603. Case 2.") << path << res;

    path.clear();
    path << QPointF(920.4420076574283, -630.8371653543306);
    path << QPointF(1229.6503937007876, -630.8371653543306);
    path << QPointF(1229.6503937007876, 937.6667716535435);
    path << QPointF(203.08931117793543, 937.6667716535435);
    path << QPointF(459.7677349767702, -2166.704563141019);
    path << QPointF(1229.6503937007876, -1990.077167189857);
    path << QPointF(1229.6503937007876, -555.2466141732282);
    path << QPointF(920.1053824527112, -555.2466141732282);
    path << QPointF(887.034516310979, -63.90803149606281);
    path << QPointF(816.3607592795726, -63.908031496062826);
    path << QPointF(780.7580397937137, -592.8627210002539);
    path << QPointF(816.0241340748559, -1202.917917917055);
    path << QPointF(887.3711415156957, -1202.917917917055);
    path << QPointF(920.4420076574283, -630.8371653543306);

    res.clear();
    res << QPointF(1229.6503937007876, -1990.077167189857);
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(203.08931117793543, 937.6667716535435);
    res << QPointF(459.7677349767702, -2166.704563141019);
    res << QPointF(1229.6503937007876, -1990.077167189857);

    // See the file "collection/bugs/Issue_#603.val"
    // Point T62 is first in the list
    QTest::newRow("Test issue 603. Case 3.") << path << res;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::PathRemoveLoop() const
{
    QFETCH(QVector<QPointF>, path);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> res = VAbstractDetail::CheckLoops(path);
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::BrokenDetailEquidistant() const
{
    // For more details see the file "collection/bugs/GAVAUDAN Laure - corsage - figure 4.val".
    // We will test only one detail. The second require too accurate data that we cannot get from debuger.
    // The test check an open equdistant of correct detail.
    QVector<QPointF> points;// Input points.
    points.append(QPointF(787.5835464566929, 1701.3138897637796));
    points.append(QPointF(938.7646488188976, 1701.3138897637796));
    points.append(QPointF(910.0209091217698, 1792.3369853889722));
    points.append(QPointF(878.5244039283091, 1905.2261617043234));
    points.append(QPointF(863.9159293830619, 1968.2534932384856));
    points.append(QPointF(852.8936778444679, 1919.6965437838999));
    points.append(QPointF(819.0677656132684, 1798.6758641921479));
    points.append(QPointF(787.5835464566929, 1701.3138897637796));
    points.append(QPointF(797.0323653543306, 2608.4005039370077));

    const EquidistantType eqv = EquidistantType::OpenEquidistant; // Open path
    const qreal width = 37.795275590551185; // seam allowance width

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, eqv, width);// Take result

    QVector<QPointF> ekvOrig;
    ekvOrig.append(QPointF(938.7646488188976, 1663.5186141732283));
    ekvOrig.append(QPointF(990.3348131440238, 1663.5186141732283));
    ekvOrig.append(QPointF(946.2541789190154, 1803.1092111713983));
    ekvOrig.append(QPointF(915.1544461790318, 1914.5762831871607));
    ekvOrig.append(QPointF(893.9371203604946, 2006.1170819537472));
    ekvOrig.append(QPointF(833.7229784629903, 2005.9802605640884));
    ekvOrig.append(QPointF(816.2424908106893, 1928.9724774988724));
    ekvOrig.append(QPointF(782.8721775719547, 1809.5818175001634));
    ekvOrig.append(QPointF(751.6217900811182, 1712.9429454154488));

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::CorrectEquidistantPoints_data() const
{
    // See file zigzag.val
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<QVector<QPointF>>("expect");
    QTest::addColumn<bool>("removeFirstAndLast");

    QVector<QPointF> points;
    points.append(QPointF(-741.7894588053705, 1065.7336503858917));
    points.append(QPointF(-759.696551643576, -115.81420543069257));
    points.append(QPointF(-278.17249953019325, -217.1037453126913));
    points.append(QPointF(-244.64654130659474, 1077.9548221866635));
    points.append(QPointF(-741.7894588053705, 1065.7336503858917));

    QVector<QPointF> expect;
    expect.append(QPointF(-741.7894588053705, 1065.7336503858917));
    expect.append(QPointF(-759.696551643576, -115.81420543069257));
    expect.append(QPointF(-278.17249953019325, -217.1037453126913));
    expect.append(QPointF(-244.64654130659474, 1077.9548221866635));
    expect.append(QPointF(-741.7894588053705, 1065.7336503858917));

    QTest::newRow("Closed seam allowance. Last point equal first.") << points << expect << false;

    points.clear();
    points.append(QPointF(-704.5489521643801, 1028.8424328418016));
    points.append(QPointF(-721.4335720065426, -85.24049234531904));
    points.append(QPointF(-707.7852899705758, 755.7064514429209));
    points.append(QPointF(-721.4335720065426, -85.24049234531904));
    points.append(QPointF(-314.78124296268265, -170.7806167067443));
    points.append(QPointF(-283.4579031023758, 1039.1940357173805));

    expect.clear();
    expect.append(QPointF(-704.5489521643801, 1028.8424328418016));
    expect.append(QPointF(-721.4335720065426, -85.24049234531904));
    expect.append(QPointF(-314.78124296268265, -170.7806167067443));
    expect.append(QPointF(-283.4579031023758, 1039.1940357173805));

    QTest::newRow("Clearing bad main path.") << points << expect << true;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::CorrectEquidistantPoints() const
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(QVector<QPointF>, expect);
    QFETCH(bool, removeFirstAndLast);

    const QVector<QPointF> res = VAbstractDetail::CorrectEquidistantPoints(points, removeFirstAndLast);

    // Begin comparison
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::TestCorrectEquidistantPoints_data()
{
    QTest::addColumn<QVector<QPointF>>("before");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> before;
    before << QPointF(30.0, 39.999874015748034);
    before << QPointF(785.9055118110236, 39.999874015748034);
    before << QPointF(785.9055118110236, 3819.527433070866);
    before << QPointF(483.54330708661416, 3819.527433070866);
    before << QPointF(483.54330708661416, 1929.763653543307);
    before << QPointF(407.9527559055629, 984.8817637795973);
    before << QPointF(407.9527559055118, 1929.763653543307);
    before << QPointF(407.9527559055118, 3819.527433070866);
    before << QPointF(30.0, 3819.527433070866);

    QVector<QPointF> expect;
    expect << QPointF(30.0, 39.999874015748034);
    expect << QPointF(785.9055118110236, 39.999874015748034);
    expect << QPointF(785.9055118110236, 3819.527433070866);
    expect << QPointF(483.54330708661416, 3819.527433070866);
    expect << QPointF(483.54330708661416, 1929.763653543307);
    expect << QPointF(407.9527559055629, 984.8817637795973);
    expect << QPointF(407.9527559055118, 3819.527433070866);
    expect << QPointF(30.0, 3819.527433070866);

    QTest::newRow("Test case issue #548") << before << expect;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::TestCorrectEquidistantPoints() const
{
    QFETCH(QVector<QPointF>, before);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> after = VAbstractDetail::CorrectEquidistantPoints(before);
    Comparison(after, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case3() const
{
    const QVector<QPointF> points = InputPointsCase3(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(result < 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case4() const
{
    const QVector<QPointF> points = InputPointsCase4(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(result > 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case5() const
{
    const QVector<QPointF> points = InputPointsCase5(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(qFuzzyIsNull(result));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsIssue298Case1() const
{
    QVector<QPointF> points;

    points += QPointF(35, 39.9999);
    points += QPointF(412.953, 39.9999);
    points += QPointF(417.135, 417.929);
    points += QPointF(417.135, 417.929);
    points += QPointF(408.797, 405.589);
    points += QPointF(390.909, 377.669);
    points += QPointF(362.315, 330.86);
    points += QPointF(323.075, 264.247);
    points += QPointF(286.15, 201.448);
    points += QPointF(262.477, 162.745);
    points += QPointF(249.22, 142.455);
    points += QPointF(241.092, 131.261);
    points += QPointF(236.545, 125.75);
    points += QPointF(232.808, 122.058);
    points += QPointF(230.6, 120.629);
    points += QPointF(229.393, 120.277);
    points += QPointF(228.421, 120.456);
    points += QPointF(227.69, 121.185);
    points += QPointF(227.033, 123.272);
    points += QPointF(227.112, 128.232);
    points += QPointF(228.29, 135.699);
    points += QPointF(230.625, 145.81);
    points += QPointF(234.173, 158.703);
    points += QPointF(241.73, 183.168);
    points += QPointF(248.796, 204.144);
    points += QPointF(248.796, 204.144);
    points += QPointF(251.528, 212.406);
    points += QPointF(255.482, 227.075);
    points += QPointF(257.717, 239.591);
    points += QPointF(258.279, 247.554);
    points += QPointF(258.203, 252.278);
    points += QPointF(257.756, 256.51);
    points += QPointF(256.949, 260.264);
    points += QPointF(255.795, 263.547);
    points += QPointF(254.308, 266.372);
    points += QPointF(252.501, 268.749);
    points += QPointF(250.385, 270.688);
    points += QPointF(247.974, 272.201);
    points += QPointF(245.281, 273.296);
    points += QPointF(242.319, 273.986);
    points += QPointF(239.1, 274.28);
    points += QPointF(233.846, 274.05);
    points += QPointF(226.022, 272.393);
    points += QPointF(217.402, 269.345);
    points += QPointF(208.09, 264.991);
    points += QPointF(198.186, 259.414);
    points += QPointF(187.795, 252.7);
    points += QPointF(177.019, 244.933);
    points += QPointF(165.96, 236.197);
    points += QPointF(154.721, 226.576);
    points += QPointF(143.405, 216.157);
    points += QPointF(132.113, 205.022);
    points += QPointF(120.95, 193.257);
    points += QPointF(110.017, 180.946);
    points += QPointF(99.4167, 168.174);
    points += QPointF(89.2522, 155.024);
    points += QPointF(79.626, 141.582);
    points += QPointF(70.6405, 127.933);
    points += QPointF(62.3985, 114.16);
    points += QPointF(55.0025, 100.348);
    points += QPointF(48.5551, 86.5823);
    points += QPointF(43.159, 72.9466);
    points += QPointF(38.9167, 59.5258);
    points += QPointF(35.9309, 46.4042);
    points += QPointF(35, 39.9999);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::OutputPointsIssue298Case1() const
{
    QVector<QPointF> points;
    points += QPointF(-52.3724798442221, -35.5907);
    points += QPointF(487.7117748779425, -35.5907);
    points += QPointF(493.3432017362585, 473.32371517914754);
    points += QPointF(385.98559977345093, 506.8445742667132);
    points += QPointF(345.64704646524604, 447.1446764706891);
    points += QPointF(326.82411403464874, 417.76541252489994);
    points += QPointF(297.4844355409708, 369.73572061014266);
    points += QPointF(280.35686644039447, 340.63425704493835);
    points += QPointF(268.2336759982877, 345.56366422433183);
    points += QPointF(254.38869069377708, 348.78886336684104);
    points += QPointF(240.8928242225697, 350.0214774527481);
    points += QPointF(224.29748398011193, 349.2949970081793);
    points += QPointF(205.50330859478322, 345.31468660256957);
    points += QPointF(188.72568121178054, 339.38217984347546);
    points += QPointF(173.487571907339, 332.2573164509149);
    points += QPointF(159.09346043909582, 324.15190856941325);
    points += QPointF(145.1562378134811, 315.1465661857729);
    points += QPointF(131.46917217609203, 305.28136213922494);
    points += QPointF(117.9345600633141, 294.589765121662);
    points += QPointF(104.5254725457231, 283.11108988305153);
    points += QPointF(91.25156649455745, 270.88938370179534);
    points += QPointF(78.14294517511125, 257.9630200468154);
    points += QPointF(65.25722328495372, 244.3823949426573);
    points += QPointF(52.65759889494496, 230.19470850111355);
    points += QPointF(40.412239584772514, 215.4406233233806);
    points += QPointF(28.600027181043494, 200.15894757848054);
    points += QPointF(17.304913602921047, 184.38648111018338);
    points += QPointF(6.6105681133211736, 168.14173996194046);
    points += QPointF(-3.3897319816688407, 151.43048866270516);
    points += QPointF(-12.592267484961765, 134.24479093805914);
    points += QPointF(-20.880547263016442, 116.54866956498358);
    points += QPointF(-28.111192294561146, 98.27715746242171);
    points += QPointF(-34.098213657706594, 79.33681465062016);
    points += QPointF(-38.441724866417594, 60.24852451858777);
    points += QPointF(-52.3724798442221, -35.5907);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(35, 39.9999);
    points += QPointF(35, 39.9999);
    points += QPointF(35.9309, 46.4042);
    points += QPointF(38.9167, 59.5258);
    points += QPointF(43.159, 72.9466);
    points += QPointF(48.5551, 86.5823);
    points += QPointF(55.0025, 100.348);
    points += QPointF(62.3985, 114.16);
    points += QPointF(70.6405, 127.933);
    points += QPointF(79.626, 141.582);
    points += QPointF(89.2522, 155.024);
    points += QPointF(99.4167, 168.174);
    points += QPointF(110.017, 180.946);
    points += QPointF(120.95, 193.257);
    points += QPointF(132.113, 205.022);
    points += QPointF(143.405, 216.157);
    points += QPointF(154.721, 226.576);
    points += QPointF(165.96, 236.197);
    points += QPointF(177.019, 244.933);
    points += QPointF(187.795, 252.7);
    points += QPointF(198.186, 259.414);
    points += QPointF(208.09, 264.991);
    points += QPointF(217.402, 269.345);
    points += QPointF(226.022, 272.393);
    points += QPointF(233.846, 274.05);
    points += QPointF(239.1, 274.28);
    points += QPointF(242.319, 273.986);
    points += QPointF(245.281, 273.296);
    points += QPointF(247.974, 272.201);
    points += QPointF(250.385, 270.688);
    points += QPointF(252.501, 268.749);
    points += QPointF(254.308, 266.372);
    points += QPointF(255.795, 263.547);
    points += QPointF(256.949, 260.264);
    points += QPointF(257.756, 256.51);
    points += QPointF(258.203, 252.278);
    points += QPointF(258.279, 247.554);
    points += QPointF(257.717, 239.591);
    points += QPointF(255.482, 227.075);
    points += QPointF(251.528, 212.406);
    points += QPointF(248.796, 204.144);
    points += QPointF(248.796, 204.144);
    points += QPointF(241.73, 183.168);
    points += QPointF(234.173, 158.703);
    points += QPointF(230.625, 145.81);
    points += QPointF(228.29, 135.699);
    points += QPointF(227.112, 128.232);
    points += QPointF(227.033, 123.272);
    points += QPointF(227.69, 121.185);
    points += QPointF(228.421, 120.456);
    points += QPointF(229.393, 120.277);
    points += QPointF(230.6, 120.629);
    points += QPointF(232.808, 122.058);
    points += QPointF(236.545, 125.75);
    points += QPointF(241.092, 131.261);
    points += QPointF(249.22, 142.455);
    points += QPointF(262.477, 162.745);
    points += QPointF(286.15, 201.448);
    points += QPointF(323.075, 264.247);
    points += QPointF(362.315, 330.86);
    points += QPointF(390.909, 377.669);
    points += QPointF(408.797, 405.589);
    points += QPointF(417.135, 417.929);
    points += QPointF(417.135, 417.929);
    points += QPointF(35, 417.953);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::OutputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(-2.7952999999999975, 4.8384699505981095);
    points += QPointF(67.34448942068963, -0.23248582689164274);
    points += QPointF(73.11721243320879, 39.48203774070609);
    points += QPointF(75.42415682885321, 49.62029267468959);
    points += QPointF(78.79409614728041, 60.281321268788744);
    points += QPointF(83.27292363150828, 71.59911521750833);
    points += QPointF(88.79988374248082, 83.39960453097031);
    points += QPointF(95.2926159908344, 95.5247556686474);
    points += QPointF(102.65546594334339, 107.82863001903641);
    points += QPointF(110.78654319853989, 120.17975944490887);
    points += QPointF(119.5782864094781, 132.4565262107595);
    points += QPointF(128.91893020761376, 144.54068833830968);
    points += QPointF(138.69670055252752, 156.3216457494432);
    points += QPointF(148.79638835752286, 167.69430252867102);
    points += QPointF(159.09802741244354, 178.55148997659143);
    points += QPointF(169.48171675272164, 188.79080814910267);
    points += QPointF(179.81876372713828, 198.30845505847407);
    points += QPointF(189.9727199683426, 207.00061743916868);
    points += QPointF(199.7939139119543, 214.75881893038778);
    points += QPointF(209.1143810932559, 221.476716907111);
    points += QPointF(216.03386663545683, 225.9476461661168);
    points += QPointF(215.3306509043856, 223.3387762725701);
    points += QPointF(205.75073516810195, 194.75155680967347);
    points += QPointF(197.88802785264718, 169.29686123304236);
    points += QPointF(193.97579117825833, 155.08026950731082);
    points += QPointF(191.1640933645057, 142.90507610480435);
    points += QPointF(189.3638602852325, 131.49392126360493);
    points += QPointF(189.14507682295456, 117.75764312564759);
    points += QPointF(194.42693552963567, 100.97950138920423);
    points += QPointF(210.03879336533757, 85.41035725481989);
    points += QPointF(231.36634627769158, 81.48275234606332);
    points += QPointF(246.4916615881645, 85.89378050620131);
    points += QPointF(256.60614755001956, 92.43979519799973);
    points += QPointF(264.4750900046005, 100.21398185636762);
    points += QPointF(270.9888544453203, 108.1087159300009);
    points += QPointF(280.35077918473866, 121.00209505562212);
    points += QPointF(294.42535276480356, 142.5434013797918);
    points += QPointF(318.5597512322288, 182.00074197391842);
    points += QPointF(394.73028222951507, 311.42213969492946);
    points += QPointF(422.9514429826756, 357.62079373755);
    points += QPointF(440.37197676737753, 384.8111617646563);
    points += QPointF(488.2841719585649, 455.71983154868764);
    points += QPointF(-2.795300000000013, 455.7506738094777);
    points += QPointF(-2.7952999999999975, 4.8384699505981095);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsIssue548Case1() const
{
    QVector<QPointF> points;

    points += QPointF(236.97989607468364, 65.89325192030674);
    points += QPointF(198.93409106041895, 172.04876297154925);
    points += QPointF(260.32251114299453, 75.38027418944861);
    points += QPointF(324.54110236213444, 101.48031496062993);
    points += QPointF(29.858267716535437, 300.85039370078744);
    points += QPointF(99.86433649395013, 10.166060970128015);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::OutputPointsIssue548Case1() const
{
    QVector<QPointF> points;

    points += QPointF(251.32210577118798, 59.48301432799721);
    points += QPointF(243.9841262159756, 79.95746530820585);
    points += QPointF(255.82424817748586, 61.31279754390509);
    points += QPointF(348.48337789725855, 98.9717841021069);
    points += QPointF(29.780382054543473, 314.59289909613994);
    points += QPointF(17.01672179602679, 305.7450049304056);
    points += QPointF(91.92616539550944, -5.299480329501037);
    points += QPointF(251.32210577118798, 59.48301432799721);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsIssue548Case2() const
{
    QVector<QPointF> points;

    points += QPointF(236.97989607468364, 65.89325192030674);
    points += QPointF(198.93409106041895, 172.04876297154925);
    points += QPointF(260.32251114299453, 75.38027418944861);
    points += QPointF(324.54110236213444, 101.48031496062993);
    points += QPointF(29.858267716535437, 300.85039370078744);
    points += QPointF(99.86433649395013, 10.166060970128015);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::OutputPointsIssue548Case2() const
{
    QVector<QPointF> points;

    points += QPointF(284.78726172969823, 44.52579327927505);
    points += QPointF(404.3486874792147, 93.11854543221973);
    points += QPointF(29.598648843228922, 346.6587450186291);
    points += QPointF(-12.946885351826726, 317.1657644661815);
    points += QPointF(73.40376616581447, -41.38574336196901);
    points += QPointF(284.78726172969823, 44.52579327927505);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsIssue548Case3() const
{
    QVector<QPointF> points;

    points += QPointF(236.97989607468364, 65.89325192030674);
    points += QPointF(198.93409106041895, 172.04876297154925);
    points += QPointF(260.32251114299453, 75.38027418944861);
    points += QPointF(324.54110236213444, 101.48031496062993);
    points += QPointF(29.858267716535437, 300.85039370078744);
    points += QPointF(99.86433649395013, 10.166060970128015);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::OutputPointsIssue548Case3() const
{
    QVector<QPointF> points;

    points += QPointF(332.5946273847129, 23.158334638243502);
    points += QPointF(484.15627259629446, 84.75677590380938);
    points += QPointF(29.339029969922702, 392.46709633647066);
    points += QPointF(-55.75203842018885, 333.48113523157537);
    points += QPointF(46.94319583767885, -92.9375476940661);
    points += QPointF(332.5946273847129, 23.158334638243502);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);
    points += QPointF(50, 50);
    points += QPointF(15, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase4() const
{
    QVector<QPointF> points;

    points += QPointF(15, 15);
    points += QPointF(15, 50);
    points += QPointF(50, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase5() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);

    return points;
}

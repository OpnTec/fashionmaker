/************************************************************************
 **
 **  @file   tst_vspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
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

#ifndef TST_VSPLINE_H
#define TST_VSPLINE_H

#include "../vmisc/abstracttest.h"

class VSpline;

class TST_VSpline : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_VSpline(QObject *parent = nullptr);

signals:

private slots:
    void GetSegmentPoints();
    void GetSegmentPoints_issue412();
    void GetSegmentPoints_TestPuzzle();
    void GetSegmentPoints_NullSegment();
    void GetSegmentPoints_RotateTool();
    void CompareThreeWays();
    void TestParametrT();
    void TestLengthByPoint_data();
    void TestLengthByPoint();
    void TestFlip_data();
    void TestFlip();

private:
    Q_DISABLE_COPY(TST_VSpline)
    void CompareSplines(const VSpline &spl1, const VSpline &spl2) const;
};

#endif // TST_VSPLINE_H

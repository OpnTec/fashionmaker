/************************************************************************
 **
 **  @file   tst_vcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef TST_VCUBICBEZIERPATH_H
#define TST_VCUBICBEZIERPATH_H

#include <QObject>

class TST_VCubicBezierPath : public QObject
{
    Q_OBJECT
public:
    explicit TST_VCubicBezierPath(QObject *parent = nullptr);

private slots:
    void TestCountSubSpl_data() const;
    void TestCountSubSpl() const;
    void TestSubSplOffset_data() const;
    void TestSubSplOffset() const;
    void TestSubSplPointsCount_data() const;
    void TestSubSplPointsCount() const;

private:
    Q_DISABLE_COPY(TST_VCubicBezierPath)

};

#endif // TST_VCUBICBEZIERPATH_H

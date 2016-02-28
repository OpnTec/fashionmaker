/************************************************************************
 **
 **  @file   tst_vellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 2, 2016
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

#ifndef TST_VELLIPTICALARC_H
#define TST_VELLIPTICALARC_H

#include <QObject>

class TST_VEllipticalArc : public QObject
{
    Q_OBJECT
public:
    explicit TST_VEllipticalArc(QObject *parent = nullptr);

private slots:
    void CompareTwoWays();
    void NegativeArc();
    void TestGetPoints1_data();
    void TestGetPoints2_data();
    void TestGetPoints3_data();
    void TestGetPoints4_data();
    void TestGetPoints1();
    void TestGetPoints2();
    void TestGetPoints3();
    void TestGetPoints4();

private:
    Q_DISABLE_COPY(TST_VEllipticalArc)
    void TestData();
};

#endif // TST_VELLIPTICALARC_H

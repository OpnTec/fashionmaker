/************************************************************************
 **
 **  @file   tst_misc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 10, 2015
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

#ifndef TST_MISC_H
#define TST_MISC_H

#include <QObject>

class TST_Misc :public QObject
{
    Q_OBJECT
public:
    explicit TST_Misc(QObject *parent = nullptr);

private slots:
    void TestRelativeFilePath_data();
    void TestRelativeFilePath();

    void TestAbsoluteFilePath_data();
    void TestAbsoluteFilePath();

    void TestCLocale_data();
    void TestCLocale();

    void TestIssue485();

private:
    Q_DISABLE_COPY(TST_Misc)
};

#endif // TST_MISC_H

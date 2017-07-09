/************************************************************************
 **
 **  @file   tst_vlockguard.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 10, 2015
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

#include "tst_vlockguard.h"
#include "../vmisc/vlockguard.h"

#include <QtTest>

#ifdef Q_CC_MSVC
    #include <ciso646>
#endif /* Q_CC_MSVC */

//---------------------------------------------------------------------------------------------------------------------
TST_VLockGuard::TST_VLockGuard(QObject *parent)
    :QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VLockGuard::TryLock() const
{
    QString fileName(QCoreApplication::applicationDirPath() + "/lockFile.txt");
    std::shared_ptr<VLockGuard<char>> lock;
    VlpCreateLock(lock, fileName);

    fileName = lock->GetLockFile();
    QVERIFY2(QFileInfo(fileName).exists(), "Lock file doesn't exist!");

    lock.reset();
    QVERIFY2(not QFileInfo(fileName).exists(), "Lock file still exists!");
}

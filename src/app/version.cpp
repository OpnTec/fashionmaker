/************************************************************************
 **
 **  @file   version.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 4, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "version.h"
#include <QString>
#include <QApplication>
#include <QSysInfo>

extern const int MAJOR_VERSION = 0;
extern const int MINOR_VERSION = 3;
extern const int DEBUG_VERSION = 2;

extern const QString APP_VERSION_STR(QStringLiteral("%1.%2.%3.%4").arg(MAJOR_VERSION).arg(MINOR_VERSION)
                                 .arg(DEBUG_VERSION).arg(LATEST_TAG_DISTANCE));

//---------------------------------------------------------------------------------------------------------------------
QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
            + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC " ) + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER >= 1800) // 1800: MSVC 2013 (yearly release cycle)
    {
        compiler = QLatin1String("MSVC ") + QString::number(2008 + ((_MSC_VER / 100) - 13));
    }
    if (_MSC_VER >= 1500) // 1500: MSVC 2008, 1600: MSVC 2010, ... (2-year release cycle)
    {
        return QLatin1String("MSVC ") + QString::number(2008 + 2 * ((_MSC_VER / 100) - 15));
    }
#else
    return QLatin1String("<unknown compiler>");
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString buildCompatibilityString()
{
    return QCoreApplication::tr("Based on Qt %1 (%2, %3 bit)").arg(QLatin1String(qVersion()), compilerString(),
                                                                   QString::number(QSysInfo::WordSize));
}

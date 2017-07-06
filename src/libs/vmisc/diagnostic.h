/************************************************************************
 **
 **  @file   diagnostic.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 8, 2016
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

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)

#include <QtCore/qcompilerdetection.h>

#else

/*
 * Warning/diagnostic handling
 */

#define QT_DO_PRAGMA(text)                      _Pragma(#text)
#if defined(Q_CC_INTEL) && defined(Q_CC_MSVC)
/* icl.exe: Intel compiler on Windows */
#  undef QT_DO_PRAGMA                           /* not needed */
#  define QT_WARNING_PUSH                       __pragma(warning(push))
#  define QT_WARNING_POP                        __pragma(warning(pop))
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_INTEL(number)      __pragma(warning(disable: number))
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#elif defined(Q_CC_INTEL)
/* icc: Intel compiler on Linux or OS X */
#  define QT_WARNING_PUSH                       QT_DO_PRAGMA(warning(push))
#  define QT_WARNING_POP                        QT_DO_PRAGMA(warning(pop))
#  define QT_WARNING_DISABLE_INTEL(number)      QT_DO_PRAGMA(warning(disable: number))
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#elif defined(Q_CC_MSVC) && _MSC_VER >= 1500
#  undef QT_DO_PRAGMA                           /* not needed */
#  define QT_WARNING_PUSH                       __pragma(warning(push))
#  define QT_WARNING_POP                        __pragma(warning(pop))
#  define QT_WARNING_DISABLE_MSVC(number)       __pragma(warning(disable: number))
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#elif defined(Q_CC_CLANG)
#  define QT_WARNING_PUSH                       QT_DO_PRAGMA(clang diagnostic push)
#  define QT_WARNING_POP                        QT_DO_PRAGMA(clang diagnostic pop)
#  define QT_WARNING_DISABLE_CLANG(text)        QT_DO_PRAGMA(clang diagnostic ignored text)
#  define QT_WARNING_DISABLE_GCC(text)          QT_DO_PRAGMA(GCC diagnostic ignored text)   // GCC directives work in Clang too
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#elif defined(Q_CC_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define QT_WARNING_PUSH                       QT_DO_PRAGMA(GCC diagnostic push)
#  define QT_WARNING_POP                        QT_DO_PRAGMA(GCC diagnostic pop)
#  define QT_WARNING_DISABLE_GCC(text)          QT_DO_PRAGMA(GCC diagnostic ignored text)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#else       // All other compilers, GCC < 4.6 and MSVC < 2008
#  define QT_WARNING_DISABLE_GCC(text)
#  define QT_WARNING_PUSH
#  define QT_WARNING_POP
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#endif

#endif // QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)

#endif // DIAGNOSTIC_H

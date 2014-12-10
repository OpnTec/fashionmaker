/************************************************************************
 **
 **  @file   ifcdef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef IFCDEF_H
#define IFCDEF_H

#include <csignal>

static const quint32 null_id = 0;

#define NULL_ID null_id//use this value for initialization variables that keeps id values. 0 mean uknown id value.
#define NULL_ID_STR "0"

/*
 * This macros SCASSERT (for Stop and Continue Assert) will break into the debugger on the line of the assert and allow
 * you to continue afterwards should you choose to.
 * idea: Q_ASSERT no longer pauses debugger - http://qt-project.org/forums/viewthread/13148
 * Usefull links:
 * 1. What's the difference between __PRETTY_FUNCTION__, __FUNCTION__, __func__? -
 *    https://stackoverflow.com/questions/4384765/whats-the-difference-between-pretty-function-function-func
 *
 * 2. Windows Predefined Macros - http://msdn.microsoft.com/library/b0084kay.aspx
 *
 * 3. Windows DebugBreak function - http://msdn.microsoft.com/en-us/library/ms679297%28VS.85%29.aspx
 *
 * 4. Continue to debug after failed assertion on Linux? [C/C++] -
 * https://stackoverflow.com/questions/1721543/continue-to-debug-after-failed-assertion-on-linux-c-c
 */
#ifndef QT_NO_DEBUG
#ifdef Q_OS_WIN32
#ifdef Q_CC_MSVC
#define SCASSERT(cond)                                      \
{                                                           \
    if (!(cond))                                            \
    {                                                       \
        qDebug("ASSERT: %s in %s (%s:%u)",                  \
            #cond, __FUNCSIG__, __FILE__, __LINE__);        \
        DebugBreak();                                       \
    }                                                       \
}                                                           \

#else

#define SCASSERT(cond)                                      \
{                                                           \
    if (!(cond))                                            \
    {                                                       \
        qDebug("ASSERT: %s in %s (%s:%u)",                  \
            #cond, __PRETTY_FUNCTION__, __FILE__, __LINE__);\
        DebugBreak();                                       \
    }                                                       \
}                                                           \

#endif /*Q_CC_MSVC*/
#else
#define SCASSERT(cond)                                      \
{                                                           \
    if (!(cond))                                            \
    {                                                       \
        qDebug("ASSERT: %s in %s (%s:%u)",                  \
            #cond, __PRETTY_FUNCTION__, __FILE__, __LINE__);\
        std::raise(SIGTRAP);                                \
    }                                                       \
}                                                           \

#endif /* Q_OS_WIN32 */
#else // define but disable this function if debugging is not set
#define SCASSERT(cond) qt_noop();
#endif /* QT_NO_DEBUG */

// Detect whether the compiler supports C++11 noexcept exception specifications.
#  if   defined(__clang__)
#    if __has_feature(cxx_noexcept)
#      define V_NOEXCEPT_EXPR(x) noexcept(x) // Clang 3.0 and above have noexcept
#    endif
#  elif defined(__GNUC__)
#    if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#      define V_NOEXCEPT_EXPR(x) noexcept(x) // GCC 4.7 and following have noexcept
#    endif
#  elif defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
#    define QMUP_NOEXCEPT_EXPR(x) noexcept(x)
#  else
#    define V_NOEXCEPT_EXPR(x)
#  endif

#endif // IFCDEF_H

/************************************************************************
 **
 **  @file   def.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
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

#ifndef DEF_H
#define DEF_H

#include <csignal>
#include <QtGlobal>

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
#ifndef V_NO_ASSERT
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

#else // GCC (Windows)

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
#else // UNIX
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
#endif /* V_NO_ASSERT */

#endif // DEF_H

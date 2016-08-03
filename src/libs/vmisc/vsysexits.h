/************************************************************************
 **
 **  @file   vsysexits.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 9, 2015
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

#ifndef VSYSEXITS_H
#define VSYSEXITS_H

#include <QTextStream>

static const auto V_EX_OK = 0;               /*Indicate the successful exit.*/

static const auto V_EX_USAGE = 64;           /*The command was used incorrectly, e.g., with the wrong number of
                                             arguments, a bad flag, a bad syntax in a parameter, or whatever.*/

static const auto V_EX_DATAERR = 65;         /*The input data was incorrect in some way.  This should only be used for
                                             user's data and not system files.*/

static const auto V_EX_NOINPUT = 66;         /*An input file (not a system file) did not exist or was not readable.*/

static const auto V_EX_UNAVAILABLE = 69;     /*A service is unavailable.  This can occur if a support program or file
                                             does not exist.  This can also be used as a catchall message when
                                             something you wanted to do doesn't work, but you don't know why.*/

static const auto V_EX_SOFTWARE = 70;        /*An internal software error has been detected.  This should be limited to
                                             nonoperating operating system related errors as possible.*/

static const auto V_EX_OSERR = 71;           /*An operating system error has been detected.  This is intended to be
                                             used for such things as ``cannot fork'', ``cannot create pipe'', or the
                                             like. It includes things like getuid returning a user that does not exist
                                             in the passwd file.*/

static const auto V_EX_OSFILE = 72;          /*Some system file (e.g., /etc/passwd, /var/run/utmp, etc.) does not exist,
                                             cannot be opened, or has some sort of error (e.g., syntax error).*/

static const auto V_EX_CANTCREAT = 73;       /*A (user specified) output file cannot be created.*/

static const auto V_EX_IOERR = 74;           /*An error occurred while doing I/O on some file.*/

static const auto V_EX_NOPERM = 77;          /*You did not have sufficient permission to perform the operation.  This
                                             is not intended for file system problems, which should use EX_NOINPUT or
                                             EX_CANTCREAT, but rather for higher level permissions.*/

static const auto V_EX_CONFIG = 78;          /*Something was found in an unconfigured or misconfigured state.*/

//---------------------------------------------------------------------------------------------------------------------
inline QTextStream& vStdErr()
{
    static QTextStream ts( stderr );
    ts.flush();
    return ts;
}

//---------------------------------------------------------------------------------------------------------------------
inline QTextStream& vStdOut()
{
    static QTextStream ts( stdout );
    ts.flush();
    return ts;
}

#endif // VSYSEXITS_H

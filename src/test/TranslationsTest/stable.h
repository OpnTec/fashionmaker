/************************************************************************
 **
 **  @file   stable.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef STABLE_H
#define STABLE_H

/* I like to include this pragma too, so the build log indicates if pre-compiled headers were in use. */
#pragma message("Compiling precompiled headers for Translation Tests.\n")

/* Add C includes here */

#if defined __cplusplus
/* Add C++ includes here */
#include <csignal>

/*In all cases we need include core header for getting defined values*/
#ifdef QT_CORE_LIB
#   include <QtCore>
#endif

#ifdef QT_GUI_LIB
#   include <QtGui>
#endif

#ifdef QT_XML_LIB
#   include <QtXml>
#endif

//In Windows you can't use same header in all modes.
#if !defined(Q_OS_WIN)
#   ifdef QT_WIDGETS_LIB
#       include <QtWidgets>
#   endif

#   ifdef QT_SVG_LIB
#       include <QtSvg/QtSvg>
#   endif

#   ifdef QT_PRINTSUPPORT_LIB
#       include <QtPrintSupport>
#   endif

    //Build doesn't work, if include this headers on Windows.
#   ifdef QT_XMLPATTERNS_LIB
#       include <QtXmlPatterns>
#   endif

#   ifdef QT_NETWORK_LIB
#       include <QtNetwork>
#   endif
#endif/*Q_OS_WIN*/

#endif /*__cplusplus*/

#endif // STABLE_H

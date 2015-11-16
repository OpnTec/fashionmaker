/************************************************************************
 **
 **  @file   stable.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 9, 2014
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#ifndef STABLE_H
#define STABLE_H

/* I like to include this pragma too, so the build log indicates if pre-compiled headers were in use. */
#pragma message("Compiling precompiled headers for VPropertyExplorer library.\n")

/* Add C includes here */

#if defined __cplusplus
/* Add C++ includes here */

/*In all cases we need include core header for getting defined values*/
#ifdef QT_CORE_LIB
#   include <QtCore>
#endif

#endif /*__cplusplus*/

#endif // STABLE_H

/************************************************************************
 **
 **  @file   vpropertyexplorer_global.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 8, 2014
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

#ifndef VPROPERTYEXPLORER_GLOBAL_H
#define VPROPERTYEXPLORER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VPROPERTYEXPLORER_LIBRARY)
#  define VPROPERTYEXPLORERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VPROPERTYEXPLORERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VPROPERTYEXPLORER_GLOBAL_H

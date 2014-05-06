/************************************************************************
 **
 **  @file   version.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VERSION_H
#define VERSION_H

extern const int MAJOR_VERSION;
extern const int MINOR_VERSION;
extern const int DEBUG_VERSION;

extern const QString APP_VERSION;
extern const QString WARRANTY;

#define VER_FILEVERSION             0,2,2,0
#define VER_FILEVERSION_STR         "0.2.2.0\0"

#define VER_PRODUCTVERSION          0,2,2,0
#define VER_PRODUCTVERSION_STR      "0.2\0"

#define VER_COMPANYNAME_STR         "ValentinaTeam"
#define VER_FILEDESCRIPTION_STR     "Valentina"
#define VER_INTERNALNAME_STR        "Valentina"
#define VER_LEGALCOPYRIGHT_STR      "Copyright © 2014 Valentina Team"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "valentina.exe"
#define VER_PRODUCTNAME_STR         "Valentina"

#define VER_COMPANYDOMAIN_STR       "www.valentina-project.org"

#endif // VERSION_H

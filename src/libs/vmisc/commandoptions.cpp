/************************************************************************
 **
 **  @file   commandoptions.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2015
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

#include "commandoptions.h"
#include "def.h"

#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QStringList>

const QString LONG_OPTION_BASENAME          = QStringLiteral("basename");
const QString SINGLE_OPTION_BASENAME        = QStringLiteral("b");

const QString LONG_OPTION_DESTINATION       = QStringLiteral("destination");
const QString SINGLE_OPTION_DESTINATION     = QStringLiteral("d");

const QString LONG_OPTION_MEASUREFILE       = QStringLiteral("mfile");
const QString SINGLE_OPTION_MEASUREFILE     = QStringLiteral("m");

const QString LONG_OPTION_PAGETEMPLATE      = QStringLiteral("pageformat");
const QString SINGLE_OPTION_PAGETEMPLATE    = QStringLiteral("p");

const QString LONG_OPTION_EXP2FORMAT        = QStringLiteral("format");
const QString SINGLE_OPTION_EXP2FORMAT      = QStringLiteral("f");

const QString LONG_OPTION_BINARYDXF         = QStringLiteral("bdxf");
const QString LONG_OPTION_TEXT2PATHS        = QStringLiteral("text2paths");
const QString LONG_OPTION_EXPORTONLYDETAILS = QStringLiteral("exportOnlyDetails");

const QString LONG_OPTION_ROTATE            = QStringLiteral("rotate");
const QString SINGLE_OPTION_ROTATE          = QStringLiteral("r");

const QString LONG_OPTION_CROP              = QStringLiteral("crop");
const QString SINGLE_OPTION_CROP            = QStringLiteral("c");

const QString LONG_OPTION_UNITE             = QStringLiteral("unite");
const QString SINGLE_OPTION_UNITE           = QStringLiteral("u");

const QString LONG_OPTION_PAGEW             = QStringLiteral("pagew");
const QString SINGLE_OPTION_PAGEW           = QStringLiteral("W");

const QString LONG_OPTION_PAGEH             = QStringLiteral("pageh");
const QString SINGLE_OPTION_PAGEH           = QStringLiteral("H");

const QString LONG_OPTION_PAGEUNITS         = QStringLiteral("pageunits");
const QString SINGLE_OPTION_PAGEUNITS       = QStringLiteral("U");

const QString LONG_OPTION_SAVELENGTH        = QStringLiteral("savelen");
const QString SINGLE_OPTION_SAVELENGTH      = QStringLiteral("S");

const QString LONG_OPTION_SHIFTLENGTH       = QStringLiteral("shiftlen");
const QString SINGLE_OPTION_SHIFTLENGTH     = QStringLiteral("s");

const QString LONG_OPTION_SHIFTUNITS        = QStringLiteral("layounits");
const QString SINGLE_OPTION_SHIFTUNITS      = QStringLiteral("l");

const QString LONG_OPTION_GAPWIDTH          = QStringLiteral("gapwidth");
const QString SINGLE_OPTION_GAPWIDTH        = QStringLiteral("G");

const QString LONG_OPTION_GROUPPING         = QStringLiteral("groups");
const QString SINGLE_OPTION_GROUPPING       = QStringLiteral("g");

const QString LONG_OPTION_TEST              = QStringLiteral("test");
const QString SINGLE_OPTION_TEST            = QStringLiteral("t");

const QString LONG_OPTION_GRADATIONSIZE     = QStringLiteral("gsize");
const QString SINGLE_OPTION_GRADATIONSIZE   = QStringLiteral("x");

const QString LONG_OPTION_GRADATIONHEIGHT   = QStringLiteral("gheight");
const QString SINGLE_OPTION_GRADATIONHEIGHT = QStringLiteral("e");

const QString LONG_OPTION_IGNORE_MARGINS    = QStringLiteral("ignoremargins");
const QString SINGLE_OPTION_IGNORE_MARGINS  = QStringLiteral("i");

const QString LONG_OPTION_LEFT_MARGIN       = QStringLiteral("lmargin");
const QString SINGLE_OPTION_LEFT_MARGIN     = QStringLiteral("L");

const QString LONG_OPTION_RIGHT_MARGIN      = QStringLiteral("rmargin");
const QString SINGLE_OPTION_RIGHT_MARGIN    = QStringLiteral("R");

const QString LONG_OPTION_TOP_MARGIN        = QStringLiteral("tmargin");
const QString SINGLE_OPTION_TOP_MARGIN      = QStringLiteral("T");

const QString LONG_OPTION_BOTTOM_MARGIN     = QStringLiteral("bmargin");
const QString SINGLE_OPTION_BOTTOM_MARGIN   = QStringLiteral("B");

//---------------------------------------------------------------------------------------------------------------------
QStringList AllKeys()
{
    QStringList list;
    list << LONG_OPTION_BASENAME << SINGLE_OPTION_BASENAME
         << LONG_OPTION_DESTINATION << SINGLE_OPTION_DESTINATION
         << LONG_OPTION_MEASUREFILE << SINGLE_OPTION_MEASUREFILE
         << LONG_OPTION_PAGETEMPLATE << SINGLE_OPTION_PAGETEMPLATE
         << LONG_OPTION_EXP2FORMAT << SINGLE_OPTION_EXP2FORMAT
         << LONG_OPTION_BINARYDXF
         << LONG_OPTION_ROTATE << SINGLE_OPTION_ROTATE
         << LONG_OPTION_CROP << SINGLE_OPTION_CROP
         << LONG_OPTION_UNITE << SINGLE_OPTION_UNITE
         << LONG_OPTION_PAGEW << SINGLE_OPTION_PAGEW
         << LONG_OPTION_PAGEH << SINGLE_OPTION_PAGEH
         << LONG_OPTION_PAGEUNITS << SINGLE_OPTION_PAGEUNITS
         << LONG_OPTION_SAVELENGTH << SINGLE_OPTION_SAVELENGTH
         << LONG_OPTION_SHIFTLENGTH << SINGLE_OPTION_SHIFTLENGTH
         << LONG_OPTION_SHIFTUNITS << SINGLE_OPTION_SHIFTUNITS
         << LONG_OPTION_GAPWIDTH << SINGLE_OPTION_GAPWIDTH
         << LONG_OPTION_GROUPPING << SINGLE_OPTION_GROUPPING
         << LONG_OPTION_TEST << SINGLE_OPTION_TEST
         << LONG_OPTION_GRADATIONSIZE << SINGLE_OPTION_GRADATIONSIZE
         << LONG_OPTION_GRADATIONHEIGHT << SINGLE_OPTION_GRADATIONHEIGHT
         << LONG_OPTION_IGNORE_MARGINS << SINGLE_OPTION_IGNORE_MARGINS
         << LONG_OPTION_LEFT_MARGIN << SINGLE_OPTION_LEFT_MARGIN
         << LONG_OPTION_RIGHT_MARGIN << SINGLE_OPTION_RIGHT_MARGIN
         << LONG_OPTION_TOP_MARGIN << SINGLE_OPTION_TOP_MARGIN
         << LONG_OPTION_BOTTOM_MARGIN << SINGLE_OPTION_BOTTOM_MARGIN
         << LONG_OPTION_NO_HDPI_SCALING;

    return list;
}

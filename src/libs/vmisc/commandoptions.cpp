/************************************************************************
 **
 **  @file   commandoptions.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
const QString LONG_OPTION_EXPORTSUCHDETAILS = QStringLiteral("exportSuchDetails");
const QString LONG_OPTION_EXPXSCALE         = QStringLiteral("xscale");
const QString LONG_OPTION_EXPYSCALE         = QStringLiteral("yscale");

const QString LONG_OPTION_CROP_LENGTH       = QStringLiteral("crop");
const QString SINGLE_OPTION_CROP_LENGTH     = QStringLiteral("c");

const QString LONG_OPTION_CROP_WIDTH        = QStringLiteral("cropWidth");

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

const QString LONG_OPTION_SHIFTUNITS        = QStringLiteral("layounits");
const QString SINGLE_OPTION_SHIFTUNITS      = QStringLiteral("l");

const QString LONG_OPTION_GAPWIDTH          = QStringLiteral("gapwidth");
const QString SINGLE_OPTION_GAPWIDTH        = QStringLiteral("G");

const QString LONG_OPTION_GROUPPING         = QStringLiteral("groups");
const QString SINGLE_OPTION_GROUPPING       = QStringLiteral("g");

const QString LONG_OPTION_TEST              = QStringLiteral("test");
const QString SINGLE_OPTION_TEST            = QStringLiteral("t");

const QString LONG_OPTION_PENDANTIC         = QStringLiteral("pedantic");

const QString LONG_OPTION_GRADATIONSIZE     = QStringLiteral("gsize");
const QString SINGLE_OPTION_GRADATIONSIZE   = QStringLiteral("x");

const QString LONG_OPTION_GRADATIONHEIGHT   = QStringLiteral("gheight");
const QString SINGLE_OPTION_GRADATIONHEIGHT = QStringLiteral("e");

const QString LONG_OPTION_USER_MATERIAL     = QStringLiteral("userMaterial");

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

const QString LONG_OPTION_NESTING_TIME      = QStringLiteral("time");
const QString SINGLE_OPTION_NESTING_TIME    = QStringLiteral("n");

const QString LONG_OPTION_EFFICIENCY_COEFFICIENT  = QStringLiteral("coefficient");

const QString LONG_OPTION_CSVWITHHEADER = QStringLiteral("csvWithHeader");
const QString LONG_OPTION_CSVCODEC      = QStringLiteral("csvCodec");
const QString LONG_OPTION_CSVSEPARATOR  = QStringLiteral("csvSeparator");
const QString LONG_OPTION_CSVEXPORTFM   = QStringLiteral("csvExportFM");

const QString LONG_OPTION_TILED_PDF_PAGE_TEMPLATE = QStringLiteral("tiledPageformat");
const QString LONG_OPTION_TILED_PDF_LEFT_MARGIN   = QStringLiteral("tiledlmargin");
const QString LONG_OPTION_TILED_PDF_RIGHT_MARGIN  = QStringLiteral("tiledrmargin");
const QString LONG_OPTION_TILED_PDF_TOP_MARGIN    = QStringLiteral("tiledtmargin");
const QString LONG_OPTION_TILED_PDF_BOTTOM_MARGIN = QStringLiteral("tiledbmargin");
const QString LONG_OPTION_TILED_PDF_LANDSCAPE     = QStringLiteral("tiledLandscape");

const QString LONG_OPTION_FOLLOW_GRAINLINE = QStringLiteral("followGrainline");
const QString LONG_OPTION_MANUAL_PRIORITY = QStringLiteral("manualPriority");

const QString LONG_OPTION_LANDSCAPE_ORIENTATION = QStringLiteral("landscapeOrientation");

const QString LONG_OPTION_NEST_QUANTITY = QStringLiteral("nestQuantity");
const QString LONG_OPTION_PREFER_ONE_SHEET_SOLUTION = QStringLiteral("preferOneSheetSolution");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AllKeys return list with all command line keys (short and long forms). Used for testing on conflicts.
 * @return list with all command line keys
 */
QStringList AllKeys()
{
    return QStringList
    {
        LONG_OPTION_BASENAME, SINGLE_OPTION_BASENAME,
        LONG_OPTION_DESTINATION, SINGLE_OPTION_DESTINATION,
        LONG_OPTION_MEASUREFILE, SINGLE_OPTION_MEASUREFILE,
        LONG_OPTION_PAGETEMPLATE, SINGLE_OPTION_PAGETEMPLATE,
        LONG_OPTION_EXP2FORMAT, SINGLE_OPTION_EXP2FORMAT,
        LONG_OPTION_BINARYDXF,
        LONG_OPTION_TEXT2PATHS,
        LONG_OPTION_EXPORTONLYDETAILS,
        LONG_OPTION_EXPORTSUCHDETAILS,
        LONG_OPTION_EXPXSCALE,
        LONG_OPTION_EXPYSCALE,
        LONG_OPTION_CROP_LENGTH, SINGLE_OPTION_CROP_LENGTH,
        LONG_OPTION_CROP_WIDTH,
        LONG_OPTION_UNITE, SINGLE_OPTION_UNITE,
        LONG_OPTION_PAGEW, SINGLE_OPTION_PAGEW,
        LONG_OPTION_PAGEH, SINGLE_OPTION_PAGEH,
        LONG_OPTION_PAGEUNITS, SINGLE_OPTION_PAGEUNITS,
        LONG_OPTION_SAVELENGTH, SINGLE_OPTION_SAVELENGTH,
        LONG_OPTION_SHIFTUNITS, SINGLE_OPTION_SHIFTUNITS,
        LONG_OPTION_GAPWIDTH, SINGLE_OPTION_GAPWIDTH,
        LONG_OPTION_GROUPPING, SINGLE_OPTION_GROUPPING,
        LONG_OPTION_TEST, SINGLE_OPTION_TEST,
        LONG_OPTION_PENDANTIC,
        LONG_OPTION_GRADATIONSIZE, SINGLE_OPTION_GRADATIONSIZE,
        LONG_OPTION_GRADATIONHEIGHT, SINGLE_OPTION_GRADATIONHEIGHT,
        LONG_OPTION_USER_MATERIAL,
        LONG_OPTION_IGNORE_MARGINS, SINGLE_OPTION_IGNORE_MARGINS,
        LONG_OPTION_LEFT_MARGIN, SINGLE_OPTION_LEFT_MARGIN,
        LONG_OPTION_RIGHT_MARGIN, SINGLE_OPTION_RIGHT_MARGIN,
        LONG_OPTION_TOP_MARGIN, SINGLE_OPTION_TOP_MARGIN,
        LONG_OPTION_BOTTOM_MARGIN, SINGLE_OPTION_BOTTOM_MARGIN,
        LONG_OPTION_NESTING_TIME, SINGLE_OPTION_NESTING_TIME,
        LONG_OPTION_EFFICIENCY_COEFFICIENT,
        LONG_OPTION_NO_HDPI_SCALING,
        LONG_OPTION_CSVWITHHEADER,
        LONG_OPTION_CSVCODEC,
        LONG_OPTION_CSVSEPARATOR,
        LONG_OPTION_CSVEXPORTFM,
        LONG_OPTION_TILED_PDF_PAGE_TEMPLATE,
        LONG_OPTION_TILED_PDF_LEFT_MARGIN,
        LONG_OPTION_TILED_PDF_RIGHT_MARGIN,
        LONG_OPTION_TILED_PDF_TOP_MARGIN,
        LONG_OPTION_TILED_PDF_BOTTOM_MARGIN,
        LONG_OPTION_TILED_PDF_LANDSCAPE,
        LONG_OPTION_FOLLOW_GRAINLINE,
        LONG_OPTION_MANUAL_PRIORITY,
        LONG_OPTION_LANDSCAPE_ORIENTATION,
        LONG_OPTION_NEST_QUANTITY,
        LONG_OPTION_PREFER_ONE_SHEET_SOLUTION
    };
}

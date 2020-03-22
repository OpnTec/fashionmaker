/************************************************************************
 **
 **  @file   commandoptions.h
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

#ifndef COMMANDOPTIONS_H
#define COMMANDOPTIONS_H

#include <QString>

extern const QString LONG_OPTION_BASENAME;
extern const QString SINGLE_OPTION_BASENAME;

extern const QString LONG_OPTION_DESTINATION;
extern const QString SINGLE_OPTION_DESTINATION;

extern const QString LONG_OPTION_MEASUREFILE;
extern const QString SINGLE_OPTION_MEASUREFILE;

extern const QString LONG_OPTION_PAGETEMPLATE;
extern const QString SINGLE_OPTION_PAGETEMPLATE;

extern const QString LONG_OPTION_EXP2FORMAT;
extern const QString SINGLE_OPTION_EXP2FORMAT;

extern const QString LONG_OPTION_BINARYDXF;
extern const QString LONG_OPTION_TEXT2PATHS;
extern const QString LONG_OPTION_EXPORTONLYDETAILS;
extern const QString LONG_OPTION_EXPORTSUCHDETAILS;
extern const QString LONG_OPTION_EXPXSCALE;
extern const QString LONG_OPTION_EXPYSCALE;

extern const QString LONG_OPTION_CROP_LENGTH;
extern const QString SINGLE_OPTION_CROP_LENGTH;

extern const QString LONG_OPTION_CROP_WIDTH;

extern const QString LONG_OPTION_UNITE;
extern const QString SINGLE_OPTION_UNITE;

extern const QString LONG_OPTION_PAGEW;
extern const QString SINGLE_OPTION_PAGEW;

extern const QString LONG_OPTION_PAGEH;
extern const QString SINGLE_OPTION_PAGEH;

extern const QString LONG_OPTION_PAGEUNITS;
extern const QString SINGLE_OPTION_PAGEUNITS;

extern const QString LONG_OPTION_SAVELENGTH;
extern const QString SINGLE_OPTION_SAVELENGTH;

extern const QString LONG_OPTION_SHIFTUNITS;
extern const QString SINGLE_OPTION_SHIFTUNITS;

extern const QString LONG_OPTION_GAPWIDTH;
extern const QString SINGLE_OPTION_GAPWIDTH;

extern const QString LONG_OPTION_GROUPPING;
extern const QString SINGLE_OPTION_GROUPPING;

extern const QString LONG_OPTION_TEST;
extern const QString SINGLE_OPTION_TEST;

extern const QString LONG_OPTION_PENDANTIC;

extern const QString LONG_OPTION_GRADATIONSIZE;
extern const QString SINGLE_OPTION_GRADATIONSIZE;

extern const QString LONG_OPTION_GRADATIONHEIGHT;
extern const QString SINGLE_OPTION_GRADATIONHEIGHT;

extern const QString LONG_OPTION_USER_MATERIAL;

extern const QString LONG_OPTION_IGNORE_MARGINS;
extern const QString SINGLE_OPTION_IGNORE_MARGINS;

extern const QString LONG_OPTION_LEFT_MARGIN;
extern const QString SINGLE_OPTION_LEFT_MARGIN;

extern const QString LONG_OPTION_RIGHT_MARGIN;
extern const QString SINGLE_OPTION_RIGHT_MARGIN;

extern const QString LONG_OPTION_TOP_MARGIN;
extern const QString SINGLE_OPTION_TOP_MARGIN;

extern const QString LONG_OPTION_BOTTOM_MARGIN;
extern const QString SINGLE_OPTION_BOTTOM_MARGIN;

extern const QString LONG_OPTION_NESTING_TIME;
extern const QString SINGLE_OPTION_NESTING_TIME;

extern const QString LONG_OPTION_EFFICIENCY_COEFFICIENT;

extern const QString LONG_OPTION_CSVWITHHEADER;
extern const QString LONG_OPTION_CSVCODEC;
extern const QString LONG_OPTION_CSVSEPARATOR;
extern const QString LONG_OPTION_CSVEXPORTFM;

extern const QString LONG_OPTION_TILED_PDF_PAGE_TEMPLATE;
extern const QString LONG_OPTION_TILED_PDF_LEFT_MARGIN;
extern const QString LONG_OPTION_TILED_PDF_RIGHT_MARGIN;
extern const QString LONG_OPTION_TILED_PDF_TOP_MARGIN;
extern const QString LONG_OPTION_TILED_PDF_BOTTOM_MARGIN;
extern const QString LONG_OPTION_TILED_PDF_LANDSCAPE;
extern const QString LONG_OPTION_FOLLOW_GRAINLINE;
extern const QString LONG_OPTION_MANUAL_PRIORITY;
extern const QString LONG_OPTION_LANDSCAPE_ORIENTATION;
extern const QString LONG_OPTION_NEST_QUANTITY;
extern const QString LONG_OPTION_PREFER_ONE_SHEET_SOLUTION;

QStringList AllKeys();

#endif // COMMANDOPTIONS_H

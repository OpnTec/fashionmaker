/************************************************************************
 **
 **  @file   vtranslation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 5, 2014
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

#ifndef VTRANSLATION_H
#define VTRANSLATION_H

#include <QCoreApplication>
#include <QString>

/**
 * @file
 * @brief The VTranslation class help store string for translation.
 *
 * I took idea from this article http://ololoepepe.blogspot.com/2013/08/qt.html.
 * As you know, if wrap string to a function translate, it will be marked for translation. No matter what namespace
 * contains this function. In class Translation used this circumstance.
 * Instead of using QT_TRANSLATE_NOOP3 macros we can store strings in QMap.
 * Example:
 * create map and fill up its
 * QMap<QString, VTranslation> map;
 * map.insert("head_girth", VTranslation::translate("Measurements", "head_girth", "Around fullest part of Head."));
 * get translated string
 * map.value(measurement).translate();
 */
class VTranslation
{
public:
    VTranslation(const QString &context, const QString &sourceText, const QString &disambiguation = 0, int n = -1);
    QString translate() const;
    static VTranslation translate(const QString &context, const QString &sourceText, const QString &disambiguation = 0,
                                  int n = -1);
private:
    QString mcontext;
    QString msourceText;
    QString mdisambiguation;
    int     mn;
};

#endif // VTRANSLATION_H

/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "dxfdef.h"

#include <QMap>
#include <QString>

const QString endStringPlaceholder = QStringLiteral("%&?_?&%");

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> LocaleMap()
{
    QMap <QString, QString> locMap
    {
        {"croatian", "ISO8859-2"},
        {"cs", "ISO8859-2"},
        {"cs_CS", "ISO8859-2"},
        {"cs_CZ", "ISO8859-2"},
        {"cz", "ISO8859-2"},
        {"cz_CZ", "ISO8859-2"},
        {"czech", "ISO8859-2"},
        {"hr", "ISO8859-2"},
        {"hr_HR", "ISO8859-2"},
        {"hu", "ISO8859-2"},
        {"hu_HU", "ISO8859-2"},
        {"hungarian", "ISO8859-2"},
        {"pl", "ISO8859-2"},
        {"pl_PL", "ISO8859-2"},
        {"polish", "ISO8859-2"},
        {"ro", "ISO8859-2"},
        {"ro_RO", "ISO8859-2"},
        {"rumanian", "ISO8859-2"},
        {"serbocroatian", "ISO8859-2"},
        {"sh", "ISO8859-2"},
        {"sh_SP", "ISO8859-2"},
        {"sh_YU", "ISO8859-2"},
        {"sk", "ISO8859-2"},
        {"sk_SK", "ISO8859-2"},
        {"sl", "ISO8859-2"},
        {"sl_CS", "ISO8859-2"},
        {"sl_SI", "ISO8859-2"},
        {"slovak", "ISO8859-2"},
        {"slovene", "ISO8859-2"},
        {"sr_SP", "ISO8859-2"},

        {"eo", "ISO8859-3"},

        {"ee", "ISO8859-4"},
        {"ee_EE", "ISO8859-4"},

        {"mk", "ISO8859-5"},
        {"mk_MK", "ISO8859-5"},
        {"sp", "ISO8859-5"},
        {"sp_YU", "ISO8859-5"},

        {"id", "ISO8859-1"},
        {"id_ID", "ISO8859-1"},

        {"ar_AA", "ISO8859-6"},
        {"ar_SA", "ISO8859-6"},
        {"arabic", "ISO8859-6"},

        {"el", "ISO8859-7"},
        {"el_GR", "ISO8859-7"},
        {"greek", "ISO8859-7"},

        {"hebrew", "ISO8859-8"},
        {"he", "ISO8859-8"},
        {"he_IL", "ISO8859-8"},
        {"iw", "ISO8859-8"},
        {"iw_IL", "ISO8859-8"},

        {"tr", "ISO8859-9"},
        {"tr_TR", "ISO8859-9"},
        {"turkish", "ISO8859-9"},

        {"lt", "ISO8859-13"},
        {"lt_LT", "ISO8859-13"},
        {"lv", "ISO8859-13"},
        {"lv_LV", "ISO8859-13"},

        {"et", "ISO8859-15"},
        {"et_EE", "ISO8859-15"},
        {"br_FR", "ISO8859-15"},
        {"ca_ES", "ISO8859-15"},
        {"de", "ISO8859-15"},
        {"de_AT", "ISO8859-15"},
        {"de_BE", "ISO8859-15"},
        {"de_DE", "ISO8859-15"},
        {"de_LU", "ISO8859-15"},
        {"en_IE", "ISO8859-15"},
        {"es", "ISO8859-15"},
        {"es_ES", "ISO8859-15"},
        {"eu_ES", "ISO8859-15"},
        {"fi", "ISO8859-15"},
        {"fi_FI", "ISO8859-15"},
        {"finnish", "ISO8859-15"},
        {"fr", "ISO8859-15"},
        {"fr_FR", "ISO8859-15"},
        {"fr_BE", "ISO8859-15"},
        {"fr_LU", "ISO8859-15"},
        {"french", "ISO8859-15"},
        {"ga_IE", "ISO8859-15"},
        {"gl_ES", "ISO8859-15"},
        {"it", "ISO8859-15"},
        {"it_IT", "ISO8859-15"},
        {"oc_FR", "ISO8859-15"},
        {"nl", "ISO8859-15"},
        {"nl_BE", "ISO8859-15"},
        {"nl_NL", "ISO8859-15"},
        {"pt", "ISO8859-15"},
        {"pt_PT", "ISO8859-15"},
        {"pt_BR", "ISO8859-15"},
        {"sv_FI", "ISO8859-15"},
        {"wa_BE", "ISO8859-15"},

        {"uk", "KOI8-U"},
        {"uk_UA", "KOI8-U"},
        {"ru_YA", "KOI8-U"},
        {"ukrainian", "KOI8-U"},
        {"ru_RU", "KOI8-U"},

        {"be", "KOI8-R"},
        {"be_BY", "KOI8-R"},
        {"bg", "KOI8-R"},
        {"bg_BG", "KOI8-R"},
        {"bulgarian", "KOI8-R"},
        {"ba_RU", "KOI8-R"},
        {"ky", "KOI8-R"},
        {"ky_KG", "KOI8-R"},
        {"kk", "KOI8-R"},
        {"kk_KZ", "KOI8-R"},

        {"zh", "CP932"},
        {"zh_CN", "CP932"},
    };

    return locMap;
}

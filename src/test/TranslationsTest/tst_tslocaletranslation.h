/************************************************************************
 **
 **  @file   tst_tslocaletranslation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 2, 2018
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2018 Valentina project
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
#ifndef TST_TSLOCALETRANSLATION_H
#define TST_TSLOCALETRANSLATION_H

#include "tst_abstracttranslation.h"

class TST_TSLocaleTranslation : public TST_AbstractTranslation
{
    Q_OBJECT
public:
    explicit TST_TSLocaleTranslation(const QString &locale, QObject *parent = nullptr);

private slots:
    void CheckPlaceMarkerExist_data();
    void CheckPlaceMarkerExist();
    void TestPunctuation_data();
    void TestPunctuation();
    void TestHTMLTags_data();
    void TestHTMLTags();

private:
    Q_DISABLE_COPY(TST_TSLocaleTranslation)

    QString m_locale;
};

#endif // TST_TSLOCALETRANSLATION_H

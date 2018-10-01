/************************************************************************
 **
 **  @file   tst_tstranslation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 12, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef TST_TSTRANSLATION_H
#define TST_TSTRANSLATION_H

#include "tst_abstracttranslation.h"

class TST_TSTranslation : public TST_AbstractTranslation
{
    Q_OBJECT
public:
    explicit TST_TSTranslation(QObject *parent = nullptr);

private slots:
    void CheckEnglishLocalization_data();
    void CheckEnglishLocalization();
    void CheckEmptyToolButton_data();
    void CheckEmptyToolButton();
    void CheckEllipsis_data();
    void CheckEllipsis();

private:
    Q_DISABLE_COPY(TST_TSTranslation)

    void PrepareOriginalStrings();
};

#endif // TST_TSTRANSLATION_H

/************************************************************************
 **
 **  @file   dialogpatternmaterials.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef DIALOGPATTERNMATERIALS_H
#define DIALOGPATTERNMATERIALS_H

#include <QDialog>

namespace Ui
{
    class DialogPatternMaterials;
}

class VComboBoxDelegate;

class DialogPatternMaterials : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPatternMaterials(const QMap<int, QString> &list, bool rememberPM, QWidget *parent = nullptr);
    virtual ~DialogPatternMaterials();


    QMap<int, QString> GetPatternMaterials() const;
    QStringList        GetKnownMaterials() const;

private:
    Q_DISABLE_COPY(DialogPatternMaterials)
    Ui::DialogPatternMaterials *ui;
    QStringList m_knownMaterials;

    void SetPatternMaterials(const QMap<int, QString> &list);
};

#endif // DIALOGPATTERNMATERIALS_H

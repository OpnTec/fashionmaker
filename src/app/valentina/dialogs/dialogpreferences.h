/************************************************************************
 **
 **  @file   dialogpreferences.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
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

#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>

namespace Ui
{
    class DialogPreferences;
}

class PreferencesConfigurationPage;
class PreferencesPatternPage;
class PreferencesPathPage;
class QListWidgetItem;
class VPattern;

class DialogPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPreferences(QWidget *parent = nullptr);
    virtual ~DialogPreferences();
signals:
    void UpdateProperties();
protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private slots:
    void PageChanged(QListWidgetItem *current, QListWidgetItem *previous);
private:
    Q_DISABLE_COPY(DialogPreferences)
    Ui::DialogPreferences *ui;
    bool m_isInitialized;
    PreferencesConfigurationPage *m_configurePage;
    PreferencesPatternPage       *m_patternPage;
    PreferencesPathPage          *m_pathPage;

    void Apply();
    void Ok();
};

#endif // DIALOGPREFERENCES_H

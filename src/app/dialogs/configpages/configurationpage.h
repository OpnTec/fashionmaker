/************************************************************************
 **
 **  @file   configurationpage.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef CONFIGURATIONPAGE_H
#define CONFIGURATIONPAGE_H

#include <QObject>
#include <QWidget>

class QCheckBox;
class QSpinBox;
class QComboBox;
class QGroupBox;

class ConfigurationPage : public QWidget
{
    Q_OBJECT
public:
    ConfigurationPage(QWidget *parent = nullptr);
    void      Apply();
public slots:
    void      LangChanged();
    void      UnitChanged();
    void      LabelLangChanged();
private:
    Q_DISABLE_COPY(ConfigurationPage)
    QCheckBox *autoSaveCheck;
    QSpinBox  *autoTime;
    QComboBox *langCombo;
    QComboBox *labelCombo;
    QComboBox *unitCombo;
    QCheckBox *osOptionCheck;
    bool      langChanged;
    bool      unitChanged;
    bool      labelLangChanged;
    QCheckBox *sendReportCheck;
    QCheckBox *askPointDeletionCheck;
    QCheckBox *toolBarStyleCheck;

    QGroupBox *SaveGroup();
    QGroupBox *LangGroup();
    QGroupBox *SendGroup();
    QGroupBox *DrawGroup();
    QGroupBox *ToolBarGroup();
    void      SetLabelComboBox(const QStringList &list);
};

#endif // CONFIGURATIONPAGE_H

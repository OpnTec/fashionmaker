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
#include <QPlainTextEdit>
#include <QWidget>

class QCheckBox;
class QSpinBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;

class ConfigurationPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationPage(QWidget *parent = nullptr);
    void      Apply();
public slots:
    void      LangChanged();
    void      SystemChanged();
    void      UnitChanged();
    void      LabelLangChanged();
    void      ClearUserDefinedMaterials();
protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(ConfigurationPage)
    QCheckBox *autoSaveCheck;
    QSpinBox  *autoTime;
    QComboBox *langCombo;
    QComboBox *labelCombo;
    QComboBox *unitCombo;
    QCheckBox *osOptionCheck;
    bool      langChanged;
    bool      systemChanged;
    bool      unitChanged;
    bool      labelLangChanged;
    QCheckBox *sendReportCheck;
    QCheckBox *askPointDeletionCheck;
    QCheckBox *toolBarStyleCheck;

    QGroupBox *saveGroup;
    QLabel    *intervalLabel;
    QGroupBox *langGroup;
    QLabel    *guiLabel;
    QLabel    *separatorLabel;
    QLabel    *unitLabel;
    QLabel    *languageLabel;

    QGroupBox *pmSystemGroup;
    QLabel    *systemLabel;
    QComboBox *systemCombo;
    QLabel    *systemAuthorLabel;
    QLabel    *systemBookLabel;
    QLabel    *systemAuthorValueLabel;
    QPlainTextEdit *systemBookValueLabel;

    QGroupBox *sendGroup;
    QLabel    *description;

    QGroupBox *drawGroup;
    QGroupBox *toolBarGroup;

    QGroupBox *userMaterialsGroup;
    QPushButton* userMaterialClearButton;

    QGroupBox *SaveGroup() Q_REQUIRED_RESULT;
    QGroupBox *LangGroup() Q_REQUIRED_RESULT;
    QGroupBox *PMSystemGroup() Q_REQUIRED_RESULT;
    QGroupBox *SendGroup() Q_REQUIRED_RESULT;
    QGroupBox *DrawGroup() Q_REQUIRED_RESULT;
    QGroupBox *ToolBarGroup() Q_REQUIRED_RESULT;
    QGroupBox *UserMaterialGroup() Q_REQUIRED_RESULT;
    void      SetLabelComboBox(const QStringList &list);

    void      RetranslateUi();
};

#endif // CONFIGURATIONPAGE_H

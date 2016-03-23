/************************************************************************
 **
 **  @file   tapeconfigurationpage.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   02 08, 2015
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

class TapeConfigurationPage : public QWidget
{
    Q_OBJECT
public:
    explicit TapeConfigurationPage(QWidget *parent = nullptr);
    void      Apply();
public slots:
    void      LangChanged();
    void      SystemChanged();
    void      DefGradationChanged();
    void      UnitChanged();
    void      LabelLangChanged();
protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(TapeConfigurationPage)
    QComboBox *langCombo;
    QComboBox *systemCombo;
    QComboBox *labelCombo;
    QComboBox *unitCombo;
    QCheckBox *osOptionCheck;
    bool      langChanged;
    bool      systemChanged;
    bool      defGradationChanged;
    bool      unitChanged;
    bool      labelLangChanged;
    QCheckBox *sendReportCheck;
    QCheckBox *askPointDeletionCheck;
    QCheckBox *toolBarStyleCheck;
    QLabel    *systemAuthorValueLabel;
    QPlainTextEdit *systemBookValueLabel;

    QGroupBox *langGroup;
    QLabel    *guiLabel;
    QLabel    *separatorLabel;

    QGroupBox *pmSystemGroup;
    QLabel    *systemLabel;
    QLabel    *systemAuthorLabel;
    QLabel    *systemBookLabel;

    QGroupBox *gradationGroup;
    QLabel    *defHeightLabel;
    QLabel    *defSizeLabel;
    QComboBox *defHeightCombo;
    QComboBox *defSizeCombo;

    QGroupBox *LangGroup() Q_REQUIRED_RESULT;
    QGroupBox *PMSystemGroup() Q_REQUIRED_RESULT;
    QGroupBox *GradationGroup() Q_REQUIRED_RESULT;
    void      SetLabelComboBox(const QStringList &list);
    void      RetranslateUi();
};

#endif // CONFIGURATIONPAGE_H

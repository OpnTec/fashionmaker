/************************************************************************
 **
 **  @file   patternpage.h
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

#ifndef PATTERNPAGE_H
#define PATTERNPAGE_H

#include <QObject>
#include <QWidget>

class QCheckBox;
class QSpinBox;
class QGroupBox;
class QLineEdit;
class QLabel;
class QPushButton;

class PatternPage : public QWidget
{
    Q_OBJECT
public:
    explicit PatternPage(QWidget *parent = nullptr);
    void      Apply();
public slots:
    void      ClearUserDefinedMaterials();
protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(PatternPage)
    QGroupBox *userGroup;
    QLineEdit *userName;
    QLabel    *userNameLabel;
    QGroupBox *graphOutputGroup;
    QCheckBox *graphOutputCheck;
    QGroupBox *undoGroup;
    QSpinBox  *undoCount;
    QLabel    *countStepsLabel;
    QGroupBox *userMaterialsGroup;
    QPushButton* userMaterialClearButton;
    QGroupBox *workpieceGroup;
    QCheckBox *forbidFlippingCheck;
    QCheckBox *doublePassmarkCheck;

    QGroupBox *UserGroup() Q_REQUIRED_RESULT;
    QGroupBox *GraphOutputGroup() Q_REQUIRED_RESULT;
    QGroupBox *UndoGroup() Q_REQUIRED_RESULT;
    QGroupBox *UserMaterialGroup() Q_REQUIRED_RESULT;
    QGroupBox *UserWorkpieceGroup() Q_REQUIRED_RESULT;

    void      RetranslateUi();
};

#endif // PATTERNPAGE_H

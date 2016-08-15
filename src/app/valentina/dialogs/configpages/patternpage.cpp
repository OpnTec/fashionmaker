/************************************************************************
 **
 **  @file   patternpage.cpp
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

#include "patternpage.h"
#include "../../options.h"
#include "../../core/vapplication.h"
#include "../vmisc/vsettings.h"
#include "../vwidgets/vmaingraphicsview.h"
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
PatternPage::PatternPage(QWidget *parent):
    QWidget(parent),
    userGroup(nullptr),
    userName(nullptr),
    userNameLabel(nullptr),
    graphOutputGroup(nullptr),
    graphOutputCheck(nullptr),
    undoGroup(nullptr),
    undoCount(nullptr),
    countStepsLabel(nullptr),
    userMaterialsGroup(nullptr),
    userMaterialClearButton(nullptr)
{
    QGroupBox *userGroup = UserGroup();
    QGroupBox *graphOutputGroup = GraphOutputGroup();
    QGroupBox *undoGroup = UndoGroup();
    QGroupBox *userMatGroup  = UserMaterialGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(userGroup);
    mainLayout->addWidget(graphOutputGroup);
    mainLayout->addWidget(undoGroup);
    mainLayout->addWidget(userMatGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::Apply()
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetUser(userName->text());

    // Scene antialiasing
    settings->SetGraphicalOutput(graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    settings->SetUndoCount(undoCount->value());
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::ClearUserDefinedMaterials()
{
    VSettings* pSet = qApp->ValentinaSettings();
    pSet->ClearUserDefinedMaterial();
    pSet->sync();
    QString qsMsg = tr("All user defined materials have been deleted!");
    QMessageBox::information(this, QApplication::applicationName(), qsMsg);
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        RetranslateUi();
    }

    // remember to call base class implementation
    QWidget::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UserGroup()
{
    userGroup = new QGroupBox(tr("User"));

    userName = new QLineEdit;
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    userName->setClearButtonEnabled(true);
#endif
    userName->setText(qApp->ValentinaSettings()->GetUser());

    QFormLayout *nameLayout = new QFormLayout;
    userNameLabel = new QLabel(tr("User name:"));
    nameLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    nameLayout->addRow(userNameLabel, userName);

    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->addLayout(nameLayout);
    userGroup->setLayout(userLayout);
    return userGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::GraphOutputGroup()
{
    graphOutputGroup = new QGroupBox(tr("Graphical output"));

    graphOutputCheck = new QCheckBox(tr("Use antialiasing"));
    graphOutputCheck->setChecked(qApp->ValentinaSettings()->GetGraphicalOutput());

    QHBoxLayout *graphLayout = new QHBoxLayout;
    graphLayout->addWidget(graphOutputCheck);

    QVBoxLayout *graphOutputLayout = new QVBoxLayout;
    graphOutputLayout->addLayout(graphLayout);
    graphOutputGroup->setLayout(graphOutputLayout);
    return graphOutputGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UndoGroup()
{
    undoGroup = new QGroupBox(tr("Undo"));
    undoCount = new QSpinBox;
    undoCount->setMinimum(0);
    undoCount->setValue(qApp->ValentinaSettings()->GetUndoCount());

    QFormLayout *countLayout = new QFormLayout;
    countStepsLabel = new QLabel(tr("Count steps (0 - no limit):"));
    countLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    countLayout->addRow(countStepsLabel, undoCount);

    QVBoxLayout *undoLayout = new QVBoxLayout;
    undoLayout->addLayout(countLayout);
    undoGroup->setLayout(undoLayout);
    return undoGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UserMaterialGroup()
{
    userMaterialsGroup = new QGroupBox(tr("User defined materials"));
    userMaterialClearButton = new QPushButton(tr("Delete all"));
    connect(userMaterialClearButton, &QPushButton::clicked, this, &PatternPage::ClearUserDefinedMaterials);

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->addWidget(userMaterialClearButton);
    pLayout->addStretch(1);

    userMaterialsGroup->setLayout(pLayout);
    return userMaterialsGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::RetranslateUi()
{
    userGroup->setTitle(tr("User"));
    userNameLabel->setText(tr("User name:"));
    graphOutputGroup->setTitle(tr("Graphical output"));
    graphOutputCheck->setText(tr("Use antialiasing"));
    undoGroup->setTitle(tr("Undo"));
    countStepsLabel->setText(tr("Count steps (0 - no limit):"));
}

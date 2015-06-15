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
#include "../../../options.h"
#include "../../../core/vapplication.h"
#include "../../../core/vsettings.h"
#include "../../../../libs/vwidgets/vmaingraphicsview.h"
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>

//---------------------------------------------------------------------------------------------------------------------
PatternPage::PatternPage(QWidget *parent):
    QWidget(parent), userName(nullptr), graphOutputCheck(nullptr), undoCount(nullptr)
{
    QGroupBox *userGroup = UserGroup();
    QGroupBox *graphOutputGroup = GraphOutputGroup();
    QGroupBox *undoGroup = UndoGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(userGroup);
    mainLayout->addWidget(graphOutputGroup);
    mainLayout->addWidget(undoGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::Apply()
{
    qApp->getSettings()->SetUser(userName->text());

    // Scene antialiasing
    qApp->getSettings()->SetGraphicalOutput(graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    qApp->getSettings()->SetUndoCount(undoCount->value());
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UserGroup()
{
    QGroupBox *userGroup = new QGroupBox(tr("User"));
    QLabel *nameLabel = new QLabel(tr("User name"));

    userName = new QLineEdit;
    userName->setText(qApp->getSettings()->GetUser());

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(userName);

    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->addLayout(nameLayout);
    userGroup->setLayout(userLayout);
    return userGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::GraphOutputGroup()
{
    QGroupBox *graphOutputGroup = new QGroupBox(tr("Graphical output"));

    graphOutputCheck = new QCheckBox(tr("Use antialiasing"));
    graphOutputCheck->setChecked(qApp->getSettings()->GetGraphicalOutput());

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
    QGroupBox *undoGroup = new QGroupBox(tr("Undo"));
    QLabel *undoLabel = new QLabel(tr("Count steps (0 - no limit)"));
    undoCount = new QSpinBox;
    undoCount->setMinimum(0);
    undoCount->setValue(qApp->getSettings()->GetUndoCount());

    QHBoxLayout *countLayout = new QHBoxLayout;
    countLayout->addWidget(undoLabel);
    countLayout->addWidget(undoCount);

    QVBoxLayout *undoLayout = new QVBoxLayout;
    undoLayout->addLayout(countLayout);
    undoGroup->setLayout(undoLayout);
    return undoGroup;
}

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
 **  Copyright (C) 2014 Valentina project
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
#include "../../../widgets/vapplication.h"
#include "../../../widgets/vmaingraphicsview.h"
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>

//---------------------------------------------------------------------------------------------------------------------
PatternPage::PatternPage(QWidget *parent):
    QWidget(parent), userName(0), graphOutputCheck(0), undoCount(0)
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
    qApp->getSettings()->setValue("pattern/user", userName->text());

    // Scene antialiasing
    qApp->getSettings()->setValue("pattern/graphicalOutput", graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    qApp->getSettings()->setValue("pattern/undo", undoCount->value());
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UserGroup()
{
    QSettings *settings = qApp->getSettings();
    SCASSERT(settings != nullptr);

    QGroupBox *userGroup = new QGroupBox(tr("User"));
    QLabel *nameLabel = new QLabel(tr("User name"));

    userName = new QLineEdit;
#ifdef Q_OS_WIN
    QString user = settings->value("pattern/user", QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    QString user = settings->value("pattern/user", QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
#endif
    userName->setText(user);

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
    bool graphOutputValue = qApp->getSettings()->value("pattern/graphicalOutput", 1).toBool();
    graphOutputCheck->setChecked(graphOutputValue);

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
    bool ok = true;
    qint32 count = qApp->getSettings()->value("pattern/undo", 0).toInt(&ok);
    if (ok == false)
    {
        count = 0;
    }
    undoCount->setValue(count);

    QHBoxLayout *countLayout = new QHBoxLayout;
    countLayout->addWidget(undoLabel);
    countLayout->addWidget(undoCount);

    QVBoxLayout *undoLayout = new QVBoxLayout;
    undoLayout->addLayout(countLayout);
    undoGroup->setLayout(undoLayout);
    return undoGroup;
}

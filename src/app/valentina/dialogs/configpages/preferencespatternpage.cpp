/************************************************************************
 **
 **  @file   preferencespatternpage.cpp
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

#include "preferencespatternpage.h"
#include "ui_preferencespatternpage.h"
#include "../../core/vapplication.h"
#include "../ifc/xml/vabstractpattern.h"

#include <QMessageBox>

//---------------------------------------------------------------------------------------------------------------------
PreferencesPatternPage::PreferencesPatternPage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PreferencesPatternPage)
{
    ui->setupUi(this);
    ui->userName->setText(qApp->ValentinaSettings()->GetUser());
    ui->graphOutputCheck->setChecked(qApp->ValentinaSettings()->GetGraphicalOutput());
    ui->undoCount->setValue(qApp->ValentinaSettings()->GetUndoCount());

    connect(ui->userMaterialClearButton, &QPushButton::clicked, this, [this]()
    {
        VSettings* pSet = qApp->ValentinaSettings();
        pSet->ClearUserDefinedMaterial();
        pSet->sync();
        QString qsMsg = tr("All user defined materials have been deleted!");
        QMessageBox::information(this, QCoreApplication::applicationName(), qsMsg);
    });

    InitDefaultSeamAllowance();

    ui->forbidFlippingCheck->setChecked(qApp->ValentinaSettings()->GetForbidWorkpieceFlipping());
    ui->doublePassmarkCheck->setChecked(qApp->ValentinaSettings()->IsDoublePassmark());
    ui->checkBoxHideMainPath->setChecked(qApp->ValentinaSettings()->IsHideMainPath());
    ui->fontComboBoxLabelFont->setCurrentFont(qApp->ValentinaSettings()->GetLabelFont());
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesPatternPage::~PreferencesPatternPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::Apply()
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetUser(ui->userName->text());

    // Scene antialiasing
    settings->SetGraphicalOutput(ui->graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, ui->graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, ui->graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    settings->SetUndoCount(ui->undoCount->value());

    settings->SetDefaultSeamAllowance(ui->defaultSeamAllowance->value());

    settings->SetForbidWorkpieceFlipping(ui->forbidFlippingCheck->isChecked());
    settings->SetHideMainPath(ui->checkBoxHideMainPath->isChecked());
    qApp->ValentinaSettings()->SetLabelFont(ui->fontComboBoxLabelFont->currentFont());

    if (settings->IsDoublePassmark() != ui->doublePassmarkCheck->isChecked())
    {
        settings->SetDoublePassmark(ui->doublePassmarkCheck->isChecked());
        qApp->getCurrentDocument()->LiteParseTree(Document::LiteParse);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitDefaultSeamAllowance()
{
    ui->defaultSeamAllowance->setValue(qApp->ValentinaSettings()->GetDefaultSeamAllowance());
    ui->defaultSeamAllowance->setSuffix(UnitsToStr(StrToUnits(qApp->ValentinaSettings()->GetUnit()), true));
}

/************************************************************************
 **
 **  @file   tmainwindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tmainwindow.h"
#include "ui_tmainwindow.h"
#include "mapplication.h"
#include "dialogs/dialogabouttape.h"
#include "dialogs/dialognewmeasurements.h"

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::TMainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::TMainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setVisible(false);

    SetupMenu();
}

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::~TMainWindow()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::LoadFile(const QString &path)
{
    ui->labelToolTip->setVisible(false);
    ui->tabWidget->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileNew()
{
    DialogNewMeasurements measurements(this);
    if (measurements.exec() == QDialog::Rejected)
    {
        return;
    }

    InitNew(measurements.Type());
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileOpen()
{

}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileSave()
{

}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileSaveAs()
{

}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AboutToShowWindowMenu()
{
    ui->menuWindow->clear();
    QList<TMainWindow*> windows = MApplication::instance()->MainWindows();
    for (int i = 0; i < windows.count(); ++i)
    {
        TMainWindow *window = windows.at(i);
        QAction *action = ui->menuWindow->addAction(window->windowTitle(), this, SLOT(ShowWindow()));
        action->setData(i);
        action->setCheckable(true);
        if (window == this)
        {
            action->setChecked(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowWindow()
{
    if (QAction *action = qobject_cast<QAction*>(sender()))
    {
        const QVariant v = action->data();
        if (v.canConvert<int>())
        {
            const int offset = qvariant_cast<int>(v);
            QList<TMainWindow*> windows = MApplication::instance()->MainWindows();
            windows.at(offset)->activateWindow();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AboutApplication()
{
    DialogAboutTape * aboutDialog = new DialogAboutTape(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDialog->show();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetupMenu()
{
    // File
    connect(ui->actionNew, &QAction::triggered, this, &TMainWindow::FileNew);
    ui->actionNew->setShortcuts(QKeySequence::New);

    connect(ui->actionOpen, &QAction::triggered, this, &TMainWindow::FileOpen);
    ui->actionOpen->setShortcuts(QKeySequence::Open);

    connect(ui->actionSave, &QAction::triggered, this, &TMainWindow::FileSave);
    ui->actionSave->setShortcuts(QKeySequence::Save);

    connect(ui->actionSaveAs, &QAction::triggered, this, &TMainWindow::FileSaveAs);
    ui->actionSaveAs->setShortcuts(QKeySequence::SaveAs);

    QAction *separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuFile->insertAction(ui->actionQuit, separatorAct);

    connect(ui->actionQuit, &QAction::triggered, this, &TMainWindow::close);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);

    // Edit
    //ui->actionUndo->setShortcuts(QKeySequence::Undo);
    //ui->actionRedo->setShortcuts(QKeySequence::Redo);

    // Window
    connect(ui->menuWindow, &QMenu::aboutToShow, this, &TMainWindow::AboutToShowWindowMenu);
    AboutToShowWindowMenu();

    // Help
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionAboutTape, &QAction::triggered, this, &TMainWindow::AboutApplication);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitNew(MeasurementsType type)
{
    ui->labelToolTip->setVisible(false);
    ui->tabWidget->setVisible(true);

    if (type == MeasurementsType::Standard)
    {
        // Tab Measurements
        delete ui->labelValue;
        delete ui->horizontalLayoutValue;

        // Tab Information
        delete ui->labelGivenName;
        delete ui->lineEditGivenName;
        delete ui->labelFamilyName;
        delete ui->lineEditFamilyName;
        delete ui->labelBirthDate;
        delete ui->dateEditBirthDate;
        delete ui->labelSex;
        delete ui->comboBoxSex;
        delete ui->labelEmail;
        delete ui->lineEditEmail;
    }
    else
    {
        // Tab Measurements
        delete ui->labelBaseValue;
        delete ui->doubleSpinBoxBaseValue;
        delete ui->labelInSizes;
        delete ui->doubleSpinBoxInSizes;
        delete ui->labelInHeights;
        delete ui->doubleSpinBoxInHeights;

        // Tab Information
        delete ui->labelBaseSize;
        delete ui->labelBaseSizeValue;
        delete ui->labelBaseHeight;
        delete ui->labelBaseHeightValue;
    }

    InitTable(type);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitTable(MeasurementsType type)
{
    if (type == MeasurementsType::Standard)
    {
        ui->tableWidget->setColumnHidden( 1, true );// value
    }
    else
    {
        ui->tableWidget->setColumnHidden( 2, true );// base value
        ui->tableWidget->setColumnHidden( 3, true );// in sizes
        ui->tableWidget->setColumnHidden( 4, true );// in heights
    }
}

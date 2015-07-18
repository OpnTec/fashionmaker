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

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::TMainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::TMainWindow),
      m(nullptr),
      data(nullptr),
      mUnit(Unit::Cm),
      mType(MeasurementsType::Individual),
      curFile()
{
    ui->setupUi(this);
    ui->tabWidget->setVisible(false);

    SetupMenu();

    setWindowTitle(tr("untitled"));
}

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::~TMainWindow()
{
    delete data;
    delete m;
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
    if (m == nullptr)
    {
        DialogNewMeasurements measurements(this);
        if (measurements.exec() == QDialog::Rejected)
        {
            return;
        }

        mUnit = measurements.MUnit();
        mType = measurements.Type();

        data = new VContainer(qApp->TrVars(), &mUnit);

        if (mType == MeasurementsType::Standard)
        {
            m = new VMeasurements(mUnit, measurements.BaseSize(), measurements.BaseHeight(), data);
        }
        else
        {
            m = new VMeasurements(mUnit, data);
        }

        SetCurrentFile("");
        MeasurementsWasSaved(false);

        InitWindow();
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->FileNew();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileOpen()
{
    if (m == nullptr)
    {

    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->FileOpen();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileSave()
{
    if (curFile.isEmpty())
    {
        return FileSaveAs();
    }
    else
    {
        QString error;
        bool result = SaveMeasurements(curFile, error);
        if (not result)
        {
            QMessageBox messageBox;
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setInformativeText(tr("Could not save file"));
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.setDetailedText(error);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FileSaveAs()
{
    QString filters;
    QString fName = tr("measurements");
    QString suffix;
    if (mType == MeasurementsType::Individual)
    {
        filters = tr("Standard measurements (*.vst)");
        suffix = "vst";
        fName += "." + suffix;
    }
    else
    {
        filters = tr("Individual measurements (*.vit)");
        suffix = "vit";
        fName += "." + suffix;
    }

    QString dir;
    if (curFile.isEmpty())
    {
        if (mType == MeasurementsType::Individual)
        {
            dir = qApp->Settings()->GetPathStandardMeasurements() + "/" + fName;
        }
        else
        {
            dir = qApp->Settings()->GetPathIndividualMeasurements() + "/" + fName;
        }

    }
    else
    {
        dir = QFileInfo(curFile).absolutePath() + "/" + fName;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), dir, filters);

    if (fileName.isEmpty())
    {
        return;
    }

    QFileInfo f( fileName );
    if (f.suffix().isEmpty() && f.suffix() != suffix)
    {
        fileName += "." + suffix;
    }
    QString error;
    bool result = SaveMeasurements(fileName, error);
    if (result == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AboutToShowWindowMenu()
{
    ui->menuWindow->clear();
    QList<TMainWindow*> windows = qApp->MainWindows();
    for (int i = 0; i < windows.count(); ++i)
    {
        TMainWindow *window = windows.at(i);

        QString title = window->windowTitle();
        const int index = title.lastIndexOf("[*]");
        if (index != -1)
        {
            title.replace(index, 3, "*");
        }

        QAction *action = ui->menuWindow->addAction(title, this, SLOT(ShowWindow()));
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
            QList<TMainWindow*> windows = qApp->MainWindows();
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
void TMainWindow::SaveGivenName()
{
    m->SetGivenName(ui->lineEditGivenName->text());
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveFamilyName()
{
    m->SetFamilyName(ui->lineEditFamilyName->text());
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveEmail()
{
    m->SetEmail(ui->lineEditEmail->text());
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveSex(int index)
{
    m->SetSex(static_cast<SexType>(ui->comboBoxSex->itemData(index).toInt()));
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveBirthDate(const QDate &date)
{
    m->SetBirthDate(date);
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveNotes()
{
    m->SetNotes(ui->plainTextEditNotes->toPlainText());
    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ReadOnly(bool ro)
{
    ui->actionReadOnly->setChecked(ro);
    if (ro)
    {
        ui->actionReadOnly->setIcon(QIcon("://tapeicon/24x24/padlock_locked.png"));
    }
    else
    {
        ui->actionReadOnly->setIcon(QIcon("://tapeicon/24x24/padlock_opened.png"));
    }
    m->SetReadOnly(ro);
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

    connect(ui->actionReadOnly, &QAction::triggered, this, &TMainWindow::ReadOnly);

    connect(ui->actionQuit, &QAction::triggered, this, &TMainWindow::close);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);

    // Window
    connect(ui->menuWindow, &QMenu::aboutToShow, this, &TMainWindow::AboutToShowWindowMenu);
    AboutToShowWindowMenu();

    // Help
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionAboutTape, &QAction::triggered, this, &TMainWindow::AboutApplication);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitWindow()
{
    SCASSERT(m != nullptr);
    ui->labelToolTip->setVisible(false);
    ui->tabWidget->setVisible(true);

    ui->plainTextEditNotes->setEnabled(true);

    if (mType == MeasurementsType::Standard)
    {
        ui->labelMType->setText(tr("Standard measurements"));
        ui->labelBaseSizeValue->setText(QString().setNum(m->BaseSize()) + " " +
                                        VDomDocument::UnitsToStr(m->MUnit(), true));
        ui->labelBaseHeightValue->setText(QString().setNum(m->BaseHeight()) + " " +
                                          VDomDocument::UnitsToStr(m->MUnit(), true));

        // Tab Measurements
        delete ui->labelValue;
        delete ui->horizontalLayoutValue;
        delete ui->plainTextEditFormula;
        delete ui->pushButtonGrowLength;
        delete ui->toolButtonExprLength;
        delete ui->labelEqual;
        delete ui->labelResultCalculation;

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
        ui->labelMType->setText(tr("Individual measurements"));

        ui->lineEditGivenName->setEnabled(true);
        ui->lineEditFamilyName->setEnabled(true);
        ui->dateEditBirthDate->setEnabled(true);
        ui->comboBoxSex->setEnabled(true);
        ui->lineEditEmail->setEnabled(true);

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

        ui->lineEditGivenName->setText(m->GivenName());
        ui->lineEditFamilyName->setText(m->FamilyName());

        ui->comboBoxSex->addItem(tr("male"), QVariant(static_cast<int>(SexType::Male)));
        ui->comboBoxSex->addItem(tr("female"), QVariant(static_cast<int>(SexType::Female)));
        const qint32 index = ui->comboBoxSex->findData(static_cast<int>(m->Sex()));
        ui->comboBoxSex->setCurrentIndex(index);

        ui->dateEditBirthDate->setDate(m->BirthDate());
        ui->lineEditEmail->setText(m->Email());
        ui->plainTextEditNotes->setPlainText(m->Notes());

        connect(ui->lineEditGivenName, &QLineEdit::editingFinished, this, &TMainWindow::SaveGivenName);
        connect(ui->lineEditFamilyName, &QLineEdit::editingFinished, this, &TMainWindow::SaveFamilyName);
        connect(ui->lineEditEmail, &QLineEdit::editingFinished, this, &TMainWindow::SaveEmail);
        connect(ui->comboBoxSex, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                &TMainWindow::SaveSex);
        connect(ui->dateEditBirthDate, &QDateEdit::dateChanged, this, &TMainWindow::SaveBirthDate);
        connect(ui->plainTextEditNotes, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveNotes);
    }

    ui->actionAddCustom->setEnabled(true);
    ui->actionAddKnown->setEnabled(true);
    ui->actionReadOnly->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

    InitTable();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitTable()
{
    if (mType == MeasurementsType::Standard)
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

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MeasurementsWasSaved(bool saved)
{
    setWindowModified(!saved);
    ui->actionSave->setEnabled(!saved);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetCurrentFile(const QString &fileName)
{
    curFile = fileName;
    QString shownName = QFileInfo(curFile).fileName();
    if (curFile.isEmpty())
    {
        shownName = tr("untitled");
        if (mType == MeasurementsType::Standard)
        {
            shownName += ".vst";
        }
        else
        {
            shownName += ".vit";
        }
        ui->labelPathToFile->setText(tr("<Empty>"));
        ui->pushButtonShowInFolder->setEnabled(false);
    }
    else
    {
        ui->labelPathToFile->setText(curFile);
        ui->pushButtonShowInFolder->setEnabled(true);
    }
    shownName += "[*]";
    setWindowTitle(shownName);
}

//---------------------------------------------------------------------------------------------------------------------
bool TMainWindow::SaveMeasurements(const QString &fileName, QString &error)
{
    const bool result = m->SaveDocument(fileName, error);
    if (result)
    {
        SetCurrentFile(fileName);
        MeasurementsWasSaved(result);
    }
    return result;
}

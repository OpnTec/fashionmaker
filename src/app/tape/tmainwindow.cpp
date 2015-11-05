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
#include "dialogs/dialogabouttape.h"
#include "dialogs/dialognewmeasurements.h"
#include "dialogs/dialogmdatabase.h"
#include "dialogs/tapeconfigdialog.h"
#include "../vpatterndb/calculator.h"
#include "../ifc/ifcdef.h"
#include "../ifc/xml/vvitconverter.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vmisc/vlockguard.h"
#include "../vmisc/vsysexits.h"
#include "vlitepattern.h"
#include "../qmuparser/qmudef.h"
#include "../vtools/dialogs/support/dialogeditwrongformula.h"
#include "version.h"
#include "mapplication.h" // Should be last because of definning qApp

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QComboBox>
#include <QProcess>

#define DIALOG_MAX_FORMULA_HEIGHT 64

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-prototypes"
#elif defined(Q_CC_INTEL)
    #pragma warning( push )
    #pragma warning( disable: 1418 )
#endif

Q_LOGGING_CATEGORY(tMainWindow, "t.mainwindow")

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic pop
#elif defined(Q_CC_INTEL)
    #pragma warning( pop )
#endif

// We need this enum in case we will add or delete a column. And also make code more readable.
enum {ColumnName = 0, ColumnFullName, ColumnCalcValue, ColumnFormula, ColumnBaseValue, ColumnInSizes, ColumnInHeights};

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::TMainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::TMainWindow),
      m(nullptr),
      data(nullptr),
      mUnit(Unit::Cm),
      pUnit(Unit::Cm),
      mType(MeasurementsType::Individual),
      curFile(),
      gradationHeights(nullptr),
      gradationSizes(nullptr),
      comboBoxUnits(nullptr),
      formulaBaseHeight(0),
      lock(nullptr),
      search(),
      labelGradationHeights(nullptr),
      labelGradationSizes(nullptr),
      labelPatternUnit(nullptr),
      actionDockDiagram(nullptr),
      dockDiagramVisible(false),
      isInitialized(false)
{
    ui->setupUi(this);
    search = QSharedPointer<VTableSearch>(new VTableSearch(ui->tableWidget));
    ui->tabWidget->setVisible(false);

    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBarGradation->setContextMenuPolicy(Qt::PreventContextMenu);

    SetupMenu();

    setWindowTitle(tr("untitled %1").arg(qApp->MainWindows().size()+1));

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::~TMainWindow()
{
    delete data;
    delete m;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString TMainWindow::CurrentFile() const
{
    return curFile;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::RetranslateTable()
{
    if (m != nullptr)
    {
        const int row = ui->tableWidget->currentRow();
        RefreshTable();
        ShowUnits();
        ui->tableWidget->selectRow(row);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetBaseMHeight(int height)
{
    if (m != nullptr)
    {
        if (mType == MeasurementsType::Standard)
        {
            const int row = ui->tableWidget->currentRow();
            data->SetHeight(UnitConvertor(height, Unit::Cm, mUnit));
            RefreshData();
            ui->tableWidget->selectRow(row);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetBaseMSize(int size)
{
    if (m != nullptr)
    {
        if (mType == MeasurementsType::Standard)
        {
            const int row = ui->tableWidget->currentRow();
            data->SetSize(UnitConvertor(size, Unit::Cm, mUnit));
            RefreshData();
            ui->tableWidget->selectRow(row);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetPUnit(Unit unit)
{
    pUnit = unit;
    UpdatePatternUnit();
}

//---------------------------------------------------------------------------------------------------------------------
bool TMainWindow::LoadFile(const QString &path)
{
    if (m == nullptr)
    {
        if (not QFileInfo(path).exists())
        {
            qCCritical(tMainWindow, "%s", qUtf8Printable(tr("File '%1' doesn't exist!").arg(path)));
            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }

        // Check if file already opened
        QList<TMainWindow*>list = qApp->MainWindows();
        for (int i = 0; i < list.size(); ++i)
        {
            if (list.at(i)->CurrentFile() == path)
            {
                list.at(i)->activateWindow();
                close();
                return false;
            }
        }

        VlpCreateLock(lock, QFileInfo(path).fileName());

        if (not lock->IsLocked())
        {
            qCCritical(tMainWindow, "%s", qUtf8Printable(tr("This file already opened in another window.")));
            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }

        try
        {
            data = new VContainer(qApp->TrVars(), &mUnit);

            m = new VMeasurements(data);
            m->setXMLContent(path);

            mType = m->Type();

            if (mType == MeasurementsType::Unknown)
            {
                VException e(tr("File has unknown format."));
                throw e;
            }

            if (mType == MeasurementsType::Standard)
            {
                VVSTConverter converter(path);
                converter.Convert();

                VDomDocument::ValidateXML(VVSTConverter::CurrentSchema, path);
            }
            else
            {
                VVITConverter converter(path);
                converter.Convert();

                VDomDocument::ValidateXML(VVITConverter::CurrentSchema, path);
            }

            m->setXMLContent(path);// Read again after conversion

            if (not m->IsDefinedKnownNamesValid())
            {
                VException e(tr("File contains invalid known measurement(s)."));
                throw e;
            }

            mUnit = m->MUnit();
            pUnit = mUnit;

            data->SetHeight(m->BaseHeight());
            data->SetSize(m->BaseSize());

            ui->labelToolTip->setVisible(false);
            ui->tabWidget->setVisible(true);

            SetCurrentFile(path);

            InitWindow();

            RefreshData();

            if (ui->tableWidget->rowCount() > 0)
            {
                ui->tableWidget->selectRow(0);
            }

            GUIReadOnly(m->ReadOnly()); // Keep last
        }
        catch (VException &e)
        {
            qCCritical(tMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                       qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
            ui->labelToolTip->setVisible(true);
            ui->tabWidget->setVisible(false);
            delete m;
            m = nullptr;
            delete data;
            data = nullptr;
            lock.reset();

            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }
    }
    else
    {
        qApp->NewMainWindow();
        return qApp->MainWindow()->LoadFile(path);
    }

    return true;
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
        pUnit = mUnit;
        mType = measurements.Type();

        data = new VContainer(qApp->TrVars(), &mUnit);
        data->SetHeight(measurements.BaseHeight());
        data->SetSize(measurements.BaseSize());

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

        GUIReadOnly(m->ReadOnly()); // Keep last
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->FileNew();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::OpenIndividual()
{
    if (m == nullptr)
    {
        const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst);;All files (*.*)");
        //Use standard path to individual measurements
        const QString pathTo = qApp->TapeSettings()->GetPathIndividualMeasurements();
        Open(pathTo, filter);
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->OpenIndividual();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::OpenStandard()
{
    if (m == nullptr)
    {
        const QString filter = tr("Standard measurements (*.vst);;Individual measurements (*.vit);;All files (*.*)");
        //Use standard path to standard measurements
        const QString pathTo = qApp->TapeSettings()->GetPathStandardMeasurements();
        Open(pathTo, filter);
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->OpenStandard();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::OpenTemplate()
{
    if (m == nullptr)
    {
        const QString filter = tr("Measurements (*.vst *.vit);;All files (*.*)");
        //Use standard path to template files
        const QString pathTo = qApp->TapeSettings()->GetPathTemplate();
        Open(pathTo, filter);

        if (m != nullptr)
        {// The file was opened.
            SetCurrentFile(""); // Force user to to save new file
            lock.reset();// remove lock from template
        }
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->OpenTemplate();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::CreateFromExisting()
{
    if (m == nullptr)
    {
        const QString filter = tr("Individual measurements (*.vit)");
        //Use standard path to standard measurements
        const QString pathTo = qApp->TapeSettings()->GetPathIndividualMeasurements();
        const QString mPath = QFileDialog::getOpenFileName(this, tr("Select file"), pathTo, filter);

        if (not mPath.isEmpty())
        {
            LoadFromExistingFile(mPath);
        }
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->CreateFromExisting();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Find(const QString &term)
{
    search->Find(term);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FindPrevious()
{
    search->FindPrevious();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::FindNext()
{
    search->FindNext();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::closeEvent(QCloseEvent *event)
{
    if (MaybeSave())
    {
        WriteSettings();
        event->accept();
        deleteLater();
    }
    else
    {
        event->ignore();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);

        if (mType == MeasurementsType::Standard)
        {
            ui->labelMType->setText(tr("Standard measurements"));
            ui->labelBaseSizeValue->setText(QString().setNum(m->BaseSize()) + " " +
                                            VDomDocument::UnitsToStr(m->MUnit(), true));
            ui->labelBaseHeightValue->setText(QString().setNum(m->BaseHeight()) + " " +
                                              VDomDocument::UnitsToStr(m->MUnit(), true));

            labelGradationHeights = new QLabel(tr("Height: "));
            labelGradationSizes = new QLabel(tr("Size: "));
        }
        else
        {
            ui->labelMType->setText(tr("Individual measurements"));

            const qint32 index = ui->comboBoxGender->currentIndex();
            ui->comboBoxGender->blockSignals(true);
            ui->comboBoxGender->clear();
            InitGender(ui->comboBoxGender);
            ui->comboBoxGender->setCurrentIndex(index);
            ui->comboBoxGender->blockSignals(false);
        }

        {
            const qint32 index = ui->comboBoxPMSystem->currentIndex();
            ui->comboBoxPMSystem->blockSignals(true);
            ui->comboBoxPMSystem->clear();
            InitPMSystems(ui->comboBoxPMSystem);
            ui->comboBoxPMSystem->setCurrentIndex(index);
            ui->comboBoxPMSystem->blockSignals(false);
        }

        {
            labelPatternUnit = new QLabel(tr("Pattern unit:"));

            if (comboBoxUnits != nullptr)
            {
                const qint32 index = comboBoxUnits->currentIndex();
                comboBoxUnits->blockSignals(true);
                comboBoxUnits->clear();
                InitComboBoxUnits();
                comboBoxUnits->setCurrentIndex(index);
                comboBoxUnits->blockSignals(false);
            }
        }
    }

    // remember to call base class implementation
    QMainWindow::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    dockDiagramVisible = ui->dockWidgetDiagram->isVisible();
    ui->dockWidgetDiagram->setVisible(false);

    isInitialized = true;//first show windows are held
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
        filters = tr("Individual measurements (*.vit)");
        suffix = "vit";
        fName += "." + suffix;
    }
    else
    {
        filters = tr("Standard measurements (*.vst)");
        suffix = "vst";
        fName += "." + suffix;
    }

    QString dir;
    if (curFile.isEmpty())
    {
        if (mType == MeasurementsType::Individual)
        {
            dir = qApp->TapeSettings()->GetPathIndividualMeasurements() + "/" + fName;
        }
        else
        {
            dir = qApp->TapeSettings()->GetPathStandardMeasurements() + "/" + fName;
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

    if (QFileInfo(fileName).exists())
    {
        // Temporary try to lock the file before saving
        VLockGuard<char> tmp(fileName);
        if (not tmp.IsLocked())
        {
            qCCritical(tMainWindow, "%s",
                       qUtf8Printable(tr("Failed to lock. This file already opened in another window.")));
            return;
        }
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

        return;
    }

    VlpCreateLock(lock, fileName);
    if (not lock->IsLocked())
    {
        qCCritical(tMainWindow, "%s", qUtf8Printable(tr("Failed to lock. This file already opened in another window. "
                                                        "Expect collissions when run 2 copies of the program.")));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AboutToShowWindowMenu()
{
    ui->menuWindow->clear();
    ui->menuWindow->addAction(tr("&New Window"), this, SLOT(NewWindow()));
    ui->menuWindow->addSeparator();

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
    if (m->GivenName() != ui->lineEditGivenName->text())
    {
        m->SetGivenName(ui->lineEditGivenName->text());
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveFamilyName()
{
    if (m->FamilyName() != ui->lineEditFamilyName->text())
    {
        m->SetFamilyName(ui->lineEditFamilyName->text());
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveEmail()
{
    if (m->Email() != ui->lineEditEmail->text())
    {
        m->SetEmail(ui->lineEditEmail->text());
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveGender(int index)
{
    const GenderType type = static_cast<GenderType>(ui->comboBoxGender->itemData(index).toInt());
    if (m->Gender() != type)
    {
        m->SetGender(type);
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveBirthDate(const QDate &date)
{
    if (m->BirthDate() != date)
    {
        m->SetBirthDate(date);
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveNotes()
{
    if (m->Notes() != ui->plainTextEditNotes->toPlainText())
    {
        m->SetNotes(ui->plainTextEditNotes->toPlainText());
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SavePMSystem(int index)
{
    QString system = ui->comboBoxPMSystem->itemData(index).toString();
    system.remove(0, 1);// clear p

    if (m->PMSystem() != system)
    {
        m->SetPMSystem(system);
        MeasurementsWasSaved(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ReadOnly(bool ro)
{
    m->SetReadOnly(ro);
    MeasurementsWasSaved(false);

    GUIReadOnly(ro);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Remove()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->Remove(nameField->data(Qt::UserRole).toString());

    MeasurementsWasSaved(false);

    search->RemoveRow(row);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());

    if (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->selectRow(row);
    }
    else
    {
        MFields(false);

        ui->lineEditName->blockSignals(true);
        ui->lineEditName->setText("");
        ui->lineEditName->blockSignals(false);

        ui->plainTextEditDescription->blockSignals(true);
        ui->plainTextEditDescription->setPlainText("");
        ui->plainTextEditDescription->blockSignals(false);

        ui->lineEditFullName->blockSignals(true);
        ui->lineEditFullName->setText("");
        ui->lineEditFullName->blockSignals(false);

        if (mType == MeasurementsType::Standard)
        {
            ui->labelCalculatedValue->blockSignals(true);
            ui->doubleSpinBoxBaseValue->blockSignals(true);
            ui->doubleSpinBoxInSizes->blockSignals(true);
            ui->doubleSpinBoxInHeights->blockSignals(true);

            ui->labelCalculatedValue->setText("");
            ui->doubleSpinBoxBaseValue->setValue(0);
            ui->doubleSpinBoxInSizes->setValue(0);
            ui->doubleSpinBoxInHeights->setValue(0);

            ui->labelCalculatedValue->blockSignals(false);
            ui->doubleSpinBoxBaseValue->blockSignals(false);
            ui->doubleSpinBoxInSizes->blockSignals(false);
            ui->doubleSpinBoxInHeights->blockSignals(false);
        }
        else
        {
            ui->labelCalculatedValue->blockSignals(true);
            ui->labelCalculatedValue->setText("");
            ui->labelCalculatedValue->blockSignals(false);

            ui->plainTextEditFormula->blockSignals(true);
            ui->plainTextEditFormula->setPlainText("");
            ui->plainTextEditFormula->blockSignals(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MoveTop()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);
    m->MoveTop(nameField->data(Qt::UserRole).toString());
    MeasurementsWasSaved(false);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(0);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MoveUp()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);
    m->MoveUp(nameField->data(Qt::UserRole).toString());
    MeasurementsWasSaved(false);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(row-1);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MoveDown()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);
    m->MoveDown(nameField->data(Qt::UserRole).toString());
    MeasurementsWasSaved(false);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(row+1);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MoveBottom()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);
    m->MoveBottom(nameField->data(Qt::UserRole).toString());
    MeasurementsWasSaved(false);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(ui->tableWidget->rowCount()-1);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Fx()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
       // Translate to internal look.
       meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId & e)
    {
        qCCritical(tMainWindow, "%s\n\n%s\n\n%s",
                   qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(meash->GetData(), NULL_ID, this);
    dialog->setWindowTitle(tr("Edit measurement"));

    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");
    try
    {
        text = qApp->TrVars()->FormulaFromUser(text, true);
    }
    catch (qmu::QmuParserError &e) // Just in case something bad will happen
    {
        Q_UNUSED(e)
        text = ui->plainTextEditFormula->toPlainText();
    }
    dialog->SetFormula(text);
    const QString postfix = VDomDocument::UnitsToStr(mUnit, true);//Show unit in dialog lable (cm, mm or inch)
    dialog->setPostfix(postfix);

    if (dialog->exec() == QDialog::Accepted)
    {
        m->SetMValue(nameField->data(Qt::UserRole).toString(), dialog->GetFormula());

        MeasurementsWasSaved(false);

        RefreshData();

        search->RefreshList(ui->lineEditFind->text());

        ui->tableWidget->selectRow(row);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AddCustom()
{
    qint32 num = 1;
    QString name;
    do
    {
        name = CustomMSign + tr("M_%1").arg(num);
        num++;
    } while (data->IsUnique(name) == false);

    qint32 currentRow;

    if (ui->tableWidget->currentRow() == -1)
    {
        currentRow  = ui->tableWidget->rowCount();
        m->AddEmpty(name);
    }
    else
    {
        currentRow  = ui->tableWidget->currentRow()+1;
        const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);
        m->AddEmptyAfter(nameField->data(Qt::UserRole).toString(), name);
    }

    search->AddRow(currentRow);
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());

    ui->tableWidget->selectRow(currentRow);

    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::AddKnown()
{
    DialogMDataBase *dialog = new DialogMDataBase(m->ListKnown(), this);
    if (dialog->exec() == QDialog::Accepted)
    {
        qint32 currentRow;

        const QStringList list = dialog->GetNewNames();
        if (ui->tableWidget->currentRow() == -1)
        {
            currentRow  = ui->tableWidget->rowCount() + list.size() - 1;
            for (int i = 0; i < list.size(); ++i)
            {
                if (mType == MeasurementsType::Individual)
                {
                    m->AddEmpty(list.at(i), qApp->TrVars()->MFormula(list.at(i)));
                }
                else
                {
                    m->AddEmpty(list.at(i));
                }

                search->AddRow(currentRow);
            }
        }
        else
        {
            currentRow  = ui->tableWidget->currentRow() + list.size();
            const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);
            QString after = nameField->data(Qt::UserRole).toString();
            for (int i = 0; i < list.size(); ++i)
            {
                if (mType == MeasurementsType::Individual)
                {
                    m->AddEmptyAfter(after, list.at(i), qApp->TrVars()->MFormula(list.at(i)));
                }
                else
                {
                    m->AddEmptyAfter(after, list.at(i));
                }
                search->AddRow(currentRow);
                after = list.at(i);
            }
        }

        RefreshData();
        search->RefreshList(ui->lineEditFind->text());

        ui->tableWidget->selectRow(currentRow);

        MeasurementsWasSaved(false);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ImportFromPattern()
{
    if (m == nullptr)
    {
        return;
    }

    const QString filter(tr("Pattern files (*.val)"));
    //Use standard path to individual measurements
    const QString pathTo = qApp->TapeSettings()->GetPathTemplate();

    const QString mPath = QFileDialog::getOpenFileName(this, tr("Import from a pattern"), pathTo, filter);
    if (mPath.isEmpty())
    {
        return;
    }

    VLockGuard<char> tmp(QFileInfo(mPath).fileName());
    if (not tmp.IsLocked())
    {
        qCCritical(tMainWindow, "%s", qUtf8Printable(tr("This file already opened in another window.")));
        return;
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/

    QStringList measurements;
    try
    {
        VPatternConverter converter(mPath);
        converter.Convert();

        VDomDocument::ValidateXML(VPatternConverter::CurrentSchema, mPath);
        VLitePattern *doc = new VLitePattern();
        doc->setXMLContent(mPath);
        measurements = doc->ListMeasurements();
        delete doc; // close a pattern
    }
    catch (VException &e)
    {
        qCCritical(tMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

    measurements = FilterMeasurements(measurements, m->ListAll());

    qint32 currentRow;

    if (ui->tableWidget->currentRow() == -1)
    {
        currentRow  = ui->tableWidget->rowCount() + measurements.size() - 1;
        for (int i = 0; i < measurements.size(); ++i)
        {
            m->AddEmpty(measurements.at(i));
        }
    }
    else
    {
        currentRow  = ui->tableWidget->currentRow() + measurements.size();
        const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);
        QString after = nameField->data(Qt::UserRole).toString();
        for (int i = 0; i < measurements.size(); ++i)
        {
            m->AddEmptyAfter(after, measurements.at(i));
            after = measurements.at(i);
        }
    }

    RefreshData();

    search->RefreshList(ui->lineEditFind->text());

    ui->tableWidget->selectRow(currentRow);

    MeasurementsWasSaved(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ChangedSize(const QString &text)
{
    const int row = ui->tableWidget->currentRow();
    data->SetSize(text.toInt());
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ChangedHeight(const QString &text)
{
    const int row = ui->tableWidget->currentRow();
    data->SetHeight(text.toInt());
    RefreshData();
    search->RefreshList(ui->lineEditFind->text());
    ui->tableWidget->selectRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowMData()
{
    if (ui->tableWidget->rowCount() > 0)
    {
        MFields(true);

        const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName); // name
        QSharedPointer<VMeasurement> meash;

        try
        {
            // Translate to internal look.
            meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
        }
        catch(const VExceptionBadId &e)
        {
            Q_UNUSED(e);
            MFields(false);
            return;
        }

        ShowMDiagram(meash->GetName());

        ui->lineEditName->blockSignals(true);
        ui->plainTextEditDescription->blockSignals(true);
        ui->lineEditFullName->blockSignals(true);
        if (meash->IsCustom())
        {
            ui->plainTextEditDescription->setPlainText(meash->GetDescription());
            ui->lineEditFullName->setText(meash->GetGuiText());
            ui->lineEditName->setText(ClearCustomName(meash->GetName()));
        }
        else
        {
            //Show known
            ui->plainTextEditDescription->setPlainText(qApp->TrVars()->Description(meash->GetName()));
            ui->lineEditFullName->setText(qApp->TrVars()->GuiText(meash->GetName()));
            ui->lineEditName->setText(nameField->text());
        }
        ui->lineEditName->blockSignals(false);
        ui->plainTextEditDescription->blockSignals(false);
        ui->lineEditFullName->blockSignals(false);

        if (mType == MeasurementsType::Standard)
        {
            ui->labelCalculatedValue->blockSignals(true);
            ui->doubleSpinBoxBaseValue->blockSignals(true);
            ui->doubleSpinBoxInSizes->blockSignals(true);
            ui->doubleSpinBoxInHeights->blockSignals(true);

            const QString postfix = VDomDocument::UnitsToStr(pUnit);//Show unit in dialog lable (cm, mm or inch)
            const qreal value = UnitConvertor(data->GetTableValue(meash->GetName(), mType), mUnit, pUnit);
            ui->labelCalculatedValue->setText(qApp->LocaleToString(value) + " " +postfix);

            ui->doubleSpinBoxBaseValue->setValue(static_cast<int>(meash->GetBase()));
            ui->doubleSpinBoxInSizes->setValue(static_cast<int>(meash->GetKsize()));
            ui->doubleSpinBoxInHeights->setValue(static_cast<int>(meash->GetKheight()));

            ui->labelCalculatedValue->blockSignals(false);
            ui->doubleSpinBoxBaseValue->blockSignals(false);
            ui->doubleSpinBoxInSizes->blockSignals(false);
            ui->doubleSpinBoxInHeights->blockSignals(false);
        }
        else
        {
            EvalFormula(meash->GetFormula(), false, meash->GetData(), ui->labelCalculatedValue);

            ui->plainTextEditFormula->blockSignals(true);

            QString formula;
            try
            {
                formula = qApp->TrVars()->FormulaToUser(meash->GetFormula());
            }
            catch (qmu::QmuParserError &e)
            {
                Q_UNUSED(e);
                formula = meash->GetFormula();
            }

            ui->plainTextEditFormula->setPlainText(formula);
            ui->plainTextEditFormula->blockSignals(false);
        }

        MeasurementReadOnly(m->ReadOnly());
    }
    else
    {
        MFields(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowMDiagram(const QString &name)
{
    const VTranslateVars *trv = qApp->TrVars();
    const QString number = trv->MNumber(name);

    if (number.isEmpty())
    {
        ui->labelDiagram->setText(tr("<html><head/><body><p><span style=\" font-size:340pt;\">?</span></p>"
                                     "<p align=\"center\">Unknown measurement</p></body></html>"));
    }
    else
    {
        const QString iconPath = QString("://diagrams/%1.png").arg(MapDiagrams(number));
        ui->labelDiagram->setText(QString("<html><head/><body><img src=\"%1\" align=\"center\">"
                                          "<p align=\"center\"><b>%2</b>. <i>%3</i></p></body></html>")
                                          .arg(iconPath).arg(number).arg(trv->GuiText(name)));
    }
    // This part is very ugly, can't find better way to resize dockWidget.
    ui->labelDiagram->adjustSize();
    // And also thos 50 px. DockWidget has some border. And i can't find how big it is.
    // Can lead to problem in future.
    ui->dockWidgetDiagram->setMaximumWidth(ui->labelDiagram->width()+50);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::DeployFormula()
{
    SCASSERT(ui->plainTextEditFormula != nullptr);
    SCASSERT(ui->pushButtonGrow != nullptr);

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();

    if (ui->plainTextEditFormula->height() < DIALOG_MAX_FORMULA_HEIGHT)
    {
        ui->plainTextEditFormula->setFixedHeight(DIALOG_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        ui->pushButtonGrow->setIcon(QIcon::fromTheme("go-next",
                                                     QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       ui->plainTextEditFormula->setFixedHeight(formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       ui->pushButtonGrow->setIcon(QIcon::fromTheme("go-down",
                                                    QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    setUpdatesEnabled(false);
    repaint();
    setUpdatesEnabled(true);

    ui->plainTextEditFormula->setFocus();
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMName()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId &e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    QString newName = ui->lineEditName->text();
    if (meash->IsCustom())
    {
        newName = CustomMSign + newName;

        if (data->IsUnique(newName))
        {
            m->SetMName(nameField->text(), newName);
            MeasurementsWasSaved(false);
            RefreshData();
            search->RefreshList(ui->lineEditFind->text());

            ui->tableWidget->blockSignals(true);
            ui->tableWidget->selectRow(row);
            ui->tableWidget->blockSignals(false);
        }
        else
        {
            ui->lineEditName->setText(ClearCustomName(nameField->text()));
        }
    }
    else
    {
        qCWarning(tMainWindow, "%s", qUtf8Printable(tr("The name of known measurement forbidden to change.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMValue()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(row, ColumnName);

    // Replace line return character with spaces for calc if exist
    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");

    QTableWidgetItem *formulaField = ui->tableWidget->item(row, ColumnFormula);
    if (formulaField->text() == text)
    {
        QTableWidgetItem *result = ui->tableWidget->item(row, ColumnCalcValue);
        const QString postfix = VDomDocument::UnitsToStr(mUnit);//Show unit in dialog lable (cm, mm or inch)
        ui->labelCalculatedValue->setText(result->text() + " " +postfix);
        return;
    }

    if (text.isEmpty())
    {
        const QString postfix = VDomDocument::UnitsToStr(mUnit);//Show unit in dialog lable (cm, mm or inch)
        ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        return;
    }

    QSharedPointer<VMeasurement> meash;
    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId & e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    const bool ok = EvalFormula(text, true, meash->GetData(), ui->labelCalculatedValue);

    if (not ok)
    {
        return;
    }

    try
    {
        const QString formula = qApp->TrVars()->FormulaFromUser(text, true);
        m->SetMValue(nameField->data(Qt::UserRole).toString(), formula);
    }
    catch (qmu::QmuParserError &e) // Just in case something bad will happen
    {
        Q_UNUSED(e)
        return;
    }

    MeasurementsWasSaved(false);

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();

    RefreshData();
    search->RefreshList(ui->lineEditFind->text());

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);

    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMBaseValue(double value)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId &e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    if (meash->IsCustom())
    {
        m->SetMBaseValue(nameField->data(Qt::UserRole).toString(), value);

        MeasurementsWasSaved(false);

        RefreshData();
        search->RefreshList(ui->lineEditFind->text());

        ui->tableWidget->blockSignals(true);
        ui->tableWidget->selectRow(row);
        ui->tableWidget->blockSignals(false);
    }
    else
    {
        qCWarning(tMainWindow, "%s", qUtf8Printable(tr("The base value of known measurement forbidden to change.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMSizeIncrease(double value)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId &e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    if (meash->IsCustom())
    {
        m->SetMSizeIncrease(nameField->data(Qt::UserRole).toString(), value);

        MeasurementsWasSaved(false);

        RefreshData();
        search->RefreshList(ui->lineEditFind->text());

        ui->tableWidget->blockSignals(true);
        ui->tableWidget->selectRow(row);
        ui->tableWidget->blockSignals(false);
    }
    else
    {
        qCWarning(tMainWindow, "%s",
                  qUtf8Printable(tr("The size increase value of known measurement forbidden to change.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMHeightIncrease(double value)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId &e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    if (meash->IsCustom())
    {
        m->SetMHeightIncrease(nameField->data(Qt::UserRole).toString(), value);

        MeasurementsWasSaved(false);

        RefreshData();
        search->RefreshList(ui->lineEditFind->text());

        ui->tableWidget->selectRow(row);
    }
    else
    {
        qCWarning(tMainWindow, "%s",
                  qUtf8Printable(tr("The height increase value of known measurement forbidden to change.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMDescription()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);
    m->SetMDescription(nameField->data(Qt::UserRole).toString(), ui->plainTextEditDescription->toPlainText());

    MeasurementsWasSaved(false);

    const QTextCursor cursor = ui->plainTextEditDescription->textCursor();

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);

    ui->plainTextEditDescription->setTextCursor(cursor);
}


//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMFullName()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName);

    QSharedPointer<VMeasurement> meash;

    try
    {
        // Translate to internal look.
        meash = data->GetVariable<VMeasurement>(nameField->data(Qt::UserRole).toString());
    }
    catch(const VExceptionBadId &e)
    {
        qCWarning(tMainWindow, "%s\n\n%s\n\n%s",
                  qUtf8Printable(tr("Can't find measurement '%1'.").arg(nameField->text())),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }

    if (meash->IsCustom())
    {
        m->SetMFullName(nameField->data(Qt::UserRole).toString(), ui->lineEditFullName->text());

        MeasurementsWasSaved(false);

        RefreshData();

        ui->tableWidget->blockSignals(true);
        ui->tableWidget->selectRow(row);
        ui->tableWidget->blockSignals(false);
    }
    else
    {
        qCWarning(tMainWindow, "%s", qUtf8Printable(tr("The full name of known measurement forbidden to change.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::NewWindow()
{
    qApp->NewMainWindow();
    qApp->MainWindow()->activateWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Preferences()
{
    TapeConfigDialog dlg(this);
    dlg.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::PatternUnitChanged(int index)
{
    pUnit = static_cast<Unit>(comboBoxUnits->itemData(index).toInt());

    UpdatePatternUnit();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetupMenu()
{
    // File
    connect(ui->actionNew, &QAction::triggered, this, &TMainWindow::FileNew);
    ui->actionNew->setShortcuts(QKeySequence::New);

    connect(ui->actionOpenIndividual, &QAction::triggered, this, &TMainWindow::OpenIndividual);
    connect(ui->actionOpenStandard, &QAction::triggered, this, &TMainWindow::OpenStandard);
    connect(ui->actionOpenTemplate, &QAction::triggered, this, &TMainWindow::OpenTemplate);
    connect(ui->actionCreateFromExisting, &QAction::triggered, this, &TMainWindow::CreateFromExisting);

    connect(ui->actionSave, &QAction::triggered, this, &TMainWindow::FileSave);
    ui->actionSave->setShortcuts(QKeySequence::Save);

    connect(ui->actionSaveAs, &QAction::triggered, this, &TMainWindow::FileSaveAs);
    ui->actionSaveAs->setShortcuts(QKeySequence::SaveAs);

    connect(ui->actionReadOnly, &QAction::triggered, this, &TMainWindow::ReadOnly);
    connect(ui->actionPreferences, &QAction::triggered, this, &TMainWindow::Preferences);

    connect(ui->actionQuit, &QAction::triggered, this, &TMainWindow::close);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);

    // Measurements
    connect(ui->actionAddCustom, &QAction::triggered, this, &TMainWindow::AddCustom);
    connect(ui->actionAddKnown, &QAction::triggered, this, &TMainWindow::AddKnown);
    connect(ui->actionDatabase, &QAction::triggered, qApp, &MApplication::ShowDataBase);
    connect(ui->actionImportFromPattern, &QAction::triggered, this, &TMainWindow::ImportFromPattern);
    actionDockDiagram = ui->dockWidgetDiagram->toggleViewAction();
    ui->menuMeasurements->addAction(actionDockDiagram);
    actionDockDiagram->setEnabled(false);

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
    ui->dockWidgetDiagram->setVisible(dockDiagramVisible);
    actionDockDiagram->setEnabled(true);
    ui->tabWidget->setCurrentIndex(0);

    ui->plainTextEditNotes->setEnabled(true);
    ui->toolBarGradation->setVisible(true);

    if (mType == MeasurementsType::Standard)
    {
        ui->labelMType->setText(tr("Standard measurements"));
        ui->labelBaseSizeValue->setText(QString().setNum(m->BaseSize()) + " " +
                                        VDomDocument::UnitsToStr(m->MUnit(), true));
        ui->labelBaseHeightValue->setText(QString().setNum(m->BaseHeight()) + " " +
                                          VDomDocument::UnitsToStr(m->MUnit(), true));

        // Because Qt Designer doesn't know about our deleting we will create empty objects for correct
        // working the retranslation UI
        // Tab Measurements
        delete ui->horizontalLayoutValue;
        delete ui->plainTextEditFormula;
        delete ui->toolButtonExpr;

        delete ui->labelFormula;
        ui->labelFormula = new QLabel(this);

        delete ui->pushButtonGrow;
        ui->pushButtonGrow = new QPushButton(this);

        // Tab Information
        delete ui->lineEditGivenName;
        delete ui->lineEditFamilyName;
        delete ui->comboBoxGender;
        delete ui->lineEditEmail;

        delete ui->labelGivenName;
        ui->labelGivenName = new QLabel(this);

        delete ui->labelFamilyName;
        ui->labelFamilyName = new QLabel(this);

        delete ui->labelBirthDate;
        ui->labelBirthDate = new QLabel(this);

        delete ui->dateEditBirthDate;
        ui->dateEditBirthDate = new QDateEdit(this);

        delete ui->labelGender;
        ui->labelGender = new QLabel(this);

        delete ui->labelEmail;
        ui->labelEmail = new QLabel(this);

        const QStringList listHeights = VMeasurement::WholeListHeights(mUnit);
        const QStringList listSizes = VMeasurement::WholeListSizes(mUnit);

        labelGradationHeights = new QLabel(tr("Height: "));
        gradationHeights = SetGradationList(labelGradationHeights, listHeights);
        SetDefaultHeight(static_cast<int>(data->height()));
        connect(gradationHeights, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &TMainWindow::ChangedHeight);

        labelGradationSizes = new QLabel(tr("Size: "));
        gradationSizes = SetGradationList(labelGradationSizes, listSizes);
        SetDefaultSize(static_cast<int>(data->size()));
        connect(gradationSizes, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &TMainWindow::ChangedSize);

        connect(ui->doubleSpinBoxBaseValue,
                static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &TMainWindow::SaveMBaseValue);
        connect(ui->doubleSpinBoxInSizes,
                static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &TMainWindow::SaveMSizeIncrease);
        connect(ui->doubleSpinBoxInHeights,
                static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &TMainWindow::SaveMHeightIncrease);

        SetDecimals();
    }
    else
    {
        ui->labelMType->setText(tr("Individual measurements"));

        ui->lineEditGivenName->setEnabled(true);
        ui->lineEditFamilyName->setEnabled(true);
        ui->dateEditBirthDate->setEnabled(true);
        ui->comboBoxGender->setEnabled(true);
        ui->lineEditEmail->setEnabled(true);

        // Tab Measurements
        delete ui->doubleSpinBoxBaseValue;
        delete ui->doubleSpinBoxInSizes;
        delete ui->doubleSpinBoxInHeights;

        delete ui->labelBaseValue;
        ui->labelBaseValue = new QLabel(this);

        delete ui->labelInSizes;
        ui->labelInSizes = new QLabel(this);

        delete ui->labelInHeights;
        ui->labelInHeights = new QLabel(this);

        // Tab Information
        delete ui->labelBaseSize;
        ui->labelBaseSize = new QLabel(this);

        delete ui->labelBaseSizeValue;
        ui->labelBaseSizeValue = new QLabel(this);

        delete ui->labelBaseHeight;
        ui->labelBaseHeight = new QLabel(this);

        delete ui->labelBaseHeightValue;
        ui->labelBaseHeightValue = new QLabel(this);

        ui->lineEditGivenName->setText(m->GivenName());
        ui->lineEditFamilyName->setText(m->FamilyName());

        ui->comboBoxGender->clear();
        InitGender(ui->comboBoxGender);
        const qint32 index = ui->comboBoxGender->findData(static_cast<int>(m->Gender()));
        ui->comboBoxGender->setCurrentIndex(index);

        ui->dateEditBirthDate->setDate(m->BirthDate());
        ui->lineEditEmail->setText(m->Email());

        connect(ui->lineEditGivenName, &QLineEdit::editingFinished, this, &TMainWindow::SaveGivenName);
        connect(ui->lineEditFamilyName, &QLineEdit::editingFinished, this, &TMainWindow::SaveFamilyName);
        connect(ui->lineEditEmail, &QLineEdit::editingFinished, this, &TMainWindow::SaveEmail);
        connect(ui->comboBoxGender, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                &TMainWindow::SaveGender);
        connect(ui->dateEditBirthDate, &QDateEdit::dateChanged, this, &TMainWindow::SaveBirthDate);
        connect(ui->pushButtonGrow, &QPushButton::clicked, this, &TMainWindow::DeployFormula);

        this->formulaBaseHeight = ui->plainTextEditFormula->height();
        connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveMValue,
                Qt::UniqueConnection);

        connect(ui->toolButtonExpr, &QToolButton::clicked, this, &TMainWindow::Fx);
    }

    ui->comboBoxPMSystem->setEnabled(true);
    ui->comboBoxPMSystem->clear();
    InitPMSystems(ui->comboBoxPMSystem);
    const qint32 index = ui->comboBoxPMSystem->findData(QLatin1Char('p')+m->PMSystem());
    ui->comboBoxPMSystem->setCurrentIndex(index);
    connect(ui->comboBoxPMSystem, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TMainWindow::SavePMSystem);

    connect(ui->lineEditFind, &QLineEdit::textEdited, this, &TMainWindow::Find);
    connect(ui->toolButtonFindPrevious, &QToolButton::clicked, this, &TMainWindow::FindPrevious);
    connect(ui->toolButtonFindNext, &QToolButton::clicked, this, &TMainWindow::FindNext);

    ui->plainTextEditNotes->setPlainText(m->Notes());
    connect(ui->plainTextEditNotes, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveNotes);

    ui->actionAddCustom->setEnabled(true);
    ui->actionAddKnown->setEnabled(true);
    ui->actionImportFromPattern->setEnabled(true);
    ui->actionReadOnly->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

#if QT_VERSION > QT_VERSION_CHECK(5, 1, 0)
    ui->lineEditName->setValidator( new QRegularExpressionValidator(QRegularExpression(NameRegExp()), this));
#else
    ui->lineEditName->setValidator( new QRegExpValidator(QRegExp(NameRegExp()), this));
#endif

    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &TMainWindow::Remove);
    connect(ui->toolButtonTop, &QToolButton::clicked, this, &TMainWindow::MoveTop);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &TMainWindow::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &TMainWindow::MoveDown);
    connect(ui->toolButtonBottom, &QToolButton::clicked, this, &TMainWindow::MoveBottom);

    connect(ui->lineEditName, &QLineEdit::editingFinished, this, &TMainWindow::SaveMName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveMDescription);
    connect(ui->lineEditFullName, &QLineEdit::editingFinished, this, &TMainWindow::SaveMFullName);

    connect(ui->pushButtonShowInExplorer, &QPushButton::clicked, this, &TMainWindow::ShowInGraphicalShell);

    InitUnits();

    InitTable();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitTable()
{
    if (mType == MeasurementsType::Standard)
    {
        ui->tableWidget->setColumnHidden( ColumnFormula, true );// formula
    }
    else
    {
        ui->tableWidget->setColumnHidden( ColumnBaseValue, true );// base value
        ui->tableWidget->setColumnHidden( ColumnInSizes, true );// in sizes
        ui->tableWidget->setColumnHidden( ColumnInHeights, true );// in heights
    }

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &TMainWindow::ShowMData);

    ShowUnits();

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowUnits()
{
    const QString unit = VDomDocument::UnitsToStr(mUnit);

    ShowHeaderUnits(ui->tableWidget, ColumnCalcValue, VDomDocument::UnitsToStr(pUnit));// calculated value
    ShowHeaderUnits(ui->tableWidget, ColumnFormula, unit);// formula
    ShowHeaderUnits(ui->tableWidget, ColumnBaseValue, unit);// base value
    ShowHeaderUnits(ui->tableWidget, ColumnInSizes, unit);// in sizes
    ShowHeaderUnits(ui->tableWidget, ColumnInHeights, unit);// in heights
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowHeaderUnits(QTableWidget *table, int column, const QString &unit)
{
    SCASSERT(table != nullptr);

    QString header = table->horizontalHeaderItem(column)->text();
    const int index = header.indexOf(QLatin1Literal("("));
    if (index != -1)
    {
        header.remove(index-1, 100);
    }
    const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
    table->horizontalHeaderItem(column)->setText(unitHeader);
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
        ui->pushButtonShowInExplorer->setEnabled(false);
    }
    else
    {
        ui->labelPathToFile->setText(curFile);
        ui->labelPathToFile->setToolTip(curFile);
        ui->pushButtonShowInExplorer->setEnabled(true);
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

//---------------------------------------------------------------------------------------------------------------------
bool TMainWindow::MaybeSave()
{
    if (this->isWindowModified())
    {
        if (curFile.isEmpty() && ui->tableWidget->rowCount() == 0)
        {
            return true;// Don't ask if file was created without modifications.
        }

        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Unsaved changes"), tr("Measurements have been modified.\n"
                                                                   "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            FileSave();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QTableWidgetItem *TMainWindow::AddCell(const QString &text, int row, int column, int aligment, bool ok)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(aligment);

    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);

    if (not ok)
    {
        QBrush brush = item->foreground();
        brush.setColor(Qt::red);
        item->setForeground(brush);
    }

    ui->tableWidget->setItem(row, column, item);

    return item;
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox *TMainWindow::SetGradationList(QLabel *label, const QStringList &list)
{
    ui->toolBarGradation->addWidget(label);

    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(list);
    ui->toolBarGradation->addWidget(comboBox);

    return comboBox;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetDefaultHeight(int value)
{
    const qint32 index = gradationHeights->findText(QString("%1").arg(value));
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        data->SetHeight(gradationHeights->currentText().toInt());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetDefaultSize(int value)
{
    const qint32 index = gradationSizes->findText(QString("%1").arg(value));
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        data->SetSize(gradationSizes->currentText().toInt());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::RefreshData()
{
    data->ClearUniqueNames();
    data->ClearVariables(VarType::Measurement);
    m->ReadMeasurements();

    RefreshTable();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::RefreshTable()
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearContents();

    const QMap<QString, QSharedPointer<VMeasurement> > table = data->DataMeasurements();
    QMap<int, QSharedPointer<VMeasurement> > orderedTable;
    QMap<QString, QSharedPointer<VMeasurement> >::const_iterator iterMap;
    for (iterMap = table.constBegin(); iterMap != table.constEnd(); ++iterMap)
    {
        QSharedPointer<VMeasurement> meash = iterMap.value();
        orderedTable.insert(meash->Index(), meash);
    }

    qint32 currentRow = -1;
    QMap<int, QSharedPointer<VMeasurement> >::const_iterator iMap;
    ui->tableWidget->setRowCount ( orderedTable.size() );
    for (iMap = orderedTable.constBegin(); iMap != orderedTable.constEnd(); ++iMap)
    {
        QSharedPointer<VMeasurement> meash = iMap.value();
        currentRow++;

        if (mType == MeasurementsType::Individual)
        {
            QTableWidgetItem *item = AddCell(qApp->TrVars()->MToUser(meash->GetName()), currentRow, ColumnName,
                                             Qt::AlignVCenter); // name
            item->setData(Qt::UserRole, meash->GetName());

            if (meash->IsCustom())
            {
                AddCell(meash->GetGuiText(), currentRow, ColumnFullName, Qt::AlignVCenter);
            }
            else
            {
                AddCell(qApp->TrVars()->GuiText(meash->GetName()), currentRow, ColumnFullName, Qt::AlignVCenter);
            }

            const qreal value = UnitConvertor(*meash->GetValue(), mUnit, pUnit);
            AddCell(QString().setNum(value), currentRow, ColumnCalcValue, Qt::AlignHCenter | Qt::AlignVCenter,
                    meash->IsFormulaOk()); // calculated value

            QString formula;
            try
            {
                formula = qApp->TrVars()->FormulaToUser(meash->GetFormula());
            }
            catch (qmu::QmuParserError &e)
            {
                Q_UNUSED(e);
                formula = meash->GetFormula();
            }

            AddCell(formula, currentRow, ColumnFormula, Qt::AlignVCenter); // formula
        }
        else
        {
            QTableWidgetItem *item = AddCell(qApp->TrVars()->MToUser(meash->GetName()), currentRow, 0,
                                             Qt::AlignVCenter); // name
            item->setData(Qt::UserRole, meash->GetName());

            if (meash->IsCustom())
            {
                AddCell(meash->GetGuiText(), currentRow, ColumnFullName, Qt::AlignVCenter);
            }
            else
            {
                AddCell(qApp->TrVars()->GuiText(meash->GetName()), currentRow, ColumnFullName, Qt::AlignVCenter);
            }

            const qreal value = UnitConvertor(data->GetTableValue(meash->GetName(), mType), mUnit, pUnit);
            AddCell(QString().setNum(value), currentRow, ColumnCalcValue,
                    Qt::AlignHCenter | Qt::AlignVCenter, meash->IsFormulaOk()); // calculated value

            AddCell(QString().setNum(meash->GetBase()), currentRow, ColumnBaseValue,
                    Qt::AlignHCenter | Qt::AlignVCenter); // base value

            AddCell(QString().setNum(meash->GetKsize()), currentRow, ColumnInSizes,
                    Qt::AlignHCenter | Qt::AlignVCenter); // in sizes

            AddCell(QString().setNum(meash->GetKheight()), currentRow, ColumnInHeights,
                    Qt::AlignHCenter | Qt::AlignVCenter); // in heights
        }
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Controls()
{
    if (m->ReadOnly())
    {
        ui->toolButtonRemove->setEnabled(false);
        ui->toolButtonTop->setEnabled(false);
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
        ui->toolButtonBottom->setEnabled(false);
        return;
    }

    if (ui->tableWidget->rowCount() > 0)
    {
        ui->toolButtonRemove->setEnabled(true);
    }
    else
    {
        ui->toolButtonRemove->setEnabled(false);
    }

    if (ui->tableWidget->rowCount() >= 2)
    {
        if (ui->tableWidget->currentRow() == 0)
        {
            ui->toolButtonTop->setEnabled(false);
            ui->toolButtonUp->setEnabled(false);
            ui->toolButtonDown->setEnabled(true);
            ui->toolButtonBottom->setEnabled(true);
        }
        else if (ui->tableWidget->currentRow() == ui->tableWidget->rowCount()-1)
        {
            ui->toolButtonTop->setEnabled(true);
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(false);
            ui->toolButtonBottom->setEnabled(false);
        }
        else
        {
            ui->toolButtonTop->setEnabled(true);
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(true);
            ui->toolButtonBottom->setEnabled(true);
        }
    }
    else
    {
        ui->toolButtonTop->setEnabled(false);
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
        ui->toolButtonBottom->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MFields(bool enabled)
{
    ui->lineEditName->setEnabled(enabled);
    ui->plainTextEditDescription->setEnabled(enabled);
    ui->lineEditFullName->setEnabled(enabled);

    if (mType == MeasurementsType::Standard)
    {
        ui->doubleSpinBoxBaseValue->setEnabled(enabled);
        ui->doubleSpinBoxInSizes->setEnabled(enabled);
        ui->doubleSpinBoxInHeights->setEnabled(enabled);
    }
    else
    {
        ui->plainTextEditFormula->setEnabled(enabled);
        ui->pushButtonGrow->setEnabled(enabled);
        ui->toolButtonExpr->setEnabled(enabled);
    }

    ui->lineEditFind->setEnabled(enabled);
    ui->toolButtonFindPrevious->setEnabled(enabled);
    ui->toolButtonFindNext->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
QString TMainWindow::ClearCustomName(const QString &name) const
{
    QString clear = name;
    const int index = clear.indexOf(CustomMSign);
    if (index == 0)
    {
        clear.remove(0, 1);
    }
    return clear;
}

//---------------------------------------------------------------------------------------------------------------------
bool TMainWindow::EvalFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label)
{
    const QString postfix = VDomDocument::UnitsToStr(pUnit);//Show unit in dialog lable (cm, mm or inch)
    if (formula.isEmpty())
    {
        label->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        label->setToolTip(tr("Empty field"));
        return false;
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            QString f;
            if (fromUser)
            {
                f = qApp->TrVars()->FormulaFromUser(formula, true);
            }
            else
            {
                f = formula;
            }
            f.replace("\n", " ");
            Calculator *cal = new Calculator();
            const qreal result = UnitConvertor(cal->EvalFormula(data->PlainVariables(), f), mUnit, pUnit);
            delete cal;

            label->setText(qApp->LocaleToString(result) + " " +postfix);
            label->setToolTip(tr("Value"));
            return true;
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error") + " (" + postfix + "). " + tr("Parser error: %1").arg(e.GetMsg()));
            label->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            return false;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Open(const QString &pathTo, const QString &filter)
{
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), pathTo, filter);

    if (mPath.isEmpty())
    {
        return;
    }
    else
    {
        LoadFile(mPath);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::GUIReadOnly(bool ro)
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

    ui->actionAddCustom->setDisabled(ro);
    ui->actionAddKnown->setDisabled(ro);

    ui->plainTextEditNotes->setReadOnly(ro);

    if (mType == MeasurementsType::Individual)
    {
        ui->lineEditGivenName->setReadOnly(ro);
        ui->lineEditFamilyName->setReadOnly(ro);
        ui->dateEditBirthDate->setReadOnly(ro);
        ui->comboBoxGender->setDisabled(ro);
        ui->lineEditEmail->setReadOnly(ro);
    }

    ui->comboBoxPMSystem->setDisabled(ro);

    MeasurementReadOnly(ro);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MeasurementReadOnly(bool ro)
{
    if (ro == false)
    {
        if (const QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), ColumnName))
        {
            if (nameField->text().indexOf(CustomMSign) == 0) // Check if custom
            {
                ui->lineEditName->setReadOnly(ro);
                ui->plainTextEditDescription->setReadOnly(ro);
                ui->lineEditFullName->setReadOnly(ro);

                // Need to block signals for QLineEdit in readonly mode because it still emits
                // QLineEdit::editingFinished signal.
                ui->lineEditName->blockSignals(ro);
                ui->lineEditFullName->blockSignals(ro);
            }
            else
            { // known measurement
                ui->lineEditName->setReadOnly(not ro);
                ui->plainTextEditDescription->setReadOnly(not ro);
                ui->lineEditFullName->setReadOnly(not ro);

                // Need to block signals for QLineEdit in readonly mode because it still emits
                // QLineEdit::editingFinished signal.
                ui->lineEditName->blockSignals(not ro);
                ui->lineEditFullName->blockSignals(not ro);
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        ui->lineEditName->setReadOnly(ro);
        ui->plainTextEditDescription->setReadOnly(ro);
        ui->lineEditFullName->setReadOnly(ro);

        // Need to block signals for QLineEdit in readonly mode because it still emits
        // QLineEdit::editingFinished signal.
        ui->lineEditName->blockSignals(ro);
        ui->lineEditFullName->blockSignals(ro);
    }

    if (mType == MeasurementsType::Individual)
    {
        ui->plainTextEditFormula->setReadOnly(ro);

        // Need to block signals for QLineEdit in readonly mode because it still emits QLineEdit::editingFinished
        // signal.
        ui->lineEditGivenName->blockSignals(ro);
        ui->lineEditFamilyName->blockSignals(ro);
        ui->lineEditEmail->blockSignals(ro);
    }
    else
    {
        ui->doubleSpinBoxBaseValue->setReadOnly(ro);
        ui->doubleSpinBoxInSizes->setReadOnly(ro);
        ui->doubleSpinBoxInHeights->setReadOnly(ro);
    }

    Controls(); // Buttons remove, up, down
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ReadSettings()
{
    const VTapeSettings *settings = qApp->TapeSettings();
    restoreGeometry(settings->GetGeometry());
    restoreState(settings->GetWindowState());
    restoreState(settings->GetToolbarsState(), APP_VERSION);

    // Stack limit
    //qApp->getUndoStack()->setUndoLimit(settings->GetUndoCount());
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::WriteSettings()
{
    VTapeSettings *settings = qApp->TapeSettings();
    settings->SetGeometry(saveGeometry());
    settings->SetWindowState(saveState());
    settings->SetToolbarsState(saveState(APP_VERSION));
}

//---------------------------------------------------------------------------------------------------------------------
QStringList TMainWindow::FilterMeasurements(const QStringList &mNew, const QStringList &mFilter)
{
    const QSet<QString> import = mNew.toSet().subtract(mFilter.toSet());
    return QStringList(import.toList());
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::UpdatePatternUnit()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    ShowUnits();
    RefreshTable();

    search->RefreshList(ui->lineEditFind->text());

    ui->tableWidget->selectRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
bool TMainWindow::LoadFromExistingFile(const QString &path)
{
    if (m == nullptr)
    {
        if (not QFileInfo(path).exists())
        {
            qCCritical(tMainWindow, "%s", qUtf8Printable(tr("File '%1' doesn't exist!").arg(path)));
            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }

        // Check if file already opened
        QList<TMainWindow*>list = qApp->MainWindows();
        for (int i = 0; i < list.size(); ++i)
        {
            if (list.at(i)->CurrentFile() == path)
            {
                list.at(i)->activateWindow();
                close();
                return false;
            }
        }

        VlpCreateLock(lock, QFileInfo(path).fileName());

        if (not lock->IsLocked())
        {
            qCCritical(tMainWindow, "%s", qUtf8Printable(tr("This file already opened in another window.")));
            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }

        try
        {
            data = new VContainer(qApp->TrVars(), &mUnit);

            m = new VMeasurements(data);
            m->setXMLContent(path);

            mType = m->Type();

            if (mType == MeasurementsType::Unknown)
            {
                VException e(tr("File has unknown format."));
                throw e;
            }

            if (mType == MeasurementsType::Standard)
            {
                VException e(tr("Export standard measurements not supported."));
                throw e;
            }
            else
            {
                VVITConverter converter(path);
                converter.Convert();

                VDomDocument::ValidateXML(VVITConverter::CurrentSchema, path);
            }

            m->setXMLContent(path);// Read again after conversion

            if (not m->IsDefinedKnownNamesValid())
            {
                VException e(tr("File contains invalid known measurement(s)."));
                throw e;
            }

            mUnit = m->MUnit();
            pUnit = mUnit;

            data->SetHeight(m->BaseHeight());
            data->SetSize(m->BaseSize());

            ui->labelToolTip->setVisible(false);
            ui->tabWidget->setVisible(true);

            InitWindow();

            m->ClearForExport();
            RefreshData();

            if (ui->tableWidget->rowCount() > 0)
            {
                ui->tableWidget->selectRow(0);
            }

            lock.reset();// Now we can unlock the file

            GUIReadOnly(m->ReadOnly()); // Keep last
        }
        catch (VException &e)
        {
            qCCritical(tMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                       qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
            ui->labelToolTip->setVisible(true);
            ui->tabWidget->setVisible(false);
            delete m;
            m = nullptr;
            delete data;
            data = nullptr;
            lock.reset();

            if (qApp->IsTestMode())
            {
                qApp->exit(V_EX_NOINPUT);
            }
            return false;
        }
    }
    else
    {
        qApp->NewMainWindow();
        return qApp->MainWindow()->LoadFile(path);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SetDecimals()
{
    switch (mUnit)
    {
        case Unit::Cm:
            ui->doubleSpinBoxBaseValue->setDecimals(1);
            ui->doubleSpinBoxInSizes->setDecimals(1);
            ui->doubleSpinBoxInHeights->setDecimals(1);
            break;
        case Unit::Mm:
            ui->doubleSpinBoxBaseValue->setDecimals(0);
            ui->doubleSpinBoxInSizes->setDecimals(0);
            ui->doubleSpinBoxInHeights->setDecimals(0);
            break;
        case Unit::Inch:
            ui->doubleSpinBoxBaseValue->setDecimals(5);
            ui->doubleSpinBoxInSizes->setDecimals(5);
            ui->doubleSpinBoxInHeights->setDecimals(5);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitUnits()
{
    labelPatternUnit = new QLabel(tr("Pattern unit:"));
    ui->toolBarGradation->addWidget(labelPatternUnit);

    comboBoxUnits = new QComboBox(this);
    InitComboBoxUnits();

    // set default unit
    const qint32 indexUnit = comboBoxUnits->findData(static_cast<int>(pUnit));
    if (indexUnit != -1)
    {
        comboBoxUnits->setCurrentIndex(indexUnit);
    }

    connect(comboBoxUnits, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TMainWindow::PatternUnitChanged);

    ui->toolBarGradation->addWidget(comboBoxUnits);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitComboBoxUnits()
{
    SCASSERT(comboBoxUnits != nullptr);
    comboBoxUnits->addItem(VDomDocument::UnitsToStr(Unit::Cm, true), QVariant(static_cast<int>(Unit::Cm)));
    comboBoxUnits->addItem(VDomDocument::UnitsToStr(Unit::Mm, true), QVariant(static_cast<int>(Unit::Mm)));
    comboBoxUnits->addItem(VDomDocument::UnitsToStr(Unit::Inch, true), QVariant(static_cast<int>(Unit::Inch)));
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitGender(QComboBox *gender)
{
    SCASSERT(gender != nullptr);
    gender->addItem(tr("unknown", "gender"), QVariant(static_cast<int>(GenderType::Unknown)));
    gender->addItem(tr("male", "gender"), QVariant(static_cast<int>(GenderType::Male)));
    gender->addItem(tr("female", "gender"), QVariant(static_cast<int>(GenderType::Female)));
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowInGraphicalShell()
{
#ifdef Q_WS_MAC
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \""+curFile+"\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#elif defined(Q_WS_WIN)
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(curFile);
    const QString command = "explorer" + " " + args;
    QProcess::startDetached(command);
#else
    const QString app = "xdg-open %d";
    QString cmd;
    for (int i = 0; i < app.size(); ++i)
    {
        QChar c = app.at(i);
        if (c == QLatin1Char('%') && i < app.size()-1)
        {
            c = app.at(++i);
            QString s;
            if (c == QLatin1Char('d'))
            {
                s = QLatin1Char('"') + QFileInfo(curFile).path() + QLatin1Char('"');
            }
            else if (c == QLatin1Char('%'))
            {
                s = c;
            }
            else
            {
                s = QLatin1Char('%');
                s += c;
            }
            cmd += s;
            continue;
        }
        cmd += c;
    }
    QProcess::startDetached(cmd);
#endif

}

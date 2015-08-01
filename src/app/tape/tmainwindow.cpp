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
#include "../vpatterndb/calculator.h"
#include "../ifc/ifcdef.h"
#include "../ifc/xml/vvitconverter.h"
#include "../ifc/xml/vvstconverter.h"
#include "../qmuparser/qmudef.h"
#include "../vtools/dialogs/support/dialogeditwrongformula.h"
#include "version.h"
#include "mapplication.h" // Should be last because of definning qApp

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QComboBox>
#include <QProcess>
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
#   include <QLockFile>
#endif

#define DIALOG_MAX_FORMULA_HEIGHT 64

Q_LOGGING_CATEGORY(tMainWindow, "t.mainwindow")

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::TMainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::TMainWindow),
      m(nullptr),
      data(nullptr),
      mUnit(Unit::Cm),
      mType(MeasurementsType::Individual),
      curFile(),
      gradationHeights(nullptr),
      gradationSizes(nullptr),
      formulaBaseHeight(0),
      lock(nullptr)
{
    ui->setupUi(this);
    ui->tabWidget->setVisible(false);

    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBarGradation->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->toolBarGradation->setVisible(false);

    SetupMenu();

    setWindowTitle(tr("untitled %1").arg(qApp->MainWindows().size()+1));

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
TMainWindow::~TMainWindow()
{
    if (data != nullptr)
    {
        delete data;
    }

    if (m != nullptr)
    {
        delete m;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    delete lock; // Unlock pattern file
#endif

    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString TMainWindow::CurrentFile() const
{
    return curFile;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::LoadFile(const QString &path)
{
    if (m == nullptr)
    {
        // Check if file already opened
        QList<TMainWindow*>list = qApp->MainWindows();
        for (int i = 0; i < list.size(); ++i)
        {
            if (list.at(i)->CurrentFile() == path)
            {
                list.at(i)->activateWindow();
                close();
                return;
            }
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
        lock = new QLockFile(QFileInfo(path).fileName()+".lock");
        lock->setStaleLockTime(0);
        if (not MApplication::TryLock(lock))
        {
            if (lock->error() == QLockFile::LockFailedError)
            {
                qCCritical(tMainWindow, "%s", tr("This file already opened in another window.").toUtf8().constData());
                return;
            }
        }
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)

        try
        {
            data = new VContainer(qApp->TrVars(), &mUnit);

            m = new VMeasurements(data);
            m->setXMLContent(path);

            mType = m->Type();

            if (mType == MeasurementsType::Unknown)
            {
                VException e("File has unknown format.");
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

            mUnit = m->MUnit();

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
            e.CriticalMessageBox(tr("File error."), this);
            ui->labelToolTip->setVisible(true);
            ui->tabWidget->setVisible(false);
            delete m;
            m = nullptr;
            delete data;
            data = nullptr;
            return;
        }
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->LoadFile(path);
    }

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
        const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst)");
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
        const QString filter = tr("Standard measurements (*.vst);;Individual measurements (*.vit)");
        //Use standard path to individual measurements
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
        const QString filter = tr("Measurements (*.vst, *.vit)");
        //Use standard path to individual measurements
        const QString pathTo = qApp->TapeSettings()->GetPathTemplate();
        Open(pathTo, filter);
    }
    else
    {
        qApp->NewMainWindow();
        qApp->MainWindow()->OpenTemplate();
    }
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
void TMainWindow::SaveSex(int index)
{
    const SexType type = static_cast<SexType>(ui->comboBoxSex->itemData(index).toInt());
    if (m->Sex() != type)
    {
        m->SetSex(type);
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

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->Remove(nameField->text());

    MeasurementsWasSaved(false);

    RefreshData();

    if (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->selectRow(0);
    }
    else
    {
        ui->groupBoxDetails->setEnabled(false);

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
void TMainWindow::MoveUp()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->MoveUp(nameField->text());

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row-1);
    ui->tableWidget->blockSignals(false);

    Controls(); // Buttons remove, up, down
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MoveDown()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->MoveDown(nameField->text());

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row+1);
    ui->tableWidget->blockSignals(false);

    Controls(); // Buttons remove, up, down
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::Fx()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(row, 0);
    QSharedPointer<VMeasurement> meash = data->GetVariable<VMeasurement>(nameField->text());

    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(meash->GetData(), NULL_ID, this);
    dialog->setWindowTitle(tr("Edit measurement"));

    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");

    dialog->SetFormula(text);
    const QString postfix = VDomDocument::UnitsToStr(mUnit, true);//Show unit in dialog lable (cm, mm or inch)
    dialog->setPostfix(postfix);

    if (dialog->exec() == QDialog::Accepted)
    {
        m->SetMValue(nameField->text(), dialog->GetFormula());

        MeasurementsWasSaved(false);

        RefreshData();

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
        name = QString(CustomSign + tr("M_%1")).arg(num);
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
        QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
        m->AddEmptyAfter(nameField->text(), name);
    }

    RefreshData();

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
            }
        }
        else
        {
            currentRow  = ui->tableWidget->currentRow() + list.size();
            QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
            QString after = nameField->text();
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
                after = list.at(i);
            }
        }

        RefreshData();

        ui->tableWidget->selectRow(currentRow);

        MeasurementsWasSaved(false);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ChangedSize(const QString &text)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    data->SetSize(text.toInt());
    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ChangedHeight(const QString &text)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    data->SetHeight(text.toInt());
    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowMData()
{
    if (ui->tableWidget->rowCount() > 0)
    {
        ui->groupBoxDetails->setEnabled(true);

        QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0); // name
        QSharedPointer<VMeasurement> meash;

        try
        {
            meash = data->GetVariable<VMeasurement>(qApp->TrVars()->MFromUser(nameField->text()));
        }
        catch(const VExceptionBadId &e)
        {
            Q_UNUSED(e);
            ui->groupBoxDetails->setEnabled(false);
            return;
        }

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

            ui->labelCalculatedValue->setText(QString().setNum(data->GetTableValue(meash->GetName(), mType)));
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
            EvalFormula(meash->GetFormula(), meash->GetData(), ui->labelCalculatedValue);

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

        if (m->ReadOnly())
        {
            MFields(false);
        }
        else
        {
            MFields(true);

            if (mType == MeasurementsType::Individual)
            {
                ui->plainTextEditFormula->setReadOnly(false);
            }

            if (meash->IsCustom())
            {
                ui->plainTextEditDescription->setReadOnly(false);
                ui->lineEditName->setReadOnly(false);
                ui->lineEditFullName->setReadOnly(false);
            }
            else
            {
                ui->plainTextEditDescription->setReadOnly(true);
                ui->lineEditName->setReadOnly(true);
                ui->lineEditFullName->setReadOnly(true);
            }
        }

        Controls(); // Buttons remove, up, down
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::DeployFormula()
{
    SCASSERT(ui->plainTextEditFormula != nullptr);
    SCASSERT(ui->pushButtonGrow != nullptr)
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
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMName()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    QSharedPointer<VMeasurement> meash = data->GetVariable<VMeasurement>(nameField->text());

    QString newName = ui->lineEditName->text();
    if (meash->IsCustom())
    {
        newName = CustomSign + newName;
    }

    if (data->IsUnique(newName))
    {
        m->SetMName(nameField->text(), newName);
        MeasurementsWasSaved(false);
        RefreshData();

        ui->tableWidget->blockSignals(true);
        ui->tableWidget->selectRow(row);
        ui->tableWidget->blockSignals(false);
    }
    else
    {
        ui->lineEditName->setText(ClearCustomName(nameField->text()));
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

    QTableWidgetItem *nameField = ui->tableWidget->item(row, 0);

    // Replace line return character with spaces for calc if exist
    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");

    QTableWidgetItem *formulaField = ui->tableWidget->item(row, 2);
    if (formulaField->text() == text)
    {
        QTableWidgetItem *result = ui->tableWidget->item(row, 1);
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

    QString formula;
    try
    {
        // Translate to internal look.
        formula = qApp->TrVars()->FormulaFromUser(text, true);
        QSharedPointer<VMeasurement> meash = data->GetVariable<VMeasurement>(nameField->text());
        EvalFormula(formula, meash->GetData(), ui->labelCalculatedValue);
    }
    catch (qmu::QmuParserError &e)
    {
        const QString postfix = VDomDocument::UnitsToStr(mUnit);//Show unit in dialog lable (cm, mm or inch)
        ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " +
                                          tr("Parser error: %1").arg(e.GetMsg()));
        return;
    }

    m->SetMValue(nameField->text(), formula);

    MeasurementsWasSaved(false);

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();

    RefreshData();

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

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->SetMBaseValue(nameField->text(), value);

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMSizeIncrease(double value)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->SetMSizeIncrease(nameField->text(), value);

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMHeightIncrease(double value)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->SetMHeightIncrease(nameField->text(), value);

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->selectRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::SaveMDescription()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1)
    {
        return;
    }

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->SetMDescription(nameField->text(), ui->plainTextEditDescription->toPlainText());

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

    QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    m->SetMFullName(nameField->text(), ui->lineEditFullName->text());

    MeasurementsWasSaved(false);

    RefreshData();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::NewWindow()
{
    qApp->NewMainWindow();
    qApp->MainWindow()->activateWindow();
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

    connect(ui->actionSave, &QAction::triggered, this, &TMainWindow::FileSave);
    ui->actionSave->setShortcuts(QKeySequence::Save);

    connect(ui->actionSaveAs, &QAction::triggered, this, &TMainWindow::FileSaveAs);
    ui->actionSaveAs->setShortcuts(QKeySequence::SaveAs);

    connect(ui->actionReadOnly, &QAction::triggered, this, &TMainWindow::ReadOnly);

    connect(ui->actionQuit, &QAction::triggered, this, &TMainWindow::close);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);

    // Measurements
    connect(ui->actionAddCustom, &QAction::triggered, this, &TMainWindow::AddCustom);
    connect(ui->actionAddKnown, &QAction::triggered, this, &TMainWindow::AddKnown);
    connect(ui->actionDatabase, &QAction::triggered, qApp, &MApplication::ShowDataBase);

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
    ui->tabWidget->setCurrentIndex(0);

    ui->plainTextEditNotes->setEnabled(true);

    if (mType == MeasurementsType::Standard)
    {
        ui->labelMType->setText(tr("Standard measurements"));
        ui->labelBaseSizeValue->setText(QString().setNum(m->BaseSize()) + " " +
                                        VDomDocument::UnitsToStr(m->MUnit(), true));
        ui->labelBaseHeightValue->setText(QString().setNum(m->BaseHeight()) + " " +
                                          VDomDocument::UnitsToStr(m->MUnit(), true));

        // Tab Measurements
        delete ui->labelFormula;
        delete ui->horizontalLayoutValue;
        delete ui->plainTextEditFormula;
        delete ui->pushButtonGrow;
        delete ui->toolButtonExpr;

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

        ui->toolBarGradation->setVisible(true);
        const QStringList listHeights = VMeasurement::WholeListHeights(mUnit);
        const QStringList listSizes = VMeasurement::WholeListSizes(mUnit);

        gradationHeights = SetGradationList(tr("Height: "), listHeights);
        SetDefaultHeight(static_cast<int>(data->height()));
        connect(gradationHeights, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &TMainWindow::ChangedHeight);

        gradationSizes = SetGradationList(tr("Size: "), listSizes);
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
        connect(ui->pushButtonGrow, &QPushButton::clicked, this, &TMainWindow::DeployFormula);

        this->formulaBaseHeight = ui->plainTextEditFormula->height();
        connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveMValue,
                Qt::UniqueConnection);

        connect(ui->toolButtonExpr, &QToolButton::clicked, this, &TMainWindow::Fx);
    }

    ui->actionAddCustom->setEnabled(true);
    ui->actionAddKnown->setEnabled(true);
    ui->actionReadOnly->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

    ui->lineEditName->setValidator( new QRegularExpressionValidator(QRegularExpression(NameRegExp())) );

    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &TMainWindow::Remove);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &TMainWindow::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &TMainWindow::MoveDown);

    connect(ui->lineEditName, &QLineEdit::editingFinished, this, &TMainWindow::SaveMName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &TMainWindow::SaveMDescription);
    connect(ui->lineEditFullName, &QLineEdit::editingFinished, this, &TMainWindow::SaveMFullName);

    connect(ui->pushButtonShowInExplorer, &QPushButton::clicked, this, &TMainWindow::ShowInGraphicalShell);

    InitTable();
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::InitTable()
{
    if (mType == MeasurementsType::Standard)
    {
        ui->tableWidget->setColumnHidden( 2, true );// formula
    }
    else
    {
        ui->tableWidget->setColumnHidden( 3, true );// base value
        ui->tableWidget->setColumnHidden( 4, true );// in sizes
        ui->tableWidget->setColumnHidden( 5, true );// in heights
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

    ShowHeaderUnits(ui->tableWidget, 1, unit);// calculated value
    ShowHeaderUnits(ui->tableWidget, 3, unit);// base value
    ShowHeaderUnits(ui->tableWidget, 4, unit);// in sizes
    ShowHeaderUnits(ui->tableWidget, 5, unit);// in heights
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ShowHeaderUnits(QTableWidget *table, int column, const QString &unit)
{
    SCASSERT(table != nullptr);

    const QString header = table->horizontalHeaderItem(column)->text();
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
void TMainWindow::AddCell(const QString &text, int row, int column, int aligment, bool ok)
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
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox *TMainWindow::SetGradationList(const QString &label, const QStringList &list)
{
    ui->toolBarGradation->addWidget(new QLabel(label));

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
            AddCell(meash->GetName(), currentRow, 0, Qt::AlignVCenter); // name
            AddCell(QString().setNum(*meash->GetValue()), currentRow, 1, Qt::AlignHCenter | Qt::AlignVCenter,
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

            AddCell(formula, currentRow, 2, Qt::AlignVCenter); // formula
        }
        else
        {
            AddCell(meash->GetName(), currentRow, 0, Qt::AlignVCenter); // name

            AddCell(QString().setNum(data->GetTableValue(meash->GetName(), mType)), currentRow, 1,
                    Qt::AlignHCenter | Qt::AlignVCenter, meash->IsFormulaOk()); // calculated value

            AddCell(QString().setNum(meash->GetBase()), currentRow, 3,
                    Qt::AlignHCenter | Qt::AlignVCenter); // base value

            AddCell(QString().setNum(meash->GetKsize()), currentRow, 4,
                    Qt::AlignHCenter | Qt::AlignVCenter); // in sizes

            AddCell(QString().setNum(meash->GetKheight()), currentRow, 5,
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
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
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
            ui->toolButtonUp->setEnabled(false);
            ui->toolButtonDown->setEnabled(true);
        }
        else if (ui->tableWidget->currentRow() == ui->tableWidget->rowCount()-1)
        {
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(false);
        }
        else
        {
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(true);
        }
    }
    else
    {
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::MFields(bool enabled)
{
    if (mType == MeasurementsType::Standard)
    {
        ui->doubleSpinBoxBaseValue->setEnabled(enabled);
        ui->doubleSpinBoxInSizes->setEnabled(enabled);
        ui->doubleSpinBoxInHeights->setEnabled(enabled);
    }
    else
    {
        ui->pushButtonGrow->setEnabled(enabled);
        ui->toolButtonExpr->setEnabled(enabled);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString TMainWindow::ClearCustomName(const QString &name) const
{
    QString clear = name;
    const int index = clear.indexOf(CustomSign);
    if (index == 0)
    {
        clear.remove(0, 1);
    }
    return clear;
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::EvalFormula(const QString &formula, VContainer *data, QLabel *label)
{
    const QString postfix = VDomDocument::UnitsToStr(mUnit);//Show unit in dialog lable (cm, mm or inch)
    if (formula.isEmpty())
    {
        label->setText(tr("Error") + " (" + postfix + ")");
        label->setToolTip(tr("Empty field"));
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            QString f = formula;
            f.replace("\n", " ");
            Calculator *cal = new Calculator(data, mType);
            const qreal result = cal->EvalFormula(f);
            delete cal;

            label->setText(qApp->LocaleToString(result) + " " +postfix);
            label->setToolTip(tr("Value"));
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error") + " (" + postfix + ")");
            label->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
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

    ui->plainTextEditNotes->setReadOnly(ro);
    ui->actionAddCustom->setDisabled(ro);
    ui->actionAddKnown->setDisabled(ro);

    ui->lineEditName->setDisabled(false);
    if (not ro)
    {
        if (QTableWidgetItem *nameField = ui->tableWidget->item(ui->tableWidget->currentRow(), 0))
        {
            if (nameField->text().indexOf(CustomSign) == 0) // Check if custom
            {
                ui->lineEditName->setReadOnly(ro);
            }
        }
    }
    else
    {
        ui->lineEditName->setReadOnly(ro);
    }

    ui->plainTextEditDescription->setDisabled(false);
    ui->plainTextEditDescription->setReadOnly(ro);
    ui->lineEditFullName->setDisabled(false);
    ui->lineEditFullName->setReadOnly(ro);

    if (mType == MeasurementsType::Individual)
    {
        ui->lineEditGivenName->setReadOnly(ro);
        ui->lineEditFamilyName->setReadOnly(ro);
        ui->dateEditBirthDate->setReadOnly(ro);
        ui->comboBoxSex->setDisabled(ro);
        ui->lineEditEmail->setReadOnly(ro);

        ui->plainTextEditFormula->setDisabled(false);
        ui->plainTextEditFormula->setReadOnly(ro);
    }
    else
    {
        ui->doubleSpinBoxBaseValue->setDisabled(ro);
        ui->doubleSpinBoxInSizes->setDisabled(ro);
        ui->doubleSpinBoxInHeights->setDisabled(ro);

        gradationHeights->setDisabled(ro);
        gradationSizes->setDisabled(ro);
    }

    Controls(); // Buttons remove, up, down
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::ReadSettings()
{
    restoreGeometry(qApp->TapeSettings()->GetGeometry());
    restoreState(qApp->TapeSettings()->GetWindowState());
    restoreState(qApp->TapeSettings()->GetToolbarsState(), APP_VERSION);

    // Stack limit
    //qApp->getUndoStack()->setUndoLimit(qApp->TapeSettings()->GetUndoCount());
}

//---------------------------------------------------------------------------------------------------------------------
void TMainWindow::WriteSettings()
{
    qApp->TapeSettings()->SetGeometry(saveGeometry());
    qApp->TapeSettings()->SetWindowState(saveState());
    qApp->TapeSettings()->SetToolbarsState(saveState(APP_VERSION));
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

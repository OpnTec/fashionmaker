/************************************************************************
 **
 **  @file   dialogeditlabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2017 Valentina project
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

#include "dialogeditlabel.h"
#include "ui_dialogeditlabel.h"
#include "../vmisc/vabstractapplication.h"
#include "../vformat/vlabeltemplate.h"
#include "../ifc/xml/vlabeltemplateconverter.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/exception/vexception.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QDate>

//---------------------------------------------------------------------------------------------------------------------
DialogEditLabel::DialogEditLabel(VAbstractPattern *doc, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogEditLabel),
      m_placeholdersMenu(new QMenu(this)),
      m_doc(doc),
      m_placeholders()
{
    ui->setupUi(this);

    ui->lineEditLine->setClearButtonEnabled(true);

    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &DialogEditLabel::AddLine);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogEditLabel::RemoveLine);
    connect(ui->lineEditLine, &QLineEdit::textEdited, this, &DialogEditLabel::SaveLineText);
    connect(ui->toolButtonBold, &QToolButton::toggled, this, &DialogEditLabel::SaveFontStyle);
    connect(ui->toolButtonItalic, &QToolButton::toggled, this, &DialogEditLabel::SaveFontStyle);
    connect(ui->toolButtonTextLeft, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->toolButtonTextCenter, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->toolButtonTextRight, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->listWidgetEdit, &QListWidget::itemSelectionChanged, this, &DialogEditLabel::ShowLineDetails);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &DialogEditLabel::TabChanged);
    connect(ui->toolButtonNewLabel, &QToolButton::clicked, this, &DialogEditLabel::NewTemplate);
    connect(ui->toolButtonExportLabel, &QToolButton::clicked, this, &DialogEditLabel::ExportTemplate);
    connect(ui->toolButtonImportLabel, &QToolButton::clicked, this, &DialogEditLabel::ImportTemplate);
    connect(ui->spinBoxFontSize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
            &DialogEditLabel::SaveAdditionalFontSize);

    InitPlaceholders();
    InitPlaceholdersMenu();

    ui->pushButtonInsert->setMenu(m_placeholdersMenu);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEditLabel::~DialogEditLabel()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ShowLineDetails()
{
    if (ui->listWidgetEdit->count() > 0)
    {
        const QListWidgetItem *line = ui->listWidgetEdit->currentItem();
        if (line)
        {
            ui->lineEditLine->blockSignals(true);
            ui->lineEditLine->setText(line->text());
            ui->lineEditLine->blockSignals(false);

            const QFont lineFont = line->font();

            ui->toolButtonBold->blockSignals(true);
            ui->toolButtonBold->setChecked(lineFont.bold());
            ui->toolButtonBold->blockSignals(false);

            ui->toolButtonItalic->blockSignals(true);
            ui->toolButtonItalic->setChecked(lineFont.italic());
            ui->toolButtonItalic->blockSignals(false);

            ui->toolButtonTextLeft->blockSignals(true);
            ui->toolButtonTextCenter->blockSignals(true);
            ui->toolButtonTextRight->blockSignals(true);

            const int lineAlignment = line->textAlignment();

            if (lineAlignment == 0 || lineAlignment & Qt::AlignLeft)
            {
                ui->toolButtonTextLeft->setChecked(true);
                ui->toolButtonTextCenter->setChecked(false);
                ui->toolButtonTextRight->setChecked(false);
            }
            else if (lineAlignment & Qt::AlignHCenter)
            {
                ui->toolButtonTextLeft->setChecked(false);
                ui->toolButtonTextCenter->setChecked(true);
                ui->toolButtonTextRight->setChecked(false);
            }
            else if (lineAlignment & Qt::AlignRight)
            {
                ui->toolButtonTextLeft->setChecked(false);
                ui->toolButtonTextCenter->setChecked(false);
                ui->toolButtonTextRight->setChecked(true);
            }

            ui->toolButtonTextLeft->blockSignals(false);
            ui->toolButtonTextCenter->blockSignals(false);
            ui->toolButtonTextRight->blockSignals(false);

            ui->spinBoxFontSize->blockSignals(true);
            ui->spinBoxFontSize->setValue(line->data(Qt::UserRole).toInt());
            ui->spinBoxFontSize->blockSignals(false);
        }
    }

    SetupControls();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::AddLine()
{
    int row = ui->listWidgetEdit->currentRow();
    ui->listWidgetEdit->insertItem(++row, new QListWidgetItem(tr("<empty>")));
    ui->listWidgetEdit->setCurrentRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::RemoveLine()
{
    ui->listWidgetEdit->blockSignals(true);
    QListWidgetItem *curLine = ui->listWidgetEdit->takeItem(ui->listWidgetEdit->currentRow());
    if (curLine)
    {
        delete curLine;
    }
    ui->listWidgetEdit->blockSignals(false);
    ShowLineDetails();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveLineText(const QString &text)
{
    QListWidgetItem *curLine = ui->listWidgetEdit->currentItem();
    if (curLine)
    {
        curLine->setText(text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveFontStyle(bool checked)
{
    QListWidgetItem *curLine = ui->listWidgetEdit->currentItem();
    if (curLine)
    {
        QFont lineFont = curLine->font();

        QToolButton *button = qobject_cast<QToolButton *>(sender());
        if (button)
        {
            if (button == ui->toolButtonBold)
            {
                lineFont.setBold(checked);
            }
            else if (button == ui->toolButtonItalic)
            {
                lineFont.setItalic(checked);
            }
        }

        curLine->setFont(lineFont);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveTextFormating(bool checked)
{
    QListWidgetItem *curLine = ui->listWidgetEdit->currentItem();
    if (curLine)
    {
        QToolButton *button = qobject_cast<QToolButton *>(sender());
        if (button)
        {
            ui->toolButtonTextLeft->blockSignals(true);
            ui->toolButtonTextCenter->blockSignals(true);
            ui->toolButtonTextRight->blockSignals(true);

            if (button == ui->toolButtonTextLeft)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignLeft);

                    ui->toolButtonTextCenter->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }
            else if (button == ui->toolButtonTextCenter)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignHCenter);

                    ui->toolButtonTextLeft->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }
            else if (button == ui->toolButtonTextRight)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignRight);

                    ui->toolButtonTextCenter->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }

            ui->toolButtonTextLeft->blockSignals(false);
            ui->toolButtonTextCenter->blockSignals(false);
            ui->toolButtonTextRight->blockSignals(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::NewTemplate()
{
    if (ui->listWidgetEdit->count() > 0)
    {
        const QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Create new template"),
                                                            tr("Creating new template will overwrite the current, do "
                                                               "you want to continue?"),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    ui->listWidgetEdit->blockSignals(true);
    ui->listWidgetEdit->clear();
    ui->listWidgetEdit->blockSignals(false);
    ShowLineDetails();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ExportTemplate()
{
    QString filters(tr("Label template") + QLatin1String("(*.xml)"));
    QString dir = qApp->Settings()->GetPathLabelTemplate();

    bool usedNotExistedDir = false;
    QDir directory(dir);
    if (not directory.exists())
    {
        usedNotExistedDir = directory.mkpath(".");
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export label template"),
                                                    dir + QLatin1String("/") + tr("template") + QLatin1String(".xml"),
                                                    filters, nullptr, QFileDialog::DontUseNativeDialog);

    auto RemoveTempDir = [usedNotExistedDir, dir]()
    {
        if (usedNotExistedDir)
        {
            QDir directory(dir);
            directory.rmpath(".");
        }
    };

    if (fileName.isEmpty())
    {
        RemoveTempDir();
        return;
    }

    QFileInfo f( fileName );
    if (f.suffix().isEmpty() && f.suffix() != QLatin1String("xml"))
    {
        fileName += QLatin1String(".xml");
    }

    VLabelTemplate ltemplate;
    ltemplate.CreateEmptyTemplate();
    ltemplate.AddLines(GetTemplate());

    QString error;
    const bool result = ltemplate.SaveDocument(fileName, error);
    if (result == false)
    {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }

    RemoveTempDir();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ImportTemplate()
{
    if (ui->listWidgetEdit->count() > 0)
    {
        const QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Import template"),
                                                            tr("Import template will overwrite the current, do "
                                                               "you want to continue?"),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    QString filter(tr("Label template") + QLatin1String("(*.xml)"));
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Import template"),
                                                          qApp->Settings()->GetPathLabelTemplate(), filter, nullptr,
                                                          QFileDialog::DontUseNativeDialog);
    if (fileName.isEmpty())
    {
        return;
    }

    try
    {
        VLabelTemplate ltemplate;
        ltemplate.setXMLContent(VLabelTemplateConverter(fileName).Convert());
        SetTemplate(ltemplate.ReadLines());
    }
    catch (VException &e)
    {
        qCritical("%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")), qUtf8Printable(e.ErrorMessage()),
                  qUtf8Printable(e.DetailedInformation()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::InsertPlaceholder()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        ui->lineEditLine->insert(action->data().toString());
        ui->lineEditLine->setFocus();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::TabChanged(int index)
{
    if (index == ui->tabWidget->indexOf(ui->tabPreview))
    {
        ui->toolButtonNewLabel->setDisabled(true);
        ui->toolButtonImportLabel->setDisabled(true);
        InitPreviewLines(GetTemplate());
    }
    else
    {
        ui->toolButtonNewLabel->setEnabled(ui->listWidgetEdit->count() > 0);
        ui->toolButtonImportLabel->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveAdditionalFontSize(int i)
{
    QListWidgetItem *curLine = ui->listWidgetEdit->currentItem();
    if (curLine)
    {
        QFont lineFont = curLine->font();
        lineFont.setPointSize(lineFont.pointSize() - curLine->data(Qt::UserRole).toInt() + i);
        curLine->setFont(lineFont);
        curLine->setData(Qt::UserRole, i);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SetupControls()
{
    const bool enabled = ui->listWidgetEdit->count() > 0;

    if (not enabled)
    {
        ui->lineEditLine->blockSignals(true);
        ui->lineEditLine->clear();
        ui->lineEditLine->blockSignals(false);
    }

    ui->toolButtonAdd->setEnabled(true);
    ui->toolButtonImportLabel->setEnabled(true);

    ui->toolButtonRemove->setEnabled(enabled);
    ui->toolButtonBold->setEnabled(enabled);
    ui->toolButtonItalic->setEnabled(enabled);
    ui->toolButtonTextLeft->setEnabled(enabled);
    ui->toolButtonTextCenter->setEnabled(enabled);
    ui->toolButtonTextRight->setEnabled(enabled);
    ui->pushButtonInsert->setEnabled(enabled);
    ui->toolButtonNewLabel->setEnabled(enabled);
    ui->toolButtonExportLabel->setEnabled(enabled);
    ui->lineEditLine->setEnabled(enabled);
    ui->spinBoxFontSize->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::InitPlaceholdersMenu()
{
    QChar per('%');
    auto i = m_placeholders.constBegin();
    while (i != m_placeholders.constEnd())
    {
        auto value = i.value();
        QAction *action = m_placeholdersMenu->addAction(value.first);
        action->setData(per + qApp->TrVars()->PlaceholderToUser(i.key()) + per);
        connect(action, &QAction::triggered, this, &DialogEditLabel::InsertPlaceholder);
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::InitPlaceholders()
{
    // Pattern tags
    QLocale locale(qApp->Settings()->GetLocale());

    const QString date = locale.toString(QDate::currentDate(), m_doc->GetLabelDateFormat());
    m_placeholders.insert(pl_date, qMakePair(tr("Date"), date));

    const QString time = locale.toString(QTime::currentTime(), m_doc->GetLabelTimeFormat());
    m_placeholders.insert(pl_time, qMakePair(tr("Time"), time));

    m_placeholders.insert(pl_patternName, qMakePair(tr("Pattern name"), m_doc->GetPatternName()));
    m_placeholders.insert(pl_patternNumber, qMakePair(tr("Pattern number"), m_doc->GetPatternNumber()));
    m_placeholders.insert(pl_author, qMakePair(tr("Company name or designer name"),
                                                           m_doc->GetCompanyName()));
    m_placeholders.insert(pl_customer, qMakePair(tr("Customer name"), m_doc->GetCustomerName()));
    m_placeholders.insert(pl_pExt, qMakePair(tr("Pattern extension"), QString("val")));

    const QString patternFilePath = QFileInfo(qApp->GetPPath()).baseName();
    m_placeholders.insert(pl_pFileName, qMakePair(tr("Pattern file name"), patternFilePath));

    const QString measurementsFilePath = QFileInfo(m_doc->MPath()).baseName();
    m_placeholders.insert(pl_mFileName, qMakePair(tr("Measurments file name"), measurementsFilePath));

    QString curSize;
    QString curHeight;
    QString mExt;
    if (qApp->patternType() == MeasurementsType::Multisize)
    {
        curSize = QString::number(VContainer::size());
        curHeight = QString::number(VContainer::height());
        mExt = "vst";
    }
    else if (qApp->patternType() == MeasurementsType::Individual)
    {
        curSize = QString::number(VContainer::size());
        curHeight = QString::number(VContainer::height());
        mExt = "vit";
    }

    m_placeholders.insert(pl_size, qMakePair(tr("Size"), curSize));
    m_placeholders.insert(pl_height, qMakePair(tr("Height"), curHeight));
    m_placeholders.insert(pl_mExt, qMakePair(tr("Measurments extension"), mExt));

    // Piece tags
    m_placeholders.insert(pl_pLetter, qMakePair(tr("Piece letter"), QString("")));
    m_placeholders.insert(pl_pAnnotation, qMakePair(tr("Piece annotation"), QString("")));
    m_placeholders.insert(pl_pOrientation, qMakePair(tr("Piece orientation"), QString("")));
    m_placeholders.insert(pl_pRotation, qMakePair(tr("Piece rotation"), QString("")));
    m_placeholders.insert(pl_pTilt, qMakePair(tr("Piece tilt"), QString("")));
    m_placeholders.insert(pl_pFoldPosition, qMakePair(tr("Piece fold position"), QString("")));
    m_placeholders.insert(pl_pName, qMakePair(tr("Piece name"), QString("")));
    m_placeholders.insert(pl_pQuantity, qMakePair(tr("Quantity"), QString("")));
    m_placeholders.insert(pl_mFabric, qMakePair(tr("Material: Fabric"), tr("Fabric")));
    m_placeholders.insert(pl_mLining, qMakePair(tr("Material: Lining"), tr("Lining")));
    m_placeholders.insert(pl_mInterfacing, qMakePair(tr("Material: Interfacing"), tr("Interfacing")));
    m_placeholders.insert(pl_mInterlining, qMakePair(tr("Material: Interlining"), tr("Interlining")));
    m_placeholders.insert(pl_wCut, qMakePair(tr("Word: Cut"), tr("Cut")));
    m_placeholders.insert(pl_wOnFold, qMakePair(tr("Word: on fold"), QString("")));// By default should be empty
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEditLabel::ReplacePlaceholders(QString line) const
{
    auto i = m_placeholders.constBegin();
    while (i != m_placeholders.constEnd())
    {
        line.replace(QChar('%')+i.key()+QChar('%'), i.value().second);
        ++i;
    }
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLabelTemplateLine> DialogEditLabel::GetTemplate() const
{
    QVector<VLabelTemplateLine> lines;

    for (int i=0; i<ui->listWidgetEdit->count(); ++i)
    {
        const QListWidgetItem *lineItem = ui->listWidgetEdit->item(i);
        if (lineItem)
        {
            VLabelTemplateLine line;
            line.line = qApp->TrVars()->PlaceholderFromUserText(lineItem->text());
            line.alignment = lineItem->textAlignment();
            line.fontSizeIncrement = lineItem->data(Qt::UserRole).toInt();

            const QFont font = lineItem->font();
            line.bold = font.bold();
            line.italic = font.italic();

            lines.append(line);
        }
    }

    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SetTemplate(const QVector<VLabelTemplateLine> &lines)
{
    ui->listWidgetEdit->blockSignals(true);
    ui->listWidgetEdit->clear();

    int row = -1;

    for (int i=0; i<lines.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(qApp->TrVars()->PlaceholderToUserText(lines.at(i).line));
        item->setTextAlignment(lines.at(i).alignment);
        item->setData(Qt::UserRole, lines.at(i).fontSizeIncrement);

        QFont font = item->font();
        font.setBold(lines.at(i).bold);
        font.setItalic(lines.at(i).italic);
        font.setPointSize(font.pointSize() + lines.at(i).fontSizeIncrement);
        item->setFont(font);

        ui->listWidgetEdit->insertItem(++row, item);
    }

    ui->listWidgetEdit->blockSignals(false);

    if (ui->listWidgetEdit->count() > 0)
    {
        ui->listWidgetEdit->setCurrentRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SetPiece(const VPiece &piece)
{
    const VPieceLabelData& pieceData = piece.GetPatternPieceData();
    m_placeholders[pl_pLetter].second = pieceData.GetLetter();
    m_placeholders[pl_pAnnotation].second = pieceData.GetAnnotation();
    m_placeholders[pl_pOrientation].second = pieceData.GetOrientation();
    m_placeholders[pl_pRotation].second = pieceData.GetRotationWay();
    m_placeholders[pl_pTilt].second = pieceData.GetTilt();
    m_placeholders[pl_pFoldPosition].second = pieceData.GetFoldPosition();
    m_placeholders[pl_pName].second = piece.GetName();
    m_placeholders[pl_pQuantity].second = QString::number(pieceData.GetQuantity());
    if (pieceData.IsOnFold())
    {
        m_placeholders[pl_wOnFold].second = tr("on fold");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::InitPreviewLines(const QVector<VLabelTemplateLine> &lines)
{
    ui->listWidgetPreview->clear();

    int row = -1;

    for (int i=0; i<lines.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(ReplacePlaceholders(lines.at(i).line));
        item->setTextAlignment(lines.at(i).alignment);
        item->setData(Qt::UserRole, lines.at(i).fontSizeIncrement);

        QFont font = item->font();
        font.setBold(lines.at(i).bold);
        font.setItalic(lines.at(i).italic);
        font.setPointSize(font.pointSize() + lines.at(i).fontSizeIncrement);
        item->setFont(font);

        ui->listWidgetPreview->insertItem(++row, item);
    }

    if (ui->listWidgetPreview->count() > 0)
    {
        ui->listWidgetPreview->setCurrentRow(0);
    }
}

/************************************************************************
 **
 **  @file   dialogpatternproperties.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 2, 2014
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

#include "dialogpatternproperties.h"
#include "ui_dialogpatternproperties.h"
#include <QBuffer>
#include <QPushButton>
#include <QFileDialog>
#include <QMenu>
#include <QDate>
#include "../xml/vpattern.h"
#include "../vpatterndb/vcontainer.h"
#include "../core/vapplication.h"

#define MAX_HEIGHTS 18
#define MAX_SIZES 18

//---------------------------------------------------------------------------------------------------------------------
DialogPatternProperties::DialogPatternProperties(const QString &filePath, VPattern *doc,  VContainer *pattern,
                                                 QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogPatternProperties),
      doc(doc),
      pattern(pattern),
      heightsChecked(MAX_HEIGHTS),
      sizesChecked(MAX_SIZES),
      heights (QMap<GHeights, bool>()),
      sizes(QMap<GSizes, bool>()),
      data(QMap<QCheckBox *, int>()),
      descriptionChanged(false),
      gradationChanged(false),
      defaultChanged(false),
      securityChanged(false),
      generalInfoChanged(false),
      deleteAction(nullptr),
      changeImageAction(nullptr),
      saveImageAction(nullptr),
      showImageAction(nullptr),
      m_filePath(filePath)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditAuthor->setClearButtonEnabled(true);
#endif

    SCASSERT(doc != nullptr);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    if (m_filePath.isEmpty())
    {
        ui->lineEditPathToFile->setText(tr("<Empty>"));
        ui->lineEditPathToFile->setToolTip(tr("File was not saved yet."));
        ui->pushButtonShowInExplorer->setEnabled(false);
    }
    else
    {
        ui->lineEditPathToFile->setText(QDir::toNativeSeparators(m_filePath));
        ui->lineEditPathToFile->setToolTip(QDir::toNativeSeparators(m_filePath));
        ui->pushButtonShowInExplorer->setEnabled(true);
    }
    ui->lineEditPathToFile->setCursorPosition(0);

    connect(ui->pushButtonShowInExplorer, &QPushButton::clicked, [this](){ShowInGraphicalShell(m_filePath);});
#if defined(Q_OS_MAC)
    ui->pushButtonShowInExplorer->setText(tr("Show in Finder"));
#endif //defined(Q_OS_MAC)

    ui->lineEditAuthor->setText(doc->GetAuthor());
    connect(ui->lineEditAuthor, &QLineEdit::editingFinished, this, &DialogPatternProperties::DescEdited);

    ui->plainTextEditDescription->setPlainText(doc->GetDescription());
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &DialogPatternProperties::DescEdited);

    ui->plainTextEditTechNotes->setPlainText(doc->GetNotes());
    connect(ui->plainTextEditTechNotes, &QPlainTextEdit::textChanged, this, &DialogPatternProperties::DescEdited);

    InitImage();

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DialogPatternProperties::Ok);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this,
            &DialogPatternProperties::Apply);

    QPushButton *bCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCancel != nullptr);
    connect(bCancel, &QPushButton::clicked, this, &DialogPatternProperties::close);

    ui->tabWidget->setCurrentIndex(0);
    if (qApp->patternType() != MeasurementsType::Standard)
    {
        ui->tabWidget->setTabEnabled(1, false);
    }

    InitHeights();
    InitSizes();

    heights = doc->GetGradationHeights();
    sizes = doc->GetGradationSizes();

    SetOptions(heights);
    SetOptions(sizes);

    InitComboBox(ui->comboBoxHeight, heights);
    InitComboBox(ui->comboBoxSize, sizes);

    const QString height = QString().setNum(doc->GetDefCustomHeight());
    SetDefaultHeight(height);

    const QString size = QString().setNum(doc->GetDefCustomSize());
    SetDefaultSize(size);

    connect(ui->radioButtonDefFromP, &QRadioButton::toggled, [this]()
    {
        ui->comboBoxHeight->setEnabled(ui->radioButtonDefFromP->isChecked());
        ui->comboBoxSize->setEnabled(ui->radioButtonDefFromP->isChecked());
    });

    auto DefValueChanged = [this](){defaultChanged = true;};

    connect(ui->radioButtonDefFromP, &QRadioButton::toggled, DefValueChanged);

    ui->radioButtonDefFromP->setChecked(doc->IsDefCustom());

    connect(ui->comboBoxHeight, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            DefValueChanged);
    connect(ui->comboBoxSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), DefValueChanged);

    const bool readOnly = doc->IsReadOnly();
    ui->checkBoxPatternReadOnly->setChecked(readOnly);
    if (not readOnly)
    {
        connect(ui->checkBoxPatternReadOnly, &QRadioButton::toggled, [this](){securityChanged = true;});
    }
    else
    {
        ui->checkBoxPatternReadOnly->setDisabled(true);
    }

    //Initialization change value. Set to default value after initialization
    gradationChanged = false;
    defaultChanged = false;
    securityChanged = false;

    ui->lineEditPatternName->setText(doc->GetPatternName());
    ui->lineEditPatternNumber->setText(doc->GetPatternNumber());
    ui->lineEditCompanyName->setText(doc->GetCompanyName());
    ui->lineEditCustomerName->setText(doc->GetCustomerName());
    ui->labelCreationDate->setText(QDate::currentDate().toString(Qt::SystemLocaleLongDate));
    ui->lineEditSize->setText(doc->GetPatternSize());

    const QString plSize = QLatin1String("%") + qApp->TrVars()->PlaceholderToUser(pl_size) + QLatin1String("%");
    const QString plHeight = QLatin1String("%") + qApp->TrVars()->PlaceholderToUser(pl_height) + QLatin1String("%");
    ui->lineEditSize->setToolTip(tr("Use %1 and %2 to insert pattern size and height").arg(plSize, plHeight));

    ui->checkBoxShowDate->setChecked(doc->IsDateVisible());
    if (doc->MPath().isEmpty() == true)
    {
        ui->checkBoxShowMeasurements->setChecked(false);
        ui->checkBoxShowMeasurements->setEnabled(false);
    }
    else
    {
        ui->checkBoxShowMeasurements->setChecked(doc->IsMeasurementsVisible());
    }

    connect(ui->lineEditPatternName, &QLineEdit::editingFinished, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->lineEditPatternNumber, &QLineEdit::editingFinished, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->lineEditCompanyName, &QLineEdit::editingFinished, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->lineEditCustomerName, &QLineEdit::editingFinished, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->lineEditSize, &QLineEdit::editingFinished, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->checkBoxShowDate, &QCheckBox::stateChanged, this, &DialogPatternProperties::GeneralInfoChanged);
    connect(ui->checkBoxShowMeasurements, &QCheckBox::stateChanged, this, &DialogPatternProperties::GeneralInfoChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPatternProperties::~DialogPatternProperties()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::Apply()
{
    switch (ui->tabWidget->currentIndex())
    {
        case 0:
            SaveDescription();
            descriptionChanged = false;
            emit doc->patternChanged(false);
            break;
        case 1:
            SaveGradation();
            gradationChanged = false;
            SaveDefValues();
            defaultChanged = false;
            emit doc->patternChanged(false);
            break;
        case 2:
            doc->SetReadOnly(ui->checkBoxPatternReadOnly->isChecked());
            securityChanged = false;
            emit doc->patternChanged(false);
            break;
        case 3:
            SaveGeneralInfo();
            generalInfoChanged = false;
            emit doc->patternChanged(false);
            break;

        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::Ok()
{
    if (descriptionChanged)
    {
        SaveDescription();
        descriptionChanged = false;
        emit doc->patternChanged(false);
    }

    if (gradationChanged)
    {
        SaveGradation();
        gradationChanged = false;
        emit doc->patternChanged(false);
    }

    if (defaultChanged)
    {
        SaveDefValues();
        defaultChanged = false;
        emit doc->patternChanged(false);
    }

    if (securityChanged)
    {
        doc->SetReadOnly(ui->checkBoxPatternReadOnly->isChecked());
        securityChanged = false;
        emit doc->patternChanged(false);
    }

    if (generalInfoChanged == true)
    {
        SaveGeneralInfo();
        generalInfoChanged = false;
        emit doc->patternChanged(false);
    }

    close();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SelectAll(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {
        if (box == ui->checkBoxAllHeights)
        {
            if (state == Qt::Checked)
            {
                SetHeightsChecked(true);
            }
            else if (state == Qt::Unchecked)
            {
                SetHeightsChecked(false);
            }

            if (data.contains(box))
            {
                heights.insert(static_cast<GHeights>(data.value(box)), box->isChecked());
            }
        }

        if (box == ui->checkBoxAllSizes)
        {
            if (state == Qt::Checked)
            {
                SetSizesChecked(true);
            }
            else if (state == Qt::Unchecked)
            {
                SetSizesChecked(false);
            }

            if (data.contains(box))
            {
                sizes.insert(static_cast<GSizes>(data.value(box)), box->isChecked());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::CheckStateHeight(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {
        if (state == Qt::Checked)
        {
            ++heightsChecked;
            if (heightsChecked == MAX_HEIGHTS)
            {
                ui->checkBoxAllHeights->blockSignals(true);//don't touch anothers checkboxes
                ui->checkBoxAllHeights->setCheckState(Qt::Checked);
                heights.insert(GHeights::ALL, true);
                ui->checkBoxAllHeights->blockSignals(false);
            }
        }
        else if (state == Qt::Unchecked)
        {
            if (heightsChecked == MAX_HEIGHTS)
            {
                ui->checkBoxAllHeights->blockSignals(true);//don't touch anothers checkboxes
                ui->checkBoxAllHeights->setCheckState(Qt::Unchecked);
                heights.insert(GHeights::ALL, false);
                ui->checkBoxAllHeights->blockSignals(false);
            }
            --heightsChecked;
        }

        if (data.contains(box))
        {
            heights.insert(static_cast<GHeights>(data.value(box)), box->isChecked());
        }

        UpdateDefHeight();

        CheckApplyOk();
        gradationChanged = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::CheckStateSize(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {

        if (state == Qt::Checked)
        {
            ++sizesChecked;
            if (sizesChecked == MAX_SIZES)
            {
                ui->checkBoxAllSizes->blockSignals(true);//don't touch anothers checkboxes
                ui->checkBoxAllSizes->setCheckState(Qt::Checked);
                sizes.insert(GSizes::ALL, true);
                ui->checkBoxAllSizes->blockSignals(false);
            }
        }
        else if (state == Qt::Unchecked)
        {
            if (sizesChecked == MAX_SIZES)
            {
                ui->checkBoxAllSizes->blockSignals(true);//don't touch anothers checkboxes
                ui->checkBoxAllSizes->setCheckState(Qt::Unchecked);
                sizes.insert(GSizes::ALL, false);
                ui->checkBoxAllSizes->blockSignals(false);
            }
            --sizesChecked;
        }

        if (data.contains(box))
        {
            sizes.insert(static_cast<GSizes>(data.value(box)), box->isChecked());
        }

        UpdateDefSize();

        CheckApplyOk();
        gradationChanged = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::DescEdited()
{
    descriptionChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::ToggleComboBox()
{
    ui->comboBoxHeight->setEnabled(ui->radioButtonDefFromP->isChecked());
    ui->comboBoxSize->setEnabled(ui->radioButtonDefFromP->isChecked());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::DefValueChanged()
{
    defaultChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SecurityValueChanged()
{
    securityChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::GeneralInfoChanged()
{
    generalInfoChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetHeightsChecked(bool enabled)
{
    ui->checkBoxH92->setChecked(enabled);
    ui->checkBoxH98->setChecked(enabled);
    ui->checkBoxH104->setChecked(enabled);
    ui->checkBoxH110->setChecked(enabled);
    ui->checkBoxH116->setChecked(enabled);
    ui->checkBoxH122->setChecked(enabled);
    ui->checkBoxH128->setChecked(enabled);
    ui->checkBoxH134->setChecked(enabled);
    ui->checkBoxH140->setChecked(enabled);
    ui->checkBoxH146->setChecked(enabled);
    ui->checkBoxH152->setChecked(enabled);
    ui->checkBoxH158->setChecked(enabled);
    ui->checkBoxH164->setChecked(enabled);
    ui->checkBoxH170->setChecked(enabled);
    ui->checkBoxH176->setChecked(enabled);
    ui->checkBoxH182->setChecked(enabled);
    ui->checkBoxH188->setChecked(enabled);
    ui->checkBoxH194->setChecked(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetSizesChecked(bool enabled)
{
    ui->checkBoxS22->setChecked(enabled);
    ui->checkBoxS24->setChecked(enabled);
    ui->checkBoxS26->setChecked(enabled);
    ui->checkBoxS28->setChecked(enabled);
    ui->checkBoxS30->setChecked(enabled);
    ui->checkBoxS32->setChecked(enabled);
    ui->checkBoxS34->setChecked(enabled);
    ui->checkBoxS36->setChecked(enabled);
    ui->checkBoxS38->setChecked(enabled);
    ui->checkBoxS40->setChecked(enabled);
    ui->checkBoxS42->setChecked(enabled);
    ui->checkBoxS44->setChecked(enabled);
    ui->checkBoxS46->setChecked(enabled);
    ui->checkBoxS48->setChecked(enabled);
    ui->checkBoxS50->setChecked(enabled);
    ui->checkBoxS52->setChecked(enabled);
    ui->checkBoxS54->setChecked(enabled);
    ui->checkBoxS56->setChecked(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::InitHeights()
{
    Init(ui->checkBoxAllHeights, static_cast<int>(GHeights::ALL), &DialogPatternProperties::SelectAll);

    Init(ui->checkBoxH92, static_cast<int>(GHeights::H92), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH98, static_cast<int>(GHeights::H98), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH104, static_cast<int>(GHeights::H104), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH110, static_cast<int>(GHeights::H110), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH116, static_cast<int>(GHeights::H116), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH122, static_cast<int>(GHeights::H122), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH128, static_cast<int>(GHeights::H128), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH134, static_cast<int>(GHeights::H134), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH140, static_cast<int>(GHeights::H140), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH146, static_cast<int>(GHeights::H146), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH152, static_cast<int>(GHeights::H152), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH158, static_cast<int>(GHeights::H158), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH164, static_cast<int>(GHeights::H164), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH170, static_cast<int>(GHeights::H170), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH176, static_cast<int>(GHeights::H176), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH182, static_cast<int>(GHeights::H182), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH188, static_cast<int>(GHeights::H188), &DialogPatternProperties::CheckStateHeight);
    Init(ui->checkBoxH194, static_cast<int>(GHeights::H194), &DialogPatternProperties::CheckStateHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::InitSizes()
{
    Init(ui->checkBoxAllSizes, static_cast<int>(GSizes::ALL), &DialogPatternProperties::SelectAll);

    Init(ui->checkBoxS22, static_cast<int>(GSizes::S22), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS24, static_cast<int>(GSizes::S24), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS26, static_cast<int>(GSizes::S26), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS28, static_cast<int>(GSizes::S28), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS30, static_cast<int>(GSizes::S30), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS32, static_cast<int>(GSizes::S32), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS34, static_cast<int>(GSizes::S34), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS36, static_cast<int>(GSizes::S36), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS38, static_cast<int>(GSizes::S38), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS40, static_cast<int>(GSizes::S40), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS42, static_cast<int>(GSizes::S42), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS44, static_cast<int>(GSizes::S44), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS46, static_cast<int>(GSizes::S46), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS48, static_cast<int>(GSizes::S48), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS50, static_cast<int>(GSizes::S50), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS52, static_cast<int>(GSizes::S52), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS54, static_cast<int>(GSizes::S54), &DialogPatternProperties::CheckStateSize);
    Init(ui->checkBoxS56, static_cast<int>(GSizes::S56), &DialogPatternProperties::CheckStateSize);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::CheckApplyOk()
{
    bool enable = !(heightsChecked == 0 || sizesChecked == 0);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enable);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SaveDescription()
{
    doc->SetNotes(ui->plainTextEditTechNotes->document()->toPlainText());
    doc->SetDescription(ui->plainTextEditDescription->document()->toPlainText());
    doc->SetAuthor(ui->lineEditAuthor->text());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SaveGradation()
{
    doc->SetGradationHeights(heights);
    doc->SetGradationSizes(sizes);
    emit UpdateGradation();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SaveDefValues()
{
    if (ui->radioButtonDefFromM->isChecked())
    {
        doc->SetDefCustom(false);
    }
    else
    {
        doc->SetDefCustom(true);
        doc->SetDefCustomHeight(ui->comboBoxHeight->currentText().toInt());
        doc->SetDefCustomSize(ui->comboBoxSize->currentText().toInt());
    }
    defaultChanged = false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SaveGeneralInfo()
{
    doc->SetPatternName(ui->lineEditPatternName->text());
    doc->SetPatternNumber(ui->lineEditPatternNumber->text());
    doc->SetCompanyName(ui->lineEditCompanyName->text());
    doc->SetCustomerName(ui->lineEditCustomerName->text());
    doc->SetPatternSize(ui->lineEditSize->text());
    doc->SetDateVisible(ui->checkBoxShowDate->isChecked());
    doc->SetMesurementsVisible(ui->checkBoxShowMeasurements->isChecked());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetDefaultHeight(const QString &def)
{
    int index = ui->comboBoxHeight->findText(def);
    if (index != -1)
    {
        ui->comboBoxHeight->setCurrentIndex(index);
        defaultChanged = true;
    }
    else
    {
        const int height = static_cast<int>(pattern->height());
        index = ui->comboBoxHeight->findText(QString().setNum(height));
        if (index != -1)
        {
            ui->comboBoxHeight->setCurrentIndex(index);
            defaultChanged = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetDefaultSize(const QString &def)
{
    int index = ui->comboBoxSize->findText(def);
    if (index != -1)
    {
        ui->comboBoxSize->setCurrentIndex(index);
        defaultChanged = true;
    }
    else
    {
        const int size = static_cast<int>(pattern->size());
        index = ui->comboBoxSize->findText(QString().setNum(size));
        if (index != -1)
        {
            ui->comboBoxSize->setCurrentIndex(index);
            defaultChanged = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::UpdateDefHeight()
{
    const QString def = ui->comboBoxHeight->currentText();
    InitComboBox(ui->comboBoxHeight, heights);
    SetDefaultHeight(def);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::UpdateDefSize()
{
    const QString def = ui->comboBoxSize->currentText();
    InitComboBox(ui->comboBoxSize, sizes);
    SetDefaultSize(def);
}

//---------------------------------------------------------------------------------------------------------------------
template<typename Func>
void DialogPatternProperties::Init(QCheckBox *check, int val, Func slot)
{
    connect(check, &QCheckBox::stateChanged, this, slot);
    data.insert(check, val);
}

//---------------------------------------------------------------------------------------------------------------------
template<typename GVal>
void DialogPatternProperties::SetOptions(const QMap<GVal, bool> &option)
{
    if (option.value(GVal::ALL) == false)
    {
        QMapIterator<GVal, bool> i(option);
        while (i.hasNext())
        {
            i.next();
            if (i.value() == false && i.key() != GVal::ALL)
            {
                QCheckBox *box = data.key(static_cast<int>(i.key()));
                if (box != nullptr)
                {
                    box->setCheckState(Qt::Unchecked);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<typename GVal>
void DialogPatternProperties::InitComboBox(QComboBox *box, const QMap<GVal, bool> &option)
{
    SCASSERT(box != nullptr);

    box->clear();

    QMapIterator<GVal, bool> i(option);
    while (i.hasNext())
    {
        i.next();
        if (i.value() && i.key() != GVal::ALL)
        {
            box->addItem(QString().setNum(static_cast<int>(UnitConvertor(static_cast<int>(i.key()), Unit::Cm,
                                                                         *pattern->GetPatternUnit()))));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QImage DialogPatternProperties::GetImage()
{
// we set an image from file.val
    QImage image;
    QByteArray byteArray;
    byteArray.append(doc->GetImage().toUtf8());
    QByteArray ba = QByteArray::fromBase64(byteArray);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::ReadOnly);
    QString extension = doc->GetImageExtension();
    image.load(&buffer, extension.toLatin1().data()); // writes image into ba in 'extension' format
    return image;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::InitImage()
{
    ui->imageLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->imageLabel->setScaledContents(true);
    connect(ui->imageLabel, &QWidget::customContextMenuRequested, [this]()
    {
        QMenu menu(this);
        menu.addAction(deleteAction);
        menu.addAction(changeImageAction);
        menu.addAction(saveImageAction);
        menu.addAction(showImageAction);
        menu.exec(QCursor::pos());
        menu.show();
    });

    deleteAction      = new QAction(tr("Delete image"), this);
    changeImageAction = new QAction(tr("Change image"), this);
    saveImageAction   = new QAction(tr("Save image to file"), this);
    showImageAction   = new QAction(tr("Show image"), this);

    connect(deleteAction, &QAction::triggered, [this]()
    {
        doc->DeleteImage();
        ui->imageLabel->setText(tr("Change image"));
        deleteAction->setEnabled(false);
        saveImageAction->setEnabled(false);
        showImageAction->setEnabled(false);
    });

    connect(changeImageAction, &QAction::triggered, this, &DialogPatternProperties::ChangeImage);
    connect(saveImageAction, &QAction::triggered, this, &DialogPatternProperties::SaveImage);
    connect(showImageAction, &QAction::triggered, [this]()
    {
        QLabel *label = new QLabel(this, Qt::Window);
        const QImage image = GetImage();
        label->setPixmap(QPixmap::fromImage(image));
        label->setGeometry(QRect(QCursor::pos(), image.size()));
        label->show();
    });

    const QImage image = GetImage();
    if (not image.isNull())
    {
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        deleteAction->setEnabled(false);
        saveImageAction->setEnabled(false);
        showImageAction->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::ChangeImage()
{
    const QString filter = tr("Images") + QLatin1String(" (*.png *.jpg *.jpeg *.bmp)");
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Image for pattern"), QString(), filter);
    QImage image;
    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        if (not image.load(fileName))
        {
            return;
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
        QFileInfo f(fileName);
        QString extension = f.suffix().toUpper();

        if (extension == QLatin1String("JPEG"))
        {
            extension = "JPG";
        }
        if (extension == QLatin1String("PNG") || extension == QLatin1String("JPG") || extension == QLatin1String("BMP"))
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, extension.toLatin1().data()); //writes the image in 'extension' format inside the buffer
            QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());

            // save our image to file.val
            doc->SetImage(iconBase64, extension);
        }
        deleteAction->setEnabled(true);
        saveImageAction->setEnabled(true);
        showImageAction->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SaveImage()
{
    QByteArray byteArray;
    byteArray.append(doc->GetImage().toUtf8());
    QByteArray ba = QByteArray::fromBase64(byteArray);
    const QString extension = QLatin1String(".") + doc->GetImageExtension();
    QString filter = tr("Images") + QLatin1String(" (*") + extension + QLatin1String(")");
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), tr("untitled"), filter, &filter);
    if (not filename.isEmpty())
    {
        if (not filename.endsWith(extension.toUpper()))
        {
            filename.append(extension);
        }
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(ba);
            file.close();
        }
    }
}

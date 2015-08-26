/************************************************************************
 **
 **  @file   dialogsavelayout.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2015
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

#include "dialogsavelayout.h"
#include "ui_dialogsavelayout.h"
#include "../options.h"
#include "../core/vapplication.h"
#include "../../libs/vmisc/vsettings.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

using namespace nm_DialogSaveLayout;

bool VFrmWithTest::havePdf = false;
bool VFrmWithTest::tested  = false;

const std::vector<VFrmWithTest> DialogSaveLayout::availFormats = {
    VFrmWithTest(tr("Svg files (*.svg)"), ".svg"),
    VFrmWithTest(tr("PDF files (*.pdf)"), ".pdf"),
    VFrmWithTest(tr("Images (*.png)"), ".png"),
    VFrmWithTest(tr("Wavefront OBJ (*.obj)"), ".obj"),
    VFrmWithTest(tr("PS files (*.ps)"), ".ps", 1), //fixme: use 1 to have exe once tested on 1st run, or any other value to test always as original do
    VFrmWithTest(tr("EPS files (*.eps)"), ".eps", 1),
};


//---------------------------------------------------------------------------------------------------------------------
DialogSaveLayout::DialogSaveLayout(int count, const QString &fileName,
                                   QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogSaveLAyout), count(count)
{
    ui->setupUi(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(false);

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("^[\\w\\-. ]+$"), this);
    ui->lineEditFileName->setValidator(validator);
    ui->lineEditFileName->setText(fileName+"_");

    foreach (auto& v , availFormats)
    {
            if (v.test())
            {
                ui->comboBoxFormat->addItem(v.pair.first, QVariant(v.pair.second));
            }
    }
    connect(bOk, &QPushButton::clicked, this, &DialogSaveLayout::Save);
    connect(ui->lineEditFileName, &QLineEdit::textChanged, this, &DialogSaveLayout::ShowExample);
    connect(ui->comboBoxFormat, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &DialogSaveLayout::ShowExample);
    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &DialogSaveLayout::Browse);
    connect(ui->lineEditPath, &QLineEdit::textChanged, this, &DialogSaveLayout::PathChanged);

    ui->lineEditPath->setText(qApp->Settings()->GetPathLayout());
    ShowExample();//Show example for current format.

    setMaximumSize(size());
    setMinimumSize(size());
}
//---------------------------------------------------------------------------------------------------------------------

void DialogSaveLayout::SelectFormate(const size_t formate)
{
    if (formate >= availFormats.size())
    {
        AppAbort(tr("Tried to use out of range format number."), INVALID_PARAMS_STATUS);
    }

    int i = ui->comboBoxFormat->findData(availFormats[formate].pair.second);
    if (i < 0)
    {
        AppAbort(tr("Selected not present format."), INVALID_PARAMS_STATUS);
    }
    ui->comboBoxFormat->setCurrentIndex(i);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::MakeHelpFormatList()
{
   QString out = "\n";
   int cntr = 0;
   foreach(auto& v, availFormats)
   {
       if (v.test())
       {
           out += "\t"+v.pair.first+" = "+ QString::number(cntr++)+"\n";
       }
   }
   return out;
}

//---------------------------------------------------------------------------------------------------------------------
DialogSaveLayout::~DialogSaveLayout()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::Path() const
{
    return ui->lineEditPath->text();
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::FileName() const
{
    return ui->lineEditFileName->text();
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::Formate() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    return ui->comboBoxFormat->itemData(ui->comboBoxFormat->currentIndex()).toString();
#else
    return ui->comboBoxFormat->currentData().toString();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::Save()
{
    for (int i=0; i < count; ++i)
    {
        const QString name = Path()+"/"+FileName()+QString::number(i+1)+Formate();
        if (QFile::exists(name))
        {
            QMessageBox::StandardButton res = QMessageBox::question(this, tr("Name conflict"),
                                  tr("Folder already contain file with name %1. Rewrite all conflict file names?")
                                  .arg(name), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
            if (res == QMessageBox::No)
            {
                return;
            }
            else
            {
                break;
            }
        }
    }
    accept();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::ShowExample()
{
    ui->labelExample->setText(tr("Example:") + FileName() + "1" + Formate());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::Browse()
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select folder"), QDir::homePath(),
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditPath->setText(dir);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::PathChanged(const QString &text)
{
    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);

    QPalette palette = ui->lineEditPath->palette();

    QDir dir(text);
    dir.setPath(text);
    if (dir.exists(text))
    {
        bOk->setEnabled(true);
        palette.setColor(ui->lineEditPath->foregroundRole(), Qt::black);
    }
    else
    {
        bOk->setEnabled(false);
        palette.setColor(ui->lineEditPath->foregroundRole(), Qt::red);
    }

    ui->lineEditPath->setPalette(palette);
}
//---------------------------------------------------------------------------------------------------------------------

bool VFrmWithTest::TestPdf()
{
    bool res = false;

    QProcess proc;
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    proc.start(qApp->applicationDirPath()+"/"+PDFTOPS); // Seek pdftops in app bundle or near valentin.exe
#else
    proc.start(PDFTOPS); // Seek pdftops in standard path
#endif
    if (proc.waitForFinished(15000))
    {
        res = true;
    }
    else
    {
        qDebug()<<PDFTOPS<<"error"<<proc.error()<<proc.errorString();
    }
    return res;
}

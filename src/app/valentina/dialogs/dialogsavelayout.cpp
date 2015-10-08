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
#include "../vmisc/vsettings.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

using namespace nm_DialogSaveLayout;

const QString baseFilenameRegExp = QStringLiteral("^[\\w\\-. ]+$");

bool VFrmWithTest::havePdf = false;
bool VFrmWithTest::tested  = false;

const std::vector<VFrmWithTest> DialogSaveLayout::availFormats =
{
    VFrmWithTest(tr("Svg files (*.svg)"), ".svg"),
    VFrmWithTest(tr("PDF files (*.pdf)"), ".pdf"),
    VFrmWithTest(tr("Images (*.png)"), ".png"),
    VFrmWithTest(tr("Wavefront OBJ (*.obj)"), ".obj"),
    VFrmWithTest(tr("PS files (*.ps)"), ".ps", 1), //fixme: use 1 to have exe once tested on 1st run, or any other value to test always as original do
    VFrmWithTest(tr("EPS files (*.eps)"), ".eps", 1),
    VFrmWithTest(tr("DXF files (*.dxf)"), ".dxf"),
};


//---------------------------------------------------------------------------------------------------------------------
DialogSaveLayout::DialogSaveLayout(int count, const QString &fileName, QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogSaveLAyout), count(count)
{
    ui->setupUi(this);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(false);

#if QT_VERSION > QT_VERSION_CHECK(5, 1, 0)
    ui->lineEditFileName->setValidator( new QRegularExpressionValidator(QRegularExpression(baseFilenameRegExp), this));
#else
    ui->lineEditFileName->setValidator( new QRegExpValidator(QRegExp(baseFilenameRegExp), this));
#endif

    const QString mask = fileName+QLatin1Literal("_");
    if (VApplication::CheckGUI())
    {
        ui->lineEditFileName->setText(mask);
    }
    else
    {
        if (QRegularExpression(baseFilenameRegExp).match(mask).hasMatch())
        {
            ui->lineEditFileName->setText(mask);
        }
        else
        {
            qCritical() << tr("The base filename has not match regular expression.");
            std::exit(V_EX_USAGE);
        }
    }

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

    ui->lineEditPath->setText(qApp->ValentinaSettings()->GetPathLayout());
    ShowExample();//Show example for current format.

    setMaximumSize(size());
    setMinimumSize(size());
}
//---------------------------------------------------------------------------------------------------------------------

void DialogSaveLayout::SelectFormate(const size_t formate)
{
    if (formate >= availFormats.size())
    {
        qCritical() << tr("Tried to use out of range format number.");
        std::exit(V_EX_USAGE);
    }

    int i = ui->comboBoxFormat->findData(availFormats[formate].pair.second);
    if (i < 0)
    {
        qCritical() << tr("Selected not present format.");
        std::exit(V_EX_USAGE);
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
void DialogSaveLayout::SetDestinationPath(const QString &cmdDestinationPath)
{
    QString path;
    if (cmdDestinationPath.isEmpty())
    {
        path = QDir::currentPath();
    }
    else if (QDir(cmdDestinationPath).isAbsolute())
    {
        path = cmdDestinationPath;
    }
    else
    {
        QDir dir;
        if (not dir.cd(cmdDestinationPath))
        {
            qCritical() << tr("The destination directory doesn't exists or is not readable.");
            std::exit(V_EX_DATAERR);
        }
        path = dir.absolutePath();
    }

    qDebug() << "Output full path: " << path << "\n";
    ui->lineEditPath->setText(path);
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

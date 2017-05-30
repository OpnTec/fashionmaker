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
#include "../ifc/exception/vexception.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QtDebug>
#include <QRegularExpression>

const QString baseFilenameRegExp = QStringLiteral("^[\\w\\-. ]+$");

bool DialogSaveLayout::havePdf = false;
bool DialogSaveLayout::tested  = false;

//---------------------------------------------------------------------------------------------------------------------
DialogSaveLayout::DialogSaveLayout(int count, const QString &fileName, QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogSaveLAyout), count(count), isInitialized(false), availFormats(InitAvailFormats())
{
    ui->setupUi(this);

    ui->lineEditPath->setClearButtonEnabled(true);
    ui->lineEditFileName->setClearButtonEnabled(true);

    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(false);

    ui->lineEditFileName->setValidator( new QRegularExpressionValidator(QRegularExpression(baseFilenameRegExp), this));

    const QString mask = fileName+QLatin1String("_");
    if (VApplication::IsGUIMode())
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
            VException e(tr("The base filename does not match a regular expression."));
            throw e;
        }
    }

    foreach (auto& v , availFormats)
    {
        ui->comboBoxFormat->addItem(v.first, QVariant(v.second));
    }
    connect(bOk, &QPushButton::clicked, this, &DialogSaveLayout::Save);

    auto ShowExample = [this]()
    {
        ui->labelExample->setText(tr("Example:") + FileName() + QLatin1String("1") + Format());
    };

    connect(ui->lineEditFileName, &QLineEdit::textChanged, this, ShowExample);
    connect(ui->comboBoxFormat, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, ShowExample);
    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, [this]()
    {
        const QString dirPath = qApp->ValentinaSettings()->GetPathLayout();
        bool usedNotExistedDir = false;
        QDir directory(dirPath);
        if (not directory.exists())
        {
            usedNotExistedDir = directory.mkpath(".");
        }

        const QString dir = QFileDialog::getExistingDirectory(this, tr("Select folder"), dirPath,
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (not dir.isEmpty())
        {// If paths equal the signal will not be called, we will do this manually
            dir == ui->lineEditPath->text() ? PathChanged(dir) : ui->lineEditPath->setText(dir);
        }

        if (usedNotExistedDir)
        {
            QDir directory(dirPath);
            directory.rmpath(".");
        }
    });
    connect(ui->lineEditPath, &QLineEdit::textChanged, this, &DialogSaveLayout::PathChanged);

    ui->lineEditPath->setText(qApp->ValentinaSettings()->GetPathLayout());
    ShowExample();//Show example for current format.
}
//---------------------------------------------------------------------------------------------------------------------

void DialogSaveLayout::SelectFormate(const int formate)
{
    if (formate >= availFormats.size())
    {
        VException e(tr("Tried to use out of range format number."));
        throw e;
    }

    const int i = ui->comboBoxFormat->findData(availFormats.at(formate).second);
    if (i < 0)
    {
        VException e(tr("Selected not present format."));
        throw e;
    }
    ui->comboBoxFormat->setCurrentIndex(i);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::MakeHelpFormatList()
{
   QString out("\n");
   int cntr = 0;
   const QVector<std::pair<QString, QString>> availFormats = InitAvailFormats();
   foreach(auto& v, availFormats)
   {
       out += QLatin1String("\t") + v.first+QLatin1String(" = ") + QString::number(cntr++) + QLatin1String("\n");
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
            VException e(tr("The destination directory doesn't exists or is not readable."));
            throw e;
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
QString DialogSaveLayout::Format() const
{
    return ui->comboBoxFormat->currentData().toString();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::Save()
{
    for (int i=0; i < count; ++i)
    {
        const QString name = Path()+QLatin1Literal("/")+FileName()+QString::number(i+1)+Format();
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
void DialogSaveLayout::PathChanged(const QString &text)
{
    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr)

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
void DialogSaveLayout::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    setFixedHeight(size().height());

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSaveLayout::SupportPSTest()
{
    if (!tested)
    {
        havePdf = TestPdf();
        tested = true;
    }
    return havePdf;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSaveLayout::TestPdf()
{
    bool res = false;

    QProcess proc;
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    // Seek pdftops in app bundle or near valentin.exe
    proc.start(qApp->applicationDirPath() + QLatin1String("/")+ PDFTOPS);
#else
    proc.start(PDFTOPS); // Seek pdftops in standard path
#endif
    if (proc.waitForStarted(15000) && (proc.waitForFinished(15000) || proc.state() == QProcess::NotRunning))
    {
        res = true;
    }
    else
    {
        qDebug()<<PDFTOPS<<"error"<<proc.error()<<proc.errorString();
    }
    return res;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<std::pair<QString, QString>> DialogSaveLayout::InitAvailFormats()
{
    QVector<std::pair<QString, QString>> list;
    list.append(std::make_pair(QLatin1String("Svg ") + tr("files") + QLatin1String(" (*.svg)"), QLatin1String(".svg")));
    list.append(std::make_pair(QLatin1String("PDF ") + tr("files") + QLatin1String(" (*.pdf)"), QLatin1String(".pdf")));
    list.append(std::make_pair(QLatin1String("Images") + QLatin1String(" (*.png)"), QLatin1String(".png")));
#ifndef V_NO_ASSERT // Temporarily unavailable
    list.append(std::make_pair(QLatin1String("Wavefront OBJ (*.obj)"), QLatin1String(".obj")));
#endif
    if (SupportPSTest())
    {
        list.append(std::make_pair(QLatin1String("PS ") + tr("files") + QLatin1String(" (*.ps)"),
                                   QLatin1String(".ps")));
        list.append(std::make_pair(QLatin1String("EPS ") + tr("files") + QLatin1String(" (*.eps)"),
                                   QLatin1String(".eps")));
    }
    list.append(std::make_pair(QLatin1String("DXF ") + tr("files") + QLatin1String(" (*.dxf)"), QLatin1String(".dxf")));
    return list;
}

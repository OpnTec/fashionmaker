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
    : QDialog(parent),
      ui(new Ui::DialogSaveLAyout),
      count(count),
      isInitialized(false)
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

    foreach (auto& v, InitFormats())
    {
        ui->comboBoxFormat->addItem(v.first, QVariant(static_cast<int>(v.second)));
    }
#ifdef V_NO_ASSERT // Temporarily unavailable
    const int index = ui->comboBoxFormat->findData(static_cast<int>(LayoutExportFormats::OBJ));
    if (index != -1)
    {
        ui->comboBoxFormat->removeItem(index);
    }
#endif
    connect(bOk, &QPushButton::clicked, this, &DialogSaveLayout::Save);
    connect(ui->lineEditFileName, &QLineEdit::textChanged, this, &DialogSaveLayout::ShowExample);
    connect(ui->comboBoxFormat, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSaveLayout::ShowExample);
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
                                                              QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks
                                                              | QFileDialog::DontUseNativeDialog);
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

void DialogSaveLayout::SelectFormat(LayoutExportFormats format)
{
    if (static_cast<int>(format) < 0 || format >= LayoutExportFormats::COUNT)
    {
        VException e(tr("Tried to use out of range format number."));
        throw e;
    }

    const int i = ui->comboBoxFormat->findData(static_cast<int>(format));
    if (i < 0)
    {
        VException e(tr("Selected not present format."));
        throw e;
    }
    ui->comboBoxFormat->setCurrentIndex(i);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::SetBinaryDXFFormat(bool binary)
{
    switch(Format())
    {
        case LayoutExportFormats::DXF_AC1006_Flat:
        case LayoutExportFormats::DXF_AC1009_Flat:
        case LayoutExportFormats::DXF_AC1012_Flat:
        case LayoutExportFormats::DXF_AC1014_Flat:
        case LayoutExportFormats::DXF_AC1015_Flat:
        case LayoutExportFormats::DXF_AC1018_Flat:
        case LayoutExportFormats::DXF_AC1021_Flat:
        case LayoutExportFormats::DXF_AC1024_Flat:
        case LayoutExportFormats::DXF_AC1027_Flat:
        case LayoutExportFormats::DXF_AC1006_AAMA:
        case LayoutExportFormats::DXF_AC1009_AAMA:
        case LayoutExportFormats::DXF_AC1012_AAMA:
        case LayoutExportFormats::DXF_AC1014_AAMA:
        case LayoutExportFormats::DXF_AC1015_AAMA:
        case LayoutExportFormats::DXF_AC1018_AAMA:
        case LayoutExportFormats::DXF_AC1021_AAMA:
        case LayoutExportFormats::DXF_AC1024_AAMA:
        case LayoutExportFormats::DXF_AC1027_AAMA:
        case LayoutExportFormats::DXF_AC1006_ASTM:
        case LayoutExportFormats::DXF_AC1009_ASTM:
        case LayoutExportFormats::DXF_AC1012_ASTM:
        case LayoutExportFormats::DXF_AC1014_ASTM:
        case LayoutExportFormats::DXF_AC1015_ASTM:
        case LayoutExportFormats::DXF_AC1018_ASTM:
        case LayoutExportFormats::DXF_AC1021_ASTM:
        case LayoutExportFormats::DXF_AC1024_ASTM:
        case LayoutExportFormats::DXF_AC1027_ASTM:
            ui->checkBoxBinaryDXF->setChecked(binary);
            break;
        case LayoutExportFormats::SVG:
        case LayoutExportFormats::PDF:
        case LayoutExportFormats::PDFTiled:
        case LayoutExportFormats::PNG:
        case LayoutExportFormats::OBJ:
        case LayoutExportFormats::PS:
        case LayoutExportFormats::EPS:
        default:
            ui->checkBoxBinaryDXF->setChecked(false);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogSaveLayout::IsBinaryDXFFormat() const
{
    switch(Format())
    {
        case LayoutExportFormats::DXF_AC1006_Flat:
        case LayoutExportFormats::DXF_AC1009_Flat:
        case LayoutExportFormats::DXF_AC1012_Flat:
        case LayoutExportFormats::DXF_AC1014_Flat:
        case LayoutExportFormats::DXF_AC1015_Flat:
        case LayoutExportFormats::DXF_AC1018_Flat:
        case LayoutExportFormats::DXF_AC1021_Flat:
        case LayoutExportFormats::DXF_AC1024_Flat:
        case LayoutExportFormats::DXF_AC1027_Flat:
        case LayoutExportFormats::DXF_AC1006_AAMA:
        case LayoutExportFormats::DXF_AC1009_AAMA:
        case LayoutExportFormats::DXF_AC1012_AAMA:
        case LayoutExportFormats::DXF_AC1014_AAMA:
        case LayoutExportFormats::DXF_AC1015_AAMA:
        case LayoutExportFormats::DXF_AC1018_AAMA:
        case LayoutExportFormats::DXF_AC1021_AAMA:
        case LayoutExportFormats::DXF_AC1024_AAMA:
        case LayoutExportFormats::DXF_AC1027_AAMA:
        case LayoutExportFormats::DXF_AC1006_ASTM:
        case LayoutExportFormats::DXF_AC1009_ASTM:
        case LayoutExportFormats::DXF_AC1012_ASTM:
        case LayoutExportFormats::DXF_AC1014_ASTM:
        case LayoutExportFormats::DXF_AC1015_ASTM:
        case LayoutExportFormats::DXF_AC1018_ASTM:
        case LayoutExportFormats::DXF_AC1021_ASTM:
        case LayoutExportFormats::DXF_AC1024_ASTM:
        case LayoutExportFormats::DXF_AC1027_ASTM:
            return ui->checkBoxBinaryDXF->isChecked();
        case LayoutExportFormats::SVG:
        case LayoutExportFormats::PDF:
        case LayoutExportFormats::PDFTiled:
        case LayoutExportFormats::PNG:
        case LayoutExportFormats::OBJ:
        case LayoutExportFormats::PS:
        case LayoutExportFormats::EPS:
        default:
            return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::MakeHelpFormatList()
{
   QString out("\n");
   foreach(auto& v, InitFormats())
   {
       out += QLatin1String("\t") + v.first + QLatin1String(" = ") + QString::number(static_cast<int>(v.second))
               + QLatin1String("\n");
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
QString DialogSaveLayout::ExportFormatDescription(LayoutExportFormats format)
{
    const QString dxfSuffix = QStringLiteral("(*.dxf)");
    const QString dxfFlatFilesStr = tr("(flat) files");
    const QString filesStr = tr("files");

    switch(format)
    {
        case LayoutExportFormats::SVG:
            return QString("Svg %1 (*.svg)").arg(filesStr);
        case LayoutExportFormats::PDF:
            return QString("PDF %1 (*.pdf)").arg(filesStr);
        case LayoutExportFormats::PNG:
            return tr("Image files") + QLatin1String(" (*.png)");
        case LayoutExportFormats::OBJ:
            return "Wavefront OBJ (*.obj)";
        case LayoutExportFormats::PS:
            return QString("PS %1 (*.ps)").arg(filesStr);
        case LayoutExportFormats::EPS:
            return QString("EPS %1 (*.eps)").arg(filesStr);
        case LayoutExportFormats::DXF_AC1006_Flat:
            return QString("AutoCAD DXF R10 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1009_Flat:
            return QString("AutoCAD DXF R11/12 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1012_Flat:
            return QString("AutoCAD DXF R13 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1014_Flat:
            return QString("AutoCAD DXF R14 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1015_Flat:
            return QString("AutoCAD DXF 2000 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1018_Flat:
            return QString("AutoCAD DXF 2004 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1021_Flat:
            return QString("AutoCAD DXF 2007 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1024_Flat:
            return QString("AutoCAD DXF 2010 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1027_Flat:
            return QString("AutoCAD DXF 2013 %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1006_AAMA:
            return QString("AutoCAD DXF R10 AAMA %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1009_AAMA:
            return QString("AutoCAD DXF R11/12 AAMA %1 %2").arg(dxfFlatFilesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1012_AAMA:
            return QString("AutoCAD DXF R13 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1014_AAMA:
            return QString("AutoCAD DXF R14 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1015_AAMA:
            return QString("AutoCAD DXF 2000 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1018_AAMA:
            return QString("AutoCAD DXF 2004 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1021_AAMA:
            return QString("AutoCAD DXF 2007 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1024_AAMA:
            return QString("AutoCAD DXF 2010 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1027_AAMA:
            return QString("AutoCAD DXF 2013 AAMA %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1006_ASTM:
            return QString("AutoCAD DXF R10 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1009_ASTM:
            return QString("AutoCAD DXF R11/12 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1012_ASTM:
            return QString("AutoCAD DXF R13 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1014_ASTM:
            return QString("AutoCAD DXF R14 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1015_ASTM:
            return QString("AutoCAD DXF 2000 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1018_ASTM:
            return QString("AutoCAD DXF 2004 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1021_ASTM:
            return QString("AutoCAD DXF 2007 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1024_ASTM:
            return QString("AutoCAD DXF 2010 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::DXF_AC1027_ASTM:
            return QString("AutoCAD DXF 2013 ASTM %1 %2").arg(filesStr, dxfSuffix);
        case LayoutExportFormats::PDFTiled:
            return QString("PDF tiled %1 (*.pdf)").arg(filesStr);
        default:
            return QString();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSaveLayout::ExportFromatSuffix(LayoutExportFormats format)
{
    switch(format)
    {
        case LayoutExportFormats::SVG:
            return ".svg";
        case LayoutExportFormats::PDF:
        case LayoutExportFormats::PDFTiled:
            return ".pdf";
        case LayoutExportFormats::PNG:
            return ".png";
        case LayoutExportFormats::OBJ:
            return ".obj";
        case LayoutExportFormats::PS:
            return ".ps";
        case LayoutExportFormats::EPS:
            return ".eps";
        case LayoutExportFormats::DXF_AC1006_Flat:
        case LayoutExportFormats::DXF_AC1009_Flat:
        case LayoutExportFormats::DXF_AC1012_Flat:
        case LayoutExportFormats::DXF_AC1014_Flat:
        case LayoutExportFormats::DXF_AC1015_Flat:
        case LayoutExportFormats::DXF_AC1018_Flat:
        case LayoutExportFormats::DXF_AC1021_Flat:
        case LayoutExportFormats::DXF_AC1024_Flat:
        case LayoutExportFormats::DXF_AC1027_Flat:
        case LayoutExportFormats::DXF_AC1006_AAMA:
        case LayoutExportFormats::DXF_AC1009_AAMA:
        case LayoutExportFormats::DXF_AC1012_AAMA:
        case LayoutExportFormats::DXF_AC1014_AAMA:
        case LayoutExportFormats::DXF_AC1015_AAMA:
        case LayoutExportFormats::DXF_AC1018_AAMA:
        case LayoutExportFormats::DXF_AC1021_AAMA:
        case LayoutExportFormats::DXF_AC1024_AAMA:
        case LayoutExportFormats::DXF_AC1027_AAMA:
        case LayoutExportFormats::DXF_AC1006_ASTM:
        case LayoutExportFormats::DXF_AC1009_ASTM:
        case LayoutExportFormats::DXF_AC1012_ASTM:
        case LayoutExportFormats::DXF_AC1014_ASTM:
        case LayoutExportFormats::DXF_AC1015_ASTM:
        case LayoutExportFormats::DXF_AC1018_ASTM:
        case LayoutExportFormats::DXF_AC1021_ASTM:
        case LayoutExportFormats::DXF_AC1024_ASTM:
        case LayoutExportFormats::DXF_AC1027_ASTM:
            return ".dxf";
        default:
            return QString();
    }
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
LayoutExportFormats DialogSaveLayout::Format() const
{
    return static_cast<LayoutExportFormats>(ui->comboBoxFormat->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSaveLayout::Save()
{
    for (int i=0; i < count; ++i)
    {
        const QString name = Path()+QLatin1Literal("/")+FileName()+QString::number(i+1)+ExportFromatSuffix(Format());
        if (QFile::exists(name))
        {
            QMessageBox::StandardButton res = QMessageBox::question(this, tr("Name conflict"),
                                  tr("Folder already contain file with name %1. Rewrite all conflict file names?")
                                  .arg(name), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
            if (res == QMessageBox::No)
            {
                reject();
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
void DialogSaveLayout::ShowExample()
{
    const LayoutExportFormats currentFormat = Format();
    ui->labelExample->setText(tr("Example:") + FileName() + QLatin1String("1") + ExportFromatSuffix(currentFormat));

    switch(currentFormat)
    {
        case LayoutExportFormats::DXF_AC1006_Flat:
        case LayoutExportFormats::DXF_AC1009_Flat:
        case LayoutExportFormats::DXF_AC1012_Flat:
        case LayoutExportFormats::DXF_AC1014_Flat:
        case LayoutExportFormats::DXF_AC1015_Flat:
        case LayoutExportFormats::DXF_AC1018_Flat:
        case LayoutExportFormats::DXF_AC1021_Flat:
        case LayoutExportFormats::DXF_AC1024_Flat:
        case LayoutExportFormats::DXF_AC1027_Flat:
        case LayoutExportFormats::DXF_AC1006_AAMA:
        case LayoutExportFormats::DXF_AC1009_AAMA:
        case LayoutExportFormats::DXF_AC1012_AAMA:
        case LayoutExportFormats::DXF_AC1014_AAMA:
        case LayoutExportFormats::DXF_AC1015_AAMA:
        case LayoutExportFormats::DXF_AC1018_AAMA:
        case LayoutExportFormats::DXF_AC1021_AAMA:
        case LayoutExportFormats::DXF_AC1024_AAMA:
        case LayoutExportFormats::DXF_AC1027_AAMA:
        case LayoutExportFormats::DXF_AC1006_ASTM:
        case LayoutExportFormats::DXF_AC1009_ASTM:
        case LayoutExportFormats::DXF_AC1012_ASTM:
        case LayoutExportFormats::DXF_AC1014_ASTM:
        case LayoutExportFormats::DXF_AC1015_ASTM:
        case LayoutExportFormats::DXF_AC1018_ASTM:
        case LayoutExportFormats::DXF_AC1021_ASTM:
        case LayoutExportFormats::DXF_AC1024_ASTM:
        case LayoutExportFormats::DXF_AC1027_ASTM:
            ui->checkBoxBinaryDXF->setEnabled(true);
            break;
        case LayoutExportFormats::SVG:
        case LayoutExportFormats::PDF:
        case LayoutExportFormats::PDFTiled:
        case LayoutExportFormats::PNG:
        case LayoutExportFormats::OBJ:
        case LayoutExportFormats::PS:
        case LayoutExportFormats::EPS:
        default:
            ui->checkBoxBinaryDXF->setEnabled(false);
            break;
    }
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
QVector<std::pair<QString, LayoutExportFormats> > DialogSaveLayout::InitFormats()
{
    QVector<std::pair<QString, LayoutExportFormats>> list;

    auto InitFormat = [&list](LayoutExportFormats format)
    {
        list.append(std::make_pair(ExportFormatDescription(format), format));
    };

    InitFormat(LayoutExportFormats::SVG);
    InitFormat(LayoutExportFormats::PDF);
    InitFormat(LayoutExportFormats::PNG);
    InitFormat(LayoutExportFormats::OBJ);
    if (SupportPSTest())
    {
        InitFormat(LayoutExportFormats::PS);
        InitFormat(LayoutExportFormats::EPS);
    }
    InitFormat(LayoutExportFormats::DXF_AC1006_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1009_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1012_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1014_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1015_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1018_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1021_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1024_Flat);
    InitFormat(LayoutExportFormats::DXF_AC1027_Flat);

    // We will support them anyway
//    InitFormat(LayoutExportFormats::DXF_AC1006_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1009_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1012_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1014_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1015_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1018_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1021_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1024_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1027_AAMA);
//    InitFormat(LayoutExportFormats::DXF_AC1006_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1009_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1012_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1014_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1015_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1018_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1021_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1024_ASTM);
//    InitFormat(LayoutExportFormats::DXF_AC1027_ASTM);
    InitFormat(LayoutExportFormats::PDFTiled); 

    return list;
}

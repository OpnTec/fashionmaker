/************************************************************************
 **
 **  @file
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   19 7, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vabstractmainwindow.h"
#include "../vpropertyexplorer/checkablemessagebox.h"
#include "../vmisc/vabstractapplication.h"
#include "dialogs/dialogexporttocsv.h"

#include <QStyle>
#include <QToolBar>
#include <QFileDialog>
#include <QAction>

#if defined(Q_OS_MAC)
#include "../vwidgets/vmaingraphicsview.h"
#include <QStyleFactory>
#endif

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QStringList SplitFilePaths(const QString &path)
{
    QStringList result;
    QString subPath = QDir::cleanPath(path);
    QString lastFileName;

    do
    {
        QFileInfo fileInfo(subPath);
        lastFileName = fileInfo.fileName();
        if (not lastFileName.isEmpty())
        {
            result.prepend(lastFileName);
            subPath = fileInfo.path();
        }
    }
    while(not lastFileName.isEmpty());

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList ReverseList(const QStringList &list)
{
    if (list.isEmpty())
    {
        return list;
    }
    QVector<QString> reversedList(list.size());
    qint32 j = 0;
    for (qint32 i = list.size() - 1; i >= 0; --i)
    {
        reversedList.replace(j, list.at(i));
        ++j;
    }
    return reversedList.toList();
}

//---------------------------------------------------------------------------------------------------------------------
QStringList SelectNumber(QStringList path, int number)
{
    path = ReverseList(path);
    QStringList subPath = path.mid(0, number);
    return ReverseList(subPath);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList RecentFiles(const QStringList &paths)
{
    QVector<QStringList> table;
    table.reserve(paths.size());

    for(auto &path : paths)
    {
        table.append(SplitFilePaths(path));
    }

    auto CreateOptimized = [table](int tableRow)
    {
        QStringList optimized;
        QStringList path = table.at(tableRow);
        for (int count = 1; count <= path.size(); ++count)
        {
            bool isUnique = true;
            optimized = SelectNumber(path, count);

            for (int row = 0; row < table.size(); ++row)
            {
                if (row == tableRow)
                {
                    continue;
                }

                QStringList testSubPath = SelectNumber(table.at(row), count);
                if (optimized.join(QDir::separator()) == testSubPath.join(QDir::separator()))
                {
                    isUnique = false;
                    break;
                }
            }

            if (isUnique)
            {
                break;
            }
        }

        if (optimized.size() >= 3)
        {
            optimized = QStringList({optimized.first(), QStringLiteral("…"), optimized.last()});
        }

        return optimized;
    };

    QVector<QStringList> optimizedPaths;
    optimizedPaths.reserve(paths.size());

    for (int row = 0; row < table.size(); ++row)
    {
        optimizedPaths.append(CreateOptimized(row));
    }

    QStringList recentFiles;
    recentFiles.reserve(paths.size());

    for(auto &path : optimizedPaths)
    {
        recentFiles.append(path.join(QDir::separator()));
    }

    return recentFiles;
}
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractMainWindow::VAbstractMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_curFileFormatVersion(0x0),
      m_curFileFormatVersionStr(QLatin1String("0.0.0"))
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        m_recentFileActs[i] = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractMainWindow::ContinueFormatRewrite(const QString &currentFormatVersion,
                                                const QString &maxFormatVersion)
{
    if (qApp->Settings()->GetConfirmFormatRewriting())
    {
        Utils::CheckableMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Confirm format rewriting"));
        msgBox.setText(tr("This file is using previous format version v%1. The current is v%2. "
                          "Saving the file with this app version will update the format version for this "
                          "file. This may prevent you from be able to open the file with older app versions. "
                          "Do you really want to continue?").arg(currentFormatVersion, maxFormatVersion));
        msgBox.setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
        msgBox.setDefaultButton(QDialogButtonBox::No);
        msgBox.setIconPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32, 32));

        int dialogResult = msgBox.exec();

        if (dialogResult == QDialog::Accepted)
        {
            qApp->Settings()->SetConfirmFormatRewriting(not msgBox.isChecked());
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::ToolBarStyle(QToolBar *bar)
{
    SCASSERT(bar != nullptr)
    if (qApp->Settings()->GetToolBarStyle())
    {
        bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else
    {
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

#if defined(Q_OS_MAC)
    // Temporary fix issue with toolbar black background on mac with OpenGL render
    if (qApp->getSceneView() && qApp->getSceneView()->IsOpenGLRender())
    {
        bar->setStyle(QStyleFactory::create("fusion"));
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractMainWindow::CSVFilePath()
{
    const QString filters = tr("Comma-Separated Values") + QLatin1String(" (*.csv)");
    const QString suffix("csv");
    const QString path = QDir::homePath() + QChar('/') + tr("values") + QChar('.') + suffix;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export to CSV"), path, filters, nullptr
#ifdef Q_OS_LINUX
                                                    , QFileDialog::DontUseNativeDialog
#endif
                                                    );

    if (fileName.isEmpty())
    {
        return fileName;
    }

    QFileInfo f( fileName );
    if (f.suffix().isEmpty() && f.suffix() != suffix)
    {
        fileName += QChar('.') + suffix;
    }

    return fileName;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::UpdateRecentFileActions()
{
    const QStringList files = RecentFileList();
    const QStringList recentFiles = RecentFiles(files);
    const int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString recent = recentFiles.at(i);
        if (not recent.isEmpty())
        {
            const QString text = QStringLiteral("&%1. %2").arg(i + 1).arg(recentFiles.at(i));
            m_recentFileActs.at(i)->setText(text);
            m_recentFileActs.at(i)->setData(files.at(i));
            m_recentFileActs.at(i)->setVisible(true);
        }
    }

    m_separatorAct->setVisible(numRecentFiles>0);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::WindowsLocale()
{
    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::ExportDataToCSV()
{
    QString fileName = CSVFilePath();

    if (fileName.isEmpty())
    {
        return;
    }

    DialogExportToCSV dialog(this);
    dialog.SetWithHeader(qApp->Settings()->GetCSVWithHeader());
    dialog.SetSelectedMib(qApp->Settings()->GetCSVCodec());
    dialog.SetSeparator(qApp->Settings()->GetCSVSeparator());

    if (dialog.exec() == QDialog::Accepted)
    {
        ExportToCSVData(fileName, dialog.IsWithHeader(), dialog.GetSelectedMib(), dialog.GetSeparator());

        qApp->Settings()->SetCSVSeparator(dialog.GetSeparator());
        qApp->Settings()->SetCSVCodec(dialog.GetSelectedMib());
        qApp->Settings()->SetCSVWithHeader(dialog.IsWithHeader());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::UpdateVisibilityGroups()
{
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::UpdateDetailsList()
{
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractMainWindow::ZoomFitBestCurrent()
{
    // do nothing
}

/************************************************************************
 **
 **  @file
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   19 7, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VABSTRACTMAINWINDOW_H
#define VABSTRACTMAINWINDOW_H

#include <qcompilerdetection.h>
#include <QMainWindow>
#include <QMetaObject>
#include <QObject>
#include <QString>

class DialogExportToCSV;

class VAbstractMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VAbstractMainWindow(QWidget *parent = nullptr);
    virtual ~VAbstractMainWindow() Q_DECL_EQ_DEFAULT;

public slots:
    virtual void ShowToolTip(const QString &toolTip)=0;

protected slots:
    void WindowsLocale();
    void ExportToCSV();

protected:
    int     m_curFileFormatVersion;
    QString m_curFileFormatVersionStr;

    bool ContinueFormatRewrite(const QString &currentFormatVersion, const QString &maxFormatVersion);
    void ToolBarStyle(QToolBar *bar);

    virtual void ExportToCSVData(const QString &fileName, const DialogExportToCSV &dialog)=0;
private:
    Q_DISABLE_COPY(VAbstractMainWindow)
};

#endif // VABSTRACTMAINWINDOW_H

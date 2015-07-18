/************************************************************************
 **
 **  @file   tmainwindow.h
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

#ifndef TMAINWINDOW_H
#define TMAINWINDOW_H

#include <QMainWindow>

#include "../vmisc/def.h"
#include "../vformat/vmeasurements.h"

namespace Ui
{
    class TMainWindow;
}

class TMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TMainWindow(QWidget *parent = 0);
    virtual ~TMainWindow() Q_DECL_OVERRIDE;

public slots:
    void LoadFile(const QString &path);
    void FileNew();
    void FileOpen();

protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FileSave();
    void FileSaveAs();
    void AboutToShowWindowMenu();
    void ShowWindow();
    void AboutApplication();
    void SaveGivenName();
    void SaveFamilyName();
    void SaveEmail();
    void SaveSex(int index);
    void SaveBirthDate(const QDate & date);
    void SaveNotes();
    void ReadOnly(bool ro);

private:
    Q_DISABLE_COPY(TMainWindow)
    Ui::TMainWindow *ui;
    VMeasurements   *m;
    VContainer      *data;
    Unit             mUnit;
    MeasurementsType mType;
    QString          curFile;

    void SetupMenu();
    void InitWindow();
    void InitTable();

    void MeasurementsWasSaved(bool saved);
    void SetCurrentFile(const QString &fileName);
    bool SaveMeasurements(const QString &fileName, QString &error);

    bool MaybeSave();
};

#endif // TMAINWINDOW_H

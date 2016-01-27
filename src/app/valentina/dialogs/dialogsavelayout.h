/************************************************************************
 **
 **  @file   dialogsavelayout.h
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

#ifndef DIALOGSAVELAYOUT_H
#define DIALOGSAVELAYOUT_H

#include <QDialog>

#ifdef Q_OS_WIN
#   define PDFTOPS "pdftops.exe"
#else
#   define PDFTOPS "pdftops"
#endif

namespace Ui
{
    class DialogSaveLAyout;
}

class DialogSaveLayout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveLayout(int count, const QString &fileName = QString(),
                     QWidget *parent = 0);
    ~DialogSaveLayout();

    QString Path() const;
    QString FileName() const;
    QString Formate() const;
    void    SelectFormate(const int formate);
    static QString MakeHelpFormatList();
    void   SetDestinationPath(const QString& cmdDestinationPath);
public slots:
    void Save();
    void ShowExample();
    void Browse();
    void PathChanged(const QString &text);
protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogSaveLayout)
    Ui::DialogSaveLAyout *ui;
    int count;
    bool isInitialized;
    // Note. We can't make availFormats static because MSVC doesn't support C++11 list initialization
    QVector<std::pair<QString, QString>> availFormats;
    static bool havePdf;
    static bool tested;
    static bool SupportPSTest();
    static bool TestPdf();
    static QVector<std::pair<QString, QString> > InitAvailFormats();
};

#endif // DIALOGSAVELAYOUT_H

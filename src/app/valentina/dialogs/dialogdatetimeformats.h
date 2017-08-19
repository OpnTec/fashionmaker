/************************************************************************
 **
 **  @file   dialogdatetimeformats.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef DIALOGDATETIMEFORMATS_H
#define DIALOGDATETIMEFORMATS_H

#include <QDialog>
#include <QDate>
#include <QTime>

namespace Ui
{
    class DialogDateTimeFormats;
}

class QListWidgetItem;

class DialogDateTimeFormats : public QDialog
{
    Q_OBJECT

public:
    DialogDateTimeFormats(const QDate &date, const QStringList &predefinedFormats,
                          const QStringList &userDefinedFormats, QWidget *parent = nullptr);
    DialogDateTimeFormats(const QTime &time, const QStringList &predefinedFormats,
                          const QStringList &userDefinedFormats, QWidget *parent = nullptr);
    virtual ~DialogDateTimeFormats();

    QStringList GetFormats() const;

private slots:
    void AddLine();
    void RemoveLine();
    void SaveFormat(const QString &text);
    void ShowFormatDetails();

private:
    Q_DISABLE_COPY(DialogDateTimeFormats)
    Ui::DialogDateTimeFormats *ui;
    bool  m_dateMode;
    QDate m_date;
    QTime m_time;
    QStringList m_predefined;

    void Init(const QStringList &predefined, const QStringList &userDefined);
    void SetFormatLines(const QStringList &predefined, const QStringList &userDefined);
    void SetupControls();

    bool IsPredefined() const;

    QListWidgetItem *AddListLine(const QString &format);
};

#endif // DIALOGDATETIMEFORMATS_H

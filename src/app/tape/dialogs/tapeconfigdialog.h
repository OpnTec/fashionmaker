/************************************************************************
 **
 **  @file   tapeconfigdialog.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   02 08, 2015
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "configpages/tapeconfigurationpage.h"
#include "configpages/tapepathpage.h"

class QListWidgetItem;
class QStackedWidget;
class QListWidget;

class TapeConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TapeConfigDialog(QWidget *parent = nullptr);
public slots:
    void              changePage(QListWidgetItem *current, QListWidgetItem *previous);
signals:
    void              UpdateProperties();
protected:
    virtual void      closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(TapeConfigDialog)
    QListWidget           *contentsWidget;
    QStackedWidget        *pagesWidget;
    TapeConfigurationPage *configurationPage;
    TapePathPage          *pathPage;

    void createIcons();
    void createIcon(const QString &icon, const QString &text);
    void Apply();
    void Ok();
};

#endif // CONFIGDIALOG_H

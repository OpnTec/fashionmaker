/************************************************************************
 **
 **  @file   configdialog.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 2, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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
#include <QListWidget>
#include "pages.h"

class QListWidgetItem;
class QStackedWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = nullptr);
public slots:
    void              changePage(QListWidgetItem *current, QListWidgetItem *previous);
protected:
    virtual void      closeEvent(QCloseEvent *event);
private:
    Q_DISABLE_COPY(ConfigDialog)
    QListWidget       *contentsWidget;
    QStackedWidget    *pagesWidget;
    ConfigurationPage *configurationPage;
    PatternPage       *patternPage;
    void              createIcons();
    void              Apply();
    void              Ok();
};

#endif // CONFIGDIALOG_H

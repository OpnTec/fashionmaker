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
#include "configpages/pages.h"

class QListWidgetItem;
class QStackedWidget;
class QListWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = nullptr);
public slots:
    void              changePage(QListWidgetItem *current, QListWidgetItem *previous);
signals:
    void              UpdateProperties();
protected:
    virtual void      closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void      changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void      showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(ConfigDialog)
    QListWidget       *contentsWidget;
    QStackedWidget    *pagesWidget;
    ConfigurationPage *configurationPage;
    PatternPage       *patternPage;
    CommunityPage     *communityPage;
    PathPage          *pathPage;
    QPushButton       *applyButton;
    QPushButton       *cancelButton;
    QPushButton       *okButton;
    bool              isInitialized;
    void              createIcons();
    void              createIcon(const QString &icon, const QString &text);
    void              Apply();
    void              Ok();

    void              RetranslateUi();
};

#endif // CONFIGDIALOG_H

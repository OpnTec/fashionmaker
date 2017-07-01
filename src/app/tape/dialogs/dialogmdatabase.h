/************************************************************************
 **
 **  @file   dialogmdatabase.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 7, 2015
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

#ifndef DIALOGMDATABASE_H
#define DIALOGMDATABASE_H

#include <QDialog>

namespace Ui
{
    class DialogMDataBase;
}

class QTreeWidgetItem;

class DialogMDataBase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMDataBase(const QStringList &list, QWidget *parent = 0);
    explicit DialogMDataBase(QWidget *parent = 0);
    virtual ~DialogMDataBase() Q_DECL_OVERRIDE;

    QStringList GetNewNames() const;

    void RetranslateGroups();

    static QString ImgTag(const QString &number);

protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *target, QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void UpdateChecks(QTreeWidgetItem *item, int column);
    void ShowDescription(QTreeWidgetItem *item, int column);
    void TreeMenu(const QPoint &pos);
    void Recheck();

private:
    Q_DISABLE_COPY(DialogMDataBase)
    Ui::DialogMDataBase *ui;
    bool selectMode;
    QStringList list;

    QTreeWidgetItem *groupA;
    QTreeWidgetItem *groupB;
    QTreeWidgetItem *groupC;
    QTreeWidgetItem *groupD;
    QTreeWidgetItem *groupE;
    QTreeWidgetItem *groupF;
    QTreeWidgetItem *groupG;
    QTreeWidgetItem *groupH;
    QTreeWidgetItem *groupI;
    QTreeWidgetItem *groupJ;
    QTreeWidgetItem *groupK;
    QTreeWidgetItem *groupL;
    QTreeWidgetItem *groupM;
    QTreeWidgetItem *groupN;
    QTreeWidgetItem *groupO;
    QTreeWidgetItem *groupP;
    QTreeWidgetItem *groupQ;

    void InitDataBase(const QStringList &list = QStringList());
    void InitGroup(QTreeWidgetItem **group, const QString &groupName, const QStringList &mList,
                   const QStringList &list = QStringList());

    Q_REQUIRED_RESULT QTreeWidgetItem *AddGroup(const QString &text);

    void AddMeasurement(QTreeWidgetItem *group, const QString &name, const QStringList &list);

    void ReadSettings();
    void WriteSettings();

    void RetranslateGroup(QTreeWidgetItem *group, const QString &groupText, const QStringList &list);
    void RetranslateMeasurement(QTreeWidgetItem *group, int index, const QString &name);

    void ChangeCheckState(QTreeWidgetItem *group, Qt::CheckState check);
    Qt::CheckState GlobalCheckState() const;
};

#endif // DIALOGMDATABASE_H

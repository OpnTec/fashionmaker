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
    DialogMDataBase(const QStringList &list, QWidget *parent = 0);
    explicit DialogMDataBase(QWidget *parent = 0);
    ~DialogMDataBase();

    QStringList GetNewNames() const;

private slots:
    void UpdateChecks(QTreeWidgetItem *item, int column);
    void ShowDescription(QTreeWidgetItem *item, int column);

private:
    Q_DISABLE_COPY(DialogMDataBase)
    Ui::DialogMDataBase *ui;
    bool selectMode;
    QStringList list;

    void InitDataBase(const QStringList &list);
    void InitDataBase();

    void InitGroupA(const QStringList &list = QStringList());
    void InitGroupB(const QStringList &list = QStringList());
    void InitGroupC(const QStringList &list = QStringList());
    void InitGroupD(const QStringList &list = QStringList());
    void InitGroupE(const QStringList &list = QStringList());
    void InitGroupF(const QStringList &list = QStringList());
    void InitGroupG(const QStringList &list = QStringList());
    void InitGroupH(const QStringList &list = QStringList());
    void InitGroupI(const QStringList &list = QStringList());
    void InitGroupJ(const QStringList &list = QStringList());
    void InitGroupK(const QStringList &list = QStringList());
    void InitGroupL(const QStringList &list = QStringList());
    void InitGroupM(const QStringList &list = QStringList());
    void InitGroupN(const QStringList &list = QStringList());
    void InitGroupO(const QStringList &list = QStringList());
    void InitGroupP(const QStringList &list = QStringList());
    void InitGroupQ(const QStringList &list = QStringList());

    void AddMeasurement(QTreeWidgetItem *group, const QString &name, const QStringList &list);

    QString MapDiagrams(const QString &number) const;
};

#endif // DIALOGMDATABASE_H

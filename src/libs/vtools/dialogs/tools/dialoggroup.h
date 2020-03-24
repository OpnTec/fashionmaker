/************************************************************************
 **
 **  @file   dialoggroup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef DIALOGGROUP_H
#define DIALOGGROUP_H

#include <qcompilerdetection.h>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "dialogtool.h"

namespace Ui
{
    class DialogGroup;
}

class DialogGroup : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogGroup(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    ~DialogGroup();

    void    SetName(const QString &name);
    QString GetName() const;

    void        SetTags(const QStringList &tags);
    QStringList GetTags() const;

    void SetGroupCategories(const QStringList &categories);

    QMap<quint32, quint32> GetGroup() const;

    virtual void ShowDialog(bool click) override;

public slots:
    virtual void SelectedObject(bool selected, quint32 object, quint32 tool) override;

protected:
    virtual bool IsValid() const final;

private slots:
    void NameChanged();

private:
    Q_DISABLE_COPY(DialogGroup)
    Ui::DialogGroup *ui;
    QMap<quint32, quint32> group;
    bool flagName;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogGroup::IsValid() const
{
    return flagName;
}

#endif // DIALOGGROUP_H

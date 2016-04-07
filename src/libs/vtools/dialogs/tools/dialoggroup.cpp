/************************************************************************
 **
 **  @file   dialoggroup.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 4, 2016
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

#include "dialoggroup.h"
#include "ui_dialoggroup.h"
#include "../vtools/tools/drawTools/drawtools.h"

//---------------------------------------------------------------------------------------------------------------------
DialogGroup::DialogGroup(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogGroup),
      group()
{
    ui->setupUi(this);
    InitOkCancel(ui);
    CheckState();

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogGroup::NameChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogGroup::~DialogGroup()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogGroup::SetName(const QString &name)
{
    ui->lineEditName->setText(name);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogGroup::GetName() const
{
    return ui->lineEditName->text();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogGroup::ShowDialog(bool click)
{
    if (not click)
    {
        if (group.isEmpty())
        {
            return;
        }

        SetName(tr("New group"));

        setModal(true);
        emit ToolTip("");
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogGroup::SelectedObject(bool selected, quint32 object, quint32 tool)
{
    if (selected)
    {
        group.insert(object, tool);
    }
    else
    {
        group.remove(object);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogGroup::NameChanged()
{
    ui->lineEditName->text().isEmpty() ? flagName = false : flagName = true;
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
QMap<quint32, quint32> DialogGroup::GetGroup() const
{
    return group;
}

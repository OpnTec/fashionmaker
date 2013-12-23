/************************************************************************
 **
 **  @file   dialoguniondetails.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   23 12, 2013
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

#include "dialoguniondetails.h"
#include "ui_dialoguniondetails.h"

DialogUnionDetails::DialogUnionDetails(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogUnionDetails), details(VDetail()), d1(0), d2(0), d1P1(0), d1P2(0),
    d2P1(0), d2P2(0), numberD(0), numberP(0)
{
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogUnionDetails::DialogAccepted);
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogUnionDetails::DialogRejected);
}

DialogUnionDetails::~DialogUnionDetails()
{
    delete ui;
}

void DialogUnionDetails::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if(numberD == 0)
    {
        ChoosedDetail(id, type, d1, d1P1, d1P2);
    }
    else
    {
        ChoosedDetail(id, type, d2, d2P1, d2P2);
    }
}

void DialogUnionDetails::DialogAccepted()
{
    emit ToolTip("");
    emit DialogClosed(QDialog::Accepted);
}

bool DialogUnionDetails::CheckObject(const qint64 &id, const qint64 &idDetail) const
{
    if (idDetail == 0)
    {
        return false;
    }
    VDetail det = data->GetDetail(idDetail);
    return det.Containes(id);
}

void DialogUnionDetails::ChoosedDetail(const qint64 &id, const Scene::Scenes &type, qint64 &idDetail, qint64 &p1,
                                       qint64 &p2)
{
    if (idDetail == 0)
    {
        if (type == Scene::Detail)
        {
            idDetail = id;
            emit ToolTip(tr("Select first point"));
            return;
        }
    }
    if (CheckObject(id, idDetail) == false)
    {
        return;
    }
    if (type == Scene::Point)
    {
        if (numberP == 0)
        {
            p1 = id;
            ++numberP;
            emit ToolTip(tr("Select second point"));
        }
        if (numberP == 1)
        {
            p2 = id;
            ++numberD;
            if(numberD > 1)
            {
                ++numberP;
                emit ToolTip("");
                this->show();
            }
            else
            {
                numberP = 0;
                emit ToolTip(tr("Select detail"));
            }
        }
    }
}

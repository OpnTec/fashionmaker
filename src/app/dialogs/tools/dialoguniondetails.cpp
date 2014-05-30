/************************************************************************
 **
 **  @file   dialoguniondetails.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

//---------------------------------------------------------------------------------------------------------------------
DialogUnionDetails::DialogUnionDetails(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogUnionDetails), indexD1(0), indexD2(0), d1(0), d2(0),  numberD(0),
    numberP(0), p1(0), p2(0)
{
    ui->setupUi(this);
    InitOkCancelApply(ui);
}

//---------------------------------------------------------------------------------------------------------------------
DialogUnionDetails::~DialogUnionDetails()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogUnionDetails::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (numberD == 0)
    {
        ChoosedDetail(id, type, d1, indexD1);
    }
    else
    {
        ChoosedDetail(id, type, d2, indexD2);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogUnionDetails::DialogAccepted()
{
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogUnionDetails::CheckObject(const quint32 &id, const quint32 &idDetail) const
{
    if (idDetail == 0)
    {
        return false;
    }
    VDetail det = data->GetDetail(idDetail);
    return det.Containes(id);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogUnionDetails::ChoosedDetail(const quint32 &id, const Valentina::Scenes &type, quint32 &idDetail,
                                       ptrdiff_t &index)
{
    if (idDetail == 0)
    {
        if (type == Valentina::Detail)
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
    if (type == Valentina::Point)
    {
        if (numberP == 0)
        {
            p1 = id;
            ++numberP;
            emit ToolTip(tr("Select second point"));
            return;
        }
        if (numberP == 1)
        {
            if (id == p1)
            {
                emit ToolTip(tr("Select another second point"));
                return;
            }
            VDetail d = data->GetDetail(idDetail);
            if (d.OnEdge(p1, id))
            {
                p2 = id;
                index = d.Edge(p1, p2);
                ++numberD;
                if (numberD > 1)
                {
                    ++numberP;
                    emit ToolTip("");
                    this->show();
                    return;
                }
                else
                {
                    numberP = 0;
                    p1 = 0;
                    p2 = 0;
                    emit ToolTip(tr("Select detail"));
                    return;
                }
            }
            else
            {
                emit ToolTip(tr("Select another second point"));
                return;
            }
        }
    }
}

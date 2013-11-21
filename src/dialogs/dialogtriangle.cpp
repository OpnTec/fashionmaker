/************************************************************************
 **
 **  @file   dialogtriangle.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#include "dialogtriangle.h"
#include "ui_dialogtriangle.h"

#include <QPushButton>

DialogTriangle::DialogTriangle(const VContainer *data, Draw::Draws mode, QWidget *parent)
    :DialogTool(data, mode, parent), ui(new Ui::DialogTriangle), number(0), pointName(QString()), axisP1Id(0),
    axisP2Id(0), firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogTriangle::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogTriangle::DialogRejected);
    FillComboBoxPoints(ui->comboBoxAxisP1);
    FillComboBoxPoints(ui->comboBoxAxisP2);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogTriangle::NamePointChanged);
}

DialogTriangle::~DialogTriangle()
{
    delete ui;
}

void DialogTriangle::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (idDetail == 0 && mode == Draw::Modeling)
    {
        if (type == Scene::Detail)
        {
            idDetail = id;
            return;
        }
    }
    if (mode == Draw::Modeling)
    {
        if (CheckObject(id) == false)
        {
            return;
        }
    }
    if (type == Scene::Point)
    {
        VPointF point;
        if (mode == Draw::Calculation)
        {
            point = data->GetPoint(id);
        }
        else
        {
            point = data->GetPointModeling(id);
        }
        switch (number)
        {
            case (0):
                ChangeCurrentText(ui->comboBoxAxisP1, point.name());
                number++;
                emit ToolTip(tr("Select second point of axis"));
                break;
            case (1):
                ChangeCurrentText(ui->comboBoxAxisP2, point.name());
                number++;
                emit ToolTip(tr("Select first point"));
                break;
            case (2):
                ChangeCurrentText(ui->comboBoxFirstPoint, point.name());
                number++;
                emit ToolTip(tr("Select second point"));
                break;
            case (3):
                ChangeCurrentText(ui->comboBoxSecondPoint, point.name());
                number = 0;
                emit ToolTip(tr(""));
                if (isInitialized == false)
                {
                    this->show();
                }
                break;
            default:
                break;
        }
    }
}

void DialogTriangle::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    firstPointId = getCurrentPointId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentPointId(ui->comboBoxSecondPoint);
    axisP1Id = getCurrentPointId(ui->comboBoxAxisP1);
    axisP2Id = getCurrentPointId(ui->comboBoxAxisP2);
    emit DialogClosed(QDialog::Accepted);
}

void DialogTriangle::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

void DialogTriangle::setSecondPointId(const qint64 &value, const qint64 &id)
{
    secondPointId = value;
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

void DialogTriangle::setFirstPointId(const qint64 &value, const qint64 &id)
{
    firstPointId = value;
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

void DialogTriangle::setAxisP2Id(const qint64 &value, const qint64 &id)
{
    axisP2Id = value;
    setCurrentPointId(ui->comboBoxAxisP2, axisP2Id, value, id);
}

void DialogTriangle::setAxisP1Id(const qint64 &value, const qint64 &id)
{
    axisP1Id = value;
    setCurrentPointId(ui->comboBoxAxisP1, axisP1Id, value, id);
}

/************************************************************************
 **
 **  @file   dialogheight.cpp
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

#include "dialogheight.h"
#include "ui_dialogheight.h"

#include <QPushButton>

DialogHeight::DialogHeight(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogHeight), number(0), pointName(QString()),
    typeLine(QString()), basePointId(0), p1LineId(0), p2LineId(0)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogHeight::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogHeight::DialogRejected);

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogHeight::NamePointChanged);
}

DialogHeight::~DialogHeight()
{
    delete ui;
}

void DialogHeight::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

void DialogHeight::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

void DialogHeight::setBasePointId(const qint64 &value, const qint64 &id)
{
    basePointId = value;
    setCurrentPointId(ui->comboBoxBasePoint, basePointId, value, id);
}

void DialogHeight::setP1LineId(const qint64 &value, const qint64 &id)
{
    p1LineId = value;
    setCurrentPointId(ui->comboBoxP1Line, p1LineId, value, id);
}

void DialogHeight::setP2LineId(const qint64 &value, const qint64 &id)
{
    p2LineId = value;
    setCurrentPointId(ui->comboBoxP2Line, p2LineId, value, id);
}

void DialogHeight::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (type == Scene::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        switch (number)
        {
            case (0):
                ChangeCurrentText(ui->comboBoxBasePoint, point->name());
                number++;
                emit ToolTip(tr("Select first point of line"));
                break;
            case (1):
                ChangeCurrentText(ui->comboBoxP1Line, point->name());
                number++;
                emit ToolTip(tr("Select second point of line"));
                break;
            case (2):
                ChangeCurrentText(ui->comboBoxP2Line, point->name());
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

void DialogHeight::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    basePointId = getCurrentObjectId(ui->comboBoxBasePoint);
    p1LineId = getCurrentObjectId(ui->comboBoxP1Line);
    p2LineId = getCurrentObjectId(ui->comboBoxP2Line);
    emit DialogClosed(QDialog::Accepted);
}

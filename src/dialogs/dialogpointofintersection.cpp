/************************************************************************
 **
 **  @file   dialogpointofintersection.cpp
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

#include "dialogpointofintersection.h"
#include "ui_dialogpointofintersection.h"

#include <QPushButton>

DialogPointOfIntersection::DialogPointOfIntersection(const VContainer *data, Draw::Draws mode, QWidget *parent)
    :DialogTool(data, mode, parent), ui(new Ui::DialogPointOfIntersection), number(0), pointName(QString()),
    firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogPointOfIntersection::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogPointOfIntersection::DialogRejected);
    if(mode == Draw::Calculation)
    {
        FillComboBoxPoints(ui->comboBoxFirstPoint);
        FillComboBoxPoints(ui->comboBoxSecondPoint);
    }
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersection::NamePointChanged);
}

DialogPointOfIntersection::~DialogPointOfIntersection()
{
    delete ui;
}

void DialogPointOfIntersection::setSecondPointId(const qint64 &value, const qint64 &id)
{
    secondPointId = value;
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

void DialogPointOfIntersection::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (idDetail == 0 && mode == Draw::Modeling)
    {
        if (type == Scene::Detail)
        {
            idDetail = id;
            FillComboBoxPoints(ui->comboBoxFirstPoint);
            FillComboBoxPoints(ui->comboBoxSecondPoint);
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
        if (number == 0)
        {
            qint32 index = ui->comboBoxFirstPoint->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select point horizontally"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxSecondPoint->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
            }
            if (isInitialized == false)
            {
                this->show();
            }
        }
    }
}

void DialogPointOfIntersection::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

void DialogPointOfIntersection::setFirstPointId(const qint64 &value, const qint64 &id)
{
    firstPointId = value;
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

void DialogPointOfIntersection::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

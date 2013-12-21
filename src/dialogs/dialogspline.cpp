/************************************************************************
 **
 **  @file   dialogspline.cpp
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

#include "dialogspline.h"
#include "ui_dialogspline.h"

#include <QPushButton>

DialogSpline::DialogSpline(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogSpline), number(0), p1(0), p4(0), angle1(0), angle2(0),
    kAsm1(1), kAsm2(1), kCurve(1)
{
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogSpline::DialogAccepted);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSpline::DialogRejected);

    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP4);
}

DialogSpline::~DialogSpline()
{
    delete ui;
}

void DialogSpline::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (type == Scene::Point)
    {
        VPointF point = data->GetPoint(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxP1->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP1->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select last point of curve"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxP4->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP4->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
                index = ui->comboBoxP1->currentIndex();
                qint64 p1Id = qvariant_cast<qint64>(ui->comboBoxP1->itemData(index));

                QPointF p1 = data->GetPoint(p1Id).toQPointF();
                QPointF p4 = data->GetPoint(id).toQPointF();

                ui->spinBoxAngle1->setValue(static_cast<qint32>(QLineF(p1, p4).angle()));
                ui->spinBoxAngle2->setValue(static_cast<qint32>(QLineF(p4, p1).angle()));
            }
            if (isInitialized == false)
            {
                this->show();
            }
        }
    }
}

void DialogSpline::DialogAccepted()
{
    p1 = getCurrentObjectId(ui->comboBoxP1);
    p4 = getCurrentObjectId(ui->comboBoxP4);
    angle1 = ui->spinBoxAngle1->value();
    angle2 = ui->spinBoxAngle2->value();
    kAsm1 = ui->doubleSpinBoxKasm1->value();
    kAsm2 = ui->doubleSpinBoxKasm2->value();
    kCurve = ui->doubleSpinBoxKcurve->value();
    emit DialogClosed(QDialog::Accepted);
}

void DialogSpline::setKCurve(const qreal &value)
{
    kCurve = value;
    ui->doubleSpinBoxKcurve->setValue(value);
}

void DialogSpline::setKAsm2(const qreal &value)
{
    kAsm2 = value;
    ui->doubleSpinBoxKasm2->setValue(value);
}

void DialogSpline::setKAsm1(const qreal &value)
{
    kAsm1 = value;
    ui->doubleSpinBoxKasm1->setValue(value);
}

void DialogSpline::setAngle2(const qreal &value)
{
    angle2 = value;
    ui->spinBoxAngle2->setValue(static_cast<qint32>(value));
}

void DialogSpline::setAngle1(const qreal &value)
{
    angle1 = value;
    ui->spinBoxAngle1->setValue(static_cast<qint32>(value));
}

void DialogSpline::setP4(const qint64 &value)
{
    p4 = value;
    ChangeCurrentData(ui->comboBoxP4, value);
}

void DialogSpline::setP1(const qint64 &value)
{
    p1 = value;
    ChangeCurrentData(ui->comboBoxP1, value);
}

/************************************************************************
 **
 **  @file   dialogsplinepath.cpp
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

#include "dialogsplinepath.h"
#include "ui_dialogsplinepath.h"
#include "../geometry/vsplinepoint.h"

DialogSplinePath::DialogSplinePath(const VContainer *data, Draw::Draws mode, QWidget *parent)
    :DialogTool(data, mode, parent), ui(new Ui::DialogSplinePath), path(VSplinePath())
{
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogSplinePath::DialogAccepted);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSplinePath::DialogRejected);
    FillComboBoxPoints(ui->comboBoxPoint);

    path = VSplinePath(data->DataPoints());

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogSplinePath::PointChenged);
    connect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSplinePath::currentPointChanged);
    connect(ui->spinBoxAngle1,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DialogSplinePath::Angle1Changed);
    connect(ui->spinBoxAngle2,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DialogSplinePath::Angle2Changed);
    connect(ui->doubleSpinBoxKasm1,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm1Changed);
    connect(ui->doubleSpinBoxKasm2,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm2Changed);
}

DialogSplinePath::~DialogSplinePath()
{
    delete ui;
}

void DialogSplinePath::SetPath(const VSplinePath &value)
{
    this->path = value;
    ui->listWidget->clear();
    for (qint32 i = 0; i < path.CountPoint(); ++i)
    {
        NewItem(path[i].P(), path[i].KAsm1(), path[i].Angle2(), path[i].KAsm2());
    }
    ui->listWidget->setFocus(Qt::OtherFocusReason);
    ui->doubleSpinBoxKcurve->setValue(path.getKCurve());
}


void DialogSplinePath::ChoosedObject(qint64 id, const Scene::Scenes &type)
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
        NewItem(id, 1, 0, 1);
        emit ToolTip(tr("Select point of curve path"));
        this->show();
    }
}

void DialogSplinePath::DialogAccepted()
{
    path.Clear();
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        path.append( qvariant_cast<VSplinePoint>(item->data(Qt::UserRole)));
    }
    path.setKCurve(ui->doubleSpinBoxKcurve->value());
    emit ToolTip("");
    emit DialogClosed(QDialog::Accepted);
}

void DialogSplinePath::PointChenged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    DataPoint(p.P(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    EnableFields();
}

void DialogSplinePath::currentPointChanged(int index)
{
    qint64 id = qvariant_cast<qint64>(ui->comboBoxPoint->itemData(index));
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetP(id);
    DataPoint(p.P(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    EnableFields();
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

void DialogSplinePath::Angle1Changed(int index)
{
    SetAngle(index+180);
}

void DialogSplinePath::Angle2Changed(int index)
{
    SetAngle(index);
}

void DialogSplinePath::KAsm1Changed(qreal d)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetKAsm1(d);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

void DialogSplinePath::KAsm2Changed(qreal d)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetKAsm2(d);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

void DialogSplinePath::NewItem(qint64 id, qreal kAsm1, qreal angle, qreal kAsm2)
{
    VPointF point;
    if (mode == Draw::Calculation)
    {
        point = data->GetPoint(id);
    }
    else
    {
        point = data->GetModelingPoint(id);
    }
    QListWidgetItem *item = new QListWidgetItem(point.name());
    item->setFont(QFont("Times", 12, QFont::Bold));
    VSplinePoint p(id, kAsm1, angle, kAsm2);
    DataPoint(id, kAsm1, angle+180, kAsm2, angle);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
    ui->listWidget->addItem(item);
    EnableFields();
}

void DialogSplinePath::DataPoint(qint64 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
{
    disconnect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &DialogSplinePath::currentPointChanged);
    disconnect(ui->spinBoxAngle1,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
               this, &DialogSplinePath::Angle1Changed);
    disconnect(ui->spinBoxAngle2,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
               this, &DialogSplinePath::Angle2Changed);
    disconnect(ui->doubleSpinBoxKasm1,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
               this, &DialogSplinePath::KAsm1Changed);
    disconnect(ui->doubleSpinBoxKasm2,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
               this, &DialogSplinePath::KAsm2Changed);

    ChangeCurrentData(ui->comboBoxPoint, id);
    ui->doubleSpinBoxKasm1->setValue(kAsm1);
    ui->doubleSpinBoxKasm2->setValue(kAsm2);
    ui->spinBoxAngle2->setValue(static_cast<qint32>(angle2));
    ui->spinBoxAngle1->setValue(static_cast<qint32>(angle1));

    connect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSplinePath::currentPointChanged);
    connect(ui->spinBoxAngle1,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DialogSplinePath::Angle1Changed);
    connect(ui->spinBoxAngle2,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DialogSplinePath::Angle2Changed);
    connect(ui->doubleSpinBoxKasm1,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm1Changed);
    connect(ui->doubleSpinBoxKasm2,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm2Changed);
}

void DialogSplinePath::EnableFields()
{
    ui->doubleSpinBoxKasm1->setEnabled(true);
    ui->spinBoxAngle1->setEnabled(true);
    ui->doubleSpinBoxKasm2->setEnabled(true);
    ui->spinBoxAngle2->setEnabled(true);
    qint32 row = ui->listWidget->currentRow();
    if (row == 0)
    {
        ui->doubleSpinBoxKasm1->setEnabled(false);
        ui->spinBoxAngle1->setEnabled(false);
        return;
    }
    if (row == ui->listWidget->count()-1)
    {
        ui->doubleSpinBoxKasm2->setEnabled(false);
        ui->spinBoxAngle2->setEnabled(false);
        return;
    }
}

void DialogSplinePath::SetAngle(qint32 angle)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetAngle(angle);
    DataPoint(p.P(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

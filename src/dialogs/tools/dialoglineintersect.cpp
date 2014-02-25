/************************************************************************
 **
 **  @file   dialoglineintersect.cpp
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

#include "dialoglineintersect.h"
#include "ui_dialoglineintersect.h"

#include <QPushButton>

DialogLineIntersect::DialogLineIntersect(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogLineIntersect), number(0), pointName(QString()),
    p1Line1(0), p2Line1(0), p1Line2(0), p2Line2(0), flagPoint(true)
{
    ui->setupUi(this);
    number = 0;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogLineIntersect::DialogAccepted);
    labelEditNamePoint = ui->labelEditNamePoint;
    flagName = false;
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogLineIntersect::DialogRejected);

    FillComboBoxPoints(ui->comboBoxP1Line1);
    FillComboBoxPoints(ui->comboBoxP2Line1);
    FillComboBoxPoints(ui->comboBoxP1Line2);
    FillComboBoxPoints(ui->comboBoxP2Line2);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogLineIntersect::NamePointChanged);
}

DialogLineIntersect::~DialogLineIntersect()
{
    delete ui;
}

void DialogLineIntersect::ChoosedObject(quint32 id, const Scene::Scenes &type)
{
    if (type == Scene::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxP1Line1->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP1Line1->setCurrentIndex(index);
                p1Line1 = id;
                number++;
                emit ToolTip(tr("Select second point of first line"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxP2Line1->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP2Line1->setCurrentIndex(index);
                p2Line1 = id;
                number++;
                emit ToolTip(tr("Select first point of second line"));
                return;
            }
        }
        if (number == 2)
        {
            qint32 index = ui->comboBoxP1Line2->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP1Line2->setCurrentIndex(index);
                p1Line2 = id;
                number++;
                emit ToolTip(tr("Select second point of second line"));
                return;
            }
        }
        if (number == 3)
        {
            qint32 index = ui->comboBoxP2Line2->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP2Line2->setCurrentIndex(index);
                p2Line2 = id;
                number = 0;
                emit ToolTip("");
            }
            if (isInitialized == false)
            {
                flagPoint = CheckIntersecion();
                CheckState();
                this->show();
                connect(ui->comboBoxP1Line1,
                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                        &DialogLineIntersect::P1Line1Changed);
                connect(ui->comboBoxP2Line1,
                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                        &DialogLineIntersect::P2Line1Changed);
                connect(ui->comboBoxP1Line2,
                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                        &DialogLineIntersect::P1Line2Changed);
                connect(ui->comboBoxP2Line2,
                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                        &DialogLineIntersect::P2Line2Changed);
            }
        }
    }
}

void DialogLineIntersect::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    p1Line1 = getCurrentObjectId(ui->comboBoxP1Line1);
    p2Line1 = getCurrentObjectId(ui->comboBoxP2Line1);
    p1Line2 = getCurrentObjectId(ui->comboBoxP1Line2);
    p2Line2 = getCurrentObjectId(ui->comboBoxP2Line2);
    emit DialogClosed(QDialog::Accepted);
}

void DialogLineIntersect::P1Line1Changed( int index)
{
    p1Line1 = qvariant_cast<quint32>(ui->comboBoxP1Line1->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();
}

void DialogLineIntersect::P2Line1Changed(int index)
{
    p2Line1 = qvariant_cast<quint32>(ui->comboBoxP2Line1->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();
}

void DialogLineIntersect::P1Line2Changed(int index)
{
    p1Line2 = qvariant_cast<quint32>(ui->comboBoxP1Line2->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();
}

void DialogLineIntersect::P2Line2Changed(int index)
{
    p2Line2 = qvariant_cast<quint32>(ui->comboBoxP2Line2->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();
}

void DialogLineIntersect::CheckState()
{
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagName && flagPoint);
}

bool DialogLineIntersect::CheckIntersecion()
{
    const VPointF *p1L1 = data->GeometricObject<const VPointF *>(p1Line1);
    const VPointF *p2L1 = data->GeometricObject<const VPointF *>(p2Line1);
    const VPointF *p1L2 = data->GeometricObject<const VPointF *>(p1Line2);
    const VPointF *p2L2 = data->GeometricObject<const VPointF *>(p2Line2);

    QLineF line1(p1L1->toQPointF(), p2L1->toQPointF());
    QLineF line2(p1L2->toQPointF(), p2L2->toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DialogLineIntersect::setP2Line2(const quint32 &value)
{
    p2Line2 = value;
    ChangeCurrentData(ui->comboBoxP2Line2, value);
}

void DialogLineIntersect::setP1Line2(const quint32 &value)
{
    p1Line2 = value;
    ChangeCurrentData(ui->comboBoxP1Line2, value);
}

void DialogLineIntersect::setP2Line1(const quint32 &value)
{
    p2Line1 = value;
    ChangeCurrentData(ui->comboBoxP2Line1, value);
}

void DialogLineIntersect::setP1Line1(const quint32 &value)
{
    p1Line1 = value;
    ChangeCurrentData(ui->comboBoxP1Line1, value);
}

void DialogLineIntersect::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

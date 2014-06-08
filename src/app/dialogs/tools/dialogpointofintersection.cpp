/************************************************************************
 **
 **  @file   dialogpointofintersection.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogPointOfIntersection create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogPointOfIntersection::DialogPointOfIntersection(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogPointOfIntersection), number(0), pointName(QString()),
    firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;
    InitOkCansel(ui);
    flagName = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersection::NamePointChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersection::~DialogPointOfIntersection()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id of second point
 * @param value id
 * @param id don't show this id in list.
 */
void DialogPointOfIntersection::setSecondPointId(const quint32 &value, const quint32 &id)
{
    secondPointId = value;
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogPointOfIntersection::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxFirstPoint->findText(point->name());
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
            qint32 index = ui->comboBoxSecondPoint->findText(point->name());
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogPointOfIntersection::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 * @param id don't show this id in list.
 */
void DialogPointOfIntersection::setFirstPointId(const quint32 &value, const quint32 &id)
{
    firstPointId = value;
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogPointOfIntersection::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

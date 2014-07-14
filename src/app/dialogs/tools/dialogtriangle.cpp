/************************************************************************
 **
 **  @file   dialogtriangle.cpp
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

#include "dialogtriangle.h"
#include "ui_dialogtriangle.h"
#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTriangle create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTriangle::DialogTriangle(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogTriangle), number(0), pointName(QString()), axisP1Id(0),
    axisP2Id(0), firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancel(ui);
    flagName = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxAxisP1);
    FillComboBoxPoints(ui->comboBoxAxisP2);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogTriangle::NamePointChanged);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxAxisP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxAxisP2, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogTriangle::~DialogTriangle()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTriangle::ChoosedObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        switch (number)
        {
            case (0):
                ChangeCurrentText(ui->comboBoxAxisP1, point->name());
                number++;
                emit ToolTip(tr("Select second point of axis"));
                break;
            case (1):
                ChangeCurrentText(ui->comboBoxAxisP2, point->name());
                number++;
                emit ToolTip(tr("Select first point"));
                break;
            case (2):
                ChangeCurrentText(ui->comboBoxFirstPoint, point->name());
                number++;
                emit ToolTip(tr("Select second point"));
                break;
            case (3):
                ChangeCurrentText(ui->comboBoxSecondPoint, point->name());
                number = 0;
                emit ToolTip(tr(""));
                if (isInitialized == false)
                {
                    this->setModal(true);
                    this->show();
                }
                break;
            default:
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogTriangle::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    axisP1Id = getCurrentObjectId(ui->comboBoxAxisP1);
    axisP2Id = getCurrentObjectId(ui->comboBoxAxisP2);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP1));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP2));

    if (set.size() != 4)
    {
        flagError = false;
        ChangeColor(ui->labelFirstPoint, Qt::red);
        ChangeColor(ui->labelSecondPoint, Qt::red);
        ChangeColor(ui->labelAxisP1, Qt::red);
        ChangeColor(ui->labelAxisP2, Qt::red);
    }
    else
    {
        flagError = true;
        ChangeColor(ui->labelFirstPoint, QColor(76, 76, 76));
        ChangeColor(ui->labelSecondPoint, QColor(76, 76, 76));
        ChangeColor(ui->labelAxisP1, QColor(76, 76, 76));
        ChangeColor(ui->labelAxisP2, QColor(76, 76, 76));
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogTriangle::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id of second point
 * @param value id
 * @param id don't show this point in list
 */
void DialogTriangle::setSecondPointId(const quint32 &value, const quint32 &id)
{
    setPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 * @param id don't show this point in list
 */
void DialogTriangle::setFirstPointId(const quint32 &value, const quint32 &id)
{
    setPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAxisP2Id set id second point of axis
 * @param value id
 * @param id don't show this point in list
 */
void DialogTriangle::setAxisP2Id(const quint32 &value, const quint32 &id)
{
    setPointId(ui->comboBoxAxisP2, axisP2Id, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAxisP1Id set id first point of axis
 * @param value id
 * @param id don't show this point in list
 */
void DialogTriangle::setAxisP1Id(const quint32 &value, const quint32 &id)
{
    setPointId(ui->comboBoxAxisP1, axisP1Id, value, id);
}

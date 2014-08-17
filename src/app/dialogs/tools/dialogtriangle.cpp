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
#include "../../visualization/vistooltriangle.h"
#include "../../widgets/vmaingraphicsscene.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTriangle create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTriangle::DialogTriangle(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogTriangle), number(0), pointName(QString()), axisP1Id(NULL_ID),
    axisP2Id(NULL_ID), firstPointId(NULL_ID), secondPointId(NULL_ID), line (nullptr)
{
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
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

    line = new VisToolTriangle(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogTriangle::~DialogTriangle()
{
    delete line;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTriangle::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case (0):
                    if (SetObject(id, ui->comboBoxAxisP1, tr("Select second point of axis")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case (1):
                    if (SetObject(id, ui->comboBoxAxisP2, tr("Select first point")))
                    {
                        number++;
                        line->setPoint2Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case (2):
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point")))
                    {
                        number++;
                        line->setHypotenuseP1Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case (3):
                    if (SetObject(id, ui->comboBoxSecondPoint, ""))
                    {
                        line->setHypotenuseP2Id(id);
                        line->RefreshGeometry();
                        prepare = true;
                        this->setModal(true);
                        this->show();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    axisP1Id = getCurrentObjectId(ui->comboBoxAxisP1);
    axisP2Id = getCurrentObjectId(ui->comboBoxAxisP2);

    line->setPoint1Id(axisP1Id);
    line->setPoint2Id(axisP2Id);
    line->setHypotenuseP1Id(firstPointId);
    line->setHypotenuseP2Id(secondPointId);
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP1));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP2));

    QColor color = okColor;
    if (set.size() < 3)//Need tree or more unique points for creation triangle
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    ChangeColor(ui->labelAxisP1, color);
    ChangeColor(ui->labelAxisP2, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
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
 */
void DialogTriangle::setSecondPointId(const quint32 &value)
{
    setPointId(ui->comboBoxSecondPoint, secondPointId, value);
    line->setHypotenuseP2Id(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 */
void DialogTriangle::setFirstPointId(const quint32 &value)
{
    setPointId(ui->comboBoxFirstPoint, firstPointId, value);
    line->setHypotenuseP1Id(firstPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAxisP2Id set id second point of axis
 * @param value id
 */
void DialogTriangle::setAxisP2Id(const quint32 &value)
{
    setPointId(ui->comboBoxAxisP2, axisP2Id, value);
    line->setPoint2Id(axisP2Id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAxisP1Id set id first point of axis
 * @param value id
 */
void DialogTriangle::setAxisP1Id(const quint32 &value)
{
    setPointId(ui->comboBoxAxisP1, axisP1Id, value);
    line->setPoint1Id(axisP1Id);
}

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
#include "../../visualization/vistoolpointofintersection.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogPointOfIntersection create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogPointOfIntersection::DialogPointOfIntersection(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfIntersection), line(nullptr)
{
    ui->setupUi(this);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersection::NamePointChanged);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersection::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersection::PointNameChanged);

    line = new VisToolPointOfIntersection(data);
    line->VisualMode(NULL_ID);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersection::~DialogPointOfIntersection()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id of second point
 * @param value id
 */
void DialogPointOfIntersection::setSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogPointOfIntersection::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select point for Y value (horizontal)")))
                    {
                        number++;
                        line->setPoint1Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, ""))
                        {
                            line->setPoint2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    line->setPoint1Id(getFirstPointId());
    line->setPoint2Id(getSecondPointId());
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::PointNameChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxFirstPoint) == getCurrentObjectId(ui->comboBoxSecondPoint))
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
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variable.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisLine::SetFactor);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 */
void DialogPointOfIntersection::setFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);
    line->setPoint1Id(value);
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPointId return id of first point
 * @return id
 */
quint32 DialogPointOfIntersection::getFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPointId return id of second point
 * @return id
 */
quint32 DialogPointOfIntersection::getSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}

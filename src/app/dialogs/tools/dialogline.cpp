/************************************************************************
 **
 **  @file   dialogline.cpp
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

#include "dialogline.h"
#include "ui_dialogline.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolline.h"
#include "../../widgets/vapplication.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogLine::DialogLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogLine), number(0), firstPoint(0), secondPoint(0),
      typeLine(QString()), line(nullptr)
{
    ui->setupUi(this);
    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    number = 0;

    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLine::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLine::PointNameChanged);

    line = new VisToolLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLine::~DialogLine()
{
    delete line;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPoint set id second point
 * @param value id
 */
void DialogLine::setSecondPoint(const quint32 &value)
{
    setPointId(ui->comboBoxSecondPoint, secondPoint, value);
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogLine::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPoint set id first point
 * @param value id
 */
void DialogLine::setFirstPoint(const quint32 &value)
{
    setPointId(ui->comboBoxFirstPoint, firstPoint, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogLine::DialogAccepted()
{
    this->SaveData();
    DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::DialogApply()
{
    this->SaveData();
    emit DialogApplied();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::PointNameChanged()
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
void DialogLine::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variables.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::ShowVisualization()
{
    VMainGraphicsScene *scene = qApp->getCurrentScene();
    connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolLine::SetFactor);
    scene->addItem(line);
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::SaveData()
{
    qint32 index = ui->comboBoxFirstPoint->currentIndex();
    firstPoint = qvariant_cast<quint32>(ui->comboBoxFirstPoint->itemData(index));
    index = ui->comboBoxSecondPoint->currentIndex();
    secondPoint = qvariant_cast<quint32>(ui->comboBoxSecondPoint->itemData(index));
    typeLine = GetTypeLine(ui->comboBoxLineType);

    line->setPoint1Id(firstPoint);
    line->setPoint2Id(secondPoint);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxFirstPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found

                VMainGraphicsScene *scene = qApp->getCurrentScene();
                line->VisualMode(id, scene->getScenePos());
                scene->addItem(line);
                connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolLine::SetFactor);
                connect(scene, &VMainGraphicsScene::mouseMove, line, &VisToolLine::MousePos);

                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select second point"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxSecondPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                if (flagError)
                {
                    number = 0;
                    emit ToolTip("");
                    DialogAccepted();
                }
            }
        }
    }
}

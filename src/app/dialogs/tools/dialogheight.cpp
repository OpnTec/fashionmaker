/************************************************************************
 **
 **  @file   dialogheight.cpp
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

#include "dialogheight.h"
#include "ui_dialogheight.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../tools/vabstracttool.h"
#include "../visualization/vistoolheight.h"
#include "../widgets/vmaingraphicsscene.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogHeight create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogHeight::DialogHeight(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogHeight), number(0),
    typeLine(QString()), basePointId(NULL_ID), p1LineId(NULL_ID), p2LineId(NULL_ID), line(nullptr)
{
    ui->setupUi(this);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogHeight::NamePointChanged);
    connect(ui->comboBoxBasePoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogHeight::PointNameChanged);
    connect(ui->comboBoxP1Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogHeight::PointNameChanged);
    connect(ui->comboBoxP2Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogHeight::PointNameChanged);

    line = new VisToolHeight(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogHeight::~DialogHeight()
{
    delete line;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogHeight::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogHeight::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setBasePointId set id base point of height
 * @param value id
 */
void DialogHeight::setBasePointId(const quint32 &value)
{
    setPointId(ui->comboBoxBasePoint, basePointId, value);
    line->setPoint1Id(basePointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP1LineId set id first point of line
 * @param value id
 */
void DialogHeight::setP1LineId(const quint32 &value)
{
    setPointId(ui->comboBoxP1Line, p1LineId, value);
    line->setLineP1Id(p1LineId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP2LineId set id second point of line
 * @param value id
 */
void DialogHeight::setP2LineId(const quint32 &value)
{
    setPointId(ui->comboBoxP2Line, p2LineId, value);
    line->setLineP2Id(p2LineId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogHeight::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case (0):
                    if (SetObject(id, ui->comboBoxBasePoint, tr("Select first point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case (1):
                    if (getCurrentObjectId(ui->comboBoxBasePoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxP1Line, tr("Select second point of line")))
                        {
                            number++;
                            line->setLineP1Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case (2):
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxP1Line));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxP2Line, ""))
                        {
                            line->setLineP2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
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
void DialogHeight::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    basePointId = getCurrentObjectId(ui->comboBoxBasePoint);
    p1LineId = getCurrentObjectId(ui->comboBoxP1Line);
    p2LineId = getCurrentObjectId(ui->comboBoxP2Line);

    line->setPoint1Id(basePointId);
    line->setLineP1Id(p1LineId);
    line->setLineP2Id(p2LineId);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogHeight::PointNameChanged()
{
    QSet<quint32> set;
    const quint32 basePointId = getCurrentObjectId(ui->comboBoxBasePoint);
    const quint32 p1LineId = getCurrentObjectId(ui->comboBoxP1Line);
    const quint32 p2LineId = getCurrentObjectId(ui->comboBoxP2Line);

    set.insert(basePointId);
    set.insert(p1LineId);
    set.insert(p2LineId);

    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> p1Line = data->GeometricObject<VPointF>(p1LineId);
    const QSharedPointer<VPointF> p2Line = data->GeometricObject<VPointF>(p2LineId);

    QColor color = okColor;
    if (set.size() != 3 || VGObject::ClosestPoint(QLineF(p1Line->toQPointF(), p2Line->toQPointF()),
                                                  basePoint->toQPointF()) == QPointF())
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelBasePoint, color);
    ChangeColor(ui->labelFirstLinePoint, color);
    ChangeColor(ui->labelSecondLinePoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogHeight::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variable.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

//---------------------------------------------------------------------------------------------------------------------
void DialogHeight::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

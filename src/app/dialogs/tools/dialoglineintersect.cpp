/************************************************************************
 **
 **  @file   dialoglineintersect.cpp
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

#include "dialoglineintersect.h"
#include "ui_dialoglineintersect.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoollineintersect.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogLineIntersect create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogLineIntersect::DialogLineIntersect(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogLineIntersect), number(0),
    p1Line1(NULL_ID), p2Line1(NULL_ID), p1Line2(NULL_ID), p2Line2(NULL_ID), flagPoint(true), line(nullptr)
{
    ui->setupUi(this);
    number = 0;
    InitOkCancelApply(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    FillComboBoxPoints(ui->comboBoxP1Line1);
    FillComboBoxPoints(ui->comboBoxP2Line1);
    FillComboBoxPoints(ui->comboBoxP1Line2);
    FillComboBoxPoints(ui->comboBoxP2Line2);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogLineIntersect::NamePointChanged);
    connect(ui->comboBoxP1Line1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersect::PointNameChanged);
    connect(ui->comboBoxP2Line1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersect::PointNameChanged);
    connect(ui->comboBoxP1Line2, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersect::PointNameChanged);
    connect(ui->comboBoxP2Line2, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersect::PointNameChanged);

    line = new VisToolLineIntersect(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLineIntersect::~DialogLineIntersect()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogLineIntersect::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP1Line1, tr("Select second point of first line")))
                    {
                        number++;
                        p1Line1 = id;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxP1Line1) != id)
                    {
                        if (SetObject(id, ui->comboBoxP2Line1, tr("Select first point of second line")))
                        {
                            number++;
                            p2Line1 = id;
                            line->setLine1P2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                    if (SetObject(id, ui->comboBoxP1Line2, tr("Select second point of second line")))
                    {
                        number++;
                        p1Line2 = id;
                        line->setLine2P1Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case 3:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxP1Line1));
                    set.insert(getCurrentObjectId(ui->comboBoxP2Line1));
                    set.insert(getCurrentObjectId(ui->comboBoxP1Line2));
                    set.insert(id);

                    if (set.size() >= 3)
                    {
                        if (SetObject(id, ui->comboBoxP2Line2, ""))
                        {
                            p2Line2 = id;
                            line->setLine2P2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            flagPoint = CheckIntersecion();
                            CheckState();
                            this->setModal(true);
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
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersect::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    p1Line1 = getCurrentObjectId(ui->comboBoxP1Line1);
    p2Line1 = getCurrentObjectId(ui->comboBoxP2Line1);
    p1Line2 = getCurrentObjectId(ui->comboBoxP1Line2);
    p2Line2 = getCurrentObjectId(ui->comboBoxP2Line2);

    line->setPoint1Id(p1Line1);
    line->setLine1P2Id(p2Line1);
    line->setLine2P1Id(p1Line2);
    line->setLine2P2Id(p2Line2);
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P1Line1Changed changed first point of first line
 * @param index index in list
 */
void DialogLineIntersect::P1Line1Changed( int index)
{
    p1Line1 = qvariant_cast<quint32>(ui->comboBoxP1Line1->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();

    line->setPoint1Id(p1Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P2Line1Changed changed second point of first line
 * @param index index in list
 */
void DialogLineIntersect::P2Line1Changed(int index)
{
    p2Line1 = qvariant_cast<quint32>(ui->comboBoxP2Line1->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();

    line->setLine1P2Id(p2Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P1Line2Changed changed first point of second line
 * @param index index in list
 */
void DialogLineIntersect::P1Line2Changed(int index)
{
    p1Line2 = qvariant_cast<quint32>(ui->comboBoxP1Line2->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();

    line->setLine2P1Id(p1Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P2Line2Changed changed second point of second line
 * @param index index in list
 */
void DialogLineIntersect::P2Line2Changed(int index)
{
    p2Line2 = qvariant_cast<quint32>(ui->comboBoxP2Line2->itemData(index));
    flagPoint = CheckIntersecion();
    CheckState();

    line->setLine2P2Id(p2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersect::PointNameChanged()
{
    QSet<quint32> set;
    const quint32 p1Line1Id = getCurrentObjectId(ui->comboBoxP1Line1);
    const quint32 p2Line1Id = getCurrentObjectId(ui->comboBoxP2Line1);
    const quint32 p1Line2Id = getCurrentObjectId(ui->comboBoxP1Line2);
    const quint32 p2Line2Id = getCurrentObjectId(ui->comboBoxP2Line2);

    set.insert(p1Line1Id);
    set.insert(p2Line1Id);
    set.insert(p1Line2Id);
    set.insert(p2Line2Id);

    const QSharedPointer<VPointF> p1Line1 = data->GeometricObject<VPointF>(p1Line1Id);
    const QSharedPointer<VPointF> p2Line1 = data->GeometricObject<VPointF>(p2Line1Id);
    const QSharedPointer<VPointF> p1Line2 = data->GeometricObject<VPointF>(p1Line2Id);
    const QSharedPointer<VPointF> p2Line2 = data->GeometricObject<VPointF>(p2Line2Id);

    QLineF line1(p1Line1->toQPointF(), p2Line1->toQPointF());
    QLineF line2(p1Line2->toQPointF(), p2Line2->toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);

    QColor color = okColor;
    if (set.size() < 3 || intersect == QLineF::NoIntersection)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelP1Line1, color);
    ChangeColor(ui->labelP2Line1, color);
    ChangeColor(ui->labelP1Line2, color);
    ChangeColor(ui->labelP2Line2, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersect::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variable.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersect::ShowVisualization()
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
 * @brief CheckState check state of dialog. Enable or disable button ok.
 */
void DialogLineIntersect::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagName && flagPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckIntersecion check intersection of points
 * @return true - line have intersection, false = don't have
 */
bool DialogLineIntersect::CheckIntersecion()
{
    const QSharedPointer<VPointF> p1L1 = data->GeometricObject<VPointF>(p1Line1);
    const QSharedPointer<VPointF> p2L1 = data->GeometricObject<VPointF>(p2Line1);
    const QSharedPointer<VPointF> p1L2 = data->GeometricObject<VPointF>(p1Line2);
    const QSharedPointer<VPointF> p2L2 = data->GeometricObject<VPointF>(p2Line2);

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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP2Line2 set id second point of second line
 * @param value id
 */
void DialogLineIntersect::setP2Line2(const quint32 &value)
{
    setPointId(ui->comboBoxP2Line2, p2Line2, value);
    line->setLine2P2Id(p2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP1Line2 set id first point of second line
 * @param value id
 */
void DialogLineIntersect::setP1Line2(const quint32 &value)
{
    setPointId(ui->comboBoxP1Line2, p1Line2, value);
    line->setLine2P1Id(p1Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP2Line1 set id second point of first line
 * @param value id
 */
void DialogLineIntersect::setP2Line1(const quint32 &value)
{
    setPointId(ui->comboBoxP2Line1, p2Line1, value);
    line->setLine1P2Id(p2Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP1Line1 set id first point of first line
 * @param value id
 */
void DialogLineIntersect::setP1Line1(const quint32 &value)
{
    setPointId(ui->comboBoxP1Line1, p1Line1, value);
    line->setPoint1Id(p1Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name of point
 */
void DialogLineIntersect::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

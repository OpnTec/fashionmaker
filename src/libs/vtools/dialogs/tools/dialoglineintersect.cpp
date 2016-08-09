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
 **  Copyright (C) 2013-2015 Valentina project
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

#include <QColor>
#include <QComboBox>
#include <QLabel>
#include <QLine>
#include <QLineEdit>
#include <QLineF>
#include <QPointF>
#include <QPointer>
#include <QPushButton>
#include <QSet>
#include <QSharedPointer>
#include <new>

#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoollineintersect.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "dialogtool.h"
#include "ui_dialoglineintersect.h"

class QWidget;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogLineIntersect create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogLineIntersect::DialogLineIntersect(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogLineIntersect), flagPoint(true)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

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

    vis = new VisToolLineIntersect(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLineIntersect::~DialogLineIntersect()
{
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
            VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
            SCASSERT(line != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP1Line1, tr("Select second point of first line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxP1Line1) != id)
                    {
                        if (SetObject(id, ui->comboBoxP2Line1, tr("Select first point of second line")))
                        {
                            number++;
                            line->setLine1P2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                    if (SetObject(id, ui->comboBoxP1Line2, tr("Select second point of second line")))
                    {
                        number++;
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
                            line->setLine2P2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            flagPoint = CheckIntersecion();
                            CheckState();
                            if (flagPoint)
                            {
                                DialogAccepted();
                            }
                            else
                            {
                                this->setModal(true);
                                this->show();
                                connect(ui->comboBoxP1Line1,
                                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                                        &DialogLineIntersect::PointChanged);
                                connect(ui->comboBoxP2Line1,
                                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                                        &DialogLineIntersect::PointChanged);
                                connect(ui->comboBoxP1Line2,
                                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                                        &DialogLineIntersect::PointChanged);
                                connect(ui->comboBoxP2Line2,
                                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                                        &DialogLineIntersect::PointChanged);
                            }
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

    VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
    SCASSERT(line != nullptr);

    line->setObject1Id(GetP1Line1());
    line->setLine1P2Id(GetP2Line1());
    line->setLine2P1Id(GetP1Line2());
    line->setLine2P2Id(GetP2Line2());
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P1Line1Changed changed first point of first line
 */
void DialogLineIntersect::PointChanged()
{
    flagPoint = CheckIntersecion();
    CheckState();
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

    QLineF line1(*p1Line1, *p2Line1);
    QLineF line2(*p1Line2, *p2Line2);
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
void DialogLineIntersect::ShowVisualization()
{
    AddVisualization<VisToolLineIntersect>();
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
    const QSharedPointer<VPointF> p1L1 = data->GeometricObject<VPointF>(GetP1Line1());
    const QSharedPointer<VPointF> p2L1 = data->GeometricObject<VPointF>(GetP2Line1());
    const QSharedPointer<VPointF> p1L2 = data->GeometricObject<VPointF>(GetP1Line2());
    const QSharedPointer<VPointF> p2L2 = data->GeometricObject<VPointF>(GetP2Line2());

    QLineF line1(*p1L1, *p2L1);
    QLineF line2(*p1L2, *p2L2);
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
 * @brief SetP2Line2 set id second point of second line
 * @param value id
 */
void DialogLineIntersect::SetP2Line2(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP2Line2, value);

    VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
    SCASSERT(line != nullptr);
    line->setLine2P2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP1Line2 set id first point of second line
 * @param value id
 */
void DialogLineIntersect::SetP1Line2(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP1Line2, value);

    VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
    SCASSERT(line != nullptr);
    line->setLine2P1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP2Line1 set id second point of first line
 * @param value id
 */
void DialogLineIntersect::SetP2Line1(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP2Line1, value);

    VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
    SCASSERT(line != nullptr);
    line->setLine1P2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP1Line1 set id first point of first line
 * @param value id
 */
void DialogLineIntersect::SetP1Line1(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP1Line1, value);

    VisToolLineIntersect *line = qobject_cast<VisToolLineIntersect *>(vis);
    SCASSERT(line != nullptr);
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name of point
 */
void DialogLineIntersect::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1Line1 return id first point of first line
 * @return id
 */
quint32 DialogLineIntersect::GetP1Line1() const
{
    return getCurrentObjectId(ui->comboBoxP1Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2Line1 return id second point of first line
 * @return id
 */
quint32 DialogLineIntersect::GetP2Line1() const
{
    return getCurrentObjectId(ui->comboBoxP2Line1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1Line2 return id first point of second line
 * @return id
 */
quint32 DialogLineIntersect::GetP1Line2() const
{
    return getCurrentObjectId(ui->comboBoxP1Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2Line2 return id second point of second line
 * @return id
 */
quint32 DialogLineIntersect::GetP2Line2() const
{
    return getCurrentObjectId(ui->comboBoxP2Line2);
}

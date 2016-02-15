/************************************************************************
 **
 **  @file   dialogspline.cpp
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

#include "dialogspline.h"
#include "ui_dialogspline.h"

#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vpatterndb/vcontainer.h"
#include "../../visualization/vistoolspline.h"

#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogSpline create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogSpline::DialogSpline(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogSpline), spl()
{
    ui->setupUi(this);
    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP4);
    FillComboBoxLineColors(ui->comboBoxColor);

    connect(ui->comboBoxP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);
    connect(ui->comboBoxP4, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);

    vis = new VisToolSpline(data);
    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    auto scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);
    connect(scene, &VMainGraphicsScene::MouseLeftPressed, path, &VisToolSpline::MouseLeftPressed);
    connect(scene, &VMainGraphicsScene::MouseLeftReleased, path, &VisToolSpline::MouseLeftReleased);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSpline::~DialogSpline()
{
    DeleteVisualization<VisToolSpline>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogSpline::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            auto *path = qobject_cast<VisToolSpline *>(vis);
            SCASSERT(path != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP1, tr("Select last point of curve")))
                    {
                        ++number;
                        path->VisualMode(id);
                    }
                    break;
                case 1:
                {
                    if (getCurrentObjectId(ui->comboBoxP1) != id)
                    {
                        if (SetObject(id, ui->comboBoxP4, ""))
                        {
                            ++number;

                            path->setObject4Id(id);
                            path->RefreshGeometry();
                            prepare = true;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SaveData()
{
    const qreal angle1 = ui->spinBoxAngle1->value();
    const qreal angle2 = ui->spinBoxAngle2->value();
    const qreal kAsm1 = ui->doubleSpinBoxKasm1->value();
    const qreal kAsm2 = ui->doubleSpinBoxKasm2->value();
    const qreal kCurve = ui->doubleSpinBoxKcurve->value();

    spl = VSpline(*GetP1(), *GetP4(), angle1, angle2, kAsm1, kAsm2, kCurve);

    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetP1()->id());
    path->setObject4Id(GetP4()->id());
    path->SetAngle1(angle1);
    path->SetAngle2(angle2);
    path->SetKAsm1(kAsm1);
    path->SetKAsm2(kAsm2);
    path->SetKCurve(kCurve);
    path->SetMode(Mode::Show);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogSpline::GetP1() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP1));
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogSpline::GetP4() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP4));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxP1));
    set.insert(getCurrentObjectId(ui->comboBoxP4));

    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxP1) == getCurrentObjectId(ui->comboBoxP4))
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
void DialogSpline::ShowDialog(bool click)
{
    if (prepare && click)
    {
        auto *path = qobject_cast<VisToolSpline *>(vis);
        SCASSERT(path != nullptr);

        VSpline spl(*GetP1(), path->GetP2(), path->GetP3(), *GetP4(), 1);

        ui->spinBoxAngle1->setValue(static_cast<qint32>(spl.GetStartAngle()));
        ui->spinBoxAngle2->setValue(static_cast<qint32>(spl.GetEndAngle()));

        ui->doubleSpinBoxKasm1->setValue(spl.GetKasm1());
        ui->doubleSpinBoxKasm2->setValue(spl.GetKasm2());

        DialogAccepted();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::ShowVisualization()
{
    AddVisualization<VisToolSpline>();
}

//---------------------------------------------------------------------------------------------------------------------
VSpline DialogSpline::GetSpline() const
{
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SetSpline(const VSpline &spline)
{
    spl = spline;

    setCurrentPointId(ui->comboBoxP1, spl.GetP1().id());
    setCurrentPointId(ui->comboBoxP4, spl.GetP4().id());
    ui->spinBoxAngle1->setValue(static_cast<qint32>(spl.GetStartAngle()));
    ui->spinBoxAngle2->setValue(static_cast<qint32>(spl.GetEndAngle()));
    ui->doubleSpinBoxKasm1->setValue(spl.GetKasm1());
    ui->doubleSpinBoxKasm2->setValue(spl.GetKasm2());
    ui->doubleSpinBoxKcurve->setValue(spl.GetKcurve());

    auto path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(spl.GetP1().id());
    path->setObject4Id(spl.GetP4().id());
    path->SetAngle1(spl.GetStartAngle());
    path->SetAngle2(spl.GetEndAngle());
    path->SetKAsm1(spl.GetKasm1());
    path->SetKAsm2(spl.GetKasm2());
    path->SetKCurve(spl.GetKcurve());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSpline::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

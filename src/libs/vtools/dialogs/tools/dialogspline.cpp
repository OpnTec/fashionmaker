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
    :DialogTool(data, toolId, parent), ui(new Ui::DialogSpline), angle1(0), angle2(0), kAsm1(1), kAsm2(1), kCurve(1)
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
 * @brief GetP1 return id first point of spline
 * @return id
 */
quint32 DialogSpline::GetP1() const
{
    return getCurrentObjectId(ui->comboBoxP1);
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
    angle1 = ui->spinBoxAngle1->value();
    angle2 = ui->spinBoxAngle2->value();
    kAsm1 = ui->doubleSpinBoxKasm1->value();
    kAsm2 = ui->doubleSpinBoxKasm2->value();
    kCurve = ui->doubleSpinBoxKcurve->value();

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetP1());
    path->setObject4Id(GetP4());
    path->SetAngle1(angle1);
    path->SetAngle2(angle2);
    path->SetKAsm1(kAsm1);
    path->SetKAsm2(kAsm2);
    path->SetKCurve(kCurve);
    path->RefreshGeometry();
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
        const auto p1 = data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP1));
        const auto p4 = data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP4));

        auto *path = qobject_cast<VisToolSpline *>(vis);
        SCASSERT(path != nullptr);

        const QPointF p2 = path->GetP2();
        const QPointF p3 = path->GetP3();

        VSpline spline(*p1, p2, p3, *p4, kCurve);

        ui->spinBoxAngle1->setValue(static_cast<qint32>(spline.GetStartAngle()));
        ui->spinBoxAngle2->setValue(static_cast<qint32>(spline.GetEndAngle()));

        ui->doubleSpinBoxKasm1->setValue(spline.GetKasm1());
        ui->doubleSpinBoxKasm2->setValue(spline.GetKasm2());

        DialogAccepted();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSpline::ShowVisualization()
{
    AddVisualization<VisToolSpline>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKCurve set coefficient curve
 * @param value value. Can be >= 0.
 */
void DialogSpline::SetKCurve(const qreal &value)
{
    kCurve = value;
    ui->doubleSpinBoxKcurve->setValue(value);

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->SetKCurve(kCurve);
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm2 set second coefficient asymmetry
 * @param value value. Can be >= 0.
 */
void DialogSpline::SetKAsm2(const qreal &value)
{
    kAsm2 = value;
    ui->doubleSpinBoxKasm2->setValue(value);

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->SetKAsm2(kAsm2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm1 set first coefficient asymmetry
 * @param value value. Can be >= 0.
 */
void DialogSpline::SetKAsm1(const qreal &value)
{
    kAsm1 = value;
    ui->doubleSpinBoxKasm1->setValue(value);

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->SetKAsm1(kAsm1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle2 set second angle of spline
 * @param value angle in degree
 */
void DialogSpline::SetAngle2(const qreal &value)
{
    angle2 = value;
    ui->spinBoxAngle2->setValue(static_cast<qint32>(value));

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->SetAngle2(angle2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle1 set first angle of spline
 * @param value angle in degree
 */
void DialogSpline::SetAngle1(const qreal &value)
{
    angle1 = value;
    ui->spinBoxAngle1->setValue(static_cast<qint32>(value));

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->SetAngle1(angle1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP4 set id fourth point of spline
 * @param value id
 */
void DialogSpline::SetP4(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP4, value);

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->setObject4Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP1 set id first point of spline
 * @param value id
 */
void DialogSpline::SetP1(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP1, value);

    VisToolSpline *path = qobject_cast<VisToolSpline *>(vis);
    SCASSERT(path != nullptr);
    path->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP4 return id fourth point of spline
 * @return id
 */
quint32 DialogSpline::GetP4() const
{
    return getCurrentObjectId(ui->comboBoxP4);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle1 return first angle of spline
 * @return angle in degree
 */
qreal DialogSpline::GetAngle1() const
{
    return angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle2 return second angle of spline
 * @return angle in degree
 */
qreal DialogSpline::GetAngle2() const
{
    return angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKAsm1 return first coefficient asymmetry
 * @return value. Can be >= 0.
 */
qreal DialogSpline::GetKAsm1() const
{
    return kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKAsm2 return second coefficient asymmetry
 * @return value. Can be >= 0.
 */
qreal DialogSpline::GetKAsm2() const
{
    return kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKCurve return coefficient curve
 * @return value. Can be >= 0.
 */
qreal DialogSpline::GetKCurve() const
{
    return kCurve;
}

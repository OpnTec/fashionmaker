/************************************************************************
 **
 **  @file   dialogcubicbezier.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogcubicbezier.h"
#include "ui_dialogcubicbezier.h"
#include "../../visualization/path/vistoolcubicbezier.h"

//---------------------------------------------------------------------------------------------------------------------
DialogCubicBezier::DialogCubicBezier(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogCubicBezier),
      spl(),
      newDuplicate(-1)
{
    ui->setupUi(this);
    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP2);
    FillComboBoxPoints(ui->comboBoxP3);
    FillComboBoxPoints(ui->comboBoxP4);
    FillComboBoxLineColors(ui->comboBoxColor);

    CheckState();

    connect(ui->comboBoxP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogCubicBezier::PointNameChanged);
    connect(ui->comboBoxP2, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogCubicBezier::PointNameChanged);
    connect(ui->comboBoxP3, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogCubicBezier::PointNameChanged);
    connect(ui->comboBoxP4, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogCubicBezier::PointNameChanged);

    vis = new VisToolCubicBezier(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCubicBezier::~DialogCubicBezier()
{
    DeleteVisualization<VisToolCubicBezier>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier DialogCubicBezier::GetSpline() const
{
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezier::SetSpline(const VCubicBezier &spline)
{
    spl = spline;

    setCurrentPointId(ui->comboBoxP1, spl.GetP1().id());
    setCurrentPointId(ui->comboBoxP2, spl.GetP2().id());
    setCurrentPointId(ui->comboBoxP3, spl.GetP3().id());
    setCurrentPointId(ui->comboBoxP4, spl.GetP4().id());

    ui->lineEditSplineName->setText(spl.name());

    auto path = qobject_cast<VisToolCubicBezier *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(spl.GetP1().id());
    path->setObject2Id(spl.GetP2().id());
    path->setObject3Id(spl.GetP3().id());
    path->setObject4Id(spl.GetP4().id());
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCubicBezier::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezier::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezier::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            auto *path = qobject_cast<VisToolCubicBezier *>(vis);
            SCASSERT(path != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP1, tr("Select the second point of curve")))
                    {
                        ++number;
                        path->VisualMode(id);
                    }
                    break;
                case 1:
                    if (SetObject(id, ui->comboBoxP2, tr("Select the third point of curve")))
                    {
                        ++number;

                        path->setObject2Id(id);
                        path->RefreshGeometry();
                    }
                    break;
                case 2:
                    if (SetObject(id, ui->comboBoxP3, tr("Select the fourth point of curve")))
                    {
                        ++number;

                        path->setObject3Id(id);
                        path->RefreshGeometry();
                    }
                    break;
                case 3:
                    if (getCurrentObjectId(ui->comboBoxP1) != id)
                    {
                        if (SetObject(id, ui->comboBoxP4, ""))
                        {
                            ++number;

                            path->setObject4Id(id);
                            path->RefreshGeometry();
                            prepare = true;
                            DialogAccepted();
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
void DialogCubicBezier::PointNameChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxP1) == getCurrentObjectId(ui->comboBoxP4))
    {
        flagError = false;
        color = errorColor;

        ui->lineEditSplineName->setText(tr("Invalid spline"));
    }
    else
    {
        flagError = true;
        color = okColor;

        if (getCurrentObjectId(ui->comboBoxP1) == spl.GetP1().id() &&
            getCurrentObjectId(ui->comboBoxP4) == spl.GetP4().id())
        {
            newDuplicate = -1;
            ui->lineEditSplineName->setText(spl.name());
        }
        else
        {
            VCubicBezier spline(*GetP1(), *GetP2(), *GetP3(), *GetP4());

            if (not data->IsUnique(spline.name()))
            {
                newDuplicate = DNumber(spline.name());
                spline.SetDuplicate(newDuplicate);
            }
            ui->lineEditSplineName->setText(spline.name());
        }
    }
    ChangeColor(ui->labelName, color);
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    ChangeColor(ui->labelThirdPoint, color);
    ChangeColor(ui->labelForthPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezier::ShowVisualization()
{
    AddVisualization<VisToolCubicBezier>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezier::SaveData()
{
    const auto p1 = GetP1();
    const auto p2 = GetP2();
    const auto p3 = GetP3();
    const auto p4 = GetP4();

    spl = VCubicBezier(*p1, *p2, *p3, *p4);

    const quint32 d = spl.GetDuplicate();//Save previous value
    newDuplicate <= -1 ? spl.SetDuplicate(d) : spl.SetDuplicate(static_cast<quint32>(newDuplicate));

    auto path = qobject_cast<VisToolCubicBezier *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(p1->id());
    path->setObject2Id(p2->id());
    path->setObject3Id(p3->id());
    path->setObject4Id(p4->id());
    path->SetMode(Mode::Show);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogCubicBezier::GetP1() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP1));
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogCubicBezier::GetP2() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP2));
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogCubicBezier::GetP3() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP3));
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VPointF> DialogCubicBezier::GetP4() const
{
    return data->GeometricObject<VPointF>(getCurrentObjectId(ui->comboBoxP4));
}

/************************************************************************
 **
 **  @file   dialogpointofintersectionarcs.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "dialogpointofintersectionarcs.h"
#include "ui_dialogpointofintersectionarcs.h"

#include "../../libs/vgeometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolpointofintersectionarcs.h"
#include "../../widgets/vmaingraphicsscene.h"

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionArcs::DialogPointOfIntersectionArcs(const VContainer *data, const quint32 &toolId,
                                                             QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfIntersectionArcs)
{
    ui->setupUi(this);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxArcs(ui->comboBoxArc1);
    FillComboBoxArcs(ui->comboBoxArc2);
    FillComboBoxCrossArcPoints();

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersectionArcs::NamePointChanged);
    connect(ui->comboBoxArc1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersectionArcs::ArcChanged);
    connect(ui->comboBoxArc1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersectionArcs::ArcChanged);

    vis = new VisToolPointOfIntersectionArcs(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionArcs::~DialogPointOfIntersectionArcs()
{
    DeleteVisualization<VisToolPointOfIntersectionArcs>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointOfIntersectionArcs::GetFirstArcId() const
{
    return getCurrentObjectId(ui->comboBoxArc1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::SetFirstArcId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxArc1, value);

    VisToolPointOfIntersectionArcs *point = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
    SCASSERT(point != nullptr);
    point->setArc1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointOfIntersectionArcs::GetSecondArcId() const
{
    return getCurrentObjectId(ui->comboBoxArc2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::SetSecondArcId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxArc2, value);

    VisToolPointOfIntersectionArcs *point = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
    SCASSERT(point != nullptr);
    point->setArc2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
CrossArcPoint DialogPointOfIntersectionArcs::GetCrossArcPoint() const
{
    int value;
    bool ok = false;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    value = ui->comboBoxResult->itemData(box->currentIndex()).toInt(&ok);
#else
    value = ui->comboBoxResult->currentData().toInt(&ok);
#endif
    if (not ok)
    {
        return CrossArcPoint::FirstPoint;
    }

    switch(value)
    {
        case 1:
            return CrossArcPoint::FirstPoint;
            break;
        case 2:
            return CrossArcPoint::SecondPoint;
            break;
        default:
            return CrossArcPoint::FirstPoint;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::SetCrossArcPoint(CrossArcPoint &p)
{
    const qint32 index = ui->comboBoxResult->findData(static_cast<int>(p));
    if (index != -1)
    {
        ui->comboBoxResult->setCurrentIndex(index);

        VisToolPointOfIntersectionArcs *point = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
        SCASSERT(point != nullptr);
        point->setCrossPoint(p);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Arc)
        {
            VisToolPointOfIntersectionArcs *point = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
            SCASSERT(point != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxArc1, tr("Select second an arc")))
                    {
                        number++;
                        point->setArc1Id(id);
                        point->RefreshGeometry();
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxArc2) != id)
                    {
                        if (SetObject(id, ui->comboBoxArc2, ""))
                        {
                            point->setArc2Id(id);
                            point->RefreshGeometry();
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
void DialogPointOfIntersectionArcs::ArcChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxArc1) == getCurrentObjectId(ui->comboBoxArc2))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelArc1, color);
    ChangeColor(ui->labelArc2, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::ShowVisualization()
{
    AddVisualization<VisToolPointOfIntersectionArcs>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    VisToolPointOfIntersectionArcs *point = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
    SCASSERT(point != nullptr);

    point->setArc1Id(GetFirstArcId());
    point->setArc2Id(GetSecondArcId());
    point->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionArcs::FillComboBoxCrossArcPoints()
{
    ui->comboBoxResult->addItem(tr("First point"), QVariant(static_cast<int>(CrossArcPoint::FirstPoint)));
    ui->comboBoxResult->addItem(tr("Second point"), QVariant(static_cast<int>(CrossArcPoint::SecondPoint)));
}

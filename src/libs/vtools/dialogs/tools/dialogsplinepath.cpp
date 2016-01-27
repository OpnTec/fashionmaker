/************************************************************************
 **
 **  @file   dialogsplinepath.cpp
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

#include "dialogsplinepath.h"
#include "ui_dialogsplinepath.h"
#include "../vgeometry/vsplinepoint.h"
#include "../vpatterndb/vcontainer.h"
#include "../../visualization/vistoolsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogSplinePath create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogSplinePath::DialogSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogSplinePath), path(VSplinePath())
{
    ui->setupUi(this);
    InitOkCancelApply(ui);
    bOk->setEnabled(false);

    FillComboBoxPoints(ui->comboBoxPoint);
    FillComboBoxLineColors(ui->comboBoxColor);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogSplinePath::PointChanged);
    connect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogSplinePath::currentPointChanged);
    connect(ui->doubleSpinBoxAngle1,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::Angle1Changed);
    connect(ui->doubleSpinBoxAngle2,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::Angle2Changed);
    connect(ui->doubleSpinBoxKasm1,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm1Changed);
    connect(ui->doubleSpinBoxKasm2,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogSplinePath::KAsm2Changed);

    vis = new VisToolSplinePath(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSplinePath::~DialogSplinePath()
{
    DeleteVisualization<VisToolSplinePath>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPath set spline path
 * @param value path
 */
void DialogSplinePath::SetPath(const VSplinePath &value)
{
    this->path = value;
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    for (qint32 i = 0; i < path.CountPoint(); ++i)
    {
        const VSplinePoint &point = path.at(i);
        NewItem(point.P().id(), point.KAsm1(), point.Angle1(), point.KAsm2(), point.Angle2());
    }
    ui->listWidget->setFocus(Qt::OtherFocusReason);
    ui->doubleSpinBoxKcurve->setValue(path.GetKCurve());

    VisToolSplinePath *visPath = qobject_cast<VisToolSplinePath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->setPath(path);
    ui->listWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogSplinePath::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type don't show this id in list
 */
void DialogSplinePath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        if (path.CountPoint() >= 2 && path.at(path.CountPoint()-1).P().id() == id)
        {
            return;
        }

        NewItem(id, 1, 0, 1, 180);
        emit ToolTip(tr("Select point of curve path"));

        SavePath();

        VisToolSplinePath *visPath = qobject_cast<VisToolSplinePath *>(vis);
        SCASSERT(visPath != nullptr);
        visPath->setPath(path);

        if (path.CountPoint() == 1)
        {
            visPath->VisualMode(NULL_ID);
            connect(visPath, &VisToolSplinePath::ToolTip, this, &DialogTool::ShowVisToolTip);
            connect(visPath, &VisToolSplinePath::PathChanged, this, &DialogSplinePath::PathUpdated);
        }
        else
        {
            visPath->RefreshGeometry();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SaveData()
{
    SavePath();

    VisToolSplinePath *visPath = qobject_cast<VisToolSplinePath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->setPath(path);
    visPath->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PointChanged selected another point in list
 * @param row number of row
 */
void DialogSplinePath::PointChanged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    DataPoint(p.P().id(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    EnableFields();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief currentPointChanged changed point in combo box
 * @param index index in list
 */
void DialogSplinePath::currentPointChanged(int index)
{
    quint32 id = qvariant_cast<quint32>(ui->comboBoxPoint->itemData(index));
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(id);
    p.SetP(*point);
    DataPoint(p.P().id(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    EnableFields();
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle1Changed changed first angle
 * @param index index in list
 */
void DialogSplinePath::Angle1Changed(qreal index)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    SCASSERT(item != nullptr);
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetAngle1(index);
    DataPoint(p.P().id(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle2Changed changed second angle
 * @param index index in list
 */
void DialogSplinePath::Angle2Changed(qreal index)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    SCASSERT(item != nullptr);
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetAngle2(index);
    DataPoint(p.P().id(), p.KAsm1(), p.Angle1(), p.KAsm2(), p.Angle2());
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm1Changed changed first coefficient asymmetry
 * @param d value
 */
void DialogSplinePath::KAsm1Changed(qreal d)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetKAsm1(d);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm2Changed changed second coefficient asymmetry
 * @param d value
 */
void DialogSplinePath::KAsm2Changed(qreal d)
{
    qint32 row = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->item( row );
    VSplinePoint p = qvariant_cast<VSplinePoint>(item->data(Qt::UserRole));
    p.SetKAsm2(d);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::ShowDialog(bool click)
{
    if (click == false)
    {
        if (path.CountPoint() >= 3)
        {
            emit ToolTip("");
            DialogAccepted();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::PathUpdated(const VSplinePath &path)
{
    SetPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr);

        VisToolSplinePath *visPath = qobject_cast<VisToolSplinePath *>(vis);
        SCASSERT(visPath != nullptr);

        connect(scene, &VMainGraphicsScene::NewFactor, visPath, &Visualization::SetFactor);
        scene->addItem(visPath);
        visPath->setMode(Mode::Show);
        visPath->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewItem add point to list
 * @param id id
 * @param kAsm1 first coefficient asymmetry
 * @param angle1 first angle in degree
 * @param kAsm2 second coefficient asymmetry
 * @param angle2 second angle in degree
 */
void DialogSplinePath::NewItem(quint32 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(id);
    QListWidgetItem *item = new QListWidgetItem(point->name());
    item->setFont(QFont("Times", 12, QFont::Bold));
    VSplinePoint p(*point.data(), kAsm1, angle1, kAsm2, angle2);
    DataPoint(point->id(), kAsm1, angle1, kAsm2, angle2);
    item->setData(Qt::UserRole, QVariant::fromValue(p));
    ui->listWidget->addItem(item);
    ui->listWidget->setCurrentItem(item);
    if (ui->listWidget->count() >= 2)
    {
        bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        bOk->setEnabled(true);
    }
    EnableFields();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief dataPoint show data of point in fields
 * @param id id
 * @param kAsm1 first coefficient asymmetry
 * @param angle1 first angle of spline
 * @param kAsm2 second coefficient asymmetry
 * @param angle2 second angle of spline
 */
void DialogSplinePath::DataPoint(quint32 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
{
    ui->comboBoxPoint->blockSignals(true);
    ui->doubleSpinBoxAngle1->blockSignals(true);
    ui->doubleSpinBoxAngle2->blockSignals(true);
    ui->doubleSpinBoxKasm1->blockSignals(true);
    ui->doubleSpinBoxKasm2->blockSignals(true);

    ChangeCurrentData(ui->comboBoxPoint, id);
    ui->doubleSpinBoxKasm1->setValue(kAsm1);
    ui->doubleSpinBoxKasm2->setValue(kAsm2);
    ui->doubleSpinBoxAngle2->setValue(angle2);
    ui->doubleSpinBoxAngle1->setValue(angle1);

    ui->comboBoxPoint->blockSignals(false);
    ui->doubleSpinBoxAngle1->blockSignals(false);
    ui->doubleSpinBoxAngle2->blockSignals(false);
    ui->doubleSpinBoxKasm1->blockSignals(false);
    ui->doubleSpinBoxKasm2->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EnableFields enable or disable fields
 */
void DialogSplinePath::EnableFields()
{
    ui->doubleSpinBoxKasm1->setEnabled(true);
    ui->doubleSpinBoxAngle1->setEnabled(true);
    ui->doubleSpinBoxKasm2->setEnabled(true);
    ui->doubleSpinBoxAngle2->setEnabled(true);
    qint32 row = ui->listWidget->currentRow();
    if (row == 0)
    {
        ui->doubleSpinBoxKasm1->setEnabled(false);
        ui->doubleSpinBoxAngle1->setEnabled(false);
        return;
    }
    if (row == ui->listWidget->count()-1)
    {
        ui->doubleSpinBoxKasm2->setEnabled(false);
        ui->doubleSpinBoxAngle2->setEnabled(false);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogSplinePath::SavePath()
{
    path.Clear();
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        path.append( qvariant_cast<VSplinePoint>(item->data(Qt::UserRole)));
    }
    path.SetKCurve(ui->doubleSpinBoxKcurve->value());
}

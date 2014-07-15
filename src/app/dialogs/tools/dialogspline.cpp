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

#include "dialogspline.h"
#include "ui_dialogspline.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogSpline create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogSpline::DialogSpline(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogSpline), number(0), p1(0), p4(0), angle1(0), angle2(0), kAsm1(1),
      kAsm2(1), kCurve(1)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP4);
    connect(ui->comboBoxP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);
    connect(ui->comboBoxP4, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogSpline::PointNameChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogSpline::~DialogSpline()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP1 return id first point of spline
 * @return id
 */
quint32 DialogSpline::getP1() const
{
    return p1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogSpline::ChoosedObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxP1->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP1->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select last point of curve"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxP4->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP4->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
                index = ui->comboBoxP1->currentIndex();
                quint32 p1Id = qvariant_cast<quint32>(ui->comboBoxP1->itemData(index));

                QPointF p1 = data->GeometricObject<const VPointF *>(p1Id)->toQPointF();
                QPointF p4 = data->GeometricObject<const VPointF *>(id)->toQPointF();

                ui->spinBoxAngle1->setValue(static_cast<qint32>(QLineF(p1, p4).angle()));
                ui->spinBoxAngle2->setValue(static_cast<qint32>(QLineF(p4, p1).angle()));
            }
            if (isInitialized == false)
            {
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogSpline::DialogAccepted()
{
    p1 = getCurrentObjectId(ui->comboBoxP1);
    p4 = getCurrentObjectId(ui->comboBoxP4);
    angle1 = ui->spinBoxAngle1->value();
    angle2 = ui->spinBoxAngle2->value();
    kAsm1 = ui->doubleSpinBoxKasm1->value();
    kAsm2 = ui->doubleSpinBoxKasm2->value();
    kCurve = ui->doubleSpinBoxKcurve->value();
    emit DialogClosed(QDialog::Accepted);
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
void DialogSpline::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variable.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setKCurve set coefficient curve
 * @param value value. Can be >= 0.
 */
void DialogSpline::setKCurve(const qreal &value)
{
    kCurve = value;
    ui->doubleSpinBoxKcurve->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setKAsm2 set second coefficient asymmetry
 * @param value value. Can be >= 0.
 */
void DialogSpline::setKAsm2(const qreal &value)
{
    kAsm2 = value;
    ui->doubleSpinBoxKasm2->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setKAsm1 set first coefficient asymmetry
 * @param value value. Can be >= 0.
 */
void DialogSpline::setKAsm1(const qreal &value)
{
    kAsm1 = value;
    ui->doubleSpinBoxKasm1->setValue(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAngle2 set second angle of spline
 * @param value angle in degree
 */
void DialogSpline::setAngle2(const qreal &value)
{
    angle2 = value;
    ui->spinBoxAngle2->setValue(static_cast<qint32>(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAngle1 set first angle of spline
 * @param value angle in degree
 */
void DialogSpline::setAngle1(const qreal &value)
{
    angle1 = value;
    ui->spinBoxAngle1->setValue(static_cast<qint32>(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP4 set id fourth point of spline
 * @param value id
 */
void DialogSpline::setP4(const quint32 &value)
{
    setPointId(ui->comboBoxP4, p4, value, 0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP1 set id first point of spline
 * @param value id
 */
void DialogSpline::setP1(const quint32 &value)
{
    setPointId(ui->comboBoxP1, p1, value, 0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP4 return id fourth point of spline
 * @return id
 */
quint32 DialogSpline::getP4() const
{
    return p4;
}

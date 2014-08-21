/************************************************************************
 **
 **  @file   dialogdetail.cpp
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

#include "dialogdetail.h"

#include <QDebug>

#include "../../geometry/varc.h"
#include "../../geometry/vpointf.h"
#include "../../geometry/vsplinepath.h"
#include "../../container/vcontainer.h"
#include "../../xml/vdomdocument.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogDetail create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogDetail::DialogDetail(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(), details(VDetail()), supplement(true), closed(true)
{
    ui.setupUi(this);
    labelEditNamePoint = ui.labelEditNameDetail;
    ui.labelUnit->setText( VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    ui.labelUnitX->setText(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    ui.labelUnitY->setText(VDomDocument::UnitsToStr(qApp->patternUnit(), true));

    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);
    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCansel != nullptr);
    connect(bCansel, &QPushButton::clicked, this, &DialogTool::DialogRejected);

    flagName = true;//We have default name of detail.
    ChangeColor(labelEditNamePoint, okColor);
    CheckState();

    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogDetail::ObjectChanged);
    connect(ui.doubleSpinBoxBiasX,  static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.doubleSpinBoxBiasY,  static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    connect(ui.checkBoxSeams, &QCheckBox::clicked, this, &DialogDetail::ClickedSeams);
    connect(ui.checkBoxClosed, &QCheckBox::clicked, this, &DialogDetail::ClickedClosed);
    connect(ui.lineEditNameDetail, &QLineEdit::textChanged, this, &DialogDetail::NamePointChanged);

    connect(ui.toolButtonDelete, &QToolButton::clicked, this, &DialogDetail::DeleteItem);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of objects (points, arcs, splines, spline paths)
 * @param type type of object
 */
void DialogDetail::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type != SceneObject::Line && type != SceneObject::Detail)
    {
        switch (type)
        {
            case (SceneObject::Arc):
                NewItem(id, Tool::NodeArc, NodeDetail::Contour);
                break;
            case (SceneObject::Point):
                NewItem(id, Tool::NodePoint, NodeDetail::Contour);
                break;
            case (SceneObject::Spline):
                NewItem(id, Tool::NodeSpline, NodeDetail::Contour);
                break;
            case (SceneObject::SplinePath):
                NewItem(id, Tool::NodeSplinePath, NodeDetail::Contour);
                break;
            case (SceneObject::Line):
            case (SceneObject::Detail):
            default:
                qDebug()<<tr("Got wrong scene object. Ignore.");
                break;
        }
        ui.toolButtonDelete->setEnabled(true);
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::SaveData()
{
    details.Clear();
    for (qint32 i = 0; i < ui.listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui.listWidget->item(i);
        details.append( qvariant_cast<VNodeDetail>(item->data(Qt::UserRole)));
    }
    details.setWidth(ui.doubleSpinBoxSeams->value());
    details.setName(ui.lineEditNameDetail->text());
    details.setSeamAllowance(supplement);
    details.setClosed(closed);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewItem add new object (point, arc, spline or spline path) to list
 * @param id id of object
 * @param typeTool type of tool
 * @param typeNode type of node in detail
 * @param mx offset respect to x
 * @param my offset respect to y
 */
void DialogDetail::NewItem(quint32 id, const Tool &typeTool, const NodeDetail &typeNode,
                           qreal mx, qreal my)
{
    QString name;
    switch (typeTool)
    {
        case (Tool::NodePoint):
        {
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(id);
            name = point->name();
            break;
        }
        case (Tool::NodeArc):
        {
            const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(id);
            name = arc->name();
            break;
        }
        case (Tool::NodeSpline):
        {
            const QSharedPointer<VSpline> spl = data->GeometricObject<VSpline>(id);
            name = spl->name();
            break;
        }
        case (Tool::NodeSplinePath):
        {
            const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(id);
            name = splPath->name();
            break;
        }
        default:
            qDebug()<<"Got wrong tools. Ignore.";
            break;
    }

    QListWidgetItem *item = new QListWidgetItem(name);
    item->setFont(QFont("Times", 12, QFont::Bold));
    VNodeDetail node(id, typeTool, typeNode, mx, my);
    item->setData(Qt::UserRole, QVariant::fromValue(node));
    ui.listWidget->addItem(item);
    ui.listWidget->setCurrentRow(ui.listWidget->count()-1);
    ui.doubleSpinBoxBiasX->blockSignals(true);
    ui.doubleSpinBoxBiasY->blockSignals(true);
    ui.doubleSpinBoxBiasX->setValue(qApp->fromPixel(node.getMx()));
    ui.doubleSpinBoxBiasY->setValue(qApp->fromPixel(node.getMy()));
    ui.doubleSpinBoxBiasX->blockSignals(false);
    ui.doubleSpinBoxBiasY->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDetails set detail
 * @param value detail
 */
void DialogDetail::setDetails(const VDetail &value)
{
    details = value;
    ui.listWidget->clear();
    for (ptrdiff_t i = 0; i < details.CountNode(); ++i)
    {
        NewItem(details.at(i).getId(), details.at(i).getTypeTool(), details.at(i).getTypeNode(), details.at(i).getMx(),
                details.at(i).getMy());
    }
    ui.lineEditNameDetail->setText(details.getName());
    ui.checkBoxSeams->setChecked(details.getSeamAllowance());
    ui.checkBoxClosed->setChecked(details.getClosed());
    ClickedClosed(details.getClosed());
    ClickedSeams(details.getSeamAllowance());
    ui.doubleSpinBoxSeams->setValue(details.getWidth());
    ui.listWidget->setCurrentRow(0);
    ui.listWidget->setFocus(Qt::OtherFocusReason);
    ui.toolButtonDelete->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasXChanged changed value of offset for object respect to x
 * @param d value in mm
 */
void DialogDetail::BiasXChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMx(qApp->toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasYChanged changed value of offset for object respect to y
 * @param d value in mm
 */
void DialogDetail::BiasYChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMy(qApp->toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClickedSeams save supplement of seams for detail
 * @param checked 1 - need supplement, 0 - don't need supplement
 */
void DialogDetail::ClickedSeams(bool checked)
{
    supplement = checked;
    ui.checkBoxClosed->setEnabled(checked);
    ui.doubleSpinBoxSeams->setEnabled(checked);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClickedClosed save closed equdistant or not
 * @param checked 1 - closed, 0 - don't closed
 */
void DialogDetail::ClickedClosed(bool checked)
{
    closed = checked;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ObjectChanged changed new object (point, arc, spline or spline path) form list
 * @param row number of row
 */
void DialogDetail::ObjectChanged(int row)
{
    if (ui.listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui.listWidget->item( row );
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    ui.doubleSpinBoxBiasX->setValue(qApp->fromPixel(node.getMx()));
    ui.doubleSpinBoxBiasY->setValue(qApp->fromPixel(node.getMy()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteItem delete item from list
 */
void DialogDetail::DeleteItem()
{
    qint32 row = ui.listWidget->currentRow();
    delete ui.listWidget->item( row );
}


//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::UpdateList()
{
    /*
     * Does nothing. We redefine this slot because it is only one now way block update list of variable.
     * This dialog doesn't work with formula. Don't delete. Help avoid crash.
     */
}

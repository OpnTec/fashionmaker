/************************************************************************
 **
 **  @file   dialogdetail.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include <QPushButton>
#include <QDebug>

DialogDetail::DialogDetail(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(), details(VDetail()), supplement(true), closed(true)
{
    ui.setupUi(this);
    labelEditNamePoint = ui.labelEditNameDetail;
    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogDetail::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogDetail::DialogRejected);

    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogDetail::ObjectChanged);
    connect(ui.doubleSpinBoxBiasX,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.doubleSpinBoxBiasY,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    connect(ui.checkBoxSeams, &QCheckBox::clicked, this, &DialogDetail::ClickedSeams);
    connect(ui.checkBoxClosed, &QCheckBox::clicked, this, &DialogDetail::ClickedClosed);
    connect(ui.lineEditNameDetail, &QLineEdit::textChanged, this, &DialogDetail::NamePointChanged);
}

void DialogDetail::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (type != Scene::Line && type != Scene::Detail)
    {
        switch (type)
        {
            case (Scene::Arc):
                NewItem(id, Tool::NodeArc, NodeDetail::Contour);
                break;
            case (Scene::Point):
                NewItem(id, Tool::NodePoint, NodeDetail::Contour);
                break;
            case (Scene::Spline):
                NewItem(id, Tool::NodeSpline, NodeDetail::Contour);
                break;
            case (Scene::SplinePath):
                NewItem(id, Tool::NodeSplinePath, NodeDetail::Contour);
                break;
            default:
                qWarning()<<tr("Got wrong scene object. Ignore.");
                break;
        }
        this->show();
    }
}

void DialogDetail::DialogAccepted()
{
    details.Clear();
    for (qint32 i = 0; i < ui.listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui.listWidget->item(i);
        details.append( qvariant_cast<VNodeDetail>(item->data(Qt::UserRole)));
    }
    details.setWidth(ui.doubleSpinBoxSeams->value());
    details.setName(ui.lineEditNameDetail->text());
    details.setSupplement(supplement);
    details.setClosed(closed);
    emit ToolTip("");
    emit DialogClosed(QDialog::Accepted);
}

void DialogDetail::NewItem(qint64 id, const Tool::Tools &typeTool, const NodeDetail::NodeDetails &typeNode, qreal mx,
                           qreal my)
{
    QString name;
    switch (typeTool)
    {
        case (Tool::NodePoint):
        {
            const VPointF *point = data->GeometricObject<const VPointF *>(id);
            name = point->name();
            break;
        }
        case (Tool::NodeArc):
        {
            const VArc *arc = data->GeometricObject<const VArc *>(id);
            name = arc->name();
            break;
        }
        case (Tool::NodeSpline):
        {
            const VSpline *spl = data->GeometricObject<const VSpline *>(id);
            name = spl->name();
            break;
        }
        case (Tool::NodeSplinePath):
        {
            const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(id);
            name = splPath->name();
            break;
        }
        default:
            qWarning()<<tr("Got wrong tools. Ignore.");
            break;
    }

    QListWidgetItem *item = new QListWidgetItem(name);
    item->setFont(QFont("Times", 12, QFont::Bold));
    VNodeDetail node(id, typeTool, typeNode, mx, my);
    item->setData(Qt::UserRole, QVariant::fromValue(node));
    ui.listWidget->addItem(item);
    disconnect(ui.doubleSpinBoxBiasX,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    disconnect(ui.doubleSpinBoxBiasY,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    ui.doubleSpinBoxBiasX->setValue(toMM(node.getMx()));
    ui.doubleSpinBoxBiasY->setValue(toMM(node.getMy()));
    connect(ui.doubleSpinBoxBiasX,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.doubleSpinBoxBiasY,  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
}

void DialogDetail::setDetails(const VDetail &value)
{
    details = value;
    ui.listWidget->clear();
    for (ptrdiff_t i = 0; i < details.CountNode(); ++i)
    {
        NewItem(details[i].getId(), details[i].getTypeTool(), details[i].getTypeNode(), details[i].getMx(),
                details[i].getMy());
    }
    ui.lineEditNameDetail->setText(details.getName());
    ui.checkBoxSeams->setChecked(details.getSupplement());
    ui.checkBoxClosed->setChecked(details.getClosed());
    ui.doubleSpinBoxSeams->setValue(details.getWidth());
    ui.listWidget->setCurrentRow(0);
    ui.listWidget->setFocus(Qt::OtherFocusReason);
}

void DialogDetail::BiasXChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMx(toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

void DialogDetail::BiasYChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMy(toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

void DialogDetail::ClickedSeams(bool checked)
{
    supplement = checked;
    ui.checkBoxClosed->setEnabled(checked);
    ui.doubleSpinBoxSeams->setEnabled(checked);
}

void DialogDetail::ClickedClosed(bool checked)
{
    closed = checked;
}

void DialogDetail::ObjectChanged(int row)
{
    if (ui.listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui.listWidget->item( row );
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    ui.doubleSpinBoxBiasX->setValue(toMM(node.getMx()));
    ui.doubleSpinBoxBiasY->setValue(toMM(node.getMy()));
}

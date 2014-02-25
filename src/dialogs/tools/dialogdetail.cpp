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

    flagName = true;//We have default name of detail.
    QPalette palette = labelEditNamePoint->palette();
    palette.setColor(labelEditNamePoint->foregroundRole(), QColor(76, 76, 76));
    labelEditNamePoint->setPalette(palette);
    CheckState();

    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogDetail::DialogRejected);

    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogDetail::ObjectChanged);
    connect(ui.spinBoxBiasX,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.spinBoxBiasY,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    connect(ui.checkBoxSeams, &QCheckBox::clicked, this, &DialogDetail::ClickedSeams);
    connect(ui.checkBoxClosed, &QCheckBox::clicked, this, &DialogDetail::ClickedClosed);
    connect(ui.lineEditNameDetail, &QLineEdit::textChanged, this, &DialogDetail::NamePointChanged);

    connect(ui.toolButtonDelete, &QToolButton::clicked, this, &DialogDetail::DeleteItem);
}

void DialogDetail::ChoosedObject(quint32 id, const Scene::Scenes &type)
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
        ui.toolButtonDelete->setEnabled(true);
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
    details.setWidth(ui.spinBoxSeams->value());
    details.setName(ui.lineEditNameDetail->text());
    details.setSeamAllowance(supplement);
    details.setClosed(closed);
    emit ToolTip("");
    emit DialogClosed(QDialog::Accepted);
}

void DialogDetail::NewItem(quint32 id, const Tool::Tools &typeTool, const NodeDetail::NodeDetails &typeNode, qreal mx,
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
    ui.listWidget->setCurrentRow(ui.listWidget->count()-1);
    disconnect(ui.spinBoxBiasX,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    disconnect(ui.spinBoxBiasY,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    ui.spinBoxBiasX->setValue(static_cast<qint32>(toMM(node.getMx())));
    ui.spinBoxBiasY->setValue(static_cast<qint32>(toMM(node.getMy())));
    connect(ui.spinBoxBiasX,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.spinBoxBiasY,  static_cast<void (QSpinBox::*)(qint32)>(&QSpinBox::valueChanged),
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
    ui.checkBoxSeams->setChecked(details.getSeamAllowance());
    ui.checkBoxClosed->setChecked(details.getClosed());
    ClickedClosed(details.getClosed());
    ClickedSeams(details.getSeamAllowance());
    ui.spinBoxSeams->setValue(static_cast<qint32>(details.getWidth()));
    ui.listWidget->setCurrentRow(0);
    ui.listWidget->setFocus(Qt::OtherFocusReason);
    ui.toolButtonDelete->setEnabled(true);
}

void DialogDetail::BiasXChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    Q_CHECK_PTR(item);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMx(toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

void DialogDetail::BiasYChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    Q_CHECK_PTR(item);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMy(toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

void DialogDetail::ClickedSeams(bool checked)
{
    supplement = checked;
    ui.checkBoxClosed->setEnabled(checked);
    ui.spinBoxSeams->setEnabled(checked);
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
    ui.spinBoxBiasX->setValue(static_cast<qint32>(toMM(node.getMx())));
    ui.spinBoxBiasY->setValue(static_cast<qint32>(toMM(node.getMy())));
}

void DialogDetail::DeleteItem()
{
    qint32 row = ui.listWidget->currentRow();
    delete ui.listWidget->item( row );
}

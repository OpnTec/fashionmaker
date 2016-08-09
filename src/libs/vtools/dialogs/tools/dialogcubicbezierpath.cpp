/************************************************************************
 **
 **  @file   dialogcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
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

#include "dialogcubicbezierpath.h"

#include <QColor>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPointer>
#include <QPushButton>
#include <QSharedPointer>
#include <QVariant>
#include <QVector>
#include <Qt>
#include <new>

#include "../../visualization/path/vistoolcubicbezierpath.h"
#include "../../visualization/visualization.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "dialogtool.h"
#include "ui_dialogcubicbezierpath.h"

class QWidget;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#endif

//---------------------------------------------------------------------------------------------------------------------
DialogCubicBezierPath::DialogCubicBezierPath(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogCubicBezierPath),
      path(),
      newDuplicate(-1)
{
    ui->setupUi(this);

    InitOkCancelApply(ui);
    bOk->setEnabled(false);

    FillComboBoxPoints(ui->comboBoxPoint);
    FillComboBoxLineColors(ui->comboBoxColor);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogCubicBezierPath::PointChanged);
    connect(ui->comboBoxPoint,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogCubicBezierPath::currentPointChanged);

    vis = new VisToolCubicBezierPath(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCubicBezierPath::~DialogCubicBezierPath()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath DialogCubicBezierPath::GetPath() const
{
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::SetPath(const VCubicBezierPath &value)
{
    this->path = value;
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    for (qint32 i = 0; i < path.CountPoints(); ++i)
    {
        NewItem(path.at(i));
    }
    ui->listWidget->setFocus(Qt::OtherFocusReason);
    ui->lineEditSplPathName->setText(path.name());

    auto visPath = qobject_cast<VisToolCubicBezierPath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->setPath(path);
    ui->listWidget->blockSignals(false);

    if (ui->listWidget->count() > 0)
    {
        ui->listWidget->setCurrentRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCubicBezierPath::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        if (AllPathBackboneIds().contains(id))
        {
            return;
        }

        const auto point = data->GeometricObject<VPointF>(id);
        NewItem(*point);

        SavePath();

        auto visPath = qobject_cast<VisToolCubicBezierPath *>(vis);
        SCASSERT(visPath != nullptr);
        visPath->setPath(path);

        if (path.CountPoints() == 1)
        {
            visPath->VisualMode(NULL_ID);
            VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
            SCASSERT(window != nullptr);
            connect(visPath, &VisToolCubicBezierPath::ToolTip, window, &VAbstractMainWindow::ShowToolTip);
        }
        else
        {
            visPath->RefreshGeometry();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::ShowDialog(bool click)
{
    if (click == false)
    {
        const int size = path.CountPoints();
        if (size >= 7)
        {
            if (size - VCubicBezierPath::SubSplPointsCount(path.CountSubSpl()) == 0)
            {// Accept only if all subpaths are completed
                emit ToolTip("");

                if (not data->IsUnique(path.name()))
                {
                    path.SetDuplicate(DNumber(path.name()));
                }

                DialogAccepted();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::ShowVisualization()
{
    AddVisualization<VisToolCubicBezierPath>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::SaveData()
{
    const quint32 d = path.GetDuplicate();//Save previous value
    SavePath();
    newDuplicate <= -1 ? path.SetDuplicate(d) : path.SetDuplicate(static_cast<quint32>(newDuplicate));

    auto visPath = qobject_cast<VisToolCubicBezierPath *>(vis);
    SCASSERT(visPath != nullptr);
    visPath->setPath(path);
    visPath->SetMode(Mode::Show);
    visPath->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::PointChanged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }

    const auto p = qvariant_cast<VPointF>(ui->listWidget->item(row)->data(Qt::UserRole));
    DataPoint(p);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::currentPointChanged(int index)
{
    const quint32 id = qvariant_cast<quint32>(ui->comboBoxPoint->itemData(index));
    QListWidgetItem *item = ui->listWidget->item( ui->listWidget->currentRow() );
    const auto point = data->GeometricObject<VPointF>(id);
    DataPoint(*point);
    item->setData(Qt::UserRole, QVariant::fromValue(*point));

    QColor color = okColor;
    if (not IsPathValid())
    {
        flagError = false;
        color = errorColor;

        ui->lineEditSplPathName->setText(tr("Invalid spline path"));
    }
    else
    {
        flagError = true;
        color = okColor;

        auto first = qvariant_cast<VPointF>(ui->listWidget->item(0)->data(Qt::UserRole));
        auto last = qvariant_cast<VPointF>(ui->listWidget->item(ui->listWidget->count()-1)->data(Qt::UserRole));

        if (first.id() == path.at(0).id() && last.id() == path.at(path.CountPoints()-1).id())
        {
            newDuplicate = -1;
            ui->lineEditSplPathName->setText(path.name());
        }
        else
        {
            VCubicBezierPath newPath = ExtractPath();

            if (not data->IsUnique(newPath.name()))
            {
                newDuplicate = static_cast<qint32>(DNumber(newPath.name()));
                newPath.SetDuplicate(static_cast<quint32>(newDuplicate));
            }

            ui->lineEditSplPathName->setText(newPath.name());
        }
    }
    ChangeColor(ui->labelName, color);
    ChangeColor(ui->labelPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::NewItem(const VPointF &point)
{
    auto item = new QListWidgetItem(point.name());
    item->setFont(QFont("Times", 12, QFont::Bold));
    item->setData(Qt::UserRole, QVariant::fromValue(point));

    ui->listWidget->addItem(item);
    ui->listWidget->setCurrentItem(item);
    if (ui->listWidget->count() >= 7)
    {
        bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        bOk->setEnabled(true);
    }

    DataPoint(point);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::DataPoint(const VPointF &p)
{
    ui->comboBoxPoint->blockSignals(true);
    ChangeCurrentData(ui->comboBoxPoint, p.id());
    ui->comboBoxPoint->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCubicBezierPath::SavePath()
{
    path.Clear();
    path = ExtractPath();
}

//---------------------------------------------------------------------------------------------------------------------
QSet<quint32> DialogCubicBezierPath::AllPathBackboneIds() const
{
    QVector<quint32> points;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        points.append(qvariant_cast<VPointF>(ui->listWidget->item(i)->data(Qt::UserRole)).id());
    }

    QSet<quint32> ids;
    const qint32 count = VCubicBezierPath::CountSubSpl(points.size());// Count subpaths
    for (qint32 i = 1; i <= count; ++i)
    {
        const qint32 base = VCubicBezierPath::SubSplOffset(i);
        ids.insert(points.at(base));// The first subpath's point
        ids.insert(points.at(base + 3));// The last subpath's point
    }

    return ids;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogCubicBezierPath::IsPathValid() const
{
    if (path.CountPoints() < 7)
    {
        return false;
    }

    return (AllPathBackboneIds().size() == path.CountSubSpl() + 1);
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath DialogCubicBezierPath::ExtractPath() const
{
    QVector<VPointF> points;
    for (qint32 i = 0; i < ui->listWidget->count(); ++i)
    {
        points.append(qvariant_cast<VPointF>(ui->listWidget->item(i)->data(Qt::UserRole)));
    }
    return VCubicBezierPath(points);
}

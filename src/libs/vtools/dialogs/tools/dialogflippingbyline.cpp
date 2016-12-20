/************************************************************************
 **
 **  @file   dialogflippingbyline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#include "dialogflippingbyline.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPointF>
#include <QPointer>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSharedPointer>
#include <QStringList>
#include <QToolButton>
#include <Qt>
#include <new>

#include "../../visualization/visualization.h"
#include "../../visualization/line/operation/vistoolflippingbyline.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../qmuparser/qmudef.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "ui_dialogflippingbyline.h"

//---------------------------------------------------------------------------------------------------------------------
DialogFlippingByLine::DialogFlippingByLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogFlippingByLine),
      objects(),
      stage1(true),
      m_suffix()
{
    ui->setupUi(this);

    ui->lineEditSuffix->setText(qApp->getCurrentDocument()->GenerateSuffix());

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxFirstLinePoint);
    FillComboBoxPoints(ui->comboBoxSecondLinePoint);

    flagName = true;
    CheckState();

    connect(ui->lineEditSuffix, &QLineEdit::textChanged, this, &DialogFlippingByLine::SuffixChanged);
    connect(ui->comboBoxFirstLinePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogFlippingByLine::PointChanged);
    connect(ui->comboBoxSecondLinePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogFlippingByLine::PointChanged);

    vis = new VisToolFlippingByLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogFlippingByLine::~DialogFlippingByLine()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogFlippingByLine::GetFirstLinePointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstLinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::SetFirstLinePointId(quint32 value)
{
    ChangeCurrentData(ui->comboBoxFirstLinePoint, value);
    VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
    SCASSERT(operation != nullptr)
    operation->SetFirstLinePointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogFlippingByLine::GetSecondLinePointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondLinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::SetSecondLinePointId(quint32 value)
{
    ChangeCurrentData(ui->comboBoxSecondLinePoint, value);
    VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
    SCASSERT(operation != nullptr);
    operation->SetSecondLinePointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogFlippingByLine::GetSuffix() const
{
    return m_suffix;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::SetSuffix(const QString &value)
{
    m_suffix = value;
    ui->lineEditSuffix->setText(value);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> DialogFlippingByLine::GetObjects() const
{
    return objects.toVector();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::ShowDialog(bool click)
{
    if (stage1 && not click)
    {
        if (objects.isEmpty())
        {
            return;
        }

        stage1 = false;

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr)
        scene->clearSelection();

        VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
        SCASSERT(operation != nullptr)
        operation->SetObjects(objects.toVector());
        operation->VisualMode();

        scene->ToggleArcSelection(false);
        scene->ToggleElArcSelection(false);
        scene->ToggleSplineSelection(false);
        scene->ToggleSplinePathSelection(false);

        scene->ToggleArcHover(false);
        scene->ToggleElArcHover(false);
        scene->ToggleSplineHover(false);
        scene->ToggleSplinePathHover(false);

        emit ToolTip(tr("Select first line point"));
    }
    else if (not stage1 && prepare && click)
    {
        setModal(true);
        emit ToolTip("");
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not stage1 && not prepare)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (objects.contains(id))
                    {
                        emit ToolTip(tr("Select first line point that is not part of the list of objects"));
                        return;
                    }

                    if (SetObject(id, ui->comboBoxFirstLinePoint, tr("Select second line point")))
                    {
                        number++;
                        VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
                        SCASSERT(operation != nullptr)
                        operation->SetFirstLinePointId(id);
                        operation->RefreshGeometry();
                    }
                    break;
                case 1:
                    if (objects.contains(id))
                    {
                        emit ToolTip(tr("Select second line point that is not part of the list of objects"));
                        return;
                    }

                    if (getCurrentObjectId(ui->comboBoxFirstLinePoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondLinePoint, ""))
                        {
                            if (flagError)
                            {
                                number = 0;
                                prepare = true;

                                VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
                                SCASSERT(operation != nullptr)
                                operation->SetSecondLinePointId(id);
                                operation->RefreshGeometry();
                            }
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
void DialogFlippingByLine::SelectedObject(bool selected, quint32 object, quint32 tool)
{
    Q_UNUSED(tool)
    if (stage1)
    {
        if (selected)
        {
            if (not objects.contains(object))
            {
                objects.append(object);
            }
        }
        else
        {
            objects.removeOne(object);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::SuffixChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        const QString suffix = edit->text();
        if (suffix.isEmpty())
        {
            flagName = false;
            ChangeColor(ui->labelSuffix, Qt::red);
            CheckState();
            return;
        }
        else
        {
            if (m_suffix != suffix)
            {
                QRegularExpression rx(NameRegExp());
                const QStringList uniqueNames = data->AllUniqueNames();
                for (int i=0; i < uniqueNames.size(); ++i)
                {
                    const QString name = uniqueNames.at(i) + suffix;
                    if (not rx.match(name).hasMatch() || not data->IsUnique(name))
                    {
                        flagName = false;
                        ChangeColor(ui->labelSuffix, Qt::red);
                        CheckState();
                        return;
                    }
                }
            }
        }

        flagName = true;
        ChangeColor(ui->labelSuffix, okColor);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(flagError && flagName);
    SCASSERT(bApply != nullptr)
    bApply->setEnabled(bOk->isEnabled());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::ShowVisualization()
{
    AddVisualization<VisToolFlippingByLine>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::SaveData()
{
    m_suffix = ui->lineEditSuffix->text();

    VisToolFlippingByLine *operation = qobject_cast<VisToolFlippingByLine *>(vis);
    SCASSERT(operation != nullptr)

    operation->SetObjects(objects.toVector());
    operation->SetFirstLinePointId(GetFirstLinePointId());
    operation->SetSecondLinePointId(GetSecondLinePointId());
    operation->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFlippingByLine::PointChanged()
{
    QColor color = okColor;
    flagError = true;
    ChangeColor(ui->labelFirstLinePoint, color);
    ChangeColor(ui->labelSecondLinePoint, color);

    if (getCurrentObjectId(ui->comboBoxFirstLinePoint) == getCurrentObjectId(ui->comboBoxSecondLinePoint))
    {
        flagError = false;
        color = errorColor;
        ChangeColor(ui->labelFirstLinePoint, color);
        ChangeColor(ui->labelSecondLinePoint, color);
    }
    else if (objects.contains(getCurrentObjectId(ui->comboBoxFirstLinePoint)))
    {
        flagError = false;
        color = errorColor;
        ChangeColor(ui->labelFirstLinePoint, color);
    }
    else if (objects.contains(getCurrentObjectId(ui->comboBoxSecondLinePoint)))
    {
        flagError = false;
        color = errorColor;
        ChangeColor(ui->labelSecondLinePoint, color);
    }

    CheckState();
}

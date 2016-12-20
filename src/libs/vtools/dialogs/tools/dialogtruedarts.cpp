/************************************************************************
 **
 **  @file   dialogtruedarts.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 6, 2015
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

#include "dialogtruedarts.h"

#include <QColor>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSet>
#include <Qt>

#include "../../visualization/visualization.h"
#include "../../visualization/line/vistooltruedarts.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../qmuparser/qmudef.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "dialogtool.h"
#include "ui_dialogtruedarts.h"

class QWidget;

//---------------------------------------------------------------------------------------------------------------------
DialogTrueDarts::DialogTrueDarts(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogTrueDarts), d1PointName(), d2PointName(), ch1(NULL_ID),
      ch2(NULL_ID), flagName1(true), flagName2(true)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditFirstNewDartPoint->setClearButtonEnabled(true);
    ui->lineEditSecondNewDartPoint->setClearButtonEnabled(true);
#endif

    const QString name1 = qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel);
    const QString name2 = qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel, name1);
    ui->lineEditFirstNewDartPoint->setText(name1);
    ui->lineEditSecondNewDartPoint->setText(name2);

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxs(ch1, ch2);

    connect(ui->lineEditFirstNewDartPoint, &QLineEdit::textChanged, this, &DialogTrueDarts::NameDartPoint1Changed);
    connect(ui->lineEditSecondNewDartPoint, &QLineEdit::textChanged, this, &DialogTrueDarts::NameDartPoint2Changed);
    connect(ui->comboBoxFirstBasePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTrueDarts::PointNameChanged);
    connect(ui->comboBoxSecondBasePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTrueDarts::PointNameChanged);
    connect(ui->comboBoxFirstDartPoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTrueDarts::PointNameChanged);
    connect(ui->comboBoxSecondDartPoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTrueDarts::PointNameChanged);
    connect(ui->comboBoxThirdDartPoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTrueDarts::PointNameChanged);

    vis = new VisToolTrueDarts(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogTrueDarts::~DialogTrueDarts()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTrueDarts::GetFirstNewDartPointName()
{
    return d1PointName;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTrueDarts::GetSecondNewDartPointName()
{
    return d2PointName;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetNewDartPointNames(const QString &firstPoint, const QString &secondPoint)
{
    ui->lineEditFirstNewDartPoint->blockSignals(true);
    ui->lineEditSecondNewDartPoint->blockSignals(true);

    d1PointName = firstPoint;
    ui->lineEditFirstNewDartPoint->setText(d1PointName);

    d2PointName = secondPoint;
    ui->lineEditSecondNewDartPoint->setText(d2PointName);

    ui->lineEditSecondNewDartPoint->blockSignals(false);
    ui->lineEditFirstNewDartPoint->blockSignals(false);

    CheckName(ui->lineEditFirstNewDartPoint, ui->labelFirstNewDartPoint, d1PointName, d2PointName,
                ui->lineEditSecondNewDartPoint, flagName1);
    CheckName(ui->lineEditSecondNewDartPoint, ui->labelSecondNewDartPoint, d1PointName, d2PointName,
                ui->lineEditFirstNewDartPoint, flagName2);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTrueDarts::GetFirstBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetFirstBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstBasePoint, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)
    points->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTrueDarts::GetSecondBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetSecondBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondBasePoint, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)
    points->setObject2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTrueDarts::GetFirstDartPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstDartPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetFirstDartPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstDartPoint, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)
    points->setD1PointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTrueDarts::GetSecondDartPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondDartPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetSecondDartPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondDartPoint, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)
    points->setD2PointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTrueDarts::GetThirdDartPointId() const
{
    return getCurrentObjectId(ui->comboBoxThirdDartPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetThirdDartPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxThirdDartPoint, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)
    points->setD3PointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SetChildrenId(const quint32 &ch1, const quint32 &ch2)
{
    this->ch1 = ch1;
    this->ch2 = ch2;
    FillComboBoxs(ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
            SCASSERT(points != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstBasePoint, tr("Select the second base point")))
                    {
                        number++;
                        points->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstBasePoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondBasePoint, tr("Select the first dart point")))
                        {
                            number++;
                            points->setObject2Id(id);
                            points->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondBasePoint));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxFirstDartPoint, tr("Select the second dart point")))
                        {
                            number++;
                            points->setD1PointId(id);
                            points->RefreshGeometry();
                        }
                    }
                    break;
                }
                case 3:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxFirstDartPoint));
                    set.insert(id);

                    if (set.size() == 4)
                    {
                        if (SetObject(id, ui->comboBoxSecondDartPoint, tr("Select the third dart point")))
                        {
                            number++;
                            points->setD2PointId(id);
                            points->RefreshGeometry();
                        }
                    }
                    break;
                }
                case 4:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondBasePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxFirstDartPoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondDartPoint));
                    set.insert(id);

                    if (set.size() == 5)
                    {
                        if (SetObject(id, ui->comboBoxThirdDartPoint, ""))
                        {
                            points->setD3PointId(id);
                            points->RefreshGeometry();
                            prepare = true;
                            DialogAccepted();
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
void DialogTrueDarts::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstBasePoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondBasePoint));
    set.insert(getCurrentObjectId(ui->comboBoxFirstDartPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondDartPoint));
    set.insert(getCurrentObjectId(ui->comboBoxThirdDartPoint));

    QColor color = okColor;
    if (set.size() != 5)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelFirstBasePoint, color);
    ChangeColor(ui->labelSecondBasePoint, color);
    ChangeColor(ui->labelFirstDartPoint, color);
    ChangeColor(ui->labelSecondDartPoint, color);
    ChangeColor(ui->labelThirdDartPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::NameDartPoint1Changed()
{
    NameChanged(ui->labelFirstNewDartPoint, d1PointName, d2PointName, ui->lineEditSecondNewDartPoint, flagName1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::NameDartPoint2Changed()
{
    NameChanged(ui->labelSecondNewDartPoint, d1PointName, d2PointName, ui->lineEditFirstNewDartPoint, flagName2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::ShowVisualization()
{
    AddVisualization<VisToolTrueDarts>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::SaveData()
{
    d1PointName = ui->lineEditFirstNewDartPoint->text();
    d2PointName = ui->lineEditSecondNewDartPoint->text();

    VisToolTrueDarts *points = qobject_cast<VisToolTrueDarts *>(vis);
    SCASSERT(points != nullptr)

    points->setObject1Id(GetFirstBasePointId());
    points->setObject2Id(GetSecondBasePointId());
    points->setD1PointId(GetFirstDartPointId());
    points->setD2PointId(GetSecondDartPointId());
    points->setD3PointId(GetThirdDartPointId());
    points->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(flagName1 && flagName2 && flagError);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::NameChanged(QLabel *labelEditNamePoint, const QString &pointD1Name, const QString &pointD2Name,
                                  QLineEdit* secondPointName, bool &flagName)
{
    SCASSERT(labelEditNamePoint != nullptr)
    SCASSERT(secondPointName != nullptr)
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        CheckName(edit, labelEditNamePoint, pointD1Name, pointD2Name, secondPointName, flagName);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::FillComboBoxs(const quint32 &ch1, const quint32 &ch2)
{
    FillComboBoxPoints(ui->comboBoxFirstBasePoint, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxPoints(ui->comboBoxSecondBasePoint, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxPoints(ui->comboBoxFirstDartPoint, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxPoints(ui->comboBoxSecondDartPoint, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxPoints(ui->comboBoxThirdDartPoint, FillComboBox::NoChildren, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTrueDarts::CheckName(QLineEdit *edit, QLabel *labelEditNamePoint, const QString &pointD1Name,
                                const QString &pointD2Name, QLineEdit *secondPointName, bool &flagName)
{
    SCASSERT(labelEditNamePoint != nullptr)
    SCASSERT(secondPointName != nullptr)
    SCASSERT(edit != nullptr)

    const QString name = edit->text();
    const QString secondName = secondPointName->text();
    QRegularExpression rx(NameRegExp());
    if (name.isEmpty()
            || secondName == name
            || (pointD1Name != name && pointD2Name != name && data->IsUnique(name) == false)
            || rx.match(name).hasMatch() == false)
    {
        flagName = false;
        ChangeColor(labelEditNamePoint, Qt::red);
    }
    else
    {
        flagName = true;
        ChangeColor(labelEditNamePoint, okColor);
    }
}

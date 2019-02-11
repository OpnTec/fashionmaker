/************************************************************************
 **
 **  @file   dialogpin.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "dialogpin.h"
#include "ui_dialogpin.h"
#include "visualization/line/vistoolspecialpoint.h"
#include "../../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
DialogPin::DialogPin(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogPin),
      m_showMode(false),
      m_flagPoint(false),
      m_flagError(false)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    FillComboBoxPoints(ui->comboBoxPoint);

    connect(ui->comboBoxPiece, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        CheckPieces();
    });

    vis = new VisToolSpecialPoint(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPin::~DialogPin()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::EnbleShowMode(bool disable)
{
    m_showMode = disable;
    ui->comboBoxPiece->setDisabled(m_showMode);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPin::GetPieceId() const
{
    return getCurrentObjectId(ui->comboBoxPiece);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::SetPieceId(quint32 id)
{
    if (ui->comboBoxPiece->count() <= 0)
    {
        ui->comboBoxPiece->addItem(data->GetPiece(id).GetName(), id);
    }
    else
    {
        const qint32 index = ui->comboBoxPiece->findData(id);
        if (index != -1)
        {
            ui->comboBoxPiece->setCurrentIndex(index);
        }
        else
        {
            ui->comboBoxPiece->setCurrentIndex(0);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPin::GetPointId() const
{
    return getCurrentObjectId(ui->comboBoxPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::SetPointId(quint32 id)
{
    setCurrentPointId(ui->comboBoxPoint, id);

    VisToolSpecialPoint *point = qobject_cast<VisToolSpecialPoint *>(vis);
    SCASSERT(point != nullptr)
    point->setObject1Id(id);

    CheckPoint();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::SetPiecesList(const QVector<quint32> &list)
{
    FillComboBoxPiecesList(ui->comboBoxPiece, list);

    if (list.isEmpty())
    {
        qWarning() << tr("The list of pieces is empty. Please, first create at least one piece for current pattern "
                         "piece.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not prepare)
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxPoint, QString()))
            {
                vis->VisualMode(id);
                CheckPoint();
                prepare = true;
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::ShowVisualization()
{
    AddVisualization<VisToolSpecialPoint>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::CheckPieces()
{
    if (not m_showMode)
    {
        QColor color;
        if (ui->comboBoxPiece->count() <= 0 || ui->comboBoxPiece->currentIndex() == -1)
        {
            m_flagError = false;
            color = errorColor;
        }
        else
        {
            m_flagError = true;
            color = OkColor(this);
        }
        ChangeColor(ui->labelPiece, color);
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPin::CheckPoint()
{
    QColor color;
    if (ui->comboBoxPoint->currentIndex() != -1)
    {
        m_flagPoint = true;
        color = OkColor(this);
    }
    else
    {
        m_flagPoint = false;
        color = errorColor;
    }
    ChangeColor(ui->labelPoint, color);
    CheckState();
}

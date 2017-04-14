/************************************************************************
 **
 **  @file   dialoginsertnode.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 3, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "dialoginsertnode.h"
#include "ui_dialoginsertnode.h"

//---------------------------------------------------------------------------------------------------------------------
DialogInsertNode::DialogInsertNode(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogInsertNode),
      m_node(),
      m_flagItem(false)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    CheckPieces();
    CheckItem();

    connect(ui->comboBoxPiece, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
    {
        CheckPieces();
    });
}

//---------------------------------------------------------------------------------------------------------------------
DialogInsertNode::~DialogInsertNode()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::SetPiecesList(const QVector<quint32> &list)
{
    FillComboBoxPiecesList(ui->comboBoxPiece, list);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogInsertNode::GetPieceId() const
{
    return getCurrentObjectId(ui->comboBoxPiece);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::SetPieceId(quint32 id)
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
VPieceNode DialogInsertNode::GetNode() const
{
    return m_node;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::SetNode(const VPieceNode &node)
{
    m_node = node;
    m_flagItem = true;
    QString name = tr("Uknown");
    try
    {
        name = qApp->TrVars()->InternalVarToUser(data->GetGObject(m_node.GetId())->name());
    }
    catch (const VExceptionBadId &)
    {
        m_flagItem = false;
        // Broken id
    }

    ui->labelItemName->setText(name);
    ui->labelItemName->setToolTip(name);

    CheckItem();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not prepare)
    {
        VPieceNode node;
        switch (type)
        {
            case SceneObject::Arc:
                node = VPieceNode(id, Tool::NodeArc);
                break;
            case SceneObject::ElArc:
                node = VPieceNode(id, Tool::NodeElArc);
                break;
            case SceneObject::Point:
                node = VPieceNode(id, Tool::NodePoint);
                break;
            case SceneObject::Spline:
                node = VPieceNode(id, Tool::NodeSpline);
                break;
            case SceneObject::SplinePath:
                node = VPieceNode(id, Tool::NodeSplinePath);
                break;
            case (SceneObject::Line):
            case (SceneObject::Detail):
            case (SceneObject::Unknown):
            default:
                qDebug() << "Got wrong scene object. Ignore.";
                return;
        }

        node.SetExcluded(true);
        SetNode(node);

        prepare = true;
        this->setModal(true);
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(m_flagItem && flagError);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::CheckPieces()
{
    QColor color = okColor;
    if (ui->comboBoxPiece->count() <= 0 || ui->comboBoxPiece->currentIndex() == -1)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelPiece, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogInsertNode::CheckItem()
{
    QColor color = okColor;
    m_flagItem ? color = okColor : color = errorColor;
    ChangeColor(ui->labelItem, color);
    CheckState();
}

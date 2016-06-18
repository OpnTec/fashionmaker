/************************************************************************
 **
 **  @file   vpatternconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 12, 2014
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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>

#include "vpatternpiecedata.h"
#include "dialogpatternpiecedata.h"

//---------------------------------------------------------------------------------------------------------------------
DialogPatternPieceData::DialogPatternPieceData(const VContainer *data, const quint32 &toolId, QWidget *parent) :
    DialogTool(data, toolId, parent)
{
    // main dialog layout
    QVBoxLayout* pLayout = new QVBoxLayout(this);

    // widgets layout
    QHBoxLayout* pWidgetsLayout = new QHBoxLayout;

    setWindowTitle(tr("Pattern piece data"));
    QFormLayout* pFormLayout = new QFormLayout;
    QLabel* plb;

    plb = new QLabel(tr("Letter") + ":");
    m_pleLetter = new QLineEdit;
    m_pleLetter->setMaxLength(3);
    pFormLayout->addRow(plb, m_pleLetter);

    plb = new QLabel(tr("Name") + ":");
    m_pleName = new QLineEdit;
    m_pleName->setMaxLength(15);
    pFormLayout->addRow(plb, m_pleName);

    pWidgetsLayout->addLayout(pFormLayout);

    QGroupBox* pgb = new QGroupBox(tr("Material/Cut number/Placement"));
    QGridLayout* pLayoutGB = new QGridLayout(pgb);

    pFormLayout = new QFormLayout;

    plb = new QLabel(tr("Material type") + ":");
    m_pcbMaterial = new QComboBox;
    m_pcbMaterial->setFixedWidth(200);
    m_qslMaterials << tr("Fabric") << tr("Lining") << tr("Interfacing") << tr("Interlining");
    m_pcbMaterial->addItems(m_qslMaterials);
    m_pcbMaterial->setEditable(true);
    pFormLayout->addRow(plb, m_pcbMaterial);

    plb = new QLabel(tr("Cut number") + ":");
    m_psbCutNumber = new QSpinBox;
    m_psbCutNumber->setRange(0, 1000);
    pFormLayout->addRow(plb, m_psbCutNumber);

    plb = new QLabel(tr("Placement type") + ":");
    m_pcbPlacement = new QComboBox;
    m_qslPlacements << tr("None") << tr("Cut on fold");
    m_pcbPlacement->addItems(m_qslPlacements);
    pFormLayout->addRow(plb, m_pcbPlacement);
    pLayoutGB->addLayout(pFormLayout, 0, 0);

    QHBoxLayout* pMCPButtonLayout = new QHBoxLayout;
    m_pbOpAddUpdate = new QPushButton;
    connect(m_pbOpAddUpdate, &QPushButton::clicked, this, &DialogPatternPieceData::AddUpdate);
    pMCPButtonLayout->addStretch(1);
    pMCPButtonLayout->addWidget(m_pbOpAddUpdate);
    m_pbOpCancel = new QPushButton(tr("Cancel"));
    connect(m_pbOpCancel, &QPushButton::clicked, this, &DialogPatternPieceData::Cancel);
    pMCPButtonLayout->addWidget(m_pbOpCancel);
    m_pbOpRemove = new QPushButton(tr("Remove"));
    connect(m_pbOpRemove, &QPushButton::clicked, this, &DialogPatternPieceData::Remove);
    pMCPButtonLayout->addWidget(m_pbOpRemove);
    pLayoutGB->addLayout(pMCPButtonLayout, 1, 0);

    m_plwMCP = new QListWidget;
    connect(m_plwMCP, &QListWidget::itemClicked, this, &DialogPatternPieceData::SetEditMode);
    pLayoutGB->addWidget(m_plwMCP, 0, 1, 3, 1);
    pLayoutGB->setRowStretch(2, 1);
    pLayoutGB->setColumnStretch(2, 1);

    pWidgetsLayout->addWidget(pgb);

    pWidgetsLayout->setStretch(2, 1);

    pLayout->addLayout(pWidgetsLayout);

    QHBoxLayout* pButtonsLayout = new QHBoxLayout;
    pButtonsLayout->addStretch(0);
    bOk = new QPushButton(tr("OK"));
    connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);
    QPushButton* pbCancel = new QPushButton(tr("Cancel"));
    connect(pbCancel, &QPushButton::clicked, this, &DialogTool::DialogRejected);
    pButtonsLayout->addWidget(bOk);
    pButtonsLayout->addWidget(pbCancel);

    pLayout->addLayout(pButtonsLayout);
    pLayout->setStretch(0, 1);

    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::SetDetail(const VDetail& rDetail)
{
    m_detail = rDetail;
    m_pleLetter->setText(m_detail.GetPatternPieceData().GetLetter());
    m_pleName->setText(m_detail.GetPatternPieceData().GetName());
    UpdateList();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::SaveData()
{
    m_detail.GetPatternPieceData().SetLetter(m_pleLetter->text());
    m_detail.GetPatternPieceData().SetName(m_pleName->text());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::UpdateList()
{
    m_plwMCP->clear();
    for (int i = 0; i < m_detail.GetPatternPieceData().GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = m_detail.GetPatternPieceData().GetMCP(i);
        QString qsText = tr("Cut %1 of %2%3");
        qsText = qsText.arg(mcp.m_iCutNumber);
        if (mcp.m_eMaterial < MaterialType::mtUserDefined)
            qsText = qsText.arg(m_qslMaterials[int(mcp.m_eMaterial)]);
        else
            qsText = qsText.arg(mcp.m_qsMaterialUserDef);
        if (mcp.m_ePlacement == PlacementType::ptCutOnFold)
            qsText = qsText.arg(tr(" on Fold"));
        else
            qsText = qsText.arg("");

        m_plwMCP->addItem(qsText);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::AddUpdate()
{
    MaterialCutPlacement mcp;
    mcp.m_qsMaterialUserDef = m_pcbMaterial->currentText();
    mcp.m_eMaterial = MaterialType::mtUserDefined;
    for (int i = 0; i < m_qslMaterials.count(); ++i)
        if (mcp.m_qsMaterialUserDef == m_qslMaterials[i])
            mcp.m_eMaterial = MaterialType(i);

    mcp.m_iCutNumber = m_psbCutNumber->value();
    mcp.m_ePlacement = PlacementType(m_pcbPlacement->currentIndex());

    if (m_bAddMode == true)
    {
        m_detail.GetPatternPieceData().Append(mcp);
    }
    else
    {
        int iR = m_plwMCP->currentRow();
        SCASSERT(iR >= 0);
        m_detail.GetPatternPieceData().Set(iR, mcp);
        SetAddMode();
    }
    UpdateList();
    ClearFields();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::Cancel()
{
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::Remove()
{
    int iR = m_plwMCP->currentRow();
    SCASSERT(iR >= 0);
    m_detail.GetPatternPieceData().RemoveMCP(iR);
    UpdateList();
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::ClearFields()
{
    m_pcbMaterial->setCurrentIndex(0);
    m_psbCutNumber->setValue(0);
    m_pcbPlacement->setCurrentIndex(0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::SetAddMode()
{
    m_pbOpAddUpdate->setText(tr("Add"));
    m_pbOpCancel->hide();
    m_pbOpRemove->hide();
    m_plwMCP->setCurrentRow(-1);
    m_bAddMode = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternPieceData::SetEditMode()
{
    int iR = m_plwMCP->currentRow();
    // this method can be called by clicking on item or by update. In the latter case there is nothing else to do!
    if (iR < 0 || iR >= m_detail.GetPatternPieceData().GetMCPCount())
        return;

    m_pbOpAddUpdate->setText(tr("Update"));
    m_pbOpCancel->show();
    m_pbOpRemove->show();

    MaterialCutPlacement mcp = m_detail.GetPatternPieceData().GetMCP(iR);
    m_pcbMaterial->setCurrentText(mcp.m_qsMaterialUserDef);
    m_psbCutNumber->setValue(mcp.m_iCutNumber);
    m_pcbPlacement->setCurrentIndex(int(mcp.m_ePlacement));

    m_bAddMode = false;
}

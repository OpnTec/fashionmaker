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

#ifndef DIALOGPATTERNPIECEDATA_H
#define DIALOGPATTERNPIECEDATA_H

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>

#include "vdetail.h"
#include "dialogtool.h"

/**
 * @brief This dialog allows user to edit pattern piece meta data
 */
class DialogPatternPieceData : public DialogTool
{
    Q_OBJECT

public:
    DialogPatternPieceData(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);

    VDetail                             GetDetail() const;
    void                                SetDetail(const VDetail& rDetail);

    void                                SaveData();

protected slots:
    void                                UpdateList();
    void                                AddUpdate();
    void                                Cancel();
    void                                Remove();

private:
    VDetail                             m_detail;

    QLineEdit*                          m_pleName;
    QLineEdit*                          m_pleLetter;
    QComboBox*                          m_pcbMaterial;
    QSpinBox*                           m_psbCutNumber;
    QComboBox*                          m_pcbPlacement;

    QPushButton*                        m_pbOpAddUpdate;
    QPushButton*                        m_pbOpCancel;
    QPushButton*                        m_pbOpRemove;

    QListWidget*                        m_plwMCP;

    QStringList                         m_qslMaterials;
    QStringList                         m_qslPlacements;

    bool                                m_bAddMode;

    void                                ClearFields();

private slots:
    void                                SetAddMode();
    void                                SetEditMode();
};

inline VDetail DialogPatternPieceData::GetDetail() const
{
    return m_detail;
}

#endif // DIALOGPATTERNPIECEDATA_H

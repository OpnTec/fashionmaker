/************************************************************************
 **
 **  @file   dialogduplicatedetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 10, 2017
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
#include "dialogduplicatedetail.h"
#include "ui_dialogduplicatedetail.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../../../visualization/path/vistoolduplicatedetail.h"

//---------------------------------------------------------------------------------------------------------------------
DialogDuplicateDetail::DialogDuplicateDetail(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogDuplicateDetail),
      m_idDetail(NULL_ID),
      m_mx(0),
      m_my(0),
      m_firstRelease(false)
{
    ui->setupUi(this);
    InitOkCancel(ui);

    vis = new VisToolDuplicateDetail(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogDuplicateDetail::~DialogDuplicateDetail()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDuplicateDetail::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            // The check need to ignore first release of mouse button.
            // User should have chance to place piece.
            if (not m_firstRelease)
            {
                m_firstRelease = true;
                return;
            }

            VisToolDuplicateDetail *piece = qobject_cast<VisToolDuplicateDetail *>(vis);
            SCASSERT(piece != nullptr)

            m_mx = piece->Mx();
            m_my = piece->My();
            emit ToolTip("");
            DialogAccepted();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDuplicateDetail::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Detail && id > NULL_ID)
        {
            m_idDetail = id;
            emit ToolTip(tr("Click to place duplicate"));
            vis->VisualMode(id);
            prepare = true;
        }
    }
}

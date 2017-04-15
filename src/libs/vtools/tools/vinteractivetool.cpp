/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 4, 2017
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

#include "vinteractivetool.h"

//---------------------------------------------------------------------------------------------------------------------
VInteractiveTool::VInteractiveTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent)
    : VAbstractTool(doc, data, id),
      m_dialog()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VInteractiveTool::DialogLinkDestroy()
{
    m_dialog.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiOk refresh tool data after change in options.
 * @param result keep result working dialog.
 */
void VInteractiveTool::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SaveDialogChange();
    }
    m_dialog.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiApply refresh tool data after change in options but do not delete dialog
 */
void VInteractiveTool::FullUpdateFromGuiApply()
{
    SaveDialogChange();
}

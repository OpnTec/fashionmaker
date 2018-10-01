/************************************************************************
 **
 **  @file   vistoolduplicatedetail.cpp
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
#include "vistoolduplicatedetail.h"
#include "../vpatterndb/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolDuplicateDetail::VisToolDuplicateDetail(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      m_start(),
      m_started(false),
      m_diff()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolDuplicateDetail::RefreshGeometry()
{
    const VPiece piece = Visualization::data->GetPiece(object1Id);

    if (not m_started)
    {
        m_start = Visualization::scenePos;
        m_started = true;
        setPos(QPointF(piece.GetMx(), piece.GetMy()));
    }
    else
    {
        m_diff = Visualization::scenePos - m_start;
        m_diff = QPointF(m_diff.x() + piece.GetMx(), m_diff.y() + piece.GetMy());
        setPos(m_diff);
    }

    DrawPath(this, PiecePath(piece), mainColor, Qt::SolidLine, Qt::RoundCap);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VisToolDuplicateDetail::PiecePath(const VPiece &piece) const
{
    if (not piece.IsHideMainPath() || not piece.IsSeamAllowance() || piece.IsSeamAllowanceBuiltIn())
    {
        return piece.MainPathPath(Visualization::data);
    }
    else
    {
        return piece.SeamAllowancePath(Visualization::data);
    }
}

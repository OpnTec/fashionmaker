/************************************************************************
 **
 **  @file   vistoolduplicatedetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef VISTOOLDUPLICATEDETAIL_H
#define VISTOOLDUPLICATEDETAIL_H

#include "vispath.h"
#include "../vpatterndb/vpiece.h"

class VisToolDuplicateDetail : public VisPath
{
    Q_OBJECT
public:
    VisToolDuplicateDetail(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolDuplicateDetail() = default;

    qreal Mx() const;
    qreal My() const;

    virtual void RefreshGeometry() override;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolDuplicateDetail)};
private:
    Q_DISABLE_COPY(VisToolDuplicateDetail)
    QPointF m_start;
    bool m_started;
    QPointF m_diff;

    QPainterPath PiecePath(const VPiece &piece) const;
};

//---------------------------------------------------------------------------------------------------------------------
inline qreal VisToolDuplicateDetail::Mx() const
{
    return m_diff.x();
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VisToolDuplicateDetail::My() const
{
    return m_diff.y();
}

#endif // VISTOOLDUPLICATEDETAIL_H

/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 11, 2016
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

#ifndef VISTOOLPIECE_H
#define VISTOOLPIECE_H

#include <QtCore/QObject>
#include <QtGlobal>

#include "vispath.h"
#include "../vpatterndb/vpiece.h"

class VisToolPiece : public VisPath
{
    Q_OBJECT
public:
    VisToolPiece(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPiece();

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void         SetPiece(const VPiece &piece);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPiece)};
private:
    Q_DISABLE_COPY(VisToolPiece)
    QVector<QGraphicsEllipseItem *> m_points;

    QGraphicsLineItem *m_line1;
    QGraphicsLineItem *m_line2;
    VPiece m_piece;

    QGraphicsEllipseItem* GetPoint(quint32 i, const QColor &color);
};

#endif // VISTOOLPIECE_H

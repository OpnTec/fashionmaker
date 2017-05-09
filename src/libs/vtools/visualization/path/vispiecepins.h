/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 2, 2017
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

#ifndef VISPIECEPINS_H
#define VISPIECEPINS_H

#include "vispath.h"

class VSimplePoint;

class VisPiecePins : public VisPath
{
    Q_OBJECT
public:
    VisPiecePins(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisPiecePins() Q_DECL_EQ_DEFAULT;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void         SetPins(const QVector<quint32> &pins);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::PiecePins)};
private:
    Q_DISABLE_COPY(VisPiecePins)
    QVector<VSimplePoint *> m_points;
    QVector<quint32>        m_pins;

    VSimplePoint *GetPoint(quint32 i, const QColor &color);

    void HideAllItems();
};

#endif // VISPIECEPINS_H

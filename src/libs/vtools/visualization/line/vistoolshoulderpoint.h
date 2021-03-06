/************************************************************************
 **
 **  @file   vistoolshoulderpoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VISTOOLSHOULDERPOINT_H
#define VISTOOLSHOULDERPOINT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolShoulderPoint : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolShoulderPoint(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolShoulderPoint() = default;

    virtual void RefreshGeometry() override;
    void         setLineP1Id(const quint32 &value);
    void         setLineP2Id(const quint32 &value);
    void         setLength(const QString &expression);
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolShoulderPoint)};
private:
    Q_DISABLE_COPY(VisToolShoulderPoint)
    quint32         lineP1Id;
    quint32         lineP2Id;
    VScaledEllipse *point;
    VScaledEllipse *line1P1;
    VScaledEllipse *line1P2;
    VScaledLine    *line1;
    VScaledEllipse *line2P2;
    VScaledLine    *line2;
    VScaledLine    *line3;
    qreal           length;
};

#endif // VISTOOLSHOULDERPOINT_H

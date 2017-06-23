/************************************************************************
 **
 **  @file   vistoolcubicbezier.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
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

#ifndef VISTOOLCUBICBEZIER_H
#define VISTOOLCUBICBEZIER_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "vispath.h"

class VisToolCubicBezier : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolCubicBezier(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolCubicBezier() Q_DECL_EQ_DEFAULT;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void         setObject2Id(const quint32 &value);
    void         setObject3Id(const quint32 &value);
    void         setObject4Id(const quint32 &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolCubicBezier)};
protected:
    Q_DISABLE_COPY(VisToolCubicBezier)
    quint32         object2Id;
    quint32         object3Id;
    quint32         object4Id;
    VScaledEllipse *point1;
    VScaledEllipse *point2;
    VScaledEllipse *point3;
    VScaledEllipse *point4;
    VScaledLine    *helpLine1;
    VScaledLine    *helpLine2;
};

#endif // VISTOOLCUBICBEZIER_H

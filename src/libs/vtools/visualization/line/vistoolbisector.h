/************************************************************************
 **
 **  @file   vistoolbisector.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 7, 2014
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

#ifndef VISTOOLBISECTOR_H
#define VISTOOLBISECTOR_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolBisector :public VisLine
{
    Q_OBJECT
public:
    explicit VisToolBisector(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolBisector() = default;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void         setObject2Id(const quint32 &value);
    void         setObject3Id(const quint32 &value);
    void         setLength(const QString &expression);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolBisector)};
private:
    Q_DISABLE_COPY(VisToolBisector)
    quint32         object2Id;
    quint32         object3Id;
    VScaledEllipse *point;
    VScaledEllipse *line1P1;
    VScaledEllipse *line1P2;
    VScaledLine    *line1;
    VScaledEllipse *line2P2;
    VScaledLine    *line2;
    qreal           length;
};

#endif // VISTOOLBISECTOR_H

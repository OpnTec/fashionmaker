/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#ifndef VISOPERATION_H
#define VISOPERATION_H

#include <QtGlobal>

#include "../visline.h"

class VisOperation : public VisLine
{
    Q_OBJECT
public:
    explicit VisOperation(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisOperation();

    void SetObjects(QVector<quint32> objects);

    virtual void VisualMode(const quint32 &pointId = NULL_ID) Q_DECL_OVERRIDE;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolRotation)};
protected:
    QVector<quint32> objects;
    QColor           supportColor2;

    QVector<QGraphicsEllipseItem *> points;
    QVector<QGraphicsPathItem *>    curves;

    QGraphicsEllipseItem * GetPoint(quint32 i, const QColor &color);
    QGraphicsPathItem    * GetCurve(quint32 i, const QColor &color);
private:
    Q_DISABLE_COPY(VisOperation)
};

#endif // VISOPERATION_H

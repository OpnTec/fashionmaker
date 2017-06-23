/************************************************************************
 **
 **  @file   vistoolcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
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

#ifndef VISTOOLCUBICBEZIERPATH_H
#define VISTOOLCUBICBEZIERPATH_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../vgeometry/vcubicbezierpath.h"
#include "../vmisc/def.h"
#include "vispath.h"

class VisToolCubicBezierPath : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolCubicBezierPath(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolCubicBezierPath();

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void              setPath(const VCubicBezierPath &value);
    VCubicBezierPath  getPath();

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolCubicBezierPath)};
protected:
    Q_DISABLE_COPY(VisToolCubicBezierPath)
    QVector<VScaledEllipse *> mainPoints;
    QVector<VScaledEllipse *> ctrlPoints;
    QVector<VScaledLine *>    lines;
    VCurvePathItem           *newCurveSegment;
    VCubicBezierPath          path;
    VScaledLine              *helpLine1;
    VScaledLine              *helpLine2;

private:
    VScaledEllipse *getPoint(QVector<VScaledEllipse *> &points, quint32 i, qreal z = 0);
    VScaledLine    *getLine(quint32 i);
    void Creating(const QVector<VPointF> &pathPoints , int pointsLeft);
    void RefreshToolTip();
};

#endif // VISTOOLCUBICBEZIERPATH_H

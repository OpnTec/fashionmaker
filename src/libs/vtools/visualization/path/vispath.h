/************************************************************************
 **
 **  @file   vispath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#ifndef VISPATH_H
#define VISPATH_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../visualization.h"
#include "../vmisc/def.h"

class VSimplePoint;

class VisPath : public Visualization, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit VisPath(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisPath() Q_DECL_OVERRIDE;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::Path)};
protected:
    virtual void InitPen() Q_DECL_OVERRIDE;
    virtual void AddOnScene() Q_DECL_OVERRIDE;

    VSimplePoint *GetPoint(QVector<VSimplePoint *> &points, quint32 i, const QColor &color);
private:
    Q_DISABLE_COPY(VisPath)
};

#endif // VISPATH_H

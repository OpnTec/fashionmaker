/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#include "xml/vdomdocument.h"
#include "vdatatool.h"
#include "container/vcontainer.h"

namespace Tool{
    enum Enum
    {
        FromGui,
        FromFile
    };
}

class VAbstractTool:public VDataTool{
    Q_OBJECT
public:
                          VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual               ~VAbstractTool();
    static QPointF        LineIntersectRect(QRectF rec, QLineF line);
    static qint32         LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                              QPointF &p2);
    static QPointF        ClosestPoint(QLineF line, QPointF p);
    static QPointF        addVector (QPointF p, QPointF p1, QPointF p2, qreal k);
    qint64 getId() const;

public slots:
    virtual void          FullUpdateFromFile()=0;
signals:
    void                  toolhaveChange();
    void                  ChoosedTool(qint64 id, Scene::Scenes type);
    void                  FullUpdateTree();
protected:
    VDomDocument          *doc;
    qint64                id;
    const Qt::GlobalColor baseColor;
    Qt::GlobalColor       currentColor;
    virtual void          AddToFile()=0;
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qreal &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const QString &value);
    const VContainer      *getData() const;
private:
    Q_DISABLE_COPY(VAbstractTool)
};
#endif // VABSTRACTTOOL_H

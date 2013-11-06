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
 **  Valentina is distributed in the hope that it will be useful,
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

#include "vdatatool.h"
#include "../xml/vdomdocument.h"

class VAbstractTool: public VDataTool
{
    Q_OBJECT
public:
                            VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual                 ~VAbstractTool() {}
    static QPointF          LineIntersectRect(QRectF rec, QLineF line);
    static qint32           LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1, QPointF &p2);
    static QPointF          ClosestPoint(QLineF line, QPointF p);
    static QPointF          addVector (QPointF p, QPointF p1, QPointF p2, qreal k);
    inline qint64           getId() const {return id;}
    static void             LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c);
    static const QString    AttrId;
    static const QString    AttrType;
    static const QString    AttrMx;
    static const QString    AttrMy;
    static const QString    AttrName;
    static const QString    AttrX;
    static const QString    AttrY;
    static const QString    AttrTypeLine;
    static const QString    AttrLength;
    static const QString    AttrBasePoint;
    static const QString    AttrFirstPoint;
    static const QString    AttrSecondPoint;
    static const QString    AttrThirdPoint;
    static const QString    AttrCenter;
    static const QString    AttrRadius;
    static const QString    AttrAngle;
    static const QString    AttrAngle1;
    static const QString    AttrAngle2;
    static const QString    AttrP1Line;
    static const QString    AttrP2Line;
    static const QString    AttrP1Line1;
    static const QString    AttrP2Line1;
    static const QString    AttrP1Line2;
    static const QString    AttrP2Line2;
    static const QString    AttrPShoulder;
    static const QString    AttrPoint1;
    static const QString    AttrPoint4;
    static const QString    AttrKAsm1;
    static const QString    AttrKAsm2;
    static const QString    AttrKCurve;
    static const QString    AttrPathPoint;
    static const QString    AttrPSpline;
    static const QString    AttrAxisP1;
    static const QString    AttrAxisP2;
    static const QString    TypeLineNone;
    static const QString    TypeLineLine;
public slots:
    virtual void            FullUpdateFromFile()=0;
signals:
    void                    toolhaveChange();
    void                    ChoosedTool(qint64 id, Scene::Scenes type);
    void                    FullUpdateTree();
    void                    RemoveTool(QGraphicsItem *tool);
protected:
    VDomDocument            *doc;
    const qint64            id;
    const Qt::GlobalColor   baseColor;
    Qt::GlobalColor         currentColor;
    virtual void            AddToFile()=0;
    inline const VContainer *getData() const {return &data;}
    virtual void            RemoveReferens(){}
    void                    RemoveAllChild(QDomElement &domElement);
    template <typename T>
    void AddAttribute(QDomElement &domElement, const QString &name, const T &value)
    {
        QDomAttr domAttr = doc->createAttribute(name);
        domAttr.setValue(QString().setNum(value));
        domElement.setAttributeNode(domAttr);
    }
private:
    Q_DISABLE_COPY(VAbstractTool)
};

template <>
inline void VAbstractTool::AddAttribute<QString>(QDomElement &domElement, const QString &name, const QString &value)
{
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(value);
    domElement.setAttributeNode(domAttr);
}
#endif // VABSTRACTTOOL_H

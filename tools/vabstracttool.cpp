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

#include "vabstracttool.h"

VAbstractTool::VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent):
    VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), currentColor(Qt::black){

    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qreal &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const QString &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(value);
    domElement.setAttributeNode(domAttr);
}

VAbstractTool::~VAbstractTool(){
}

QPointF VAbstractTool::LineIntersectRect(QRectF rec, QLineF line){
    qreal x1, y1, x2, y2;
    rec.getCoords(&x1, &y1, &x2, &y2);
    QPointF point;
    QLineF::IntersectType type = line.intersect(QLineF(QPointF(x1,y1), QPointF(x1,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1,y1), QPointF(x2,y1)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1,y2), QPointF(x2,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x2,y1), QPointF(x2,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
    return point;
}

qint32 VAbstractTool::LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                          QPointF &p2){
    const qreal eps = 1e-8;
    //коефіцієнти для рівняння відрізку
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    // проекция центра окружности на прямую
    QPointF p = ClosestPoint (line, center);
    // сколько всего решений?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qAbs (d - radius) <= eps){
        flag = 1;
    } else {
        if (radius > d){
            flag = 2;
        } else {
            return 0;
        }
    }
    // находим расстояние от проекции до точек пересечения
    qreal k = sqrt (radius * radius - d * d);
    qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // добавляем к проекции векторы направленные к точкам пеерсечения
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

QPointF VAbstractTool::ClosestPoint(QLineF line, QPointF p){
    QLineF lineP2pointFrom = QLineF(line.p2(), p);
    qreal angle = 180-line.angleTo(lineP2pointFrom)-90;
    QLineF pointFromlineP2 = QLineF(p, line.p2());
    pointFromlineP2.setAngle(pointFromlineP2.angle()+angle);
    QPointF point;
    QLineF::IntersectType type = pointFromlineP2.intersect(line,&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    } else{
        if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection ){
            Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
            return point;
        }
    }
    return point;
}

QPointF VAbstractTool::addVector(QPointF p, QPointF p1, QPointF p2, qreal k){
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

void VAbstractTool::RemoveAllChild(QDomElement &domElement){
    if ( domElement.hasChildNodes() ){
        while ( domElement.childNodes().length() >= 1 ){
            domElement.removeChild( domElement.firstChild() );
        }
    }
}

void VAbstractTool::LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c){
    //коефіцієнти для рівняння відрізку
    *a = line.p2().y() - line.p1().y();
    *b = line.p1().x() - line.p2().x();
    *c = - *a * line.p1().x() - *b * line.p1().y();
}

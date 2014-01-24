/************************************************************************
 **
 **  @file   vabstracttool.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vabstracttool.h"

const QString VAbstractTool::AttrId          = QStringLiteral("id");
const QString VAbstractTool::AttrType        = QStringLiteral("type");
const QString VAbstractTool::AttrMx          = QStringLiteral("mx");
const QString VAbstractTool::AttrMy          = QStringLiteral("my");
const QString VAbstractTool::AttrName        = QStringLiteral("name");
const QString VAbstractTool::AttrX           = QStringLiteral("x");
const QString VAbstractTool::AttrY           = QStringLiteral("y");
const QString VAbstractTool::AttrTypeLine    = QStringLiteral("typeLine");
const QString VAbstractTool::AttrLength      = QStringLiteral("length");
const QString VAbstractTool::AttrBasePoint   = QStringLiteral("basePoint");
const QString VAbstractTool::AttrFirstPoint  = QStringLiteral("firstPoint");
const QString VAbstractTool::AttrSecondPoint = QStringLiteral("secondPoint");
const QString VAbstractTool::AttrThirdPoint  = QStringLiteral("thirdPoint");
const QString VAbstractTool::AttrCenter      = QStringLiteral("center");
const QString VAbstractTool::AttrRadius      = QStringLiteral("radius");
const QString VAbstractTool::AttrAngle       = QStringLiteral("angle");
const QString VAbstractTool::AttrAngle1      = QStringLiteral("angle1");
const QString VAbstractTool::AttrAngle2      = QStringLiteral("angle2");
const QString VAbstractTool::AttrP1Line      = QStringLiteral("p1Line");
const QString VAbstractTool::AttrP2Line      = QStringLiteral("p2Line");
const QString VAbstractTool::AttrP1Line1     = QStringLiteral("p1Line1");
const QString VAbstractTool::AttrP2Line1     = QStringLiteral("p2Line1");
const QString VAbstractTool::AttrP1Line2     = QStringLiteral("p1Line2");
const QString VAbstractTool::AttrP2Line2     = QStringLiteral("p2Line2");
const QString VAbstractTool::AttrPShoulder   = QStringLiteral("pShoulder");
const QString VAbstractTool::AttrPoint1      = QStringLiteral("point1");
const QString VAbstractTool::AttrPoint4      = QStringLiteral("point4");
const QString VAbstractTool::AttrKAsm1       = QStringLiteral("kAsm1");
const QString VAbstractTool::AttrKAsm2       = QStringLiteral("kAsm2");
const QString VAbstractTool::AttrKCurve      = QStringLiteral("kCurve");
const QString VAbstractTool::AttrPathPoint   = QStringLiteral("pathPoint");
const QString VAbstractTool::AttrPSpline     = QStringLiteral("pSpline");
const QString VAbstractTool::AttrAxisP1      = QStringLiteral("axisP1");
const QString VAbstractTool::AttrAxisP2      = QStringLiteral("axisP2");
const QString VAbstractTool::TypeLineNone    = QStringLiteral("none");
const QString VAbstractTool::TypeLineLine    = QStringLiteral("hair");

VAbstractTool::VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent)
    :VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), currentColor(Qt::black)
{
    Q_ASSERT(doc != 0);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
}

void VAbstractTool::NewSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    QRectF rect = sc->itemsBoundingRect();

    QRect  rec0 = view->rect();
    rec0 = QRect(0, 0, rec0.width()-2, rec0.height()-2);

    QTransform t = view->transform();

    QRectF rec1;
    if(t.m11() < 1)
    {
        rec1 = QRect(0, 0, rec0.width()/t.m11(), rec0.height()/t.m22());

        rec1.translate(rec0.center().x()-rec1.center().x(), rec0.center().y()-rec1.center().y());
        QPolygonF polygone =  view->mapToScene(rec1.toRect());
        rec1 = polygone.boundingRect();

    }
    else
    {
        rec1 = rec0;
    }

    rec1 = rec1.united(rect.toRect());
    sc->setSceneRect(rec1);
}

QPointF VAbstractTool::LineIntersectRect(QRectF rec, QLineF line)
{
    qreal x1, y1, x2, y2;
    rec.getCoords(&x1, &y1, &x2, &y2);
    QPointF point;
    QLineF::IntersectType type = line.intersect(QLineF(QPointF(x1, y1), QPointF(x1, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1, y1), QPointF(x2, y1)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1, y2), QPointF(x2, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x2, y1), QPointF(x2, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "There is no point of intersection.");
    return point;
}

qint32 VAbstractTool::LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                          QPointF &p2)
{
    const qreal eps = 1e-8;
    //coefficient for equation of segment
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    // projection center of circle on to line
    QPointF p = ClosestPoint (line, center);
    // how many solutions?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qAbs (d - radius) <= eps)
    {
        flag = 1;
    }
    else
    {
        if (radius > d)
        {
            flag = 2;
        }
        else
        {
            return 0;
        }
    }
    // find distance from projection to points of intersection
    qreal k = sqrt (radius * radius - d * d);
    qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // add to projection a vectors aimed to points of intersection
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

QPointF VAbstractTool::ClosestPoint(const QLineF &line, const QPointF &p)
{
    QLineF lineP2pointFrom = QLineF(line.p2(), p);
    //right triangle always have one angle with 90 degree
    //Now we want find angle between projection and line from p.
    qreal angle = 180-(line.angleTo(lineP2pointFrom)-90);
    //Swap first and last points line. Need for rotation.
    QLineF pointFromlineP2 = QLineF(p, line.p2());
    pointFromlineP2.setAngle(pointFromlineP2.angle()+angle);
    //After rotation we will have two intersect lines. Left just find intersect point.
    QPointF point;
    QLineF::IntersectType type = pointFromlineP2.intersect(line, &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    else
    {
        if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection )
        {
            Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Don't have point of intersection.");
            return point;
        }
    }
    return point;
}

QPointF VAbstractTool::addVector(const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k)
{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

void VAbstractTool::RemoveAllChild(QDomElement &domElement)
{
    if ( domElement.hasChildNodes() )
    {
        while ( domElement.childNodes().length() >= 1 )
        {
            domElement.removeChild( domElement.firstChild() );
        }
    }
}

//TODO see method deleteNode. I think no need have QGraphicsItem. QObject can delete yourself.
void VAbstractTool::DeleteTool(QGraphicsItem *tool)
{
    if (_referens <= 1)
    {
        //remove from xml file
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            QDomNode element = domElement.parentNode();
            if (element.isNull() == false)
            {
                if (element.isElement())
                {
                    RemoveReferens();//deincrement referens
                    element.removeChild(domElement);//remove form file
                    QGraphicsScene *scene = tool->scene();
                    if(scene != 0)//some tools haven't scene
                    {
                        scene->removeItem(tool);//remove form scene
                    }
                    doc->FullUpdateTree();
                    //emit toolhaveChange();//set enabled save button
                }
                else
                {
                    qWarning()<<"parent isn't element"<<Q_FUNC_INFO;
                }
            }
            else
            {
                qWarning()<<"parent isNull"<<Q_FUNC_INFO;
            }
        }
        else
        {
            qWarning()<<"Can't get element by id form file = "<<id<<Q_FUNC_INFO;
        }
    }
}

void VAbstractTool::LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c)
{
    //coefficient for equation of segment
    QPointF p1 = line.p1();
    *a = line.p2().y() - p1.y();
    *b = p1.x() - line.p2().x();
    *c = - *a * p1.x() - *b * p1.y();
}

void VAbstractTool::AddRecord(const qint64 id, const Tool::Tools &toolType, VDomDocument *doc)
{
    qint64 cursor = doc->getCursor();
    QVector<VToolRecord> *history = doc->getHistory();
    if (cursor <= 0)
    {
        history->append(VToolRecord(id, toolType, doc->GetNameActivDraw()));
    }
    else
    {
        qint32 index = 0;
        for (qint32 i = 0; i<history->size(); ++i)
        {
            VToolRecord rec = history->at(i);
            if (rec.getId() == cursor)
            {
                index = i;
                break;
            }
        }
        history->insert(index+1, VToolRecord(id, toolType, doc->GetNameActivDraw()));
    }
}

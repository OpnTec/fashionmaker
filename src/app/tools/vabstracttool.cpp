/************************************************************************
 **
 **  @file   vabstracttool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../xml/vpattern.h"
#include <QGraphicsView>

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

const QString VAbstractTool::TypeLineNone           = QStringLiteral("none");
const QString VAbstractTool::TypeLineLine           = QStringLiteral("hair");
const QString VAbstractTool::TypeLineDashLine       = QStringLiteral("dashLine");
const QString VAbstractTool::TypeLineDotLine        = QStringLiteral("dotLine");
const QString VAbstractTool::TypeLineDashDotLine    = QStringLiteral("dashDotLine");
const QString VAbstractTool::TypeLineDashDotDotLine = QStringLiteral("dashDotDotLine");

//---------------------------------------------------------------------------------------------------------------------
VAbstractTool::VAbstractTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), currentColor(Qt::black), typeLine(TypeLineLine)
{
    SCASSERT(doc != nullptr);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VPattern::haveLiteChange);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::LiteUpdateTree, this->doc, &VPattern::LiteParseTree);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::NewSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    QRectF rect = sc->itemsBoundingRect();

    QRect  rec0 = view->rect();
    rec0 = QRect(0, 0, rec0.width()-2, rec0.height()-2);

    QTransform t = view->transform();

    QRectF rec1;
    if (t.m11() < 1)
    {
        qreal width = rec0.width()/t.m11();
        qreal height = rec0.height()/t.m22();
        rec1 = QRect(0, 0, static_cast<qint32>(width), static_cast<qint32>(height));

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

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
qint32 VAbstractTool::LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                          QPointF &p2)
{
    //coefficient for equation of segment
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    // projection center of circle on to line
    QPointF p = ClosestPoint (line, center);
    // how many solutions?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qFuzzyCompare(d, radius))
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

//---------------------------------------------------------------------------------------------------------------------
QPointF VAbstractTool::ClosestPoint(const QLineF &line, const QPointF &point)
{
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    qreal x = point.x() + a;
    qreal y = b + point.y();
    QLineF lin (point, QPointF(x, y));
    QPointF p;
    QLineF::IntersectType intersect = line.intersect(lin, &p);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        return p;
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VAbstractTool::addVector(const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k)
{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

//---------------------------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::DeleteTool(QGraphicsItem *tool)
{
    if (_referens <= 1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Confirm the deletion."));
        msgBox.setInformativeText(tr("Do you really want delete?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Question);
        if (msgBox.exec() == QMessageBox::Cancel)
        {
            return;
        }
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
                    if (scene != 0)//some tools haven't scene
                    {
                        scene->removeItem(tool);//remove form scene
                    }
                    doc->LiteParseTree();
                    emit toolhaveChange();//set enabled save button
                }
                else
                {
                    qDebug()<<"parent isn't element"<<Q_FUNC_INFO;
                }
            }
            else
            {
                qDebug()<<"parent isNull"<<Q_FUNC_INFO;
            }
        }
        else
        {
            qDebug()<<"Can't get element by id form file = "<<id<<Q_FUNC_INFO;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle VAbstractTool::LineStyle()
{
    QStringList styles = Styles();
    switch (styles.indexOf(typeLine))
    {
        case 0: // TypeLineNone
            return Qt::NoPen;
            break;
        case 1: // TypeLineLine
            return Qt::SolidLine;
            break;
        case 2: // TypeLineDashLine
            return Qt::DashLine;
            break;
        case 3: // TypeLineDotLine
            return Qt::DotLine;
            break;
        case 4: // TypeLineDashDotLine
            return Qt::DashDotLine;
            break;
        case 5: // TypeLineDashDotDotLine
            return Qt::DashDotDotLine;
            break;
        default:
            return Qt::SolidLine;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c)
{
    //coefficient for equation of segment
    QPointF p1 = line.p1();
    *a = line.p2().y() - p1.y();
    *b = p1.x() - line.p2().x();
    *c = - *a * p1.x() - *b * p1.y();
}

//---------------------------------------------------------------------------------------------------------------------
const QStringList VAbstractTool::Styles()
{
    //Keep synchronize with DialogTool lineStyles list!!!
    QStringList styles{TypeLineNone, TypeLineLine, TypeLineDashLine, TypeLineDotLine, TypeLineDashDotLine,
                TypeLineDashDotDotLine};
    return styles;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::AddRecord(const quint32 id, const Tool &toolType, VPattern *doc)
{
    quint32 cursor = doc->getCursor();
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

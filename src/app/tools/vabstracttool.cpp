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
#include <QGraphicsView>
#include <QMessageBox>
#include "../undocommands/deltool.h"
#include "../widgets/vapplication.h"
#include "../geometry/vpointf.h"
#include "../undocommands/savetooloptions.h"

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
/**
 * @brief VAbstractTool container.
 * @param doc dom document container.
 * @param data container with data.
 * @param id object id in container.
 * @param parent parent object.
 */
VAbstractTool::VAbstractTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), currentColor(Qt::black), typeLine(TypeLineLine)
{
    SCASSERT(doc != nullptr);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VPattern::haveLiteChange);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::LiteUpdateTree, this->doc, &VPattern::LiteParseTree);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewSceneRect calculate scene rect what contains all items and doesn't less that size of scene view.
 * @param sc scene.
 * @param view view.
 */
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
/**
 * @brief LineIntersectRect find point intersection line and rect.
 * @param rec rect.
 * @param line line.
 * @return point intersection.
 */
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
/**
 * @brief LineIntersectCircle find point intersection line and circle.
 * @param center arc center.
 * @param radius arc radius.
 * @param line line
 * @param p1 first intersection point.
 * @param p2 second intersection point.
 * @return 0 - intersection doesn't exist, 1 - one intersection point, 2 - two intersection points.
 */
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
/**
 * @brief ClosestPoint find point projection of point onto line.
 * @param line line.
 * @return point on line or extended line if origin size too small.
 */
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
/**
 * @brief DeleteTool full delete object form scene and file.
 * @param tool tool
 */
void VAbstractTool::DeleteTool(bool ask)
{
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::Cancel)
        {
            return;
        }
    }
    DelTool *delTool = new DelTool(doc, id);
    connect(delTool, &DelTool::NeedFullParsing, doc, &VPattern::NeedFullParsing);
    qApp->getUndoStack()->push(delTool);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineStyle return pen style for current line style.
 * @return pen style.
 */
Qt::PenStyle VAbstractTool::LineStyle(const QString &typeLine)
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
QString VAbstractTool::getLineType() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::setTypeLine(const QString &value)
{
    typeLine = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, quint32> VAbstractTool::PointsList() const
{
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data.DataGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != id)
        {
            QSharedPointer<VGObject> obj = i.value();
            if (obj->getType() == GOType::Point && obj->getMode() == Draw::Calculation)
            {
                const QSharedPointer<VPointF> point = data.GeometricObject<VPointF>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractTool::ConfirmDeletion()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Confirm the deletion."));
    msgBox.setInformativeText(tr("Do you really want delete?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    return msgBox.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::SaveOption(QSharedPointer<VGObject> &obj)
{
    QDomElement oldDomElement = doc->elementById(QString().setNum(id));
    if (oldDomElement.isElement())
    {
        QDomElement newDomElement = oldDomElement.cloneNode().toElement();

        SaveOptions(newDomElement, obj);

        SaveToolOptions *saveOptions = new SaveToolOptions(oldDomElement, newDomElement, doc, id);
        connect(saveOptions, &SaveToolOptions::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveOptions);
    }
    else
    {
        qDebug()<<"Can't find tool with id ="<< id << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineCoefficients coefficient for equation of segment. Segment equestion ax+by+c=0.
 * @param line line
 * @param a a value
 * @param b b value
 * @param c c value
 */
void VAbstractTool::LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c)
{
    //coefficient for equation of segment
    QPointF p1 = line.p1();
    *a = line.p2().y() - p1.y();
    *b = p1.x() - line.p2().x();
    *c = - *a * p1.x() - *b * p1.y();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Styles return list of all line styles.
 * @return list of all line styles.
 */
const QStringList VAbstractTool::Styles()
{
    QStringList styles{TypeLineNone, TypeLineLine, TypeLineDashLine, TypeLineDotLine, TypeLineDashDotLine,
                TypeLineDashDotDotLine};
    return styles;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddRecord add record about tool in history.
 * @param id object id in container
 * @param toolType tool type
 * @param doc dom document container
 */
void VAbstractTool::AddRecord(const quint32 id, const Tool &toolType, VPattern *doc)
{
    QVector<VToolRecord> *history = doc->getHistory();
    VToolRecord record = VToolRecord(id, toolType, doc->GetNameActivPP());
    if (history->contains(record))
    {
        return;
    }

    quint32 cursor = doc->getCursor();
    if (cursor <= 0)
    {
        history->append(record);
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
        history->insert(index+1, record);
    }
}

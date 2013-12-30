/************************************************************************
 **
 **  @file   vcontainer.cpp
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

#include "vcontainer.h"

#include <QDebug>
#include <QtAlgorithms>

qint64 VContainer::_id = 0;

VContainer::VContainer()
    :base(QHash<QString, qint32>()),  gObjects(QHash<qint64, VGObject *>()),
      standartTable(QHash<QString, VStandartTableRow *>()), incrementTable(QHash<QString, VIncrementTableRow *>()),
      lengthLines(QHash<QString, qreal>()),lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<qint64, VDetail *>())
{
    SetSize(500);
    SetGrowth(1760);
    CreateManTableIGroup ();
}

VContainer &VContainer::operator =(const VContainer &data)
{
    setData(data);
    return *this;
}

VContainer::VContainer(const VContainer &data)
    :base(QHash<QString, qint32>()), gObjects(QHash<qint64, VGObject *>()),
      standartTable(QHash<QString, VStandartTableRow *>()), incrementTable(QHash<QString, VIncrementTableRow *>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<qint64, VDetail *>())
{
    setData(data);
}

void VContainer::setData(const VContainer &data)
{
    base = *data.DataBase();

    qDeleteAll(gObjects);
    gObjects.clear();
    const QHash<qint64, VGObject*> *obj = data.DataGObjects();
    Q_ASSERT(obj != 0);
    QHashIterator<qint64, VGObject*> i(*obj);
    while (i.hasNext())
    {
        i.next();
        switch(i.value()->getType())
        {
           case(GObject::Arc):
        {
                VArc *arc = new VArc(*data.GeometricObject<const VArc *>(i.key()));
                Q_ASSERT(arc != 0);
                gObjects.insert(i.key(), arc);
                break;
        }
           case(GObject::Point):
        {
                VPointF *point = new VPointF(*data.GeometricObject<const VPointF *>(i.key()));
                Q_ASSERT(point != 0);
                gObjects.insert(i.key(), point);
                break;
        }
           case(GObject::Spline):
        {
                VSpline *spl = new VSpline(*data.GeometricObject<const VSpline *>(i.key()));
                Q_ASSERT(spl != 0);
                gObjects.insert(i.key(), spl);
                break;
        }
           case(GObject::SplinePath):
        {
                VSplinePath *path = new VSplinePath(*data.GeometricObject<const VSplinePath *>(i.key()));
                Q_ASSERT(path != 0);
                gObjects.insert(i.key(), path);
                break;
        }
        }
    }
    //gObjects = *data.DataGObjects();
    standartTable = *data.DataStandartTable();
    incrementTable = *data.DataIncrementTable();
    lengthLines = *data.DataLengthLines();
    lineAngles = *data.DataLineAngles();
    lengthSplines = *data.DataLengthSplines();
    lengthArcs = *data.DataLengthArcs();
    details = *data.DataDetails();
}

const VGObject *VContainer::GetGObject(qint64 id)const
{
    return GetObject(gObjects, id);
}

template <typename key, typename val>
const val VContainer::GetObject(const QHash<key, val> &obj, key id) const
{
    if (obj.contains(id))
    {
        return obj.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

template <typename key, typename val>
val VContainer::GetVariable(const QHash<key, val> &obj, key id) const
{
    if (obj.contains(id))
    {
        return obj.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

const VStandartTableRow *VContainer::GetStandartTableCell(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(standartTable, name);
}

const VIncrementTableRow *VContainer::GetIncrementTableRow(const QString& name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(incrementTable, name);
}

qreal VContainer::GetLine(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthLines, name);
}

qreal VContainer::GetLengthArc(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthArcs, name);
}

qreal VContainer::GetLengthSpline(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthSplines, name);
}

qreal VContainer::GetLineAngle(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lineAngles, name);
}

const VDetail *VContainer::GetDetail(qint64 id) const
{
    return GetObject(details, id);
}

qint64 VContainer::AddGObject(VGObject *obj)
{
    return AddObject(gObjects, obj);
}

qint64 VContainer::AddDetail(VDetail *detail)
{
    return AddObject(details, detail);
}

qint64 VContainer::getNextId()
{
    _id++;
    return _id;
}

void VContainer::UpdateId(qint64 newId)
{
    if (newId > _id)
    {
       _id = newId;
    }
}

QPainterPath VContainer::ContourPath(qint64 idDetail) const
{
    const VDetail *detail = GetDetail(idDetail);
    QVector<QPointF> points;
    QVector<QPointF> pointsEkv;
    for (ptrdiff_t i = 0; i< detail->CountNode(); ++i)
    {
        switch (detail->at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const VPointF *point = GeometricObject<const VPointF*>(detail->at(i).getId());
                points.append(point->toQPointF());
                if (detail->getSupplement() == true)
                {
                    QPointF pEkv = point->toQPointF();
                    pEkv.setX(pEkv.x()+detail->at(i).getMx());
                    pEkv.setY(pEkv.y()+detail->at(i).getMy());
                    pointsEkv.append(pEkv);
                }
            }
            break;
            case (Tool::NodeArc):
            {
                const VArc *arc = GeometricObject<const VArc *>(detail->at(i).getId());
                qreal len1 = GetLengthContour(points, arc->GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(arc->GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << arc->GetPoints();
                    if (detail->getSupplement() == true)
                    {
                        pointsEkv << biasPoints(arc->GetPoints(), detail->at(i).getMx(), detail->at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(arc->GetPoints());
                    if (detail->getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(arc->GetPoints()), detail->at(i).getMx(),
                                                detail->at(i).getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSpline):
            {
                const VSpline *spline = GeometricObject<const VSpline *>(detail->at(i).getId());
                qreal len1 = GetLengthContour(points, spline->GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(spline->GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << spline->GetPoints();
                    if (detail->getSupplement() == true)
                    {
                        pointsEkv << biasPoints(spline->GetPoints(), detail->at(i).getMx(), detail->at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(spline->GetPoints());
                    if (detail->getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(spline->GetPoints()), detail->at(i).getMx(),
                                                detail->at(i).getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSplinePath):
            {
                const VSplinePath *splinePath = GeometricObject<const VSplinePath *>(detail->at(i).getId());
                qreal len1 = GetLengthContour(points, splinePath->GetPathPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(splinePath->GetPathPoints()));
                if (len1 <= lenReverse)
                {
                    points << splinePath->GetPathPoints();
                    if (detail->getSupplement() == true)
                    {
                     pointsEkv << biasPoints(splinePath->GetPathPoints(), detail->at(i).getMx(), detail->at(i).getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(splinePath->GetPathPoints());
                    if (detail->getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(splinePath->GetPathPoints()), detail->at(i).getMx(),
                                                detail->at(i).getMy());
                    }
                }
            }
            break;
            default:
                qWarning()<<"Get wrong tool type. Ignore."<<detail->at(i).getTypeTool();
                break;
        }
    }

    QPainterPath path;
    path.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points[i]);
    }
    path.lineTo(points[0]);

    if (detail->getSupplement() == true)
    {
        QPainterPath ekv;
        if (detail->getClosed() == true)
        {
            ekv = Equidistant(pointsEkv, Detail::CloseEquidistant, toPixel(detail->getWidth()));
        }
        else
        {
            ekv = Equidistant(pointsEkv, Detail::OpenEquidistant, toPixel(detail->getWidth()));
        }
        path.addPath(ekv);
        path.setFillRule(Qt::WindingFill);
    }
    return path;
}

QVector<QPointF> VContainer::biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my) const
{
    QVector<QPointF> p;
    for (qint32 i = 0; i < points.size(); ++i)
    {
        QPointF point = points.at(i);
        point.setX(point.x() + mx);
        point.setY(point.y() + my);
        p.append(point);
    }
    return p;
}

QPainterPath VContainer::Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv, const qreal &width) const
{
    QPainterPath ekv;
    QVector<QPointF> ekvPoints;
    if ( points.size() < 3 )
    {
        qDebug()<<"Not enough points for building the equidistant.\n";
        return ekv;
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if (i != points.size()-1)
        {
            if (points[i] == points[i+1])
            {
                points.remove(i+1);
            }
        }
        else
        {
            if (points[i] == points[0])
            {
                points.remove(i);
            }
        }
    }
    if (eqv == Detail::CloseEquidistant)
    {
        points.append(points.at(0));
    }
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if ( i == 0 && eqv == Detail::CloseEquidistant)
        {//first point, polyline closed
            ekvPoints<<EkvPoint(QLineF(points[points.size()-2], points[points.size()-1]), QLineF(points[1], points[0]),
                    width);
            continue;
        }
        else if (i == 0 && eqv == Detail::OpenEquidistant)
        {//first point, polyline doesn't closed
            ekvPoints.append(SingleParallelPoint(QLineF(points[0], points[1]), 90, width));
            continue;
        }
        if (i == points.size()-1 && eqv == Detail::CloseEquidistant)
        {//last point, polyline closed
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == points.size()-1 && eqv == Detail::OpenEquidistant)
        {//last point, polyline doesn't closed
                ekvPoints.append(SingleParallelPoint(QLineF(points[points.size()-1], points[points.size()-2]), -90,
                        width));
                continue;
        }
        //points in the middle of polyline
        ekvPoints<<EkvPoint(QLineF(points[i-1], points[i]), QLineF(points[i+1], points[i]), width);
    }
    ekvPoints = CheckLoops(ekvPoints);
    ekv.moveTo(ekvPoints[0]);
    for (qint32 i = 1; i < ekvPoints.count(); ++i)
    {
        ekv.lineTo(ekvPoints[i]);
    }
    return ekv;
}

QLineF VContainer::ParallelLine(const QLineF &line, qreal width)
{
    Q_ASSERT(width > 0);
    QLineF paralel = QLineF (SingleParallelPoint(line, 90, width), SingleParallelPoint(QLineF(line.p2(), line.p1()),
                                                                                       -90, width));
    return paralel;
}

QPointF VContainer::SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width)
{
    Q_ASSERT(width > 0);
    QLineF pLine = line;
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

QVector<QPointF> VContainer::EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width) const
{
    Q_ASSERT(width > 0);
    QVector<QPointF> points;
    if (line1.p2() != line2.p2())
    {
        qWarning()<<"Last point of two lines must be equal.";
    }
    QPointF CrosPoint;
    QLineF bigLine1 = ParallelLine(line1, width );
    QLineF bigLine2 = ParallelLine(QLineF(line2.p2(), line2.p1()), width );
    QLineF::IntersectType type = bigLine1.intersect( bigLine2, &CrosPoint );
    switch (type)
    {
        case (QLineF::BoundedIntersection):
            points.append(CrosPoint);
            return points;
            break;
        case (QLineF::UnboundedIntersection):
        {
            QLineF line( line1.p2(), CrosPoint );
            if (line.length() > width + toPixel(8))
            {
                QLineF lineL;
                lineL = QLineF(bigLine1.p2(), CrosPoint);
                lineL.setLength(width);
                points.append(lineL.p2());

                lineL = QLineF(bigLine2.p1(), CrosPoint);
                lineL.setLength(width);
                points.append(lineL.p2());
            }
            else
            {
                points.append(CrosPoint);
                return points;
            }
            break;
        }
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line.*/
            points.append(bigLine1.p2());
            return points;
            break;
        default:
            break;
    }
    return points;
}

QVector<QPointF> VContainer::CheckLoops(const QVector<QPointF> &points) const
{
    QVector<QPointF> ekvPoints;
    /*If we got less than 4 points no need seek loops.*/
    if (points.size() < 4)
    {
       return ekvPoints;
    }
    bool closed = false;
    if (points.at(0) == points.at(points.size()-1))
    {
        closed = true;
    }
    qint32 i, j;
    for (i = 0; i < points.size(); ++i)
    {
        /*Last three points no need check.*/
        if (i >= points.size()-3)
        {
            ekvPoints.append(points.at(i));
            continue;
        }
        QPointF crosPoint;
        QLineF::IntersectType intersect = QLineF::NoIntersection;
        QLineF line1(points.at(i), points.at(i+1));
        for (j = i+2; j < points.size()-1; ++j)
        {
            QLineF line2(points.at(j), points.at(j+1));
            intersect = line1.intersect(line2, &crosPoint);
            if (intersect == QLineF::BoundedIntersection)
            {
                break;
            }
        }
        if (intersect == QLineF::BoundedIntersection)
        {
            if (i == 0 && j+1 == points.size()-1 && closed)
            {
                /*We got closed contour.*/
                ekvPoints.append(points.at(i));
            }
            else
            {
                /*We found loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(crosPoint);
                ekvPoints.append(points.at(j+1));
                i = j + 2;
            }
        }
        else
        {
            /*We did not found loop.*/
            ekvPoints.append(points.at(i));
        }
    }
    return ekvPoints;
}

void VContainer::PrepareDetails(QVector<VItem *> &list) const
{
    QHashIterator<qint64, VDetail *> idetail(details);
    while (idetail.hasNext())
    {
        idetail.next();
        list.append(new VItem(ContourPath(idetail.key()), list.size()));
    }
}

template <typename val>
void VContainer::UpdateObject(QHash<qint64, val> &obj, const qint64 &id, val point)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    Q_ASSERT(point != 0);
    point->setId(id);  
//    if (gObjects.contains(id))
//    {
//        delete gObjects.value(id);
//    }
    obj[id] = point;
    UpdateId(id);
}

void VContainer::AddLengthSpline(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthSplines[name] = value;
}

void VContainer::AddLengthArc(const qint64 &id)
{
    const VArc * arc = GeometricObject<const VArc *>(id);
    AddLengthArc(arc->name(), toMM(arc->GetLength()));
}

void VContainer::AddLengthArc(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthArcs[name] = value;
}

void VContainer::AddLineAngle(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lineAngles[name] = value;
}

qreal VContainer::GetValueStandartTableCell(const QString& name) const
{
    const VStandartTableRow *cell =  GetStandartTableCell(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell->GetBase() + k_size*cell->GetKsize() + k_growth*cell->GetKgrowth();
    return value;
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const
{
    const VIncrementTableRow *cell =  GetIncrementTableRow(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell->getBase() + k_size*cell->getKsize() + k_growth*cell->getKgrowth();
    return value;
}

void VContainer::Clear()
{
    _id = 0;
    if(standartTable.size()>0)
    {
        qDeleteAll(standartTable);
    }
    standartTable.clear();
    if(incrementTable.size()>0)
    {
        qDeleteAll(incrementTable);   
    }
    incrementTable.clear();
    lengthLines.clear();
    lengthArcs.clear();
    lineAngles.clear();
    details.clear();
    ClearObject();
    CreateManTableIGroup ();
}

void VContainer::ClearObject()
{
    if(gObjects.size()>0)
    {
        qDeleteAll(gObjects);
    }
    gObjects.clear();
}

qreal VContainer::FindVar(const QString &name, bool *ok)const
{
    if (base.contains(name))
    {
        *ok = true;
        return base.value(name);
    }

    if (standartTable.contains(name))
    {
        *ok = true;
        return GetValueStandartTableCell(name);
    }
    if (incrementTable.contains(name))
    {
        *ok = true;
        return GetValueIncrementTableRow(name);
    }
    if (lengthLines.contains(name))
    {
        *ok = true;
        return lengthLines.value(name);
    }
    if (lengthArcs.contains(name))
    {
        *ok = true;
        return lengthArcs.value(name);
    }
    if (lineAngles.contains(name))
    {
        *ok = true;
        return lineAngles.value(name);
    }
    if (lengthSplines.contains(name))
    {
        *ok = true;
        return lengthSplines.value(name);
    }
    *ok = false;
    return 0;
}

void VContainer::AddLine(const qint64 &firstPointId, const qint64 &secondPointId)
{
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    const VPointF *first = GeometricObject<const VPointF *>(firstPointId);
    const VPointF *second = GeometricObject<const VPointF *>(secondPointId);
    AddLengthLine(nameLine, toMM(QLineF(first->toQPointF(), second->toQPointF()).length()));
    nameLine = GetNameLineAngle(firstPointId, secondPointId);
    AddLineAngle(nameLine, QLineF(first->toQPointF(), second->toQPointF()).angle());
}

template <typename key, typename val>
qint64 VContainer::AddObject(QHash<key, val> &obj, val value)
{
    Q_ASSERT(value != 0);
    qint64 id = getNextId();
    value->setId(id);
    obj[id] = value;
    return id;
}

QString VContainer::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString("Line_%1_%2").arg(first->name(), second->name());
}

QString VContainer::GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString("AngleLine_%1_%2").arg(first->name(), second->name());
}

void VContainer::UpdateGObject(qint64 id, VGObject* obj)
{
    UpdateObject(gObjects, id, obj);
}

void VContainer::UpdateDetail(qint64 id,VDetail *detail)
{
    UpdateObject(details, id, detail);
}

void VContainer::AddLengthLine(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthLines[name] = value;
}

void VContainer::CreateManTableIGroup ()
{
    AddStandartTableCell("Pkor", new VStandartTableRow(84, 0, 3));
    AddStandartTableCell("Pkor", new VStandartTableRow(84, 0, 3));
    AddStandartTableCell("Vtos", new VStandartTableRow(1450, 2, 51));
    AddStandartTableCell("Vtosh", new VStandartTableRow(1506, 2, 54));
    AddStandartTableCell("Vpt", new VStandartTableRow(1438, 3, 52));
    AddStandartTableCell("Vst", new VStandartTableRow(1257, -1, 49));
    AddStandartTableCell("Vlt", new VStandartTableRow(1102, 0, 43));
    AddStandartTableCell("Vk", new VStandartTableRow(503, 0, 22));
    AddStandartTableCell("Vsht", new VStandartTableRow(1522, 2, 54));
    AddStandartTableCell("Vzy", new VStandartTableRow(1328, 0, 49));
    AddStandartTableCell("Vlop", new VStandartTableRow(1320, 0, 49));
    AddStandartTableCell("Vps", new VStandartTableRow(811, -1, 36));
    AddStandartTableCell("Ssh", new VStandartTableRow(202, 4, 1));
    AddStandartTableCell("SgI", new VStandartTableRow(517, 18, 2));
    AddStandartTableCell("SgII", new VStandartTableRow(522, 19, 1));
    AddStandartTableCell("SgIII", new VStandartTableRow(500, 20, 0));
    AddStandartTableCell("St", new VStandartTableRow(390, 20, 0));
    AddStandartTableCell("Sb", new VStandartTableRow(492, 15, 5));
    AddStandartTableCell("SbI", new VStandartTableRow(482, 12, 6));
    AddStandartTableCell("Obed", new VStandartTableRow(566, 18, 6));
    AddStandartTableCell("Ok", new VStandartTableRow(386, 8, 8));
    AddStandartTableCell("Oi", new VStandartTableRow(380, 8, 6));
    AddStandartTableCell("Osch", new VStandartTableRow(234, 4, 4));
    AddStandartTableCell("Dsb", new VStandartTableRow(1120, 0, 44));
    AddStandartTableCell("Dsp", new VStandartTableRow(1110, 0, 43));
    AddStandartTableCell("Dn", new VStandartTableRow(826, -3, 37));
    AddStandartTableCell("Dps", new VStandartTableRow(316, 4, 7));
    AddStandartTableCell("Dpob", new VStandartTableRow(783, 14, 15));
    AddStandartTableCell("Ds", new VStandartTableRow(260, 1, 6));
    AddStandartTableCell("Op", new VStandartTableRow(316, 12, 0));
    AddStandartTableCell("Ozap", new VStandartTableRow(180, 4, 0));
    AddStandartTableCell("Pkis", new VStandartTableRow(250, 4, 0));
    AddStandartTableCell("SHp", new VStandartTableRow(160, 1, 4));
    AddStandartTableCell("Dlych", new VStandartTableRow(500, 2, 15));
    AddStandartTableCell("Dzap", new VStandartTableRow(768, 2, 24));
    AddStandartTableCell("DIIIp", new VStandartTableRow(970, 2, 29));
    AddStandartTableCell("Vprp", new VStandartTableRow(214, 3, 3));
    AddStandartTableCell("Vg", new VStandartTableRow(262, 8, 3));
    AddStandartTableCell("Dtp", new VStandartTableRow(460, 7, 9));
    AddStandartTableCell("Dp", new VStandartTableRow(355, 5, 5));
    AddStandartTableCell("Vprz", new VStandartTableRow(208, 3, 5));
    AddStandartTableCell("Dts", new VStandartTableRow(438, 2, 10));
    AddStandartTableCell("DtsI", new VStandartTableRow(469, 2, 10));
    AddStandartTableCell("Dvcht", new VStandartTableRow(929, 9, 19));
    AddStandartTableCell("SHg", new VStandartTableRow(370, 14, 4));
    AddStandartTableCell("Cg", new VStandartTableRow(224, 6, 0));
    AddStandartTableCell("SHs", new VStandartTableRow(416, 10, 2));
    AddStandartTableCell("dpzr", new VStandartTableRow(121, 6, 0));
    AddStandartTableCell("Ogol", new VStandartTableRow(576, 4, 4));
    AddStandartTableCell("Ssh1", new VStandartTableRow(205, 5, 0));
    AddStandartTableCell("St", new VStandartTableRow(410, 20, 0));
    AddStandartTableCell("Drzap", new VStandartTableRow(594, 3, 19));
    AddStandartTableCell("DbII", new VStandartTableRow(1020, 0, 44));
    AddStandartTableCell("Sb", new VStandartTableRow(504, 15, 4));
}

QVector<QPointF> VContainer::GetReversePoint(const QVector<QPointF> &points) const
{
    Q_ASSERT(points.size() > 0);
    QVector<QPointF> reversePoints;
    for (qint32 i = points.size() - 1; i >= 0; --i)
    {
        reversePoints.append(points.at(i));
    }
    return reversePoints;
}

qreal VContainer::GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints) const
{
    qreal length = 0;
    QVector<QPointF> points;
    points << contour << newPoints;
    for (qint32 i = 0; i < points.size()-1; ++i)
    {
        QLineF line(points.at(i), points.at(i+1));
        length += line.length();
    }
    return length;
}

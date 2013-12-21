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
#include "../exception/vexceptionbadid.h"

#include <QDebug>

qint64 VContainer::_id = 0;

VContainer::VContainer()
    :base(QHash<QString, qint32>()), points(QHash<qint64, VPointF>()),
      standartTable(QHash<QString, VStandartTableRow>()), incrementTable(QHash<QString, VIncrementTableRow>()),
    lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), splines(QHash<qint64, VSpline>()),
    lengthSplines(QHash<QString, qreal>()), arcs(QHash<qint64, VArc>()), lengthArcs(QHash<QString, qreal>()),
    splinePaths(QHash<qint64, VSplinePath>()), details(QHash<qint64, VDetail>())
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
    :base(QHash<QString, qint32>()), points(QHash<qint64, VPointF>()),
    standartTable(QHash<QString, VStandartTableRow>()), incrementTable(QHash<QString, VIncrementTableRow>()),
    lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), splines(QHash<qint64, VSpline>()),
    lengthSplines(QHash<QString, qreal>()), arcs(QHash<qint64, VArc>()), lengthArcs(QHash<QString, qreal>()),
    splinePaths(QHash<qint64, VSplinePath>()), details(QHash<qint64, VDetail>())
{
    setData(data);
}

void VContainer::setData(const VContainer &data)
{
    base = *data.DataBase();
    points = *data.DataPoints();
    standartTable = *data.DataStandartTable();
    incrementTable = *data.DataIncrementTable();
    lengthLines = *data.DataLengthLines();
    lineAngles = *data.DataLineAngles();
    splines = *data.DataSplines();
    lengthSplines = *data.DataLengthSplines();
    arcs = *data.DataArcs();
    lengthArcs = *data.DataLengthArcs();
    splinePaths = *data.DataSplinePaths();
    details = *data.DataDetails();
}

VPointF VContainer::GetPoint(qint64 id) const
{
    return GetObject(points, id);
}

template <typename key, typename val>
val VContainer::GetObject(const QHash<key, val> &obj, key id)
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

VStandartTableRow VContainer::GetStandartTableCell(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(standartTable, name);
}

VIncrementTableRow VContainer::GetIncrementTableRow(const QString& name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(incrementTable, name);
}

qreal VContainer::GetLine(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(lengthLines, name);
}

qreal VContainer::GetLengthArc(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(lengthArcs, name);
}

qreal VContainer::GetLengthSpline(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(lengthSplines, name);
}

qreal VContainer::GetLineAngle(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetObject(lineAngles, name);
}

VSpline VContainer::GetSpline(qint64 id) const
{
    return GetObject(splines, id);
}

VArc VContainer::GetArc(qint64 id) const
{
    return GetObject(arcs, id);
}

VSplinePath VContainer::GetSplinePath(qint64 id) const
{
    return GetObject(splinePaths, id);
}

VDetail VContainer::GetDetail(qint64 id) const
{
    return GetObject(details, id);
}

qint64 VContainer::AddPoint(const VPointF &point)
{
    return AddObject(points, point);
}

qint64 VContainer::AddDetail(const VDetail &detail)
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
    VDetail detail = GetDetail(idDetail);
    QVector<QPointF> points;
    QVector<QPointF> pointsEkv;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail[i].getTypeTool())
        {
            case (Tool::NodePoint):
            {
                VPointF point = GetPoint(detail[i].getId());
                points.append(point.toQPointF());
                if (detail.getSupplement() == true)
                {
                    QPointF pEkv = point.toQPointF();
                    pEkv.setX(pEkv.x()+detail[i].getMx());
                    pEkv.setY(pEkv.y()+detail[i].getMy());
                    pointsEkv.append(pEkv);
                }
            }
            break;
            case (Tool::NodeArc):
            {
                VArc arc = GetArc(detail[i].getId());
                qreal len1 = GetLengthContour(points, arc.GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(arc.GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << arc.GetPoints();
                    if (detail.getSupplement() == true)
                    {
                        pointsEkv << biasPoints(arc.GetPoints(), detail[i].getMx(), detail[i].getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(arc.GetPoints());
                    if (detail.getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(arc.GetPoints()), detail[i].getMx(), detail[i].getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSpline):
            {
                VSpline spline = GetSpline(detail[i].getId());
                qreal len1 = GetLengthContour(points, spline.GetPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(spline.GetPoints()));
                if (len1 <= lenReverse)
                {
                    points << spline.GetPoints();
                    if (detail.getSupplement() == true)
                    {
                        pointsEkv << biasPoints(spline.GetPoints(), detail[i].getMx(), detail[i].getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(spline.GetPoints());
                    if (detail.getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(spline.GetPoints()), detail[i].getMx(),
                                                detail[i].getMy());
                    }
                }
            }
            break;
            case (Tool::NodeSplinePath):
            {
                VSplinePath splinePath = GetSplinePath(detail[i].getId());
                qreal len1 = GetLengthContour(points, splinePath.GetPathPoints());
                qreal lenReverse = GetLengthContour(points, GetReversePoint(splinePath.GetPathPoints()));
                if (len1 <= lenReverse)
                {
                    points << splinePath.GetPathPoints();
                    if (detail.getSupplement() == true)
                    {
                     pointsEkv << biasPoints(splinePath.GetPathPoints(), detail[i].getMx(), detail[i].getMy());
                    }
                }
                else
                {
                    points << GetReversePoint(splinePath.GetPathPoints());
                    if (detail.getSupplement() == true)
                    {
                        pointsEkv << biasPoints(GetReversePoint(splinePath.GetPathPoints()), detail[i].getMx(),
                                                detail[i].getMy());
                    }
                }
            }
            break;
            case (Tool::SplineTool):
                break;//Nothing to do, just ignore.
            default:
                qWarning()<<"Get wrong tool type. Ignore."<<detail[i].getTypeTool();
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

    if (detail.getSupplement() == true)
    {
        QPainterPath ekv;
        if (detail.getClosed() == true)
        {
            ekv = Equidistant(pointsEkv, Detail::CloseEquidistant, toPixel(detail.getWidth()));
        }
        else
        {
            ekv = Equidistant(pointsEkv, Detail::OpenEquidistant, toPixel(detail.getWidth()));
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
        qDebug()<<"Not enough points for build equidistant.\n";
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
        {//перша точка, ламана замкнена
            ekvPoints<<EkvPoint(QLineF(points[points.size()-2], points[points.size()-1]), QLineF(points[1], points[0]),
                    width);
            continue;
        }
        else if (i == 0 && eqv == Detail::OpenEquidistant)
        {//перша точка, ламана не замкнена
            ekvPoints.append(SingleParallelPoint(QLineF(points[0], points[1]), 90, width));
            continue;
        }
        if (i == points.size()-1 && eqv == Detail::CloseEquidistant)
        {//остання точка, ламана замкнена
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == points.size()-1 && eqv == Detail::OpenEquidistant)
        {//остання точка, ламана не замкнена
                ekvPoints.append(SingleParallelPoint(QLineF(points[points.size()-1], points[points.size()-2]), -90,
                        width));
                continue;
        }
        //точка яка не лежить ні на початку ні в кінці
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
    QHashIterator<qint64, VDetail> iDetail(details);
    while (iDetail.hasNext())
    {
        iDetail.next();
        list.append(new VItem(ContourPath(iDetail.key()), list.size()));
    }
}

template <typename val>
void VContainer::UpdateObject(QHash<qint64, val> &obj, const qint64 &id, const val& point)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
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
    AddLengthArc(GetArc(id).name(), toMM(GetArc(id).GetLength()));
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
    VStandartTableRow cell =  GetStandartTableCell(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.GetBase() + k_size*cell.GetKsize() + k_growth*cell.GetKgrowth();
    return value;
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const
{
    VIncrementTableRow cell =  GetIncrementTableRow(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.getBase() + k_size*cell.getKsize() + k_growth*cell.getKgrowth();
    return value;
}

void VContainer::Clear()
{
    _id = 0;
    standartTable.clear();
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
    points.clear();
    splines.clear();
    arcs.clear();
    splinePaths.clear();
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
    VPointF first = GetPoint(firstPointId);
    VPointF second = GetPoint(secondPointId);
    AddLengthLine(nameLine, toMM(QLineF(first.toQPointF(), second.toQPointF()).length()));
    nameLine = GetNameLineAngle(firstPointId, secondPointId);
    AddLineAngle(nameLine, QLineF(first.toQPointF(), second.toQPointF()).angle());
}

qint64 VContainer::AddSpline(const VSpline &spl)
{
    return AddObject(splines, spl);
}

qint64 VContainer::AddSplinePath(const VSplinePath &splPath)
{
    return AddObject(splinePaths, splPath);
}

qint64 VContainer::AddArc(const VArc &arc)
{
    return AddObject(arcs, arc);
}

template <typename key, typename val>
qint64 VContainer::AddObject(QHash<key, val> &obj, const val& value)
{
    qint64 id = getNextId();
    obj[id] = value;
    return id;
}

QString VContainer::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const
{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);

    return QString("Line_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint) const
{
    VPointF first = GetPoint(firstPoint);
    VPointF second = GetPoint(secondPoint);

    return QString("AngleLine_%1_%2").arg(first.name(), second.name());
}

void VContainer::UpdatePoint(qint64 id, const VPointF &point)
{
    UpdateObject(points, id, point);
}

void VContainer::UpdateDetail(qint64 id, const VDetail &detail)
{
    UpdateObject(details, id, detail);
}

void VContainer::UpdateSpline(qint64 id, const VSpline &spl)
{
    UpdateObject(splines, id, spl);
}

void VContainer::UpdateSplinePath(qint64 id, const VSplinePath &splPath)
{
    UpdateObject(splinePaths, id, splPath);
}

void VContainer::UpdateArc(qint64 id, const VArc &arc)
{
    UpdateObject(arcs, id, arc);
}

void VContainer::AddLengthLine(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthLines[name] = value;
}

void VContainer::CreateManTableIGroup ()
{
    AddStandartTableCell("Pkor", VStandartTableRow(84, 0, 3));
    AddStandartTableCell("Pkor", VStandartTableRow(84, 0, 3));
    AddStandartTableCell("Vtos", VStandartTableRow(1450, 2, 51));
    AddStandartTableCell("Vtosh", VStandartTableRow(1506, 2, 54));
    AddStandartTableCell("Vpt", VStandartTableRow(1438, 3, 52));
    AddStandartTableCell("Vst", VStandartTableRow(1257, -1, 49));
    AddStandartTableCell("Vlt", VStandartTableRow(1102, 0, 43));
    AddStandartTableCell("Vk", VStandartTableRow(503, 0, 22));
    AddStandartTableCell("Vsht", VStandartTableRow(1522, 2, 54));
    AddStandartTableCell("Vzy", VStandartTableRow(1328, 0, 49));
    AddStandartTableCell("Vlop", VStandartTableRow(1320, 0, 49));
    AddStandartTableCell("Vps", VStandartTableRow(811, -1, 36));
    AddStandartTableCell("Ssh", VStandartTableRow(202, 4, 1));
    AddStandartTableCell("SgI", VStandartTableRow(517, 18, 2));
    AddStandartTableCell("SgII", VStandartTableRow(522, 19, 1));
    AddStandartTableCell("SgIII", VStandartTableRow(500, 20, 0));
    AddStandartTableCell("St", VStandartTableRow(390, 20, 0));
    AddStandartTableCell("Sb", VStandartTableRow(492, 15, 5));
    AddStandartTableCell("SbI", VStandartTableRow(482, 12, 6));
    AddStandartTableCell("Obed", VStandartTableRow(566, 18, 6));
    AddStandartTableCell("Ok", VStandartTableRow(386, 8, 8));
    AddStandartTableCell("Oi", VStandartTableRow(380, 8, 6));
    AddStandartTableCell("Osch", VStandartTableRow(234, 4, 4));
    AddStandartTableCell("Dsb", VStandartTableRow(1120, 0, 44));
    AddStandartTableCell("Dsp", VStandartTableRow(1110, 0, 43));
    AddStandartTableCell("Dn", VStandartTableRow(826, -3, 37));
    AddStandartTableCell("Dps", VStandartTableRow(316, 4, 7));
    AddStandartTableCell("Dpob", VStandartTableRow(783, 14, 15));
    AddStandartTableCell("Ds", VStandartTableRow(260, 1, 6));
    AddStandartTableCell("Op", VStandartTableRow(316, 12, 0));
    AddStandartTableCell("Ozap", VStandartTableRow(180, 4, 0));
    AddStandartTableCell("Pkis", VStandartTableRow(250, 4, 0));
    AddStandartTableCell("SHp", VStandartTableRow(160, 1, 4));
    AddStandartTableCell("Dlych", VStandartTableRow(500, 2, 15));
    AddStandartTableCell("Dzap", VStandartTableRow(768, 2, 24));
    AddStandartTableCell("DIIIp", VStandartTableRow(970, 2, 29));
    AddStandartTableCell("Vprp", VStandartTableRow(214, 3, 3));
    AddStandartTableCell("Vg", VStandartTableRow(262, 8, 3));
    AddStandartTableCell("Dtp", VStandartTableRow(460, 7, 9));
    AddStandartTableCell("Dp", VStandartTableRow(355, 5, 5));
    AddStandartTableCell("Vprz", VStandartTableRow(208, 3, 5));
    AddStandartTableCell("Dts", VStandartTableRow(438, 2, 10));
    AddStandartTableCell("DtsI", VStandartTableRow(469, 2, 10));
    AddStandartTableCell("Dvcht", VStandartTableRow(929, 9, 19));
    AddStandartTableCell("SHg", VStandartTableRow(370, 14, 4));
    AddStandartTableCell("Cg", VStandartTableRow(224, 6, 0));
    AddStandartTableCell("SHs", VStandartTableRow(416, 10, 2));
    AddStandartTableCell("dpzr", VStandartTableRow(121, 6, 0));
    AddStandartTableCell("Ogol", VStandartTableRow(576, 4, 4));
    AddStandartTableCell("Ssh1", VStandartTableRow(205, 5, 0));
    AddStandartTableCell("St", VStandartTableRow(410, 20, 0));
    AddStandartTableCell("Drzap", VStandartTableRow(594, 3, 19));
    AddStandartTableCell("DbII", VStandartTableRow(1020, 0, 44));
    AddStandartTableCell("Sb", VStandartTableRow(504, 15, 4));
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

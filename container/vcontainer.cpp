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

#include "vcontainer.h"
#include <QDebug>
#include "options.h"
#include <exception/vexceptionbadid.h>

qint64 VContainer::_id = 0;

VContainer::VContainer():base(QHash<QString, qint32>()), points(QHash<qint64, VPointF>()),
    modelingPoints(QHash<qint64, VPointF>()),
    standartTable(QHash<QString, VStandartTableCell>()), incrementTable(QHash<QString, VIncrementTableRow>()),
    lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), splines(QHash<qint64, VSpline>()),
    modelingSplines(QHash<qint64, VSpline>()),
    lengthSplines(QHash<QString, qreal>()), arcs(QHash<qint64, VArc>()), modelingArcs(QHash<qint64, VArc>()),
    lengthArcs(QHash<QString, qreal>()),
    splinePaths(QHash<qint64, VSplinePath>()), modelingSplinePaths(QHash<qint64, VSplinePath>()),
    details(QHash<qint64, VDetail>()){
    SetSize(500);
    SetGrowth(1760);
    CreateManTableIGroup ();
}

VContainer &VContainer::operator =(const VContainer &data){
    setData(data);
    return *this;
}

VContainer::VContainer(const VContainer &data):base(QHash<QString, qint32>()), points(QHash<qint64, VPointF>()),
    modelingPoints(QHash<qint64, VPointF>()),
    standartTable(QHash<QString, VStandartTableCell>()), incrementTable(QHash<QString, VIncrementTableRow>()),
    lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), splines(QHash<qint64, VSpline>()),
    modelingSplines(QHash<qint64, VSpline>()),
    lengthSplines(QHash<QString, qreal>()), arcs(QHash<qint64, VArc>()), modelingArcs(QHash<qint64, VArc>()),
    lengthArcs(QHash<QString, qreal>()),
    splinePaths(QHash<qint64, VSplinePath>()), modelingSplinePaths(QHash<qint64, VSplinePath>()),
    details(QHash<qint64, VDetail>()){
    setData(data);
}

void VContainer::setData(const VContainer &data){
    base = *data.DataBase();
    points = *data.DataPoints();
    modelingPoints = *data.DataModelingPoints();
    standartTable = *data.DataStandartTable();
    incrementTable = *data.DataIncrementTable();
    lengthLines = *data.DataLengthLines();
    lineAngles = *data.DataLineAngles();
    splines = *data.DataSplines();
    modelingSplines = *data.DataModelingSplines();
    lengthSplines = *data.DataLengthSplines();
    arcs = *data.DataArcs();
    modelingArcs = *data.DataModelingArcs();
    lengthArcs = *data.DataLengthArcs();
    splinePaths = *data.DataSplinePaths();
    modelingSplinePaths = *data.DataModelingSplinePaths();
    details = *data.DataDetails();
}

template <typename key, typename val>
val VContainer::GetObject(const QHash<key, val> &obj, key id){
    if(obj.contains(id)){
        return obj.value(id);
    } else {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

VPointF VContainer::GetPoint(qint64 id) const{
    return GetObject(points, id);
}

VPointF VContainer::GetModelingPoint(qint64 id) const{
    return GetObject(modelingPoints, id);
}

VStandartTableCell VContainer::GetStandartTableCell(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(standartTable, name);
}

VIncrementTableRow VContainer::GetIncrementTableRow(const QString& name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(incrementTable, name);
}

qreal VContainer::GetLine(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(lengthLines, name);
}

qreal VContainer::GetLengthArc(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(lengthArcs, name);
}

qreal VContainer::GetLengthSpline(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(lengthSplines, name);
}

qreal VContainer::GetLineAngle(const QString &name) const{
    Q_ASSERT(!name.isEmpty());
    return GetObject(lineAngles, name);
}

VSpline VContainer::GetSpline(qint64 id) const{
    return GetObject(splines, id);
}

VSpline VContainer::GetModelingSpline(qint64 id) const{
    return GetObject(modelingSplines, id);
}

VArc VContainer::GetArc(qint64 id) const{
    return GetObject(arcs, id);
}

VArc VContainer::GetModelingArc(qint64 id) const{
    return GetObject(modelingArcs, id);
}

VSplinePath VContainer::GetSplinePath(qint64 id) const{
    return GetObject(splinePaths, id);
}

VSplinePath VContainer::GetModelingSplinePath(qint64 id) const{
    return GetObject(modelingSplinePaths, id);
}

VDetail VContainer::GetDetail(qint64 id) const{
    return GetObject(details, id);
}

void VContainer::AddStandartTableCell(const QString& name, const VStandartTableCell& cell){
    standartTable[name] = cell;
}

void VContainer::AddIncrementTableRow(const QString& name, const VIncrementTableRow& cell){
    incrementTable[name] = cell;
}

qint64 VContainer::getId(){
    return _id;
}

qint64 VContainer::getNextId(){
    _id++;
    return _id;
}

void VContainer::UpdateId(qint64 newId){
    if(newId > _id){
       _id = newId;
    }
}

QPainterPath VContainer::ContourPath(qint64 idDetail) const{
    VDetail detail = GetDetail(idDetail);
    QVector<QPointF> points;
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        switch(detail[i].getTypeTool()){
        case(Tool::NodePoint):{
            VPointF point = GetModelingPoint(detail[i].getId());
            points.append(point.toQPointF());
        }
            break;
        case(Tool::NodeArc):{
            VArc arc = GetModelingArc(detail[i].getId());
            qreal len1 = GetLengthContour(points, arc.GetPoints());
            qreal lenReverse = GetLengthContour(points, GetReversePoint(arc.GetPoints()));
            if(len1 <= lenReverse){
                points << arc.GetPoints();
            } else {
                points << GetReversePoint(arc.GetPoints());
            }
        }
            break;
        case(Tool::NodeSpline):{
            VSpline spline = GetModelingSpline(detail[i].getId());
            qreal len1 = GetLengthContour(points, spline.GetPoints());
            qreal lenReverse = GetLengthContour(points, GetReversePoint(spline.GetPoints()));
            if(len1 <= lenReverse){
                points << spline.GetPoints();
            } else {
                points << GetReversePoint(spline.GetPoints());
            }
        }
            break;
        case(Tool::NodeSplinePath):{
            VSplinePath splinePath = GetModelingSplinePath(detail[i].getId());
            qreal len1 = GetLengthContour(points, splinePath.GetPathPoints());
            qreal lenReverse = GetLengthContour(points, GetReversePoint(splinePath.GetPathPoints()));
            if(len1 <= lenReverse){
                points << splinePath.GetPathPoints();
            } else {
                points << GetReversePoint(splinePath.GetPathPoints());
            }
        }
            break;
        case(Tool::SplineTool):
            break;//Nothing to do, just ignore.
        default:
            qWarning()<<"Get wrong tool type. Ignore."<<detail[i].getTypeTool();
            break;
        } 
    }
    QPainterPath ekv = Equidistant(points, Detail::CloseEquidistant, toPixel(10));
    QPainterPath path;
    path.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i){
        path.lineTo(points[i]);
    }
    path.lineTo(points[0]);
    path.addPath(ekv);
    return path;
}

QPainterPath VContainer::Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv,
                                     const qreal &width) const{
    QPainterPath ekv;
    QVector<QPointF> ekvPoints;
    if ( points.size() < 3 ){
        qDebug()<<"Not enough points for build equidistant.\n";
        return ekv;
    }
    for (qint32 i = 0; i < points.size(); ++i ){
        if(i != points.size()-1){
            if(points[i] == points[i+1]){
                points.remove(i+1);
            }
        } else {
            if(points[i] == points[0]){
                points.remove(i);
            }
        }
    }
    if(eqv == Detail::CloseEquidistant){
        points.append(points.at(0));
    }
    for (qint32 i = 0; i < points.size(); ++i ){
        if ( i == 0 && eqv == Detail::CloseEquidistant){//перша точка, ламана замкнена
            ekvPoints<<EkvPoint(QLineF(points[points.size()-2], points[points.size()-1]),
                    QLineF(points[1], points[0]), width);
            continue;
        } else if(i == 0 && eqv == Detail::OpenEquidistant){//перша точка, ламана не замкнена
            ekvPoints.append(SingleParallelPoint(QLineF(points[0], points[1]), 90, width));
            continue;
        }
        if(i == points.size()-1 && eqv == Detail::CloseEquidistant){//остання точка, ламана замкнена
            ekvPoints.append(ekvPoints.at(0));
            continue;
        } else if(i == points.size()-1 && eqv == Detail::OpenEquidistant){//остання точка, ламана не замкнена
                ekvPoints.append(SingleParallelPoint(QLineF(points[points.size()-1],
                                                   points[points.size()-2]), -90, width));
                continue;
        }
        //точка яка не лежить ні на початку ні в кінці
        ekvPoints<<EkvPoint(QLineF(points[i-1], points[i]), QLineF(points[i+1], points[i]), width);
    }
    ekvPoints = CheckLoops(ekvPoints);
    ekv.moveTo(ekvPoints[0]);
    for (qint32 i = 1; i < ekvPoints.count(); ++i){
        ekv.lineTo(ekvPoints[i]);
    }
    return ekv;
}

QLineF VContainer::ParallelLine(const QLineF &line, qreal width){
    Q_ASSERT(width > 0);
    QLineF paralel = QLineF (SingleParallelPoint(line, 90, width),
                             SingleParallelPoint(QLineF(line.p2(), line.p1()), -90, width));
    return paralel;
}

QPointF VContainer::SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width){
    Q_ASSERT(width > 0);
    QLineF l = line;
    l.setAngle( l.angle() + angle );
    l.setLength( width );
    return l.p2();
}

QVector<QPointF> VContainer::EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width) const{
    Q_ASSERT(width > 0);
    QVector<QPointF> points;
    if(line1.p2() != line2.p2()){
        qWarning()<<"Last point of two lines must be equal.";
    }
    QPointF CrosPoint;
    QLineF bigLine1 = ParallelLine(line1, width );
    QLineF bigLine2 = ParallelLine(QLineF(line2.p2(), line2.p1()), width );
    QLineF::IntersectType type = bigLine1.intersect( bigLine2, &CrosPoint );
    switch(type){
    case(QLineF::BoundedIntersection):
        points.append(CrosPoint);
        return points;
        break;
    case(QLineF::UnboundedIntersection):{
        QLineF line( line1.p2(), CrosPoint );
        if(line.length() > width + toPixel(8)){
            QLineF l;
            l = QLineF(bigLine1.p2(), CrosPoint);
            l.setLength(width);
            points.append(l.p2());

            l = QLineF(bigLine2.p1(), CrosPoint);
            l.setLength(width);
            points.append(l.p2());
        } else {
            points.append(CrosPoint);
            return points;
        }
        break;
    }
    case(QLineF::NoIntersection):
        /*If we have correct lines this means lines lie on a line.*/
        points.append(bigLine1.p2());
        return points;
        break;
    }
    return points;
}

QVector<QPointF> VContainer::CheckLoops(const QVector<QPointF> &points) const{
    QVector<QPointF> ekvPoints;
    /*If we got less than 4 points no need seek loops.*/
    if(points.size() < 4){
       return ekvPoints;
    }
    bool closed = false;
    if(points.at(0) == points.at(points.size()-1)){
        closed = true;
    }
    qint32 i, j;
    for(i = 0; i < points.size(); ++i){
        /*Last three points no need check.*/
        if(i >= points.size()-3){
            ekvPoints.append(points.at(i));
            continue;
        }
        QPointF crosPoint;
        QLineF::IntersectType intersect = QLineF::NoIntersection;
        QLineF line1(points.at(i),points.at(i+1));
        for(j = i+2; j < points.size()-1; ++j){
            QLineF line2(points.at(j),points.at(j+1));
            intersect = line1.intersect(line2, &crosPoint);
            if(intersect == QLineF::BoundedIntersection){
                break;
            }
        }
        if(intersect == QLineF::BoundedIntersection){
            if(i == 0 && j+1 == points.size()-1 && closed){
                /*We got closed contour.*/
                ekvPoints.append(points.at(i));
            } else {
                /*We found loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(crosPoint);
                ekvPoints.append(points.at(j+1));
                i = j + 2;
            }
        } else {
            /*We did not found loop.*/
            ekvPoints.append(points.at(i));
        }
    }
    return ekvPoints;
}

void VContainer::PrepareDetails(QVector<VItem *> &list) const{
    QHashIterator<qint64, VDetail> iDetail(details);
    while (iDetail.hasNext()) {
        iDetail.next();
        list.append(new VItem(ContourPath(iDetail.key()), list.size()));
    }
}

void VContainer::RemoveIncrementTableRow(const QString& name){
    incrementTable.remove(name);
}

template <typename val>
void VContainer::UpdateObject(QHash<qint64, val> &obj, const qint64 &id, const val& point){
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    obj[id] = point;
    UpdateId(id);
}

void VContainer::UpdatePoint(qint64 id, const VPointF& point){
    UpdateObject(points, id, point);
}

void VContainer::UpdateModelingPoint(qint64 id, const VPointF &point){
    UpdateObject(modelingPoints, id, point);
}

void VContainer::UpdateDetail(qint64 id, const VDetail &detail){
    UpdateObject(details, id, detail);
}

void VContainer::UpdateSpline(qint64 id, const VSpline &spl){
    UpdateObject(splines, id, spl);
}

void VContainer::UpdateModelingSpline(qint64 id, const VSpline &spl){
    UpdateObject(modelingSplines, id, spl);
}

void VContainer::UpdateSplinePath(qint64 id, const VSplinePath &splPath){
    UpdateObject(splinePaths, id, splPath);
}

void VContainer::UpdateModelingSplinePath(qint64 id, const VSplinePath &splPath){
    UpdateObject(modelingSplinePaths, id, splPath);
}

void VContainer::UpdateArc(qint64 id, const VArc &arc){
    UpdateObject(arcs, id, arc);
}

void VContainer::UpdateModelingArc(qint64 id, const VArc &arc){
    UpdateObject(modelingArcs, id, arc);
}

void VContainer::UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell){
    standartTable[name] = cell;
}

void VContainer::UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell){
    incrementTable[name] = cell;
}

void VContainer::AddLengthSpline(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthSplines[name] = value;
}

void VContainer::AddLengthArc(const qint64 &center, const qint64 &id){
    AddLengthArc(GetNameArc(center, id), toMM(GetArc(id).GetLength()));
}

void VContainer::AddLengthArc(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthArcs[name] = value;
}

void VContainer::AddLineAngle(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lineAngles[name] = value;
}

qreal VContainer::GetValueStandartTableCell(const QString& name) const{
    VStandartTableCell cell =  GetStandartTableCell(name);
    qreal k_size 	= ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.GetBase() + k_size*cell.GetKsize() 	+ k_growth*cell.GetKgrowth();
    return value;
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const{
    VIncrementTableRow cell =  GetIncrementTableRow(name);
    qreal k_size 	= ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.getBase() + k_size*cell.getKsize() + k_growth*cell.getKgrowth();
    return value;
}

void VContainer::Clear(){
    _id = 0;
    standartTable.clear();
    incrementTable.clear();
    lengthLines.clear();
    lengthArcs.clear();
    lineAngles.clear();
    details.clear();
    modelingArcs.clear();
    modelingPoints.clear();
    modelingSplinePaths.clear();
    modelingSplines.clear();
    ClearObject();
    CreateManTableIGroup ();
}

void VContainer::ClearObject(){
    points.clear();
    splines.clear();
    arcs.clear();
    splinePaths.clear();
}

void VContainer::ClearIncrementTable(){
    incrementTable.clear();
}

void VContainer::ClearLengthLines(){
    lengthLines.clear();
}

void VContainer::ClearLengthSplines(){
    lengthSplines.clear();
}

void VContainer::ClearLengthArcs(){
    lengthArcs.clear();
}

void VContainer::ClearLineAngles(){
    lineAngles.clear();
}

void VContainer::SetSize(qint32 size){
    base["Сг"] = size;
}

void VContainer::SetGrowth(qint32 growth){
    base["Р"] = growth;
}

qint32 VContainer::size() const{
    return base.value("Сг");
}

qint32 VContainer::growth() const{
    return base.value("Р");
}

bool VContainer::IncrementTableContains(const QString& name){
    return incrementTable.contains(name);
}

qreal VContainer::FindVar(const QString &name, bool *ok)const{
    if(base.contains(name)){
        *ok = true;
        return base.value(name);
    }

    if(standartTable.contains(name)){
        *ok = true;
        return GetValueStandartTableCell(name);
    }
    if(incrementTable.contains(name)){
        *ok = true;
        return GetValueIncrementTableRow(name);
    }
    if(lengthLines.contains(name)){
        *ok = true;
        return lengthLines.value(name);
    }
    if(lengthArcs.contains(name)){
        *ok = true;
        return lengthArcs.value(name);
    }
    if(lineAngles.contains(name)){
        *ok = true;
        return lineAngles.value(name);
    }
    if(lengthSplines.contains(name)){
        *ok = true;
        return lengthSplines.value(name);
    }
    *ok = false;
    return 0;
}

const QHash<qint64, VPointF> *VContainer::DataPoints() const{
    return &points;
}

const QHash<qint64, VPointF> *VContainer::DataModelingPoints() const{
    return &modelingPoints;
}

const QHash<qint64, VSpline> *VContainer::DataSplines() const{
    return &splines;
}

const QHash<qint64, VSpline> *VContainer::DataModelingSplines() const{
    return &modelingSplines;
}

const QHash<qint64, VArc> *VContainer::DataArcs() const{
    return &arcs;
}

const QHash<qint64, VArc> *VContainer::DataModelingArcs() const{
    return &modelingArcs;
}

const QHash<QString, qint32> *VContainer::DataBase() const{
    return &base;
}

const QHash<QString, VStandartTableCell> *VContainer::DataStandartTable() const{
    return &standartTable;
}

const QHash<QString, VIncrementTableRow> *VContainer::DataIncrementTable() const{
    return &incrementTable;
}

const QHash<QString, qreal> *VContainer::DataLengthLines() const{
    return &lengthLines;
}

const QHash<QString, qreal> *VContainer::DataLengthSplines() const{
    return &lengthSplines;
}

const QHash<QString, qreal> *VContainer::DataLengthArcs() const{
    return &lengthArcs;
}

const QHash<QString, qreal> *VContainer::DataLineAngles() const{
    return &lineAngles;
}

const QHash<qint64, VSplinePath> *VContainer::DataSplinePaths() const{
    return &splinePaths;
}

const QHash<qint64, VSplinePath> *VContainer::DataModelingSplinePaths() const{
    return &modelingSplinePaths;
}

const QHash<qint64, VDetail> *VContainer::DataDetails() const{
    return &details;
}

void VContainer::AddLine(const qint64 &firstPointId, const qint64 &secondPointId, Draw::Draws mode){
    QString nameLine = GetNameLine(firstPointId, secondPointId, mode);
    VPointF first;
    VPointF second;
    if(mode == Draw::Calculation){
        first = GetPoint(firstPointId);
        second = GetPoint(secondPointId);
    } else {
        first = GetModelingPoint(firstPointId);
        second = GetModelingPoint(secondPointId);
    }
    AddLengthLine(nameLine, toMM(QLineF(first.toQPointF(), second.toQPointF()).length()));
    nameLine = GetNameLineAngle(firstPointId, secondPointId, mode);
    AddLineAngle(nameLine, QLineF(first.toQPointF(), second.toQPointF()).angle());
}

template <typename key, typename val>
qint64 VContainer::AddObject(QHash<key, val> &obj, const val& value){
    qint64 id = getNextId();
    obj[id] = value;
    return id;
}

qint64 VContainer::AddPoint(const VPointF& point){
    return AddObject(points, point);
}

qint64 VContainer::AddModelingPoint(const VPointF &point){
    return AddObject(modelingPoints, point);
}

qint64 VContainer::AddDetail(const VDetail &detail){
    return AddObject(details, detail);
}

qint64 VContainer::AddSpline(const VSpline &spl){
    return AddObject(splines, spl);
}

qint64 VContainer::AddModelingSpline(const VSpline &spl){
    return AddObject(modelingSplines, spl);
}

qint64 VContainer::AddSplinePath(const VSplinePath &splPath){
    return AddObject(splinePaths, splPath);
}

qint64 VContainer::AddModelingSplinePath(const VSplinePath &splPath){
    return AddObject(modelingSplinePaths, splPath);
}

qint64 VContainer::AddArc(const VArc &arc){
    return AddObject(arcs, arc);
}

qint64 VContainer::AddModelingArc(const VArc &arc){
    return AddObject(modelingArcs, arc);
}

QString VContainer::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint, Draw::Draws mode) const{
    VPointF first;
    VPointF second;
    if(mode == Draw::Calculation){
        first = GetPoint(firstPoint);
        second = GetPoint(secondPoint);
    } else {
        first = GetModelingPoint(firstPoint);
        second = GetModelingPoint(secondPoint);
    }
    return QString("Line_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint,
                                     Draw::Draws mode) const{
    VPointF first;
    VPointF second;
    if(mode == Draw::Calculation){
        first = GetPoint(firstPoint);
        second = GetPoint(secondPoint);
    } else {
        first = GetModelingPoint(firstPoint);
        second = GetModelingPoint(secondPoint);
    }
    return QString("AngleLine_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint,
                                  Draw::Draws mode) const{
    VPointF first;
    VPointF second;
    if(mode == Draw::Calculation){
        first = GetPoint(firstPoint);
        second = GetPoint(secondPoint);
    } else {
        first = GetModelingPoint(firstPoint);
        second = GetModelingPoint(secondPoint);
    }
    return QString("Spl_%1_%2").arg(first.name(), second.name());
}

QString VContainer::GetNameSplinePath(const VSplinePath &path, Draw::Draws mode) const{
    if(path.Count() == 0){
        return QString();
    }
    QString name("SplPath");
    for(qint32 i = 1; i <= path.Count(); ++i){
        VSpline spl = path.GetSpline(i);
        VPointF first;
        VPointF second;
        if(mode == Draw::Calculation){
            first = GetPoint(spl.GetP1());
            second = GetPoint(spl.GetP4());
        } else {
            first = GetModelingPoint(spl.GetP1());
            second = GetModelingPoint(spl.GetP4());
        }
        QString splName = QString("_%1_%2").arg(first.name(), second.name());
        name.append(splName);
    }
    return name;
}

QString VContainer::GetNameArc(const qint64 &center, const qint64 &id, Draw::Draws mode) const{
    VPointF centerPoint;
    if(mode == Draw::Calculation){
        centerPoint = GetPoint(center);
    } else {
        centerPoint = GetModelingPoint(center);
    }
    return QString ("Arc_%1_%2").arg(centerPoint.name()).arg(id);
}

void VContainer::AddLengthLine(const QString &name, const qreal &value){
    Q_ASSERT(!name.isEmpty());
    lengthLines[name] = value;
}

void VContainer::CreateManTableIGroup (){
    AddStandartTableCell("Pkor", VStandartTableCell(84, 0, 3));
    AddStandartTableCell("Pkor", VStandartTableCell(84, 0, 3));
    AddStandartTableCell("Vtos", VStandartTableCell(1450, 2, 51));
    AddStandartTableCell("Vtosh", VStandartTableCell(1506, 2, 54));
    AddStandartTableCell("Vpt", VStandartTableCell(1438, 3, 52));
    AddStandartTableCell("Vst", VStandartTableCell(1257, -1, 49));
    AddStandartTableCell("Vlt", VStandartTableCell(1102, 0, 43));
    AddStandartTableCell("Vk", VStandartTableCell(503, 0, 22));
    AddStandartTableCell("Vsht", VStandartTableCell(1522, 2, 54));
    AddStandartTableCell("Vzy", VStandartTableCell(1328, 0, 49));
    AddStandartTableCell("Vlop", VStandartTableCell(1320, 0, 49));
    AddStandartTableCell("Vps", VStandartTableCell(811, -1, 36));
    AddStandartTableCell("Osh", VStandartTableCell(404,8, 2));
    AddStandartTableCell("OgI", VStandartTableCell(1034, 36, 4));
    AddStandartTableCell("OgII", VStandartTableCell(1044, 38, 2));
    AddStandartTableCell("OgIII", VStandartTableCell(1000, 40, 0));
    AddStandartTableCell("Ot", VStandartTableCell(780, 40, 0));
    AddStandartTableCell("Ob", VStandartTableCell(984, 30, 10));
    AddStandartTableCell("ObI", VStandartTableCell(964, 24, 12));
    AddStandartTableCell("Obed", VStandartTableCell(566, 18, 6));
    AddStandartTableCell("Ok", VStandartTableCell(386, 8, 8));
    AddStandartTableCell("Oi", VStandartTableCell(380, 8, 6));
    AddStandartTableCell("Osch", VStandartTableCell(234, 4, 4));
    AddStandartTableCell("Os", VStandartTableCell(350, 2, 8));
    AddStandartTableCell("Dsb", VStandartTableCell(1120, 0, 44));
    AddStandartTableCell("Dsp", VStandartTableCell(1110, 0, 43));
    AddStandartTableCell("Dn", VStandartTableCell(826, -3, 37));
    AddStandartTableCell("Dps", VStandartTableCell(316, 4, 7));
    AddStandartTableCell("Dpob", VStandartTableCell(783, 14, 15));
    AddStandartTableCell("Ds", VStandartTableCell(260, 1, 6));
    AddStandartTableCell("Op", VStandartTableCell(316, 12, 0));
    AddStandartTableCell("Ozap", VStandartTableCell(180, 4, 0));
    AddStandartTableCell("Pkis", VStandartTableCell(250, 4, 0));
    AddStandartTableCell("SHp", VStandartTableCell(160, 1, 4));
    AddStandartTableCell("Dlych", VStandartTableCell(500, 2, 15));
    AddStandartTableCell("Dzap", VStandartTableCell(768, 2, 24));
    AddStandartTableCell("DIIIp", VStandartTableCell(970, 2, 29));
    AddStandartTableCell("Vprp", VStandartTableCell(214, 3, 3));
    AddStandartTableCell("Vg", VStandartTableCell(262, 8, 3));
    AddStandartTableCell("Dtp", VStandartTableCell(460, 7, 9));
    AddStandartTableCell("Dp", VStandartTableCell(355, 5, 5));
    AddStandartTableCell("Vprz", VStandartTableCell(208, 3, 5));
    AddStandartTableCell("Dts", VStandartTableCell(438, 2, 10));
    AddStandartTableCell("DtsI", VStandartTableCell(469, 2, 10));
    AddStandartTableCell("Dvcht", VStandartTableCell(929, 9, 19));
    AddStandartTableCell("SHg", VStandartTableCell(370, 14, 4));
    AddStandartTableCell("Cg", VStandartTableCell(224, 6, 0));
    AddStandartTableCell("SHs", VStandartTableCell(416, 10, 2));
    AddStandartTableCell("dpzr", VStandartTableCell(121, 6, 0));
    AddStandartTableCell("Ogol", VStandartTableCell(576, 4, 4));
    AddStandartTableCell("Ssh1", VStandartTableCell(205, 5, 0));
    AddStandartTableCell("St", VStandartTableCell(410, 20, 0));
    AddStandartTableCell("Drzap", VStandartTableCell(594, 3, 19));
    AddStandartTableCell("DbII", VStandartTableCell(1020, 0, 44));
    AddStandartTableCell("Sb", VStandartTableCell(504, 15, 4));
}

QVector<QPointF> VContainer::GetReversePoint(const QVector<QPointF> &points) const{
    Q_ASSERT(points.size() > 0);
    QVector<QPointF> reversePoints;
    for (qint32 i = points.size() - 1; i >= 0; --i) {
        reversePoints.append(points.at(i));
    }
    return reversePoints;
}

qreal VContainer::GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints) const{
    qreal length = 0;
    QVector<QPointF> points;
    points << contour << newPoints;
    for (qint32 i = 0; i < points.size()-1; ++i) {
        QLineF line(points.at(i), points.at(i+1));
        length += line.length();
    }
    return length;
}

#include "vsplinepath.h"

VSplinePath::VSplinePath(): path(QVector<VSplinePoint>()), kCurve(1), mode(Draw::Calculation), points(0),
    _referens(0), idObject(0){
}

VSplinePath::VSplinePath(const QMap<qint64, VPointF> *points, qreal kCurve, Draw::Mode mode, qint64 idObject): path(QVector<VSplinePoint>()),
    kCurve(kCurve), mode(mode), points(points), _referens(0), idObject(idObject){
}

VSplinePath::VSplinePath(const VSplinePath &splPath): path(*splPath.GetPoint()),
    kCurve(splPath.getKCurve()), mode(splPath.getMode()), points( splPath.GetDataPoints()), _referens(0),
    idObject(splPath.getIdObject()){
}

Draw::Mode VSplinePath::getMode() const{
    return mode;
}

void VSplinePath::setMode(const Draw::Mode &value){
    mode = value;
}

void VSplinePath::append(VSplinePoint point){
    path.append(point);
}

qint32 VSplinePath::Count() const{
    if(path.size() == 0){
        return 0;
    } else {
        return path.size() - 1;
    }
}

qint32 VSplinePath::CountPoint() const{
    return path.size();
}

VSpline VSplinePath::GetSpline(qint32 index) const{
    if(Count()<1){
        throw "Недостатня кількість точок для створення сплайну.";
    }
    if(index < 1 || index > Count()){
        throw "Такого сплайну немає.";
    }
    VSpline spl(points, path[index-1].P(), path[index].P(), path[index-1].Angle2(), path[index].Angle1(),
            path[index-1].KAsm2(), path[index].KAsm1(), this->kCurve);
    return spl;
}

QPainterPath VSplinePath::GetPath() const{
    QPainterPath painterPath;
    for(qint32 i = 1; i <= Count(); ++i){
        VSpline spl(points, path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(), path[i-1].KAsm2(),
                path[i].KAsm1(), this->kCurve);
        painterPath.addPath(spl.GetPath());
    }
    return painterPath;
}

QVector<QPointF> VSplinePath::GetPathPoints() const{
    QVector<QPointF> pathPoints;
    for(qint32 i = 1; i <= Count(); ++i){
        VSpline spl(points, path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(), path[i-1].KAsm2(),
                path[i].KAsm1(), this->kCurve);
        QVector<QPointF> splP = spl.GetPoints();
        for(qint32 j = 0; i < splP.size(); ++j){
            pathPoints.append(splP[j]);
        }
    }
    return pathPoints;
}

QVector<VSplinePoint> VSplinePath::GetSplinePath() const{
    return path;
}

qreal VSplinePath::GetLength() const{
    qreal length = 0;
    for(qint32 i = 1; i <= Count(); ++i){
        VSpline spl(points, path[i-1].P(), path[i].P(), path[i-1].Angle2(), path[i].Angle1(), path[i-1].KAsm2(),
                path[i].KAsm1(), kCurve);
        length += spl.GetLength();
    }
    return length;
}

const QMap<qint64, VPointF> *VSplinePath::GetDataPoints() const{
    return points;
}

void VSplinePath::UpdatePoint(qint32 indexSpline, SplinePoint::Position pos, VSplinePoint point){
    if(indexSpline < 1 || indexSpline > Count()){
        throw "Такого сплайну немає.";
    }
    if(pos == SplinePoint::FirstPoint){
        path[indexSpline-1] = point;
    } else {
        path[indexSpline] = point;
    }
}

VSplinePoint VSplinePath::GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos){
    if(indexSpline < 1 || indexSpline > Count()){
        throw "Такого сплайну немає.";
    }
    if(pos == SplinePoint::FirstPoint){
        return path[indexSpline-1];
    } else {
        return path[indexSpline];
    }
}

void VSplinePath::Clear(){
    path.clear();
}

qreal VSplinePath::getKCurve() const{
    return kCurve;
}

void VSplinePath::setKCurve(const qreal &value){
    kCurve = value;
}

const QVector<VSplinePoint> *VSplinePath::GetPoint() const{
    return &path;
}

VSplinePath &VSplinePath::operator =(const VSplinePath &path){
    this->path = path.GetSplinePath();
    this->kCurve = path.getKCurve();
    this->mode = path.getMode();
    this->points = path.GetDataPoints();
    return *this;
}

VSplinePoint & VSplinePath::operator[](int indx){
    return path[indx];
}

qint32 VSplinePath::referens() const{
    return _referens;
}

void VSplinePath::incrementReferens(){
    ++_referens;
}

void VSplinePath::decrementReferens(){
    if(_referens > 0){
        --_referens;
    }
}

qint64 VSplinePath::getIdObject() const
{
return idObject;
}

void VSplinePath::setIdObject(const qint64 &value)
{
idObject = value;
}

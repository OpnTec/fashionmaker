#include "vsplinepath.h"

VSplinePath::VSplinePath(): path(QVector<VSplinePoint>()), kCurve(1), points(0){
}

VSplinePath::VSplinePath(const QMap<qint64, VPointF> *points, qreal kCurve): path(QVector<VSplinePoint>()),
    kCurve(kCurve), points(points){
}

VSplinePath::VSplinePath(const VSplinePath &splPath): path(*splPath.GetPoint()),
    kCurve(splPath.getKCurve()), points( splPath.GetDataPoints()){
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
    this->points = path.GetDataPoints();
    return *this;
}

VSplinePoint & VSplinePath::operator[](int indx){
    return path[indx];
}

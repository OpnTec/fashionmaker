#include "vsplinepoint.h"

VSplinePoint::VSplinePoint():pSpline(0), angle(0), kAsm1(1), kAsm2(1){
}

VSplinePoint::VSplinePoint(qint64 pSpline, qreal kAsm1, qreal angle , qreal kAsm2):pSpline(pSpline),
    angle(angle), kAsm1(kAsm1), kAsm2(kAsm2){
}

VSplinePoint::VSplinePoint(const VSplinePoint &point):pSpline(point.P()), angle(point.Angle2()),
    kAsm1(point.KAsm1()), kAsm2(point.KAsm2()){
}

VSplinePoint::~VSplinePoint(){
}

qint64 VSplinePoint::P() const{
    return pSpline;
}

void VSplinePoint::SetP(const qint64 &value){
    pSpline = value;
}

qreal VSplinePoint::Angle1() const{
    return angle+180;
}

void VSplinePoint::SetAngle(const qreal &value){
    angle = value;
}

qreal VSplinePoint::Angle2() const{
    return angle;
}

qreal VSplinePoint::KAsm1() const{
    return kAsm1;
}

void VSplinePoint::SetKAsm1(const qreal &value){
    kAsm1 = value;
}

qreal VSplinePoint::KAsm2() const{
    return kAsm2;
}

void VSplinePoint::SetKAsm2(const qreal &value){
    kAsm2 = value;
}

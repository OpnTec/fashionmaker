#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#pragma GCC diagnostic ignored "-Weffc++"
#include "vtoolpoint.h"
#pragma GCC diagnostic warning "-Weffc++"

class VToolLinePoint : public VToolPoint
{
public:
    VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                   const QString &typeLine, const QString &formula,
                   const qint64 &basePointId, const qint32 &angle, QGraphicsItem * parent = 0);
public slots:
    virtual void      ChangedActivDraw(const QString newName);
    virtual void      RefreshGeometry();
protected:
    QString           typeLine;
    QString           formula;
    qint32            angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
};

#endif // VTOOLLINEPOINT_H

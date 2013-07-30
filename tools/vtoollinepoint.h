#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#include "vtoolpoint.h"

class VToolLinePoint : public VToolPoint
{
public:
    VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                   const QString &typeLine, const QString &formula,
                   const qint64 &pointId, const qint32 &angle, QGraphicsItem * parent = 0);
public slots:
    virtual void      ChangedActivDraw(const QString newName);
protected:
    QString           typeLine;
    QString           formula;
    qint32            angle;
    QGraphicsLineItem *mainLine;
};

#endif // VTOOLLINEPOINT_H

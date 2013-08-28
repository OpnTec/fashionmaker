#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "vtoolpoint.h"
#pragma GCC diagnostic pop

class VToolLinePoint : public VToolPoint
{
public:
    VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id, Draw::Mode mode,
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
private:
    VToolLinePoint(const VToolLinePoint &tool);
    const VToolLinePoint &operator=(const VToolLinePoint &tool);
};

#endif // VTOOLLINEPOINT_H

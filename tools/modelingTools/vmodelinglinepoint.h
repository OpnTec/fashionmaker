#ifndef VMODELINGLINEPOINT_H
#define VMODELINGLINEPOINT_H

#include "vmodelingpoint.h"

class VModelingLinePoint : public VModelingPoint
{
    Q_OBJECT
public:
    VModelingLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                   const QString &typeLine, const QString &formula,
                   const qint64 &basePointId, const qreal &angle, QGraphicsItem * parent = 0);
protected:
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    virtual void      RefreshGeometry();
private:
                      VModelingLinePoint(const VModelingLinePoint &tool);
    const             VModelingLinePoint &operator=(const VModelingLinePoint &tool);
};

#endif // VMODELINGLINEPOINT_H

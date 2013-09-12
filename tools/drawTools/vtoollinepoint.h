#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#include "vtoolpoint.h"

class VToolLinePoint : public VToolPoint
{
    Q_OBJECT
public:
    VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                   const QString &typeLine, const QString &formula,
                   const qint64 &basePointId, const qreal &angle, QGraphicsItem * parent = 0);
public slots:
    virtual void      ChangedActivDraw(const QString newName); 
protected:
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    virtual void      RefreshGeometry();
private:
                      VToolLinePoint(const VToolLinePoint &tool);
    const             VToolLinePoint &operator=(const VToolLinePoint &tool);
};

#endif // VTOOLLINEPOINT_H

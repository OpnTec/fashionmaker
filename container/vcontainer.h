#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <QMap>

#include "vpointf.h"

class VContainer
{
public:
    VContainer();
    VPointF GetPoint(qint64 id) const;
    qint64  getId();
    qint64  AddPoint(const VPointF& point);
    void    UpdatePoint(qint64 id, const VPointF& point);
    void    Clear();
private:
    qint64 _id;
    QMap<qint64, VPointF> points;
    qint64 getNextId();
};

#endif // VCONTAINER_H

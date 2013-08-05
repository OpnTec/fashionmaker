#ifndef VTOOLSIMPLEPOINT_H
#define VTOOLSIMPLEPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "../options.h"
#include "vabstracttool.h"

class VToolSimplePoint: public VAbstractTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolSimplePoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
public slots:
    virtual void            ChangedActivDraw(const QString newName);
protected:
    virtual void            RefreshGeometry();
};

#endif // VTOOLSIMPLEPOINT_H

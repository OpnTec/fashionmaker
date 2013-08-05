#include "vtoolsimplepoint.h"
#include "../container/vpointf.h"

VToolSimplePoint::VToolSimplePoint(VDomDocument *doc, VContainer *data, qint64 id,
                                   QGraphicsItem *parent):VAbstractTool(doc, data, id),
    QGraphicsEllipseItem(parent){

}


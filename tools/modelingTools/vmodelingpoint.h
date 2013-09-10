#ifndef VMODELINGPOINT_H
#define VMODELINGPOINT_H

#include "vmodelingtool.h"
#include "widgets/vgraphicssimpletextitem.h"
#include "options.h"

class VModelingPoint: public VModelingTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VModelingPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
    virtual ~VModelingPoint();
public slots:
    void                    NameChangePosition(const QPointF pos);
    virtual void            FullUpdateFromGui(int result) = 0;
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
private:
    VModelingPoint(const VModelingPoint &tool);
    const VModelingPoint &operator=(const VModelingPoint &tool);
};

#endif // VMODELINGPOINT_H

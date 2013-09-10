#ifndef VGRAPHICSPOINT_H
#define VGRAPHICSPOINT_H

#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "widgets/vgraphicssimpletextitem.h"
#include "../container/vpointf.h"

class VGraphicsPoint: public QGraphicsEllipseItem
{
public:
                            VGraphicsPoint(QGraphicsItem *parent);
    virtual                 ~VGraphicsPoint();
public slots:
    virtual void            NameChangePosition(const QPointF pos)=0;
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            UpdateNamePosition(qreal mx, qreal my)=0;
    void                    RefreshLine();
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point)=0;
private:
                            VGraphicsPoint(const VGraphicsPoint &point);
    const VGraphicsPoint    &operator=(const VGraphicsPoint &point);
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 addVector (QPointF p, QPointF p1, QPointF p2, qreal k) const;
};

#endif // VGRAPHICSPOINT_H

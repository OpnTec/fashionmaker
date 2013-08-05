#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QObject>
#include "../options.h"
class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VControlPointSpline(const QPointF &controlPoint, const QPointF &splinePoint, QGraphicsItem * parent = 0);
    void RefreshLine(const QPointF &controlPoint, const QPointF &splinePoint);
signals:
    void                    ControlPointChangePosition(const QPointF pos);
protected:
    qreal                   radius;
    QGraphicsLineItem       *controlLine;
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    QVariant                itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 addVector (QPointF p, QPointF p1, QPointF p2, qreal k) const;

};

#endif // VCONTROLPOINTSPLINE_H

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QObject>
#pragma GCC diagnostic pop
#include "../options.h"
#include "../geometry/vsplinepath.h"

class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                          VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                              const QPointF &controlPoint, const QPointF &splinePoint,
                                              QGraphicsItem * parent = 0);
signals:
    void                  ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                     const QPointF pos);
public slots:
    void                  RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                      const QPointF &controlPoint, const QPointF &splinePoint);
    void                  setEnabledPoint(bool enable);
protected:
    qreal                 radius;
    QGraphicsLineItem     *controlLine;
    virtual void          hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void          hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    QVariant              itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    VControlPointSpline(const VControlPointSpline &pSpl);
    const VControlPointSpline &operator=(const VControlPointSpline &pSpl);
    qint32                indexSpline;
    SplinePoint::Position position;
    qint32                LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                              QPointF &p2) const;
    QPointF               ClosestPoint(QLineF line, QPointF p) const;
    QPointF               addVector (QPointF p, QPointF p1, QPointF p2, qreal k) const;

};

#endif // VCONTROLPOINTSPLINE_H

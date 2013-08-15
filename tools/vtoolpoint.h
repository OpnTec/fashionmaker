#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#pragma GCC diagnostic ignored "-Weffc++"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "../widgets/vgraphicssimpletextitem.h"
#include "vabstracttool.h"
#pragma GCC diagnostic warning "-Weffc++"
#include "../options.h"

class VToolPoint: public VAbstractTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
    virtual ~VToolPoint();
public slots:
    void                    NameChangePosition(const QPointF pos);
    virtual void            ChangedActivDraw(const QString newName);
    virtual void            FullUpdateFromGui(int result) = 0;
    virtual void            ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            RefreshGeometry();  
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    void                    LiteUpdateFromGui(qreal mx, qreal my);
    void                    RefreshLine();
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 addVector (QPointF p, QPointF p1, QPointF p2, qreal k) const;
};

#endif // VTOOLPOINT_H

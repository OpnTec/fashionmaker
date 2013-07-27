#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "../widgets/vgraphicssimpletextitem.h"
#include "../options.h"
#include "vabstracttool.h"

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
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    void                    RefreshBaseGeometry(const QString &name, const qreal &x, const qreal &y,
                                                const qreal &mx, const qreal &my);
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const;  
    void                    LiteUpdateFromGui(qreal mx, qreal my);
    void                    RefreshLine();
};

#endif // VTOOLPOINT_H

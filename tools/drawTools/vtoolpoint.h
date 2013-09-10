#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#include "vdrawtool.h"
#include "widgets/vgraphicssimpletextitem.h"
#include "options.h"

class VToolPoint: public VDrawTool, public QGraphicsEllipseItem
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
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
private:
    VToolPoint(const VToolPoint &tool);
    const VToolPoint &operator=(const VToolPoint &tool);
};

#endif // VTOOLPOINT_H

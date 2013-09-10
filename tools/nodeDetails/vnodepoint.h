#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include "vabstractnode.h"
#include "widgets/vgraphicssimpletextitem.h"

class VNodePoint: public VAbstractNode, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VNodePoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, Draw::Mode typeobject,
                   Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
    static VNodePoint *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                                  Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void            FullUpdateFromFile();
    void                    NameChangePosition(const QPointF pos);
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            AddToFile();
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
private:
                            VNodePoint(const VNodePoint &point);
    const VNodePoint        &operator=(const VNodePoint &point);
};

#endif // VNODEPOINT_H

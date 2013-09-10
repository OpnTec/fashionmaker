#ifndef VNODEARC_H
#define VNODEARC_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

class VNodeArc :public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VNodeArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Mode typeobject,
                 Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VNodeArc *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc,
                                Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                 FullUpdateFromFile();
protected:
    virtual void                 AddToFile();
    virtual void                 mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    void                         RefreshGeometry();
};

#endif // VNODEARC_H

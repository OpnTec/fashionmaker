#ifndef VNODESPLINEPATH_H
#define VNODESPLINEPATH_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

class VNodeSplinePath : public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VNodeSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                        Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VNodeSplinePath *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                       Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void FullUpdateFromFile();
protected:
    virtual void AddToFile();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    void         RefreshGeometry();
};

#endif // VNODESPLINEPATH_H

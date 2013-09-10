#ifndef VNODESPLINE_H
#define VNODESPLINE_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

class VNodeSpline:public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VNodeSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline, Draw::Mode typeobject,
                    Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VNodeSpline *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                   Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                   FullUpdateFromFile ();
protected:
    virtual void                   AddToFile ();
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    void                           RefreshGeometry ();
};

#endif // VNODESPLINE_H

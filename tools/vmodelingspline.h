#ifndef VMODELINGSPLINE_H
#define VMODELINGSPLINE_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>

class VModelingSpline:public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VModelingSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline, Draw::Mode typeobject,
                    Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VModelingSpline *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                   Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void                   FullUpdateFromGui(int result);
protected:
    virtual void                   AddToFile ();
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                   hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                   hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint64 idSpline;
    Draw::Mode typeobject;
    void                           RefreshGeometry ();
};

#endif // VMODELINGSPLINE_H

#ifndef VMODELINGSPLINEPATH_H
#define VMODELINGSPLINEPATH_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>

class VModelingSplinePath:public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VModelingSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                        Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VModelingSplinePath *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                       Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                     FullUpdateFromFile();
    virtual void                     ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void                     FullUpdateFromGui(int result);
protected:
    virtual void                     AddToFile();
    virtual void                     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint64 idSpline;
    Draw::Mode typeobject;
     void                             RefreshGeometry();
};

#endif // VMODELINGSPLINEPATH_H

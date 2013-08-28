#ifndef VMODELINGARC_H
#define VMODELINGARC_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>

class VModelingArc :public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VModelingArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Mode typeobject,
                 Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static VModelingArc *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc,
                                Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void                 FullUpdateFromGui(int result);
protected:
    virtual void                 AddToFile();
    virtual void                 mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                 hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                 hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint64 idArc;
    Draw::Mode typeobject;
    void                         RefreshGeometry();
};

#endif // VMODELINGARC_H

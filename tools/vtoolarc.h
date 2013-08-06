#ifndef VTOOLARC_H
#define VTOOLARC_H

#include "vabstracttool.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogarc.h"
#include "../widgets/vcontrolpointspline.h"

class VToolArc :public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolArc(VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
             QGraphicsItem * parent = 0);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
    virtual void                 ChangedActivDraw(const QString newName);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
    virtual void                 mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                 hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                 hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogArc> dialogArc;
    void                         RefreshGeometry();
};

#endif // VTOOLARC_H

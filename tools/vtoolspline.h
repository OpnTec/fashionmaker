#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include "vabstracttool.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogspline.h"
#include "../widgets/vcontrolpointspline.h"

class VToolSpline:public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                 VToolSpline(VDomDocument *doc, VContainer *data, qint64 id,
                                             Tool::Enum typeCreation, QGraphicsItem * parent = 0);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
    void                         ControlPoint1ChangePosition(const QPointF pos);
    void                         ControlPoint2ChangePosition(const QPointF pos);
    virtual void                 ChangedActivDraw(const QString newName);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
    virtual void                 mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                 hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                 hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogSpline> dialogSpline;
    VControlPointSpline *controlPoint1;
    VControlPointSpline *controlPoint2;
    void                         RefreshGeometry();
};

#endif // VTOOLSPLINE_H

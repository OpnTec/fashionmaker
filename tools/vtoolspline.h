#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include "vabstracttool.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogspline.h"
#include "../widgets/vcontrolpointspline.h"
#include "../geometry/vsplinepath.h"

class VToolSpline:public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                   VToolSpline ( VDomDocument *doc, VContainer *data, qint64 id,
                                                 Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
signals:
    void                           RefreshLine ( const qint32 &indexSpline, SplinePoint::Position position,
                                               const QPointF &controlPoint, const QPointF &splinePoint );
    void                           setEnabledPoint ( bool enable );
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   FullUpdateFromGui ( int result );
    void                           ControlPointChangePosition ( const qint32 &indexSpline,
                                                                SplinePoint::Position position,
                                                                const QPointF pos);
    virtual void                   ChangedActivDraw ( const QString newName );
protected:
    virtual void                   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                   AddToFile ();
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                   hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                   hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogSpline>   dialogSpline;
    QVector<VControlPointSpline *> controlPoints;
    void                           RefreshGeometry ();
};

#endif // VTOOLSPLINE_H

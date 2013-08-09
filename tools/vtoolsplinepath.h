#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include "vabstracttool.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogsplinepath.h"
#include "../widgets/vcontrolpointspline.h"

class VToolSplinePath:public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                     VToolSplinePath(VDomDocument *doc, VContainer *data, qint64 id,
                                                     Tool::Enum typeCreation, QGraphicsItem * parent = 0);
signals:
    void                             RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                                 const QPointF &controlPoint, const QPointF &splinePoint);
    void                             setEnabledPoint(bool enable);
public slots:
    virtual void                     FullUpdateFromFile();
    virtual void                     FullUpdateFromGui(int result);
    void                             ControlPointChangePosition(const qint32 &indexSpline,
                                                                SplinePoint::Position position,
                                                                const QPointF pos);
    virtual void                     ChangedActivDraw(const QString newName);
protected:
    virtual void                     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                     AddToFile();
    virtual void                     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogSplinePath> dialogSplinePath;
    QVector<VControlPointSpline *>   controlPoints;
    void                             RefreshGeometry();
    void                             AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint);
    void                             UpdatePathPoint(QDomNode& node, VSplinePath &path);
    void                             CorectControlPoints(const VSpline &spl, VSplinePath &splPath, qint32 index,
                                                         const qint32 &indexSpline,
                                                         SplinePoint::Position position);
};

#endif // VTOOLSPLINEPATH_H

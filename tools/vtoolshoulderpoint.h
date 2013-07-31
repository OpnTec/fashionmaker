#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

#include "vtoollinepoint.h"
#include "../dialogs/dialogshoulderpoint.h"

class VToolShoulderPoint : public VToolLinePoint
{
public:
    VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                       QGraphicsItem * parent = 0);
    static QPointF    FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                const qreal &length);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    qint64            p2Line;
    qint64            pShoulder;
    QSharedPointer<DialogShoulderPoint> dialogShoulderPoint;
};

#endif // VTOOLSHOULDERPOINT_H

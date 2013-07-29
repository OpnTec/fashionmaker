#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

#include "vtoolpoint.h"
#include "../dialogs/dialogshoulderpoint.h"

class VToolShoulderPoint : public VToolPoint
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
    virtual void      ChangedActivDraw(const QString newName);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QString           typeLine;
    QString           formula;
    qint64            p1Line;
    qint64            p2Line;
    qint64            pShoulder;
    QGraphicsLineItem *mainLine;
    QSharedPointer<DialogShoulderPoint> dialogShoulderPoint;
};

#endif // VTOOLSHOULDERPOINT_H

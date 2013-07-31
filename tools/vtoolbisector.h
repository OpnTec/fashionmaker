#ifndef VTOOLBISECTOR_H
#define VTOOLBISECTOR_H

#include <QSharedPointer>

#include "vtoollinepoint.h"
#include "../dialogs/dialogbisector.h"

class VToolBisector : public VToolLinePoint
{
public:
    VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                  const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                  const qint64 &thirdPointId, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static QPointF    FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                const QPointF &thirdPoint, const qreal& length);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    qint64            firstPointId;
    qint64            thirdPointId;
    QSharedPointer<DialogBisector> dialogBisector;
};

#endif // VTOOLBISECTOR_H

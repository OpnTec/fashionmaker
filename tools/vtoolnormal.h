#ifndef VTOOLNORMAL_H
#define VTOOLNORMAL_H

#include <QSharedPointer>

#include "vtoolpoint.h"
#include "../dialogs/dialognormal.h"

class VToolNormal : public VToolPoint
{
public:
                                 VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                                             const QString &typeLine, const QString &formula,
                                             const qint32 &angle, const qint64 &firstPointId,
                                             const qint64 &secondPointId, Tool::Enum typeCreation,
                                             QGraphicsItem * parent = 0);
    static QPointF               FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                           const qreal &length, const qint32 &angle = 0);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
    virtual void                 ChangedActivDraw(const QString newName);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
private:
    QString                      typeLine;
    QString                      formula;
    qint32                       angle;
    qint64                       firstPointId;
    qint64                       secondPointId;
    QGraphicsLineItem            *mainLine;
    QSharedPointer<DialogNormal> dialogNormal;
};

#endif // VTOOLNORMAL_H

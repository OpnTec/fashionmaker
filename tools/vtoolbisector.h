#ifndef VTOOLBISECTOR_H
#define VTOOLBISECTOR_H

#pragma GCC diagnostic ignored "-Weffc++"
#include <QSharedPointer>
#include "vtoollinepoint.h"
#include "../dialogs/dialogbisector.h"
#pragma GCC diagnostic warning "-Weffc++"

class VToolBisector : public VToolLinePoint
{
public:
    VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                  const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                  const qint64 &thirdPointId, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    static QPointF    FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                const QPointF &thirdPoint, const qreal& length);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogBisector> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data);
    static void Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                       const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                       const QString &pointName, const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
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

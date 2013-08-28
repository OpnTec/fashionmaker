#ifndef VTOOLNORMAL_H
#define VTOOLNORMAL_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QSharedPointer>
#include "vtoollinepoint.h"
#include "../dialogs/dialognormal.h"
#pragma GCC diagnostic pop

class VToolNormal : public VToolLinePoint
{
public:
                                 VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                                             Draw::Mode mode,
                                             const QString &typeLine, const QString &formula,
                                             const qreal &angle, const qint64 &firstPointId,
                                             const qint64 &secondPointId, Tool::Enum typeCreation,
                                             QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogNormal> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Draw::Mode mode);
    static void Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                       const qint64 &secondPointId, const QString typeLine, const QString pointName,
                       const qreal angle, const qreal &mx, const qreal &my, VMainGraphicsScene  *scene,
                       VDomDocument *doc, VContainer *data, Document::Enum parse, Tool::Enum typeCreation,
                       Draw::Mode mode);
    static QPointF               FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                           const qreal &length, const qreal &angle = 0);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
private:
    qint64                       secondPointId;
    QSharedPointer<DialogNormal> dialogNormal;
};

#endif // VTOOLNORMAL_H

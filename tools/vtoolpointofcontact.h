#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

#include "vtoolpoint.h"
#include "../dialogs/dialogpointofcontact.h"

class VToolPointOfContact : public VToolPoint
{
public:
    VToolPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id, Draw::Mode mode,
                        const QString &radius, const qint64 &center, const qint64 &firstPointId,
                        const qint64 &secondPointId, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static QPointF    FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                const QPointF &secondPoint);
    static void Create(QSharedPointer<DialogPointOfContact> &dialog, VMainGraphicsScene  *scene,
                       VDomDocument *doc, VContainer *data, Draw::Mode mode);
    static void Create(const qint64 _id, const QString &radius, const qint64 &center, const qint64 &firstPointId,
                       const qint64 &secondPointId, const QString &pointName,
                       const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Document::Enum parse, Tool::Enum typeCreation, Draw::Mode mode);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QString           radius;
    qint64            center;
    qint64            firstPointId;
    qint64            secondPointId;
    QSharedPointer<DialogPointOfContact> dialogPointOfContact;
};

#endif // VTOOLPOINTOFCONTACT_H

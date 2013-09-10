#ifndef VMODELINGPOINTOFCONTACT_H
#define VMODELINGPOINTOFCONTACT_H

#include "vmodelingpoint.h"
#include "dialogs/dialogpointofcontact.h"

class VModelingPointOfContact : public VModelingPoint
{
    Q_OBJECT
public:
    VModelingPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                        const QString &radius, const qint64 &center, const qint64 &firstPointId,
                        const qint64 &secondPointId, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static QPointF    FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                const QPointF &secondPoint);
    static VModelingPointOfContact* Create(QSharedPointer<DialogPointOfContact> &dialog,
                                           VDomDocument *doc, VContainer *data);
    static VModelingPointOfContact* Create(const qint64 _id, const QString &radius, const qint64 &center,
                                           const qint64 &firstPointId, const qint64 &secondPointId,
                                           const QString &pointName, const qreal &mx, const qreal &my,
                                           VDomDocument *doc, VContainer *data, Document::Enum parse,
                                           Tool::Enum typeCreation);
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

#endif // VMODELINGPOINTOFCONTACT_H

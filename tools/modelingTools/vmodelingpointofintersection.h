#ifndef VMODELINGPOINTOFINTERSECTION_H
#define VMODELINGPOINTOFINTERSECTION_H

#include "vmodelingpoint.h"
#include "dialogs/dialogpointofintersection.h"

class VModelingPointOfIntersection : public VModelingPoint
{
    Q_OBJECT
public:
                 VModelingPointOfIntersection(VDomDocument *doc, VContainer *data, const qint64 &id,
                                              const qint64 &firstPointId, const qint64 &secondPointId,
                                              Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingPointOfIntersection* Create(QSharedPointer<DialogPointOfIntersection> &dialog, VDomDocument *doc,
                                                VContainer *data);
    static VModelingPointOfIntersection* Create(const qint64 _id, const QString &pointName,
                                                const qint64 &firstPointId, const qint64 &secondPointId,
                                                const qreal &mx, const qreal &my, VDomDocument *doc,
                                                VContainer *data, const Document::Documents &parse,
                                                Tool::Sources typeCreation);
    static const QString ToolType;
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
protected:
    virtual void RemoveReferens();
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
private:
    Q_DISABLE_COPY(VModelingPointOfIntersection)
    qint64       firstPointId;
    qint64       secondPointId;
    QSharedPointer<DialogPointOfIntersection> dialogPointOfIntersection;
};

#endif // VMODELINGPOINTOFINTERSECTION_H

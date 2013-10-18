#ifndef VMODELINGTRIANGLE_H
#define VMODELINGTRIANGLE_H

#include "vmodelingpoint.h"
#include "../drawTools/vtooltriangle.h"
#include "dialogs/dialogtriangle.h"

class VModelingTriangle : public VModelingPoint{
    Q_OBJECT
public:
                              VModelingTriangle(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                const qint64 &axisP1Id, const qint64 &axisP2Id,
                                                const qint64 &firstPointId, const qint64 &secondPointId,
                                                Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void              setDialog();
    static VModelingTriangle* Create(QSharedPointer<DialogTriangle> &dialog, VDomDocument *doc,
                                     VContainer *data);
    static VModelingTriangle* Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id,
                                     const qint64 &axisP2Id, const qint64 &firstPointId,
                                     const qint64 &secondPointId, const qreal &mx, const qreal &my,
                                     VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                     Tool::Sources typeCreation);
public slots:
    virtual void               FullUpdateFromFile();
    virtual void               FullUpdateFromGui(int result);
protected:
    virtual void               RemoveReferens();
    virtual void               contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void               AddToFile();
private:
    Q_DISABLE_COPY(VModelingTriangle)
    qint64                     axisP1Id;
    qint64                     axisP2Id;
    qint64                     firstPointId;
    qint64                     secondPointId;
    QSharedPointer<DialogTriangle> dialogTriangle;
};

#endif // VMODELINGTRIANGLE_H

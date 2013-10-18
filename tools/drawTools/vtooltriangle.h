#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

#include "vtoolpoint.h"
#include "dialogs/dialogtriangle.h"

class VToolTriangle : public VToolPoint{
    Q_OBJECT
public:
                 VToolTriangle(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &axisP1Id,
                               const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                               Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogTriangle> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 _id, const QString &pointName,
                        const qint64 &axisP1Id, const qint64 &axisP2Id, const qint64 &firstPointId,
                        const qint64 &secondPointId, const qreal &mx, const qreal &my,
                        VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, Tool::Sources typeCreation);
    static QPointF  FindPoint(const QPointF axisP1, const QPointF axisP2, const QPointF firstPoint,
                              const QPointF secondPoint);
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
protected:
    virtual void RemoveReferens();
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
private:
    Q_DISABLE_COPY(VToolTriangle)
    qint64       axisP1Id;
    qint64       axisP2Id;
    qint64       firstPointId;
    qint64       secondPointId;
    QSharedPointer<DialogTriangle> dialogTriangle;
};

#endif // VTOOLTRIANGLE_H

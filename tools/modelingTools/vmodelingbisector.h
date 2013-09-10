#ifndef VMODELINGBISECTOR_H
#define VMODELINGBISECTOR_H

#include <QSharedPointer>
#include "vmodelinglinepoint.h"
#include "dialogs/dialogbisector.h"

class VModelingBisector : public VModelingLinePoint
{
    Q_OBJECT
public:
    VModelingBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                  const QString &typeLine, const QString &formula, const qint64 &firstPointId,
                  const qint64 &secondPointId, const qint64 &thirdPointId, Tool::Enum typeCreation,
                  QGraphicsItem * parent = 0);
    static QPointF    FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                const QPointF &thirdPoint, const qreal& length);
    virtual void setDialog();
    static VModelingBisector* Create(QSharedPointer<DialogBisector> &dialog, VDomDocument *doc,
                                     VContainer *data);
    static VModelingBisector* Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                                     const qint64 &secondPointId, const qint64 &thirdPointId,
                                     const QString &typeLine, const QString &pointName, const qreal &mx,
                                     const qreal &my, VDomDocument *doc, VContainer *data,
                                     Document::Enum parse, Tool::Enum typeCreation);
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

#endif // VMODELINGBISECTOR_H

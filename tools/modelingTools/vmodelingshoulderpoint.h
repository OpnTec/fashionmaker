#ifndef VMODELINGSHOULDERPOINT_H
#define VMODELINGSHOULDERPOINT_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialogshoulderpoint.h"

class VModelingShoulderPoint : public VModelingLinePoint
{
    Q_OBJECT
public:
    VModelingShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                       QGraphicsItem * parent = 0);
    virtual void setDialog();
    static QPointF    FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                const qreal &length);
    static VModelingShoulderPoint* Create(QSharedPointer<DialogShoulderPoint> &dialog,
                                          VDomDocument *doc, VContainer *data);
    static VModelingShoulderPoint* Create(const qint64 _id, const QString &formula, const qint64 &p1Line,
                                          const qint64 &p2Line, const qint64 &pShoulder,
                                          const QString &typeLine, const QString &pointName,
                                          const qreal &mx, const qreal &my, VDomDocument *doc,
                                          VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    qint64            p2Line;
    qint64            pShoulder;
    QSharedPointer<DialogShoulderPoint> dialogShoulderPoint;
};

#endif // VMODELINGSHOULDERPOINT_H

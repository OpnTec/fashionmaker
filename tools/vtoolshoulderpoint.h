#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoollinepoint.h"
#include "../dialogs/dialogshoulderpoint.h"
#pragma GCC diagnostic pop

class VToolShoulderPoint : public VToolLinePoint
{
public:
    VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                       QGraphicsItem * parent = 0);
    virtual void setDialog();
    static QPointF    FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                const qreal &length);
    static void Create(QSharedPointer<DialogShoulderPoint> &dialog, VMainGraphicsScene  *scene,
                       VDomDocument *doc, VContainer *data);
    static void Create(const qint64 _id, const QString &formula, const qint64 &p1Line, const qint64 &p2Line,
                       const qint64 &pShoulder, const QString &typeLine, const QString &pointName,
                       const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
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

#endif // VTOOLSHOULDERPOINT_H

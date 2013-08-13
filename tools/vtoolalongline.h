#ifndef VTOOLALONGLINE_H
#define VTOOLALONGLINE_H

#include "vtoollinepoint.h"
#include "../dialogs/dialogalongline.h"

class VToolAlongLine : public VToolLinePoint
{
    Q_OBJECT
public:
    VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                   const qint64 &firstPointId, const qint64 &secondPointId, const QString &typeLine,
                   Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void      setDialog();
    static void Create(QSharedPointer<DialogAlongLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data);
    static void Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                       const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                       const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();

private:
    qint64            secondPointId;
    QSharedPointer<DialogAlongLine> dialogAlongLine; 
};

#endif // VTOOLALONGLINE_H

#ifndef VTOOLALONGLINE_H
#define VTOOLALONGLINE_H

#include "vtoolpoint.h"
#include "../dialogs/dialogalongline.h"

class VToolAlongLine : public VToolPoint
{
    Q_OBJECT
public:
    VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                   const qint64 &firstPointId, const qint64 &secondPointId, const QString &typeLine,
                   Tool::Enum typeCreation, QGraphicsItem * parent = 0);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      ChangedActivDraw(const QString newName);
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QString           typeLine;
    QString           formula;
    qint64            firstPointId;
    qint64            secondPointId;
    QGraphicsLineItem *mainLine;
    QSharedPointer<DialogAlongLine> dialogAlongLine;
};

#endif // VTOOLALONGLINE_H

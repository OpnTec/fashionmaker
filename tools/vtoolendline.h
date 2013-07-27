#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#include <QSharedPointer>

#include "vtoolpoint.h"
#include "../dialogs/dialogendline.h"

class VToolEndLine : public VToolPoint
{
    Q_OBJECT
public:
                      VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                                   const QString &typeLine, const QString &formula, const qint32 &angle,
                                   const qint64 &basePointId, Tool::Enum typeCreation,
                                   QGraphicsItem * parent = 0);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
    virtual void      ChangedActivDraw(const QString newName);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QString           typeLine;
    QString           formula;
    qint32            angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    QSharedPointer<DialogEndLine> dialogEndLine;
};

#endif // VTOOLENDLINE_H

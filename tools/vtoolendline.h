#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#include <QSharedPointer>

#include "vtoollinepoint.h"
#include "../dialogs/dialogendline.h"

class VToolEndLine : public VToolLinePoint
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
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QSharedPointer<DialogEndLine> dialogEndLine;
};

#endif // VTOOLENDLINE_H

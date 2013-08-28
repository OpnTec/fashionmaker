#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QSharedPointer>
#include "vtoollinepoint.h"
#include "../dialogs/dialogendline.h"
#pragma GCC diagnostic pop

class VToolEndLine : public VToolLinePoint
{
    Q_OBJECT
public:
                      VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id, Draw::Mode mode,
                                   const QString &typeLine, const QString &formula, const qint32 &angle,
                                   const qint64 &basePointId, Tool::Enum typeCreation,
                                   QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogEndLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Draw::Mode mode);
    static void Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                       const QString &formula, const qint32 &angle, const qint64 &basePointId, const qreal &mx, const qreal &my,
                       VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data, Document::Enum parse,
                       Tool::Enum typeCreation, Draw::Mode mode);
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

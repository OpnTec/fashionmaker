#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoolpoint.h"
#include "../dialogs/dialoglineintersect.h"
#pragma GCC diagnostic pop

class VToolLineIntersect:public VToolPoint
{
public:
                                       VToolLineIntersect(VDomDocument *doc, VContainer *data,
                                                          const qint64 &id, const qint64 &p1Line1,
                                                          const qint64 &p2Line1, const qint64 &p1Line2,
                                                          const qint64 &p2Line2, Tool::Enum typeCreation,
                                                          QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogLineIntersect> &dialog, VMainGraphicsScene  *scene,
                       VDomDocument *doc,VContainer *data);
    static void Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id,
                       const qint64 &p1Line2Id, const qint64 &p2Line2Id, const QString &pointName,
                       const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                        FullUpdateFromFile();
    virtual void                        FullUpdateFromGui(int result);
protected:
    virtual void                        contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                        AddToFile();
private:
    qint64                              p1Line1;
    qint64                              p2Line1;
    qint64                              p1Line2;
    qint64                              p2Line2;
    QSharedPointer<DialogLineIntersect> dialogLineIntersect;
};

#endif // VTOOLLINEINTERSECT_H

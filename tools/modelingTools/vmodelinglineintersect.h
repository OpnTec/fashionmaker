#ifndef VMODELINGLINEINTERSECT_H
#define VMODELINGLINEINTERSECT_H

#include "vmodelingpoint.h"
#include "dialogs/dialoglineintersect.h"

class VModelingLineIntersect:public VModelingPoint
{
    Q_OBJECT
public:
                                       VModelingLineIntersect(VDomDocument *doc, VContainer *data,
                                                          const qint64 &id,
                                                          const qint64 &p1Line1,
                                                          const qint64 &p2Line1, const qint64 &p1Line2,
                                                          const qint64 &p2Line2, Tool::Enum typeCreation,
                                                          QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingLineIntersect* Create(QSharedPointer<DialogLineIntersect> &dialog,
                                          VDomDocument *doc,VContainer *data);
    static VModelingLineIntersect* Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id,
                                          const qint64 &p1Line2Id, const qint64 &p2Line2Id,
                                          const QString &pointName, const qreal &mx, const qreal &my,
                                          VDomDocument *doc, VContainer *data, Document::Enum parse,
                                          Tool::Enum typeCreation);
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

#endif // VMODELINGLINEINTERSECT_H

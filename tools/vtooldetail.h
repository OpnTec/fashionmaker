#ifndef VTOOLDETAIL_H
#define VTOOLDETAIL_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogdetail.h"

class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, VDetail &oldDetail,
                Tool::Enum typeCreation, Document::Enum parse, VMainGraphicsScene *scene,
                QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                       VContainer *data);
    static void Create(const qint64 _id, VDetail &newDetail, VDetail &oldDetail,
                       VMainGraphicsScene  *scene,
                       VDomDocument *doc, VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   FullUpdateFromGui(int result);
protected:
    virtual void                   AddToFile ();
    QVariant                       itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    QSharedPointer<DialogDetail>   dialogDetail;
    void                           RefreshGeometry ();
    void AddNode(QDomElement &domElement, VNodeDetail &node);
};

#endif // VTOOLDETAIL_H

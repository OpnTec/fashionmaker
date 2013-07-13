#ifndef VTOOLSIMPLEPOINT_H
#define VTOOLSIMPLEPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "../widgets/vgraphicssimpletextitem.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"

namespace Tool{
    enum Enum
    {
        FromGui,
        FromFile
    };
}

class VToolSimplePoint : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                            VToolSimplePoint (VDomDocument *doc, VContainer *data, qint64 id,
                                              Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
public slots:
    void                    NameChangePosition(const QPointF pos);
    void                    ChangedActivDraw(const QString newName);
    void                    ChangedNameDraw(const QString oldName, const QString newName);
    void                    FullUpdateFromFile();
signals:
    void                    FullUpdateTree();
    void                    haveLiteChange();
protected:
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
private:
    qreal                   radius;
    VDomDocument            *doc;
    VContainer              *data;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *line;
    qint64                  id;
    QString                 nameActivDraw;
    bool                    ignoreContextMenuEvent;
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const;
    void                    AddSimplePointToFile() const;
    void                    LiteUpdateFromGui(const QString& name, qreal mx, qreal my);
    void                    FullUpdateFromGui(const QString& name, qreal x, qreal y);
    void                    InitializeSimplePoint(VDomDocument *doc, VContainer *data, qint64 id);
    void                    RefreshLine();
};

#endif // VTOOLSIMPLEPOINT_H

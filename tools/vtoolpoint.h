#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

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

class VToolPoint: public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
public slots:
    void                    NameChangePosition(const QPointF pos);
    virtual void            ChangedActivDraw(const QString newName);
    virtual void            FullUpdateFromFile()=0;
    void                    ChangedNameDraw(const QString oldName, const QString newName);
signals:
    void                    haveLiteChange();
protected:
    qreal                   radius;
    VDomDocument            *doc;
    VContainer              *data;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *line;
    qint64                  id;
    QString                 nameActivDraw;
    virtual void            AddToFile()=0;
    void                    RefreshLine();
private:
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const;  
    void                    LiteUpdateFromGui(const QString& name, qreal mx, qreal my);
};

#endif // VTOOLPOINT_H

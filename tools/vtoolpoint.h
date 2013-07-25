#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "../widgets/vgraphicssimpletextitem.h"
#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include "../options.h"

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
    virtual ~VToolPoint();
public slots:
    void                    NameChangePosition(const QPointF pos);
    virtual void            ChangedActivDraw(const QString newName);
    virtual void            FullUpdateFromFile()=0;
    void                    ChangedNameDraw(const QString oldName, const QString newName);
signals:
    void                    haveLiteChange();
    void                    ChoosedPoint(qint64 id, Scene::Type type);
    void                    FullUpdateTree();
protected:
    qreal                   radius;
    VDomDocument            *doc;
    VContainer              *data;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *line;
    qint64                  id;
    QString                 nameActivDraw;
    bool                    ignoreContextMenuEvent;
    virtual void            AddToFile()=0;
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    void                    AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value);
    void                    AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value);
    void                    AddAttribute(QDomElement &domElement, const QString &name, const qreal &value);
    void                    AddAttribute(QDomElement &domElement, const QString &name, const QString &value);
    void                    RefreshBaseGeometry(const QString &name, const qreal &x, const qreal &y,
                                                const qreal &mx, const qreal &my);
    QString                 GetNameLine(qint64 firstPoint, qint64 secondPoint) const;
private:
    qint32                  LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const;
    QPointF                 LineIntersectRect(QRectF rec, QLineF line) const;
    QPointF                 ClosestPoint(QLineF line, QPointF p) const;
    QPointF                 add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const;  
    void                    LiteUpdateFromGui(qreal mx, qreal my);
    void                    RefreshLine();
};

#endif // VTOOLPOINT_H

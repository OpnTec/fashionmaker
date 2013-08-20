#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QGraphicsSimpleTextItem>
#pragma GCC diagnostic pop

class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
                 VGraphicsSimpleTextItem(QGraphicsItem * parent = 0);
                 VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );
signals:
    void         NameChangePosition(const QPointF pos);
protected:
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
};

#endif // VGRAPHICSSIMPLETEXTITEM_H

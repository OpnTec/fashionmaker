#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>

class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
             VGraphicsSimpleTextItem(QGraphicsItem * parent = 0);
             VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );
signals:
    void     NameChangePosition(const QPointF pos);
protected:
    QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
};

#endif // VGRAPHICSSIMPLETEXTITEM_H

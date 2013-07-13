#ifndef VPOINTF_H
#define VPOINTF_H

#include <QPointF>
#include <QString>

class VPointF : public QPointF
{
public:
    VPointF();
    VPointF (const VPointF &point );
    VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my );
    QString name() const;
    qreal mx() const;
    qreal my() const;
    void setName(const QString &name);
    void setMx(qreal mx);
    void setMy(qreal my);
    QPointF toQPointF()const;
private:
    QString _name;
    qreal _mx;
    qreal _my;
};

#endif // VPOINTF_H

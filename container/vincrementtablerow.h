#ifndef VINCREMENTTABLEROW_H
#define VINCREMENTTABLEROW_H

#include <QString>

class VIncrementTableRow
{
public:
    VIncrementTableRow();
    VIncrementTableRow(qint64 id, qreal base, qreal ksize, qreal kgrowth,
                        QString description = QString());
    qint64 getId() const;
    void setId(const qint64 &value);

    qreal getBase() const;
    void setBase(const qreal &value);

    qreal getKsize() const;
    void setKsize(const qreal &value);

    qreal getKgrowth() const;
    void setKgrowth(const qreal &value);

    QString getDescription() const;
    void setDescription(const QString &value);

private:
    qint64 id;
    qreal base;
    qreal ksize;
    qreal kgrowth;
    QString description;
};

#endif // VINCREMENTTABLEROW_H

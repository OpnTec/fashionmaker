#ifndef VSTANDARTTABLECELL_H
#define VSTANDARTTABLECELL_H

#include <QString>

class VStandartTableCell
{
public:
    VStandartTableCell();
    VStandartTableCell(qint32 base, qreal ksize, qreal kgrowth, QString description = QString());
    qint32 GetBase() const;
    qreal GetKsize() const;
    qreal GetKgrowth() const;
    QString GetDescription() const;
private:
    qint32 base;
    qreal ksize;
    qreal kgrowth;
    QString description;
};

#endif // VSTANDARTTABLECELL_H

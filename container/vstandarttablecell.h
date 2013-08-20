#ifndef VSTANDARTTABLECELL_H
#define VSTANDARTTABLECELL_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QString>
#pragma GCC diagnostic pop

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

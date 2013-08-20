#ifndef VTOOLRECORD_H
#define VTOOLRECORD_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QtGlobal>
#include <QString>
#pragma GCC diagnostic pop
#include "../options.h"

class VToolRecord
{
public:
    VToolRecord();
    VToolRecord(const qint64 &id, const Tools::Enum &typeTool, const QString &nameDraw);
    qint64 getId() const;
    void setId(const qint64 &value);

    Tools::Enum getTypeTool() const;
    void setTypeTool(const Tools::Enum &value);

    QString getNameDraw() const;
    void setNameDraw(const QString &value);

private:
    qint64 id;
    Tools::Enum typeTool;
    QString nameDraw;
};

#endif // VTOOLRECORD_H

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <QMap>
#include <QTableWidget>

#include "vpointf.h"
#include "vstandarttablecell.h"
#include "vincrementtablerow.h"

class VContainer
{
public:
                        VContainer();
    VPointF             GetPoint(qint64 id) const;
    VStandartTableCell  GetStandartTableCell(const QString& name) const;
    VIncrementTableRow  GetIncrementTableRow(const QString& name) const;
    qreal               GetLine(const QString &name) const;
    qint64              getId();
    qint64              AddPoint(const VPointF& point);
    void                AddStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                AddIncrementTableRow(const QString& name, const VIncrementTableRow &cell);
    void                AddLine(const QString &name, const qreal &value);
    void                UpdatePoint(qint64 id, const VPointF& point);
    void                UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell);
    qreal               GetValueStandartTableCell(const QString& name) const;
    qreal               GetValueIncrementTableRow(const QString& name) const;
    void                Clear();
    void                ClearIncrementTable();
    void                ClearLengthLines();
    void                SetSize(qint32 size);
    void                SetGrowth(qint32 growth);
    qint32              size() const;
    qint32              growth() const;
    qreal               FindVar(const QString& name, bool *ok)const;
    bool                IncrementTableContains(const QString& name);
    qint64              getNextId();
    void                RemoveIncrementTableRow(const QString& name);
    const QMap<qint64, VPointF> *DataPoints() const;
    const QMap<QString, qint32> *DataBase() const;
    const QMap<QString, VStandartTableCell> *DataStandartTable() const;
    const QMap<QString, VIncrementTableRow> *DataIncrementTable() const;
    const QMap<QString, qreal> * DataLengthLines() const;

private:
    qint64              _id;
    QMap<QString, qint32> base;
    QMap<qint64, VPointF> points;
    QMap<QString, VStandartTableCell> standartTable;
    QMap<QString, VIncrementTableRow> incrementTable;
    QMap<QString, qreal> lengthLines;
};

#endif // VCONTAINER_H

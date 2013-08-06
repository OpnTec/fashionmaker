#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <QMap>
#include <QTableWidget>
#include "vpointf.h"
#include "vstandarttablecell.h"
#include "vincrementtablerow.h"
#include "../geometry/vspline.h"
#include "../geometry/varc.h"

/**
 * @brief The VContainer class
 */
class VContainer
{
public:
    /**
                         * @brief VContainer
                         */
                        VContainer();
                        /**
     * @brief GetPoint
     * @param id
     * @return
     */
    VPointF             GetPoint(qint64 id) const;
    VStandartTableCell  GetStandartTableCell(const QString& name) const;
    VIncrementTableRow  GetIncrementTableRow(const QString& name) const;
    qreal               GetLine(const QString &name) const;
    qint32              GetLineArc(const QString &name) const;
    VSpline             GetSpline(qint64 id) const;
    VArc                GetArc(qint64 id) const;
    qint64              getId();
    qint64              AddPoint(const VPointF& point);
    void                AddStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                AddIncrementTableRow(const QString& name, const VIncrementTableRow &cell);
    void                AddLengthLine(const QString &name, const qreal &value);
    void                AddLengthSpline(const qint64 &firstPointId, const qint64 &secondPointId);
    void                AddLengthSpline(const QString &name, const qreal &value);
    void                AddLengthArc(const qint64 &center, const qint64 &id);
    void                AddLengthArc(const QString &name, const qreal &value);
    void                AddLineArc(const QString &name, const qint32 &value);
    void                AddLine(const qint64 &firstPointId, const qint64 &secondPointId);
    qint64              AddSpline(const VSpline& spl);
    qint64              AddArc(const VArc& arc);
    QString             GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameLineArc(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameArc(const qint64 &center, const qint64 &id) const;
    void                UpdatePoint(qint64 id, const VPointF& point);
    void                UpdateSpline(qint64 id, const VSpline& spl);
    void                UpdateArc(qint64 id, const VArc& arc);
    void                UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell);
    qreal               GetValueStandartTableCell(const QString& name) const;
    qreal               GetValueIncrementTableRow(const QString& name) const;
    void                Clear();
    void                ClearIncrementTable();
    void                ClearLengthLines();
    void                ClearLengthSplines();
    void                ClearLengthArcs();
    void                ClearLineArcs();
    void                SetSize(qint32 size);
    void                SetGrowth(qint32 growth);
    qint32              size() const;
    qint32              growth() const;
    qreal               FindVar(const QString& name, bool *ok)const;
    bool                IncrementTableContains(const QString& name);
    qint64              getNextId();
    void                RemoveIncrementTableRow(const QString& name);
    const QMap<qint64, VPointF> *DataPoints() const;
    const QMap<qint64, VSpline> *DataSplines() const;
    const QMap<qint64, VArc> *DataArcs() const;
    const QMap<QString, qint32> *DataBase() const;
    const QMap<QString, VStandartTableCell> *DataStandartTable() const;
    const QMap<QString, VIncrementTableRow> *DataIncrementTable() const;
    const QMap<QString, qreal> *DataLengthLines() const;
    const QMap<QString, qreal> *DataLengthSplines() const;
    const QMap<QString, qreal> *DataLengthArcs() const;
    const QMap<QString, qreal> *DataLineArcs() const;
private:
    qint64              _id;
    QMap<QString, qint32> base;
    QMap<qint64, VPointF> points;
    QMap<QString, VStandartTableCell> standartTable;
    QMap<QString, VIncrementTableRow> incrementTable;
    QMap<QString, qreal> lengthLines;
    QMap<QString, qreal> lineArcs;
    QMap<qint64, VSpline> splines;
    QMap<QString, qreal> lengthSplines;
    QMap<qint64, VArc> arcs;
    QMap<QString, qreal> lengthArcs;
};

#endif // VCONTAINER_H

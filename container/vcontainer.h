#ifndef VCONTAINER_H
#define VCONTAINER_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QMap>
#include <QTableWidget>
#pragma GCC diagnostic pop
#include "vpointf.h"
#include "vstandarttablecell.h"
#include "vincrementtablerow.h"
#include "../geometry/vspline.h"
#include "../geometry/varc.h"
#include "../geometry/vsplinepath.h"

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
                        const VContainer &operator=(const VContainer &data);
                        void setData(const VContainer &data);
                        /**
     * @brief GetPoint
     * @param id
     * @return
     */
    VPointF             GetPoint(qint64 id) const;
    VStandartTableCell  GetStandartTableCell(const QString& name) const;
    VIncrementTableRow  GetIncrementTableRow(const QString& name) const;
    qreal               GetLine(const QString &name) const;
    qreal               GetLineArc(const QString &name) const;
    VSpline             GetSpline(qint64 id) const;
    VArc                GetArc(qint64 id) const;
    VSplinePath         GetSplinePath(qint64 id) const;
    qint64              getId();
    qint64              AddPoint(const VPointF& point);
    void                AddStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                AddIncrementTableRow(const QString& name, const VIncrementTableRow &cell);
    void                AddLengthLine(const QString &name, const qreal &value);
    void                AddLengthSpline(const qint64 &firstPointId, const qint64 &secondPointId);
    void                AddLengthSpline(const QString &name, const qreal &value);
    void                AddLengthArc(const qint64 &center, const qint64 &id);
    void                AddLengthArc(const QString &name, const qreal &value);
    void                AddLineAngle(const QString &name, const qreal &value);
    void                AddLine(const qint64 &firstPointId, const qint64 &secondPointId);
    qint64              AddSpline(const VSpline& spl);
    qint64              AddSplinePath(const VSplinePath& splPath);
    qint64              AddArc(const VArc& arc);
    QString             GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint) const;
    QString             GetNameSplinePath(const VSplinePath &path) const;
    QString             GetNameArc(const qint64 &center, const qint64 &id) const;
    void                UpdatePoint(qint64 id, const VPointF& point);
    void                UpdateSpline(qint64 id, const VSpline& spl);
    void                UpdateSplinePath(qint64 id, const VSplinePath& splPath);
    void                UpdateArc(qint64 id, const VArc& arc);
    void                UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell);
    void                UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell);
    qreal               GetValueStandartTableCell(const QString& name) const;
    qreal               GetValueIncrementTableRow(const QString& name) const;
    void                Clear();
    void                ClearObject();
    void                ClearIncrementTable();
    void                ClearLengthLines();
    void                ClearLengthSplines();
    void                ClearLengthArcs();
    void                ClearLineAngles();
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
    const QMap<QString, qreal> *DataLineAngles() const;
    const QMap<qint64, VSplinePath> *DataSplinePaths() const;
    void  UpdateId(qint64 newId);
private:
    static qint64         _id;
    QMap<QString, qint32> base;
    QMap<qint64, VPointF> points;
    QMap<QString, VStandartTableCell> standartTable;
    QMap<QString, VIncrementTableRow> incrementTable;
    QMap<QString, qreal> lengthLines;
    QMap<QString, qreal> lineAngles;
    QMap<qint64, VSpline> splines;
    QMap<QString, qreal> lengthSplines;
    QMap<qint64, VArc> arcs;
    QMap<QString, qreal> lengthArcs;
    QMap<qint64, VSplinePath> splinePaths;
    template <typename key, typename val> val GetObject(const QMap<key,val> &obj, key id) const;
    template <typename val> void UpdateObject(QMap<qint64, val> &obj, const qint64 &id, const val& point);
    template <typename key, typename val> qint64 AddObject(QMap<key, val> &obj, const val& value);
    void CreateManTableIGroup ();
};

#endif // VCONTAINER_H

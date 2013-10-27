/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include "vstandarttablecell.h"
#include "vincrementtablerow.h"
#include "geometry/varc.h"
#include "geometry/vsplinepath.h"
#include "geometry/vdetail.h"
#include "widgets/vitem.h"

/**
 * @brief The VContainer class
 */
class VContainer{
    Q_DECLARE_TR_FUNCTIONS(VContainer)
public:
    /**
                         * @brief VContainer
                         */
                       VContainer();
                       VContainer &operator=(const VContainer &data);
                       VContainer(const VContainer &data);
                       void setData(const VContainer &data);
                        /**
     * @brief GetPoint
     * @param id
     * @return
     */
    inline VPointF     GetPoint(qint64 id) const {return GetObject(points, id);}
    inline VPointF     GetModelingPoint(qint64 id) const {return GetObject(modelingPoints, id);}
    VStandartTableCell GetStandartTableCell(const QString& name) const;
    VIncrementTableRow GetIncrementTableRow(const QString& name) const;
    qreal              GetLine(const QString &name) const;
    qreal              GetLengthArc(const QString &name) const;
    qreal              GetLengthSpline(const QString &name) const;
    qreal              GetLineAngle(const QString &name) const;
    inline VSpline     GetSpline(qint64 id) const {return GetObject(splines, id);}
    inline VSpline     GetModelingSpline(qint64 id) const {return GetObject(modelingSplines, id);}
    inline VArc        GetArc(qint64 id) const {return GetObject(arcs, id);}
    inline VArc        GetModelingArc(qint64 id) const {return GetObject(modelingArcs, id);}
    inline VSplinePath GetSplinePath(qint64 id) const {return GetObject(splinePaths, id);}
    inline VSplinePath GetModelingSplinePath(qint64 id) const {return GetObject(modelingSplinePaths, id);}
    inline VDetail     GetDetail(qint64 id) const {return GetObject(details, id);}
    static qint64      getId() {return _id;}
    qint64             AddPoint(const VPointF& point);
    qint64             AddModelingPoint(const VPointF& point);
    qint64             AddDetail(const VDetail& detail);
    inline void        AddStandartTableCell(const QString& name, const VStandartTableCell& cell)
    {standartTable[name] = cell;}
    inline void        AddIncrementTableRow(const QString& name, const VIncrementTableRow &cell)
    {incrementTable[name] = cell;}
    void               AddLengthLine(const QString &name, const qreal &value);
    void               AddLengthSpline(const QString &name, const qreal &value);
    void               AddLengthArc(const qint64 &center, const qint64 &id);
    void               AddLengthArc(const QString &name, const qreal &value);
    void               AddLineAngle(const QString &name, const qreal &value);
    void               AddLine(const qint64 &firstPointId, const qint64 &secondPointId,
                               Draw::Draws mode = Draw::Calculation);
    qint64             AddSpline(const VSpline& spl);
    qint64             AddModelingSpline(const VSpline& spl);
    qint64             AddSplinePath(const VSplinePath& splPath);
    qint64             AddModelingSplinePath(const VSplinePath& splPath);
    qint64             AddArc(const VArc& arc);
    qint64             AddModelingArc(const VArc& arc);
    QString            GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint,
                                   Draw::Draws mode = Draw::Calculation) const;
    QString            GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint,
                                        Draw::Draws mode = Draw::Calculation) const;
    QString            GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint,
                                     Draw::Draws mode = Draw::Calculation) const;
    QString            GetNameSplinePath(const VSplinePath &path,
                                         Draw::Draws mode = Draw::Calculation) const;
    QString            GetNameArc(const qint64 &center, const qint64 &id, Draw::Draws mode = Draw::Calculation) const;
    void               UpdatePoint(qint64 id, const VPointF& point);
    void               UpdateModelingPoint(qint64 id, const VPointF& point);
    void               UpdateDetail(qint64 id, const VDetail& detail);
    void               UpdateSpline(qint64 id, const VSpline& spl);
    void               UpdateModelingSpline(qint64 id, const VSpline& spl);
    void               UpdateSplinePath(qint64 id, const VSplinePath& splPath);
    void               UpdateModelingSplinePath(qint64 id, const VSplinePath& splPath);
    void               UpdateArc(qint64 id, const VArc& arc);
    void               UpdateModelingArc(qint64 id, const VArc& arc);
    inline void        UpdateStandartTableCell(const QString& name, const VStandartTableCell& cell)
    {standartTable[name] = cell;}
    inline void        UpdateIncrementTableRow(const QString& name, const VIncrementTableRow& cell)
    {incrementTable[name] = cell;}
    qreal              GetValueStandartTableCell(const QString& name) const;
    qreal              GetValueIncrementTableRow(const QString& name) const;
    void               Clear();
    void               ClearObject();
    inline void        ClearIncrementTable() {incrementTable.clear();}
    inline void        ClearLengthLines() {lengthLines.clear();}
    inline void        ClearLengthSplines() {lengthSplines.clear();}
    inline void        ClearLengthArcs() {lengthArcs.clear();}
    inline void        ClearLineAngles() {lineAngles.clear();}
    inline void        SetSize(qint32 size) {base["Сг"] = size;}
    inline void        SetGrowth(qint32 growth) {base["Р"] = growth;}
    inline qint32      size() const {return base.value("Сг");}
    inline qint32      growth() const {return base.value("Р");}
    qreal              FindVar(const QString& name, bool *ok)const;
    inline bool        IncrementTableContains(const QString& name) {return incrementTable.contains(name);}
    static qint64      getNextId();
    inline void        RemoveIncrementTableRow(const QString& name) {incrementTable.remove(name);}
    inline const QHash<qint64, VPointF> *DataPoints() const {return &points;}
    inline const QHash<qint64, VPointF> *DataModelingPoints() const {return &modelingPoints;}
    inline const QHash<qint64, VSpline> *DataSplines() const {return &splines;}
    inline const QHash<qint64, VSpline> *DataModelingSplines() const {return &modelingSplines;}
    inline const QHash<qint64, VArc>    *DataArcs() const {return &arcs;}
    inline const QHash<qint64, VArc>    *DataModelingArcs() const {return &modelingArcs;}
    inline const QHash<QString, qint32> *DataBase() const {return &base;}
    inline const QHash<QString, VStandartTableCell> *DataStandartTable() const {return &standartTable;}
    inline const QHash<QString, VIncrementTableRow> *DataIncrementTable() const {return &incrementTable;}
    inline const QHash<QString, qreal>  *DataLengthLines() const {return &lengthLines;}
    inline const QHash<QString, qreal>  *DataLengthSplines() const {return &lengthSplines;}
    inline const QHash<QString, qreal>  *DataLengthArcs() const {return &lengthArcs;}
    inline const QHash<QString, qreal>  *DataLineAngles() const {return &lineAngles;}
    inline const QHash<qint64, VSplinePath> *DataSplinePaths() const {return &splinePaths;}
    inline const QHash<qint64, VSplinePath> *DataModelingSplinePaths() const {return &modelingSplinePaths;}
    inline const QHash<qint64, VDetail> *DataDetails() const {return &details;}
    static void        UpdateId(qint64 newId);
    QPainterPath       ContourPath(qint64 idDetail) const;
    QVector<QPointF>   biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my) const;
    QPainterPath       Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv, const qreal &width)const;
    static QLineF      ParallelLine(const QLineF &line, qreal width );
    static QPointF     SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width);
    QVector<QPointF>   EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width)const;
    QVector<QPointF>   CheckLoops(const QVector<QPointF> &points) const;
    void               PrepareDetails(QVector<VItem *> & list) const;
private:
    static qint64          _id;
    QHash<QString, qint32> base;
    QHash<qint64, VPointF> points;
    QHash<qint64, VPointF> modelingPoints;
    QHash<QString, VStandartTableCell> standartTable;
    QHash<QString, VIncrementTableRow> incrementTable;
    QHash<QString, qreal>  lengthLines;
    QHash<QString, qreal>  lineAngles;
    QHash<qint64, VSpline> splines;
    QHash<qint64, VSpline> modelingSplines;
    QHash<QString, qreal>  lengthSplines;
    QHash<qint64, VArc>    arcs;
    QHash<qint64, VArc>    modelingArcs;
    QHash<QString, qreal>  lengthArcs;
    QHash<qint64, VSplinePath> splinePaths;
    QHash<qint64, VSplinePath> modelingSplinePaths;
    QHash<qint64, VDetail> details;
    void               CreateManTableIGroup ();
    QVector<QPointF>   GetReversePoint(const QVector<QPointF> &points)const;
    qreal              GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints)const;
    template <typename key, typename val> static val GetObject(const QHash<key,val> &obj, key id);
    template <typename val> static void UpdateObject(QHash<qint64, val> &obj, const qint64 &id, const val& point);
    template <typename key, typename val> static qint64 AddObject(QHash<key, val> &obj, const val& value);
};

#endif // VCONTAINER_H

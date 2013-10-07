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

#include <QTableWidget>
#include "vstandarttablecell.h"
#include "vincrementtablerow.h"
#include "geometry/varc.h"
#include "geometry/vsplinepath.h"
#include "geometry/vdetail.h"
#include "widgets/vitem.h"
#include <QCoreApplication>

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer{
    Q_DECLARE_TR_FUNCTIONS(VContainer)
public:
                                       /**
                                        * @brief VContainer create empty container
                                        */
                                       VContainer();
                                       /**
                                        * @brief operator = copy constructor
                                        * @param data container
                                        * @return copy container
                                        */
                                       VContainer &operator=(const VContainer &data);
                                       /**
                                        * @brief VContainer create container from another container
                                        * @param data container
                                        */
                                       VContainer(const VContainer &data);
   /**
    * @brief setData copy data from container
    * @param data container
    */
    void                               setData(const VContainer &data);
    /**
     * @brief GetPoint returns a point by id
     * @param id id of point
     * @return point
     */
    VPointF                            GetPoint(qint64 id) const;
    /**
     * @brief GetModelingPoint return a modeling point by id
     * @param id id of modeling point
     * @return modeling point
     */
    VPointF                            GetModelingPoint(qint64 id) const;
    /**
     * @brief GetStandartTableCell
     * @param name
     * @return
     */
    VStandartTableCell                 GetStandartTableCell(const QString& name) const;
    VIncrementTableRow                 GetIncrementTableRow(const QString& name) const;
    qreal                              GetLine(const QString &name) const;
    qreal                              GetLineArc(const QString &name) const;
    VSpline                            GetSpline(qint64 id) const;
    VSpline                            GetModelingSpline(qint64 id) const;
    VArc                               GetArc(qint64 id) const;
    VArc                               GetModelingArc(qint64 id) const;
    VSplinePath                        GetSplinePath(qint64 id) const;
    VSplinePath                        GetModelingSplinePath(qint64 id) const;
    VDetail                            GetDetail(qint64 id) const;
    static qint64                      getId();
    qint64                             AddPoint(const VPointF& point);
    qint64                             AddModelingPoint(const VPointF& point);
    qint64                             AddDetail(const VDetail& detail);
    void                               AddStandartTableCell(const QString& name,
                                                            const VStandartTableCell& cell);
    void                               AddIncrementTableRow(const QString& name,
                                                            const VIncrementTableRow &cell);
    void                               AddLengthLine(const QString &name, const qreal &value);
    void                               AddLengthSpline(const qint64 &firstPointId,
                                                       const qint64 &secondPointId,
                                                       Draw::Draws mode = Draw::Calculation);
    void                               AddLengthSpline(const QString &name, const qreal &value);
    void                               AddLengthArc(const qint64 &center, const qint64 &id);
    void                               AddLengthArc(const QString &name, const qreal &value);
    void                               AddLineAngle(const QString &name, const qreal &value);
    void                               AddLine(const qint64 &firstPointId, const qint64 &secondPointId,
                                               Draw::Draws mode = Draw::Calculation);
    qint64                             AddSpline(const VSpline& spl);
    qint64                             AddModelingSpline(const VSpline& spl);
    qint64                             AddSplinePath(const VSplinePath& splPath);
    qint64                             AddModelingSplinePath(const VSplinePath& splPath);
    qint64                             AddArc(const VArc& arc);
    qint64                             AddModelingArc(const VArc& arc);
    QString                            GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint,
                                                   Draw::Draws mode = Draw::Calculation) const;
    QString                            GetNameLineAngle(const qint64 &firstPoint,
                                                        const qint64 &secondPoint,
                                                        Draw::Draws mode = Draw::Calculation) const;
    QString                            GetNameSpline(const qint64 &firstPoint, const qint64 &secondPoint,
                                                     Draw::Draws mode = Draw::Calculation) const;
    QString                            GetNameSplinePath(const VSplinePath &path,
                                                         Draw::Draws mode = Draw::Calculation) const;
    QString                            GetNameArc(const qint64 &center, const qint64 &id,
                                                  Draw::Draws mode = Draw::Calculation) const;
    void                               UpdatePoint(qint64 id, const VPointF& point);
    void                               UpdateModelingPoint(qint64 id, const VPointF& point);
    void                               UpdateDetail(qint64 id, const VDetail& detail);
    void                               UpdateSpline(qint64 id, const VSpline& spl);
    void                               UpdateModelingSpline(qint64 id, const VSpline& spl);
    void                               UpdateSplinePath(qint64 id, const VSplinePath& splPath);
    void                               UpdateModelingSplinePath(qint64 id, const VSplinePath& splPath);
    void                               UpdateArc(qint64 id, const VArc& arc);
    void                               UpdateModelingArc(qint64 id, const VArc& arc);
    void                               UpdateStandartTableCell(const QString& name,
                                                               const VStandartTableCell& cell);
    void                               UpdateIncrementTableRow(const QString& name,
                                                               const VIncrementTableRow& cell);
    qreal                              GetValueStandartTableCell(const QString& name) const;
    qreal                              GetValueIncrementTableRow(const QString& name) const;
    void                               Clear();
    void                               ClearObject();
    void                               ClearIncrementTable();
    void                               ClearLengthLines();
    void                               ClearLengthSplines();
    void                               ClearLengthArcs();
    void                               ClearLineAngles();
    void                               SetSize(qint32 size);
    void                               SetGrowth(qint32 growth);
    qint32                             size() const;
    qint32                             growth() const;
    qreal                              FindVar(const QString& name, bool *ok)const;
    bool                               IncrementTableContains(const QString& name);
    static qint64                      getNextId();
    void                               RemoveIncrementTableRow(const QString& name);
    const QHash<qint64, VPointF>       *DataPoints() const;
    const QHash<qint64, VPointF>       *DataModelingPoints() const;
    const QHash<qint64, VSpline>       *DataSplines() const;
    const QHash<qint64, VSpline>       *DataModelingSplines() const;
    const QHash<qint64, VArc>          *DataArcs() const;
    const QHash<qint64, VArc>          *DataModelingArcs() const;
    const QHash<QString, qint32>       *DataBase() const;
    const QHash<QString, VStandartTableCell> *DataStandartTable() const;
    const QHash<QString, VIncrementTableRow> *DataIncrementTable() const;
    const QHash<QString, qreal>        *DataLengthLines() const;
    const QHash<QString, qreal>        *DataLengthSplines() const;
    const QHash<QString, qreal>        *DataLengthArcs() const;
    const QHash<QString, qreal>        *DataLineAngles() const;
    const QHash<qint64, VSplinePath>   *DataSplinePaths() const;
    const QHash<qint64, VSplinePath>   *DataModelingSplinePaths() const;
    const QHash<qint64, VDetail>       *DataDetails() const;
    static void                        UpdateId(qint64 newId);
    QPainterPath                       ContourPath(qint64 idDetail) const;
    QPainterPath                       Equidistant(QVector<QPointF> points,
                                                   const Detail::Equidistant &eqv,
                                                   const qreal &width)const;
    static QLineF                      ParallelLine(const QLineF &line, qreal width );
    static QPointF                     SingleParallelPoint(const QLineF &line, const qreal &angle,
                                                           const qreal &width);
    QVector<QPointF>                   EkvPoint(const QLineF &line1, const QLineF &line2,
                                                const qreal &width)const;
    QVector<QPointF>                   CheckLoops(const QVector<QPointF> &points) const;
    void                               PrepareDetails(QVector<VItem*> & list)const;
private:
    static qint64                      _id;
    QHash<QString, qint32>             base;
    QHash<qint64, VPointF>             points;
    QHash<qint64, VPointF>             modelingPoints;
    QHash<QString, VStandartTableCell> standartTable;
    QHash<QString, VIncrementTableRow> incrementTable;
    QHash<QString, qreal>              lengthLines;
    QHash<QString, qreal>              lineAngles;
    QHash<qint64, VSpline>             splines;
    QHash<qint64, VSpline>             modelingSplines;
    QHash<QString, qreal>              lengthSplines;
    QHash<qint64, VArc>                arcs;
    QHash<qint64, VArc>                modelingArcs;
    QHash<QString, qreal>              lengthArcs;
    QHash<qint64, VSplinePath>         splinePaths;
    QHash<qint64, VSplinePath>         modelingSplinePaths;
    QHash<qint64, VDetail>             details;
    template <typename key, typename val> static val GetObject(const QHash<key,val> &obj, key id);
    template <typename val> static void UpdateObject(QHash<qint64, val> &obj, const qint64 &id,
                                                     const val& point);
    template <typename key, typename val> static qint64 AddObject(QHash<key, val> &obj,
                                                                  const val& value);
    void                               CreateManTableIGroup ();
    QVector<QPointF>                   GetReversePoint(const QVector<QPointF> &points)const;
    qreal                              GetLengthContour(const QVector<QPointF> &contour,
                                                        const QVector<QPointF> &newPoints)const;
};

#endif // VCONTAINER_H

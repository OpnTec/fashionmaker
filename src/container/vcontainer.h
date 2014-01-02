/************************************************************************
 **
 **  @file   vcontainer.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include "vstandarttablerow.h"
#include "vincrementtablerow.h"
#include "../geometry/varc.h"
#include "../geometry/vsplinepath.h"
#include "../geometry/vdetail.h"
#include "../widgets/vitem.h"
#include "../geometry/vgobject.h"
#include "../exception/vexceptionbadid.h"

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer
{
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
                        ~VContainer();
    template <typename T>
    void CopyGObject(const VContainer &data, const qint64 &id)
    {
        T *obj = new T(*data.GeometricObject<const T *>(id));
        Q_ASSERT(obj != 0);
        UpdateGObject(id, obj);
    }
   /**
    * @brief setData copy data from container
    * @param data container
    */
    void                setData(const VContainer &data);
    template <typename T>
    const T GeometricObject(qint64 id) const
    {
        VGObject *gObj = 0;
        if (gObjects.contains(id))
        {
            gObj = gObjects.value(id);
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object"), id);
        }
        T obj = dynamic_cast<T>(gObj);
        Q_ASSERT(obj != 0);
        return obj;
    }

    /**
     * @brief GetGObject returns a point by id
     * @param id id of point
     * @return point
     */
    const VGObject *GetGObject(qint64 id) const;
    /**
     * @brief GetStandartTableCell return standart table row by name
     * @param name name of standart table row
     * @return row of standart table
     */
    const VStandartTableRow GetStandartTableCell(const QString& name) const;
    /**
     * @brief GetIncrementTableRow return increment table row by name
     * @param name name of increment table row
     * @return row of increment table
     */
    const VIncrementTableRow GetIncrementTableRow(const QString& name) const;
    /**
     * @brief GetLine return length of line by name
     * @param name name of line
     * @return length of line in mm
     */
    qreal               GetLine(const QString &name) const;
    /**
     * @brief GetLengthArc return length of arc by name
     * @param name name of arc
     * @return length of arc in mm
     */
    qreal               GetLengthArc(const QString &name) const;
    /**
     * @brief GetLengthSpline return length of spline by name
     * @param name name of spline
     * @return length of spline in mm
     */
    qreal               GetLengthSpline(const QString &name) const;
    /**
     * @brief GetLineAngle return angle of line
     * @param name name of line angle
     * @return angle in degree
     */
    qreal               GetLineAngle(const QString &name) const;
    /**
     * @brief GetDetail return detail by id
     * @param id id of detail
     * @return detail
     */
    const VDetail GetDetail(qint64 id) const;
    /**
     * @brief getId return current id
     * @return current id
     */
    static qint64       getId() {return _id;}
    /**
     * @brief AddPoint add new point to container
     * @param point new point
     * @return return id of new point in container
     */
    qint64              AddGObject(VGObject *obj);
    /**
     * @brief AddDetail add new detail to container
     * @param detail new detail
     * @return return id of new detail in container
     */
    qint64              AddDetail(VDetail detail);
    /**
     * @brief AddStandartTableCell add new row of standart table
     * @param name name of row of standart table
     * @param cell row of standart table
     */
    inline void         AddStandartTableCell(const QString& name, VStandartTableRow cell)
    {standartTable[name] = cell;}
    /**
     * @brief AddIncrementTableRow add new row of increment table
     * @param name name of new row of increment table
     * @param row new row of increment table
     */
    void                AddIncrementTableRow(const QString& name, VIncrementTableRow row);
    /**
     * @brief AddLengthLine add length of line to container
     * @param name name of line
     * @param value length of line
     */
    void                AddLengthLine(const QString &name, const qreal &value);
    /**
     * @brief AddLengthSpline add length of spline to container
     * @param name name of spline
     * @param value length of spline
     */
    void                AddLengthSpline(const QString &name, const qreal &value);
    /**
     * @brief AddLengthArc add length of arc to container
     * @param id id of arc
     */
    void                AddLengthArc(const qint64 &id);
    /**
     * @brief AddLengthArc add length of arc
     * @param name name of arc
     * @param value length of arc
     */
    void                AddLengthArc(const QString &name, const qreal &value);
    /**
     * @brief AddLineAngle add angle of line to container
     * @param name name of line angle
     * @param value angle in degree
     */
    void                AddLineAngle(const QString &name, const qreal &value);
    /**
     * @brief AddLine add line to container
     * @param firstPointId id of first point of line
     * @param secondPointId id of second point of line
     * @param mode mode of line
     */
    void                AddLine(const qint64 &firstPointId, const qint64 &secondPointId);
    /**
     * @brief GetNameLine return name of line
     * @param firstPoint id of first point of line
     * @param secondPoint id of second point of line
     * @return name of line
     */
    QString             GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const;
    /**
     * @brief GetNameLineAngle return name of line angle
     * @param firstPoint id of first point of line
     * @param secondPoint id of second point of line
     * @return name of angle of line
     */
    QString             GetNameLineAngle(const qint64 &firstPoint, const qint64 &secondPoint) const;
    /**
     * @brief UpdatePoint update point by id
     * @param id id of existing point
     * @param point point
     */
    void                UpdateGObject(qint64 id, VGObject* obj);
    /**
     * @brief UpdateDetail update detail by id
     * @param id id of existing detail
     * @param detail detail
     */
    void                UpdateDetail(qint64 id, VDetail detail);
    /**
     * @brief UpdateStandartTableCell update standart table row by name
     * @param name name of row
     * @param cell row of standart table
     */
    inline void         UpdateStandartTableCell(const QString& name, VStandartTableRow cell)
    {standartTable[name] = cell;}
    /**
     * @brief UpdateIncrementTableRow update increment table row by name
     * @param name name of row
     * @param row row
     */
    inline void         UpdateIncrementTableRow(const QString& name, VIncrementTableRow row)
    {incrementTable[name] = row;}
    /**
     * @brief GetValueStandartTableCell return value of standart table row by name
     * @param name name of row
     * @return value in mm
     */
    qreal               GetValueStandartTableCell(const QString& name) const;
    /**
     * @brief GetValueIncrementTableRow return value of increment table row by name
     * @param name name of row
     * @return value of row in mm
     */
    qreal               GetValueIncrementTableRow(const QString& name) const;
    /**
     * @brief Clear clear data in container. Id will be 0.
     */
    void                Clear();
    /**
     * @brief ClearObject points, splines, arcs, spline paths will be cleared.
     */
    void                ClearGObjects();
    void                ClearCalculationGObjects();
    /**
     * @brief ClearIncrementTable clear increment table
     */
    inline void         ClearIncrementTable() {incrementTable.clear();}
    /**
     * @brief ClearLengthLines clear length lines
     */
    inline void         ClearLengthLines() {lengthLines.clear();}
    /**
     * @brief ClearLengthSplines clear length splines
     */
    inline void         ClearLengthSplines() {lengthSplines.clear();}
    /**
     * @brief ClearLengthArcs clear length arcs
     */
    inline void         ClearLengthArcs() {lengthArcs.clear();}
    /**
     * @brief ClearLineAngles clear angles of lines
     */
    inline void         ClearLineAngles() {lineAngles.clear();}
    /**
     * @brief SetSize set value of size
     * @param size value of size in mm
     */
    inline void         SetSize(qint32 size) {base["Сг"] = size;}
    /**
     * @brief SetGrowth set value of growth
     * @param growth value of growth in mm
     */
    inline void         SetGrowth(qint32 growth) {base["Р"] = growth;}
    /**
     * @brief size return size
     * @return size in mm
     */
    inline qint32       size() const {return base.value("Сг");}
    /**
     * @brief growth return growth
     * @return growth in mm
     */
    inline qint32       growth() const {return base.value("Р");}
    /**
     * @brief FindVar return value of variable by name
     * @param name name of variable
     * @param ok false if can't find variable
     * @return value of variable
     */
    qreal               FindVar(const QString& name, bool *ok)const;
    /**
     * @brief IncrementTableContains check if increment table contains name
     * @param name name of row
     * @return true if contains
     */
    inline bool         IncrementTableContains(const QString& name) {return incrementTable.contains(name);}
    /**
     * @brief getNextId generate next unique id
     * @return next unique id
     */
    static qint64       getNextId();
    /**
     * @brief RemoveIncrementTableRow remove row by name from increment table
     * @param name name of existing row
     */
    inline void         RemoveIncrementTableRow(const QString& name) {incrementTable.remove(name);}
    /**
     * @brief data container with datagObjects return container of gObjects
     * @return pointer on container of gObjects
     */
    inline const QHash<qint64, VGObject*> *DataGObjects() const {return &gObjects;}
    /**
     * @brief data container with dataBase return container of data
     * @return pointer on container of base data
     */
    inline const QHash<QString, qint32> *DataBase() const {return &base;}
    /**
     * @brief data container with dataStandartTable return container of standart table
     * @return pointer on container of standart table
     */
    inline const QHash<QString, VStandartTableRow> *DataStandartTable() const {return &standartTable;}
    /**
     * @brief data container with dataIncrementTable return container of increment table
     * @return pointer on container of increment table
     */
    inline const QHash<QString, VIncrementTableRow> *DataIncrementTable() const {return &incrementTable;}
    /**
     * @brief data container with dataLengthLines return container of lines lengths
     * @return pointer on container of lines lengths
     */
    inline const QHash<QString, qreal>  *DataLengthLines() const {return &lengthLines;}
    /**
     * @brief data container with dataLengthSplines return container of splines lengths
     * @return pointer on container of splines lengths
     */
    inline const QHash<QString, qreal>  *DataLengthSplines() const {return &lengthSplines;}
    /**
     * @brief data container with dataLengthArcs return container of arcs length
     * @return pointer on container of arcs length
     */
    inline const QHash<QString, qreal>  *DataLengthArcs() const {return &lengthArcs;}
    /**
     * @brief data container with dataLineAngles return container of angles of line
     * @return pointer on container of angles of line
     */
    inline const QHash<QString, qreal>  *DataLineAngles() const {return &lineAngles;}
    /**
     * @brief data container with dataDetails return container of details
     * @return pointer on container of details
     */
    inline const QHash<qint64, VDetail> *DataDetails() const {return &details;}
    /**
     * @brief UpdateId update id. If new id bigger when current save new like current.
     * @param newId id
     */
    static void         UpdateId(qint64 newId);
    /**
     * @brief ContourPath create painter path for detail
     * @param idDetail id of detail
     * @return return painter path of contour detail
     */
    QPainterPath        ContourPath(qint64 idDetail) const;
    /**
     * @brief biasPoints bias point
     * @param points vector of points
     * @param mx offset respect to x
     * @param my offset respect to y
     * @return new vector biased points
     */
    QVector<QPointF>    biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my) const;
    /**
     * @brief Equidistant create equidistant painter path for detail
     * @param points vector of points
     * @param eqv type of equidistant
     * @param width width of equidistant
     * @return return painter path of equidistant
     */
    QPainterPath        Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv, const qreal &width)const;
    /**
     * @brief ParallelLine create parallel line
     * @param line starting line
     * @param width width to parallel line
     * @return parallel line
     */
    static QLineF       ParallelLine(const QLineF &line, qreal width );
    /**
     * @brief SingleParallelPoint return point of parallel line
     * @param line starting line
     * @param angle angle in degree
     * @param width width to parallel line
     * @return point of parallel line
     */
    static QPointF      SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width);
    /**
     * @brief EkvPoint return vector of points of equidistant two lines. Last point of two lines must be equal.
     * @param line1 first line
     * @param line2 second line
     * @param width width of equidistant
     * @return vector of points
     */
    QVector<QPointF>    EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width)const;
    /**
     * @brief CheckLoops seek and delete loops in equidistant
     * @param points vector of points of equidistant
     * @return vector of points of equidistant
     */
    QVector<QPointF>    CheckLoops(const QVector<QPointF> &points) const;
    /**
     * @brief PrepareDetails prepare detail for creation layout
     * @param list list of details
     */
    void                PrepareDetails(QVector<VItem *> & list) const;
    /**
     * @brief CreateManTableIGroup generate man standart table of measurements
     */
    void               CreateManTableIGroup ();
private:
    /**
     * @brief _id current id. New object will have value +1. For full class equal 0.
     */
    static qint64          _id;
    /**
     * @brief base container of base data (size and growth)
     */
    QHash<QString, qint32> base;
    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<qint64, VGObject*> gObjects;
    /**
     * @brief standartTable container of standart table rows
     */
    QHash<QString, VStandartTableRow> standartTable;
    /**
     * @brief incrementTable
     */
    QHash<QString, VIncrementTableRow> incrementTable;
    /**
     * @brief lengthLines container of lines lengths
     */
    QHash<QString, qreal>  lengthLines;
    /**
     * @brief lineAngles container of angles of lines
     */
    QHash<QString, qreal>  lineAngles;
    /**
     * @brief lengthSplines container of splines length
     */
    QHash<QString, qreal>  lengthSplines;
    /**
     * @brief lengthArcs container of arcs length
     */
    QHash<QString, qreal>  lengthArcs;
    /**
     * @brief details container of details
     */
    QHash<qint64, VDetail> details;
    /**
     * @brief GetReversePoint return revers container of points
     * @param points container with points
     * @return reverced points
     */
    QVector<QPointF>   GetReversePoint(const QVector<QPointF> &points)const;
    /**
     * @brief GetLengthContour return length of contour
     * @param contour container with points of contour
     * @param newPoints point whos we try to add to contour
     * @return length length of contour
     */
    qreal              GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints)const;
    template <typename key, typename val>
    /**
     * @brief GetObject return object from container
     * @param obj container
     * @param id id of object
     * @return Object
     */
    const val GetObject(const QHash<key, val> &obj, key id) const;
    template <typename key, typename val>
    /**
     * @brief GetObject return object from container
     * @param obj container
     * @param id id of object
     * @return Object
     */
    val GetVariable(const QHash<key, val> &obj, key id) const;
    template <typename val>
    /**
     * @brief UpdateObject update object in container
     * @param obj container
     * @param id id of existing object
     * @param point object
     */
    void UpdateObject(QHash<qint64, val > &obj, const qint64 &id, val point);
    template <typename key, typename val>
    /**
     * @brief AddObject add object to container
     * @param obj container
     * @param value object
     * @return id of object in container
     */
    static qint64 AddObject(QHash<key, val> &obj, val value);
};

#endif // VCONTAINER_H

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

#include "vmeasurement.h"
#include "vincrement.h"
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
    void CopyGObject(const VContainer &data, const quint32 &id)
    {
        T *obj = new T(*data.GeometricObject<const T *>(id));
        UpdateGObject(id, obj);
    }
   /**
    * @brief setData copy data from container
    * @param data container
    */
    void                setData(const VContainer &data);
    template <typename T>
    const T GeometricObject(const quint32 &id) const
    {
        VGObject *gObj = nullptr;
        if (gObjects.contains(id))
        {
            gObj = gObjects.value(id);
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object"), id);
            return nullptr;
        }

        try
        {
            T obj = dynamic_cast<T>(gObj);
            Q_CHECK_PTR(obj);
            return obj;
        }
        catch (const std::bad_alloc &)
        {
            throw VExceptionBadId(tr("Can't cast object"), id);
            return nullptr;
        }
        return nullptr;
    }

    /**
     * @brief GetGObject returns a point by id
     * @param id id of point
     * @return point
     */
    const VGObject *GetGObject(quint32 id) const;
    /**
     * @brief GetMeasurement return measurement by name
     * @param name short measurement name
     * @return measurement
     */
    const VMeasurement  GetMeasurement(const QString& name) const;
    /**
     * @brief GetIncrement return increment table row by name
     * @param name name of increment table row
     * @return increment
     */
    const VIncrement GetIncrement(const QString& name) const;
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
    const VDetail       GetDetail(quint32 id) const;
    /**
     * @brief getId return current id
     * @return current id
     */
    static quint32       getId() {return _id;}
    /**
     * @brief AddPoint add new point to container
     * @param point new point
     * @return return id of new point in container
     */
    quint32              AddGObject(VGObject *obj);
    /**
     * @brief AddDetail add new detail to container
     * @param detail new detail
     * @return return id of new detail in container
     */
    quint32              AddDetail(VDetail detail);
    /**
     * @brief AddMeasurement add new measurement
     * @param name short measurement name
     * @param row measurement
     */
    void                 AddMeasurement(const QString& name, const VMeasurement &m);
    /**
     * @brief AddIncrement add new row of increment table
     * @param name name of new row of increment table
     * @param row new row of increment table
     */
    void                AddIncrement(const QString& name, VIncrement incr);
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
    void                AddLengthArc(const quint32 &id);
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
    void                AddLine(const quint32 &firstPointId, const quint32 &secondPointId);
    /**
     * @brief GetNameLine return name of line
     * @param firstPoint id of first point of line
     * @param secondPoint id of second point of line
     * @return name of line
     */
    // cppcheck-suppress functionStatic
    QString             GetNameLine(const quint32 &firstPoint, const quint32 &secondPoint) const;
    /**
     * @brief GetNameLineAngle return name of line angle
     * @param firstPoint id of first point of line
     * @param secondPoint id of second point of line
     * @return name of angle of line
     */
    // cppcheck-suppress functionStatic
    QString             GetNameLineAngle(const quint32 &firstPoint, const quint32 &secondPoint) const;
    /**
     * @brief UpdatePoint update point by id
     * @param id id of existing point
     * @param point point
     */
    void                UpdateGObject(quint32 id, VGObject* obj);
    /**
     * @brief UpdateDetail update detail by id
     * @param id id of existing detail
     * @param detail detail
     */
    void                UpdateDetail(quint32 id, const VDetail &detail);
    /**
     * @brief UpdateMeasurement update measurement by name
     * @param name short measurement name
     * @param m measurement
     */
    void                UpdateMeasurement(const QString& name, VMeasurement m);
    /**
     * @brief UpdateIncrement update increment table row by name
     * @param name name of row
     * @param row row
     */
    void                UpdateIncrement(const QString& name, VIncrement incr);
    /**
     * @brief GetValueStandardTableRow return value of measurement by name
     * @param name name of measurement
     * @return value in measurement units
     */
    qreal               GetValueStandardTableRow(const QString& name) const;
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
    void                ClearIncrementTable();
    /**
     * @brief ClearLengthLines clear length lines
     */
    void                ClearLengthLines();
    /**
     * @brief ClearLengthSplines clear length splines
     */
    void                ClearLengthSplines();
    /**
     * @brief ClearLengthArcs clear length arcs
     */
    void                ClearLengthArcs();
    /**
     * @brief ClearLineAngles clear angles of lines
     */
    void                ClearLineAngles();
    void                ClearDetails();
    /**
     * @brief SetSize set value of size
     * @param size value of size
     */
    void                SetSize(qreal size);
    void                SetSizeName(const QString &name);
    /**
     * @brief SetGrowth set value of growth
     * @param growth value of growth
     */
    void                SetHeight(qreal height);
    void                SetHeightName(const QString &name);
    /**
     * @brief size return size
     * @return size in mm
     */
    qreal               size() const;
    QString             SizeName()const;
    /**
     * @brief height return height
     * @return height in pattern units
     */
    qreal               height() const;
    QString             HeightName()const;
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
    bool                IncrementTableContains(const QString& name);
    /**
     * @brief getNextId generate next unique id
     * @return next unique id
     */
    static quint32       getNextId();
    /**
     * @brief RemoveIncrementTableRow remove row by name from increment table
     * @param name name of existing row
     */
    void                 RemoveIncrementTableRow(const QString& name);
    /**
     * @brief data container with datagObjects return container of gObjects
     * @return pointer on container of gObjects
     */
    const QHash<quint32, VGObject*> *DataGObjects() const;
    /**
     * @brief DataMeasurements container with measurements.
     * @return pointer to measurements.
     */
    const QHash<QString, VMeasurement> *DataMeasurements() const;
    /**
     * @brief data container with dataIncrements return container of increment table
     * @return pointer on container of increment table
     */
    const QHash<QString, VIncrement> *DataIncrements() const;
    /**
     * @brief data container with dataLengthLines return container of lines lengths
     * @return pointer on container of lines lengths
     */
    const QHash<QString, qreal>  *DataLengthLines() const;
    /**
     * @brief data container with dataLengthSplines return container of splines lengths
     * @return pointer on container of splines lengths
     */
    const QHash<QString, qreal>  *DataLengthSplines() const;
    /**
     * @brief data container with dataLengthArcs return container of arcs length
     * @return pointer on container of arcs length
     */
    const QHash<QString, qreal>  *DataLengthArcs() const;
    /**
     * @brief data container with dataLineAngles return container of angles of line
     * @return pointer on container of angles of line
     */
    const QHash<QString, qreal>  *DataLineAngles() const;
    /**
     * @brief data container with dataDetails return container of details
     * @return pointer on container of details
     */
    const QHash<quint32, VDetail> *DataDetails() const;
    /**
     * @brief UpdateId update id. If new id bigger when current save new like current.
     * @param newId id
     */
    static void         UpdateId(quint32 newId);
private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32          _id;
    qreal _size;
    QString sizeName;
    qreal _height;
    QString heightName;
    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<quint32, VGObject*> gObjects;
    /**
     * @brief measurements container of measurements.
     */
    QHash<QString, VMeasurement> measurements;
    /**
     * @brief increments
     */
    QHash<QString, VIncrement> increments;
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
    QHash<quint32, VDetail> details;
    template <typename key, typename val>
    /**
     * @brief GetObject return object from container
     * @param obj container
     * @param id id of object
     * @return Object
     */
    // cppcheck-suppress functionStatic
    const val GetObject(const QHash<key, val> &obj, key id) const;
    template <typename key, typename val>
    /**
     * @brief GetObject return object from container
     * @param obj container
     * @param id id of object
     * @return Object
     */
    // cppcheck-suppress functionStatic
    val GetVariable(const QHash<key, val> &obj, key id) const;
    template <typename val>
    /**
     * @brief UpdateObject update object in container
     * @param obj container
     * @param id id of existing object
     * @param point object
     */
    void UpdateObject(QHash<quint32, val > &obj, const quint32 &id, val point);
    template <typename key, typename val>
    /**
     * @brief AddObject add object to container
     * @param obj container
     * @param value object
     * @return id of object in container
     */
    static quint32 AddObject(QHash<key, val> &obj, val value);
};

inline void VContainer::AddMeasurement(const QString &name, const VMeasurement &m)
{
    measurements[name] = m;
}

inline void VContainer::UpdateMeasurement(const QString &name, VMeasurement m)
{
    measurements[name] = m;
}

inline void VContainer::UpdateIncrement(const QString &name, VIncrement incr)
{
    increments[name] = incr;
}

inline void VContainer::ClearIncrementTable()
{
    increments.clear();
}

inline void VContainer::ClearLengthLines()
{
    lengthLines.clear();
}

inline void VContainer::ClearLengthSplines()
{
    lengthSplines.clear();
}

inline void VContainer::ClearLengthArcs()
{
    lengthArcs.clear();
}

inline void VContainer::ClearLineAngles()
{
    lineAngles.clear();
}

inline void VContainer::ClearDetails()
{
    details.clear();
}

inline void VContainer::SetSize(qreal size)
{
    _size = size;
}

inline void VContainer::SetSizeName(const QString &name)
{
    sizeName = name;
}

inline void VContainer::SetHeight(qreal height)
{
    _height = height;
}

inline void VContainer::SetHeightName(const QString &name)
{
    heightName = name;
}

inline qreal VContainer::size() const
{
    return _size;
}

inline QString VContainer::SizeName() const
{
    return sizeName;
}

inline qreal VContainer::height() const
{
    return _height;
}

inline QString VContainer::HeightName() const
{
    return heightName;
}

inline bool VContainer::IncrementTableContains(const QString &name)
{
    return increments.contains(name);
}

inline void VContainer::RemoveIncrementTableRow(const QString &name)
{
    increments.remove(name);
}

inline const QHash<quint32, VGObject *> *VContainer::DataGObjects() const
{
    return &gObjects;
}

inline const QHash<QString, VMeasurement> *VContainer::DataMeasurements() const
{
    return &measurements;
}

inline const QHash<QString, VIncrement> *VContainer::DataIncrements() const
{
    return &increments;
}

inline const QHash<QString, qreal> *VContainer::DataLengthLines() const
{
    return &lengthLines;
}

inline const QHash<QString, qreal> *VContainer::DataLengthSplines() const
{
    return &lengthSplines;
}

inline const QHash<QString, qreal> *VContainer::DataLengthArcs() const
{
    return &lengthArcs;
}

inline const QHash<QString, qreal> *VContainer::DataLineAngles() const
{
    return &lineAngles;
}

inline const QHash<quint32, VDetail> *VContainer::DataDetails() const
{
    return &details;
}

#endif // VCONTAINER_H

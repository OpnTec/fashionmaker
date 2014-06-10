/************************************************************************
 **
 **  @file   vcontainer.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
    VContainer();
    VContainer &operator=(const VContainer &data);
    VContainer(const VContainer &data);
    ~VContainer();
    template <typename T>
    void CopyGObject(const VContainer &data, const quint32 &id)
    {
        T *obj = new T(*data.GeometricObject<const T *>(id));
        UpdateGObject(id, obj);
    }
    void               setData(const VContainer &data);
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
            SCASSERT(obj != nullptr);
            return obj;
        }
        catch (const std::bad_alloc &)
        {
            throw VExceptionBadId(tr("Can't cast object"), id);
            return nullptr;
        }
        return nullptr;
    }

    const VGObject    *GetGObject(quint32 id) const;
    const VMeasurement GetMeasurement(const QString& name) const;
    const VIncrement   GetIncrement(const QString& name) const;
    qreal              GetLine(const QString &name) const;
    qreal              GetLengthArc(const QString &name) const;
    qreal              GetLengthSpline(const QString &name) const;
    qreal              GetLineAngle(const QString &name) const;
    const VDetail      GetDetail(quint32 id) const;
    static quint32     getId(){return _id;}
    quint32            AddGObject(VGObject *obj);
    quint32            AddDetail(VDetail detail);
    void               AddMeasurement(const QString& name, const VMeasurement &m);
    void               AddIncrement(const QString& name, VIncrement incr);
    void               AddLengthLine(const QString &name, const qreal &value);
    void               AddLengthSpline(const QString &name, const qreal &value);
    void               AddLengthArc(const quint32 &id);
    void               AddLineAngle(const QString &name, const qreal &value);
    void               AddLine(const quint32 &firstPointId, const quint32 &secondPointId);
    // cppcheck-suppress functionStatic
    QString            GetNameLine(const quint32 &firstPoint, const quint32 &secondPoint) const;
    // cppcheck-suppress functionStatic
    QString            GetNameLineAngle(const quint32 &firstPoint, const quint32 &secondPoint) const;
    void               UpdateGObject(quint32 id, VGObject* obj);
    void               UpdateDetail(quint32 id, const VDetail &detail);
    void               UpdateMeasurement(const QString& name, VMeasurement m);
    void               UpdateIncrement(const QString& name, VIncrement incr);
    qreal              GetValueStandardTableRow(const QString& name) const;
    qreal              GetValueIncrementTableRow(const QString& name) const;
    void               Clear();
    void               ClearGObjects();
    void               ClearCalculationGObjects();
    void               ClearIncrementTable();
    void               ClearMeasurements();
    void               ClearLengthLines();
    void               ClearLengthSplines();
    void               ClearLengthArcs();
    void               ClearLineAngles();
    void               ClearDetails();
    void               SetSize(qreal size);
    void               SetSizeName(const QString &name);
    void               SetHeight(qreal height);
    void               SetHeightName(const QString &name);
    qreal              size() const;
    QString            SizeName()const;
    qreal              height() const;
    QString            HeightName()const;
    qreal              FindVar(const QString& name, bool *ok)const;
    bool               IncrementTableContains(const QString& name);
    static quint32     getNextId();
    void               RemoveIncrementTableRow(const QString& name);
    static void        UpdateId(quint32 newId);
    const QHash<quint32, VGObject*>    *DataGObjects() const;
    const QHash<QString, VMeasurement> *DataMeasurements() const;
    const QHash<QString, VIncrement>   *DataIncrements() const;
    const QHash<QString, qreal>        *DataLengthLines() const;
    const QHash<QString, qreal>        *DataLengthSplines() const;
    const QHash<QString, qreal>        *DataLengthArcs() const;
    const QHash<QString, qreal>        *DataLineAngles() const;
    const QHash<quint32, VDetail>      *DataDetails() const;
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
    // cppcheck-suppress functionStatic
    const val GetObject(const QHash<key, val> &obj, key id) const;

    template <typename key, typename val>
    // cppcheck-suppress functionStatic
    val GetVariable(const QHash<key, val> &obj, key id) const;

    template <typename val>
    void UpdateObject(QHash<quint32, val > &obj, const quint32 &id, val point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val> &obj, val value);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddMeasurement add new measurement
 * @param name short measurement name
 * @param m measurement
 */
inline void VContainer::AddMeasurement(const QString &name, const VMeasurement &m)
{
    measurements[name] = m;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateMeasurement update measurement by name
 * @param name short measurement name
 * @param m measurement
 */
inline void VContainer::UpdateMeasurement(const QString &name, VMeasurement m)
{
    measurements[name] = m;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateIncrement update increment table row by name
 * @param name name of row
 * @param incr increment
 */
inline void VContainer::UpdateIncrement(const QString &name, VIncrement incr)
{
    increments[name] = incr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearIncrementTable clear increment table
 */
inline void VContainer::ClearIncrementTable()
{
    increments.clear();
}

//---------------------------------------------------------------------------------------------------------------------
inline void VContainer::ClearMeasurements()
{
    measurements.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearLengthLines clear length lines
 */
inline void VContainer::ClearLengthLines()
{
    lengthLines.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearLengthSplines clear length splines
 */
inline void VContainer::ClearLengthSplines()
{
    lengthSplines.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearLengthArcs clear length arcs
 */
inline void VContainer::ClearLengthArcs()
{
    lengthArcs.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearLineAngles clear angles of lines
 */
inline void VContainer::ClearLineAngles()
{
    lineAngles.clear();
}

//---------------------------------------------------------------------------------------------------------------------
inline void VContainer::ClearDetails()
{
    details.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSize set value of size
 * @param size value of size
 */
inline void VContainer::SetSize(qreal size)
{
    _size = size;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VContainer::SetSizeName(const QString &name)
{
    sizeName = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetGrowth set value of growth
 * @param height value of height
 */
inline void VContainer::SetHeight(qreal height)
{
    _height = height;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VContainer::SetHeightName(const QString &name)
{
    heightName = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief size return size
 * @return size in mm
 */
inline qreal VContainer::size() const
{
    return _size;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VContainer::SizeName() const
{
    return sizeName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief height return height
 * @return height in pattern units
 */
inline qreal VContainer::height() const
{
    return _height;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VContainer::HeightName() const
{
    return heightName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IncrementTableContains check if increment table contains name
 * @param name name of row
 * @return true if contains
 */
inline bool VContainer::IncrementTableContains(const QString &name)
{
    return increments.contains(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveIncrementTableRow remove row by name from increment table
 * @param name name of existing row
 */
inline void VContainer::RemoveIncrementTableRow(const QString &name)
{
    increments.remove(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with datagObjects return container of gObjects
 * @return pointer on container of gObjects
 */
inline const QHash<quint32, VGObject *> *VContainer::DataGObjects() const
{
    return &gObjects;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DataMeasurements container with measurements.
 * @return pointer to measurements.
 */
inline const QHash<QString, VMeasurement> *VContainer::DataMeasurements() const
{
    return &measurements;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataIncrements return container of increment table
 * @return pointer on container of increment table
 */
inline const QHash<QString, VIncrement> *VContainer::DataIncrements() const
{
    return &increments;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataLengthLines return container of lines lengths
 * @return pointer on container of lines lengths
 */
inline const QHash<QString, qreal> *VContainer::DataLengthLines() const
{
    return &lengthLines;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataLengthSplines return container of splines lengths
 * @return pointer on container of splines lengths
 */
inline const QHash<QString, qreal> *VContainer::DataLengthSplines() const
{
    return &lengthSplines;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataLengthArcs return container of arcs length
 * @return pointer on container of arcs length
 */
inline const QHash<QString, qreal> *VContainer::DataLengthArcs() const
{
    return &lengthArcs;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataLineAngles return container of angles of line
 * @return pointer on container of angles of line
 */
inline const QHash<QString, qreal> *VContainer::DataLineAngles() const
{
    return &lineAngles;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataDetails return container of details
 * @return pointer on container of details
 */
inline const QHash<quint32, VDetail> *VContainer::DataDetails() const
{
    return &details;
}

#endif // VCONTAINER_H

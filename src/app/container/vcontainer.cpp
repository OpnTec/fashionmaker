/************************************************************************
 **
 **  @file   vcontainer.cpp
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

#include "vcontainer.h"
#include <QDebug>
#include "../widgets/vapplication.h"
#include "../geometry/varc.h"
#include "../geometry/vsplinepath.h"

quint32 VContainer::_id = 0;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create empty container
 */
VContainer::VContainer()
    :_size(50), sizeName(size_M), _height(176), heightName(height_M), gObjects(QHash<quint32, VGObject *>()),
      measurements(QHash<QString, VMeasurement>()), increments(QHash<QString, VIncrement>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<quint32, VDetail>())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = copy constructor
 * @param data container
 * @return copy container
 */
VContainer &VContainer::operator =(const VContainer &data)
{
    setData(data);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create container from another container
 * @param data container
 */
VContainer::VContainer(const VContainer &data)
    :_size(50), sizeName(size_M), _height(176), heightName(height_M), gObjects(QHash<quint32, VGObject *>()),
      measurements(QHash<QString, VMeasurement>()), increments(QHash<QString, VIncrement>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<quint32, VDetail>())
{
    setData(data);
}

//---------------------------------------------------------------------------------------------------------------------
VContainer::~VContainer()
{
    qDeleteAll(gObjects);
    gObjects.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setData copy data from container
 * @param data container
 */
void VContainer::setData(const VContainer &data)
{
    _size = data.size();
    sizeName = data.SizeName();
    _height = data.height();
    heightName = data.HeightName();

    qDeleteAll(gObjects);
    gObjects.clear();
    const QHash<quint32, VGObject*> *obj = data.DataGObjects();
    Q_CHECK_PTR(obj);
    QHashIterator<quint32, VGObject*> i(*obj);
    while (i.hasNext())
    {
        i.next();
        switch (i.value()->getType())
        {
            case (GObject::Arc):
                CopyGObject<VArc>(data, i.key());
                break;
            case (GObject::Point):
                CopyGObject<VPointF>(data, i.key());
                break;
            case (GObject::Spline):
                CopyGObject<VSpline>(data, i.key());
                break;
            case (GObject::SplinePath):
                CopyGObject<VSplinePath>(data, i.key());
                break;
            default:
                qDebug()<<"Don't know how copy this type.";
                break;
        }
    }
    measurements = *data.DataMeasurements();
    increments = *data.DataIncrements();
    lengthLines = *data.DataLengthLines();
    lineAngles = *data.DataLineAngles();
    lengthSplines = *data.DataLengthSplines();
    lengthArcs = *data.DataLengthArcs();
    details = *data.DataDetails();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetGObject returns a point by id
 * @param id id of point
 * @return point
 */
// cppcheck-suppress unusedFunction
const VGObject *VContainer::GetGObject(quint32 id)const
{
    return GetObject(gObjects, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetObject return object from container
 * @param obj container
 * @param id id of object
 * @return Object
 */
template <typename key, typename val>
const val VContainer::GetObject(const QHash<key, val> &obj, key id) const
{
    if (obj.contains(id))
    {
        return obj.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetObject return object from container
 * @param obj container
 * @param id id of object
 * @return Object
 */
template <typename key, typename val>
val VContainer::GetVariable(const QHash<key, val> &obj, key id) const
{
    if (obj.contains(id))
    {
        return obj.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetMeasurement return measurement by name
 * @param name short measurement name
 * @return measurement
 */
const VMeasurement VContainer::GetMeasurement(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(measurements, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetIncrement return increment table row by name
 * @param name name of increment table row
 * @return increment
 */
const VIncrement VContainer::GetIncrement(const QString& name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(increments, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLine return length of line by name
 * @param name name of line
 * @return length of line in mm
 */
qreal VContainer::GetLine(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthLines, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLengthArc return length of arc by name
 * @param name name of arc
 * @return length of arc in mm
 */
qreal VContainer::GetLengthArc(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthArcs, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLengthSpline return length of spline by name
 * @param name name of spline
 * @return length of spline in mm
 */
qreal VContainer::GetLengthSpline(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthSplines, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLineAngle return angle of line
 * @param name name of line angle
 * @return angle in degree
 */
qreal VContainer::GetLineAngle(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lineAngles, name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetDetail return detail by id
 * @param id id of detail
 * @return detail
 */
const VDetail VContainer::GetDetail(quint32 id) const
{
    return GetVariable(details, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddGObject add new GObject to container
 * @param obj new object
 * @return return id of new object in container
 */
quint32 VContainer::AddGObject(VGObject *obj)
{
    return AddObject(gObjects, obj);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddDetail add new detail to container
 * @param detail new detail
 * @return return id of new detail in container
 */
quint32 VContainer::AddDetail(VDetail detail)
{
    quint32 id = getNextId();
    details[id] = detail;
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddIncrement add new increment
 * @param name increment name
 * @param incr increment
 */
void VContainer::AddIncrement(const QString &name, VIncrement incr)
{
    increments[name] = incr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNextId generate next unique id
 * @return next unique id
 */
quint32 VContainer::getNextId()
{
    _id++;
    return _id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateId update id. If new id bigger when current save new like current.
 * @param newId id
 */
void VContainer::UpdateId(quint32 newId)
{
    if (newId > _id)
    {
       _id = newId;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateObject update object in container
 * @param obj container
 * @param id id of existing object
 * @param point object
 */
template <typename val>
void VContainer::UpdateObject(QHash<quint32, val> &obj, const quint32 &id, val point)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    Q_CHECK_PTR(point);
    point->setId(id);
    if (gObjects.contains(id))
    {
        delete gObjects.value(id);
        gObjects.remove(id);
    }
    obj[id] = point;
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLengthSpline add length of spline to container
 * @param name name of spline
 * @param value length of spline
 */
void VContainer::AddLengthSpline(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthSplines[name] = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLengthArc add length of arc to container
 * @param id id of arc
 */
void VContainer::AddLengthArc(const quint32 &id)
{
    const VArc * arc = GeometricObject<const VArc *>(id);
    lengthArcs[arc->name()] = qApp->fromPixel(arc->GetLength());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLineAngle add angle of line to container
 * @param name name of line angle
 * @param value angle in degree
 */
void VContainer::AddLineAngle(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lineAngles[name] = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetValueStandardTableRow return value of measurement by name
 * @param name name of measurement
 * @return value in measurement units
 */
qreal VContainer::GetValueStandardTableRow(const QString& name) const
{
    const VMeasurement m = GetMeasurement(name);
    if (qApp->patternType() == Pattern::Individual)
    {
        return m.GetValue();
    }
    else
    {
        return m.GetValue(size(), height());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetValueIncrementTableRow return value of increment table row by name
 * @param name name of row
 * @return value of row in mm
 */
qreal VContainer::GetValueIncrementTableRow(const QString& name) const
{
    const VIncrement icr = GetIncrement(name);
    if (qApp->patternType() == Pattern::Individual)
    {
        return icr.GetValue();
    }
    else
    {
        return icr.GetValue(size(), height());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear data in container. Id will be 0.
 */
void VContainer::Clear()
{
    _id = 0;
    measurements.clear();
    increments.clear();
    lengthLines.clear();
    lengthArcs.clear();
    lineAngles.clear();
    details.clear();
    lengthSplines.clear();
    ClearGObjects();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearObject points, splines, arcs, spline paths will be cleared.
 */
void VContainer::ClearGObjects()
{
    if (gObjects.size()>0)
    {
        qDeleteAll(gObjects);
    }
    gObjects.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearCalculationGObjects()
{
    if (gObjects.size()>0)
    {
        QHashIterator<quint32, VGObject*> i(gObjects);
        while (i.hasNext())
        {
            i.next();
            if (i.value()->getMode() == Valentina::Calculation)
            {
                delete i.value();
                gObjects.remove(i.key());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindVar return value of variable by name
 * @param name name of variable
 * @param ok false if can't find variable
 * @return value of variable
 */
qreal VContainer::FindVar(const QString &name, bool *ok)const
{
    if (sizeName == name)
    {
        *ok = true;
        return _size;
    }
    if (heightName == name)
    {
        *ok = true;
        return _height;
    }
    if (measurements.contains(name))
    {
        *ok = true;
        return GetValueStandardTableRow(name);
    }
    if (increments.contains(name))
    {
        *ok = true;
        return GetValueIncrementTableRow(name);
    }
    if (lengthLines.contains(name))
    {
        *ok = true;
        return lengthLines.value(name);
    }
    if (lengthArcs.contains(name))
    {
        *ok = true;
        return lengthArcs.value(name);
    }
    if (lineAngles.contains(name))
    {
        *ok = true;
        return lineAngles.value(name);
    }
    if (lengthSplines.contains(name))
    {
        *ok = true;
        return lengthSplines.value(name);
    }
    *ok = false;
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLine add line to container
 * @param firstPointId id of first point of line
 * @param secondPointId id of second point of line
 */
void VContainer::AddLine(const quint32 &firstPointId, const quint32 &secondPointId)
{
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    const VPointF *first = GeometricObject<const VPointF *>(firstPointId);
    const VPointF *second = GeometricObject<const VPointF *>(secondPointId);
    AddLengthLine(nameLine, qApp->fromPixel(QLineF(first->toQPointF(), second->toQPointF()).length()));
    nameLine = GetNameLineAngle(firstPointId, secondPointId);
    AddLineAngle(nameLine, QLineF(first->toQPointF(), second->toQPointF()).angle());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddObject add object to container
 * @param obj container
 * @param value object
 * @return id of object in container
 */
template <typename key, typename val>
quint32 VContainer::AddObject(QHash<key, val> &obj, val value)
{
    Q_CHECK_PTR(value);
    quint32 id = getNextId();
    value->setId(id);
    obj[id] = value;
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameLine return name of line
 * @param firstPoint id of first point of line
 * @param secondPoint id of second point of line
 * @return name of line
 */
QString VContainer::GetNameLine(const quint32 &firstPoint, const quint32 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString(line_+"%1_%2").arg(first->name(), second->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameLineAngle return name of line angle
 * @param firstPoint id of first point of line
 * @param secondPoint id of second point of line
 * @return name of angle of line
 */
QString VContainer::GetNameLineAngle(const quint32 &firstPoint, const quint32 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString(angleLine_+"%1_%2").arg(first->name(), second->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateGObject update GObject by id
 * @param id id of existing GObject
 * @param obj object
 */
void VContainer::UpdateGObject(quint32 id, VGObject* obj)
{
    UpdateObject(gObjects, id, obj);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateDetail update detail by id
 * @param id id of existing detail
 * @param detail detail
 */
void VContainer::UpdateDetail(quint32 id, const VDetail &detail)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    details[id] = detail;
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLengthLine add length of line to container
 * @param name name of line
 * @param value length of line
 */
void VContainer::AddLengthLine(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthLines[name] = value;
}

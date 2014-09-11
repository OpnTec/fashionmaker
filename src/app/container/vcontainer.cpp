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
#include <QLineF>
#include <QtAlgorithms>

quint32 VContainer::_id = NULL_ID;
qreal VContainer::_size = 50;
qreal VContainer::_height = 176;
QSet<const QString> VContainer::uniqueNames = QSet<const QString>();

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create empty container
 */
VContainer::VContainer()
    :d(new VContainerData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = copy constructor
 * @param data container
 * @return copy container
 */
VContainer &VContainer::operator =(const VContainer &data)
{
    if ( &data == this )
    {
        return *this;
    }
    d = data.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create container from another container
 * @param data container
 */
VContainer::VContainer(const VContainer &data)
    :d(data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VContainer::~VContainer()
{
    ClearGObjects();
    ClearVariables();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetGObject returns a point by id
 * @param id id of point
 * @return point
 */
// cppcheck-suppress unusedFunction
const QSharedPointer<VGObject> VContainer::GetGObject(quint32 id)const
{
    return GetObject(d->gObjects, id);
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
 * @brief GetDetail return detail by id
 * @param id id of detail
 * @return detail
 */
const VDetail VContainer::GetDetail(quint32 id) const
{
    if (d->details.contains(id))
    {
        return d->details.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddGObject add new GObject to container
 * @param obj new object
 * @return return id of new object in container
 */
quint32 VContainer::AddGObject(VGObject *obj)
{
    SCASSERT(obj != nullptr);
    QSharedPointer<VGObject> pointer(obj);
    uniqueNames.insert(obj->name());
    return AddObject(d->gObjects, pointer);
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
    d->details[id] = detail;
    return id;
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
    Q_ASSERT_X(id > NULL_ID, Q_FUNC_INFO, "id = 0");
    SCASSERT(point.isNull() == false);
    point->setId(id);
    if (d->gObjects.contains(id))
    {
        d->gObjects[id].clear();
    }
    obj[id] = point;
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear data in container. Id will be 0.
 */
void VContainer::Clear()
{
    _id = NULL_ID;

    d->details.clear();
    ClearVariables();
    ClearGObjects();
    ClearUniqueNames();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearObject points, splines, arcs, spline paths will be cleared.
 */
void VContainer::ClearGObjects()
{
    QHash<quint32, QSharedPointer<VGObject> >::iterator i;
    for (i = d->gObjects.begin(); i != d->gObjects.end(); ++i)
    {
        i.value().clear();
    }
    d->gObjects.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearCalculationGObjects()
{
    if (d->gObjects.size()>0)
    {
        QVector<quint32> keys;
        QHash<quint32, QSharedPointer<VGObject> >::iterator i;
        for (i = d->gObjects.begin(); i != d->gObjects.end(); ++i)
        {
            if (i.value()->getMode() == Draw::Calculation)
            {
                i.value().clear();
                keys.append(i.key());
            }
        }
        if (keys.size()>0)
        {
            for (int i = 0; i < keys.size(); ++i)
            {
                d->gObjects.remove(keys.at(i));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearVariables(const VarType &type)
{
    if (d->variables.size()>0)
    {
        if (type == VarType::Unknown)
        {
            QHash<QString, QSharedPointer<VInternalVariable> >::iterator i;
            for (i = d->variables.begin(); i != d->variables.end(); ++i)
            {
                i.value().clear();
            }
            d->variables.clear();
        }
        else
        {
            QVector<QString> keys;
            QHash<QString, QSharedPointer<VInternalVariable> >::iterator i;
            for (i = d->variables.begin(); i != d->variables.end(); ++i)
            {
                if (i.value()->GetType() == type)
                {
                    i.value().clear();
                    keys.append(i.key());
                }
            }
            if (keys.size()>0)
            {
                for (int i = 0; i < keys.size(); ++i)
                {
                    d->variables.remove(keys.at(i));
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLine add line to container
 * @param firstPointId id of first point of line
 * @param secondPointId id of second point of line
 */
void VContainer::AddLine(const quint32 &firstPointId, const quint32 &secondPointId)
{
    const QSharedPointer<VPointF> first = GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> second = GeometricObject<VPointF>(secondPointId);

    VLengthLine *length = new VLengthLine(first.data(), firstPointId, second.data(), secondPointId);
    AddVariable(length->GetName(), length);

    VLineAngle *angle = new VLineAngle(first.data(), firstPointId, second.data(), secondPointId);
    AddVariable(angle->GetName(), angle);
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
    SCASSERT(value != nullptr);
    quint32 id = getNextId();
    value->setId(id);
    obj[id] = value;
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateGObject update GObject by id
 * @param id id of existing GObject
 * @param obj object
 */
void VContainer::UpdateGObject(quint32 id, VGObject* obj)
{
    SCASSERT(obj != nullptr);
    QSharedPointer<VGObject> pointer(obj);
    UpdateObject(d->gObjects, id, pointer);
    uniqueNames.insert(obj->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateDetail update detail by id
 * @param id id of existing detail
 * @param detail detail
 */
void VContainer::UpdateDetail(quint32 id, const VDetail &detail)
{
    Q_ASSERT_X(id > NULL_ID, Q_FUNC_INFO, "id = 0");
    d->details[id] = detail;
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VContainer::GetTableValue(const QString &name) const
{
    QSharedPointer<VVariable> m = GetVariable<VVariable>(name);
    if (qApp->patternType() == MeasurementsType::Standard)
    {
        m->SetValue(size(), height());
    }
    return *m->GetValue();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveIncrement remove increment by name from increment table
 * @param name name of existing increment
 */
void VContainer::RemoveIncrement(const QString &name)
{
    d->variables[name].clear();
    d->variables.remove(name);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VMeasurement> > VContainer::DataMeasurements() const
{
    return DataVar<VMeasurement>(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VIncrement> > VContainer::DataIncrements() const
{
    return DataVar<VIncrement>(VarType::Increment);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VLengthLine> > VContainer::DataLengthLines() const
{
    return DataVar<VLengthLine>(VarType::LineLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VSplineLength> > VContainer::DataLengthSplines() const
{
    return DataVar<VSplineLength>(VarType::SplineLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VArcLength> > VContainer::DataLengthArcs() const
{
    return DataVar<VArcLength>(VarType::ArcLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VLineAngle> > VContainer::DataAngleLines() const
{
    return DataVar<VLineAngle>(VarType::LineAngle);
}

//---------------------------------------------------------------------------------------------------------------------
bool VContainer::IsUnique(const QString &name)
{
    return !uniqueNames.contains(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VariableExist check if exist variable this same name.
 * @param name name of row
 * @return true if contains
 */
bool VContainer::VariableExist(const QString &name)
{
    return d->variables.contains(name);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
const QMap<QString, QSharedPointer<T> > VContainer::DataVar(const VarType &type) const
{
    QMap<QString, QSharedPointer<T> > map;
    //Sorting QHash by id
    QHash<QString, QSharedPointer<VInternalVariable> >::const_iterator i;
    for (i = d->variables.constBegin(); i != d->variables.constEnd(); ++i)
    {
        if (i.value()->GetType() == type)
        {
            QSharedPointer<T> var = GetVariable<T>(i.key());
            map.insert(qApp->VarToUser(i.key()), var);
        }
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearDetails()
{
    d->details.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearUniqueNames()
{
    uniqueNames.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSize set value of size
 * @param size value of size
 */
void VContainer::SetSize(qreal size)
{
    _size = size;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::SetSizeName(const QString &name)
{
    d->sizeName = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetGrowth set value of growth
 * @param height value of height
 */
void VContainer::SetHeight(qreal height)
{
    _height = height;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::SetHeightName(const QString &name)
{
    d->heightName = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief size return size
 * @return size in mm
 */
qreal VContainer::size()
{
    return _size;
}

//---------------------------------------------------------------------------------------------------------------------
QString VContainer::SizeName() const
{
    return d->sizeName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief height return height
 * @return height in pattern units
 */
qreal VContainer::height()
{
    return _height;
}

//---------------------------------------------------------------------------------------------------------------------
QString VContainer::HeightName() const
{
    return d->heightName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with datagObjects return container of gObjects
 * @return pointer on container of gObjects
 */
const QHash<quint32, QSharedPointer<VGObject> > *VContainer::DataGObjects() const
{
    return &d->gObjects;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with dataDetails return container of details
 * @return pointer on container of details
 */
const QHash<quint32, VDetail> *VContainer::DataDetails() const
{
    return &d->details;
}

//---------------------------------------------------------------------------------------------------------------------
const QHash<QString, QSharedPointer<VInternalVariable> > *VContainer::DataVariables() const
{
    return &d->variables;
}

//---------------------------------------------------------------------------------------------------------------------
VContainerData::~VContainerData()
{}

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

quint32 VContainer::_id = 0;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create empty container
 */
VContainer::VContainer()
    :_size(50), sizeName(size_M), _height(176), heightName(height_M),
      gObjects(QHash<quint32, VGObject *>()), variables(QHash<QString, VInternalVariable*> ()),
      details(QHash<quint32, VDetail>())
{
}

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
    setData(data);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create container from another container
 * @param data container
 */
VContainer::VContainer(const VContainer &data)
    :_size(50), sizeName(size_M), _height(176), heightName(height_M),
      gObjects(QHash<quint32, VGObject *>()), variables(QHash<QString, VInternalVariable*> ()),
      details(QHash<quint32, VDetail>())
{
    setData(data);
}

//---------------------------------------------------------------------------------------------------------------------
VContainer::~VContainer()
{
    ClearGObjects();
    ClearVariables();
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
    {
        ClearGObjects();
        const QHash<quint32, VGObject*> *obj = data.DataGObjects();
        SCASSERT(obj != nullptr);
        QHashIterator<quint32, VGObject*> i(*obj);
        while (i.hasNext())
        {
            i.next();
            switch (i.value()->getType())
            {
                case (GOType::Arc):
                    CopyGObject<VArc>(data, i.key());
                    break;
                case (GOType::Point):
                    CopyGObject<VPointF>(data, i.key());
                    break;
                case (GOType::Spline):
                    CopyGObject<VSpline>(data, i.key());
                    break;
                case (GOType::SplinePath):
                    CopyGObject<VSplinePath>(data, i.key());
                    break;
                default:
                    qDebug()<<"Don't know how copy this type.";
                    break;
            }
        }
    }

    {
        ClearVariables();
        const QHash<QString, VInternalVariable*> *vars = data.DataVariables();
        SCASSERT(vars != nullptr);
        QHashIterator<QString, VInternalVariable*> i(*vars);
        while (i.hasNext())
        {
            i.next();
            switch (i.value()->GetType())
            {
                case (VarType::Measurement):
                    CopyVar<VMeasurement>(data, i.key());
                    break;
                case (VarType::Increment):
                    CopyVar<VIncrement>(data, i.key());
                    break;
                case (VarType::LengthLine):
                    CopyVar<VLengthLine>(data, i.key());
                    break;
                case (VarType::LengthSpline):
                    CopyVar<VLengthSpline>(data, i.key());
                    break;
                case (VarType::LengthArc):
                    CopyVar<VLengthArc>(data, i.key());
                    break;
                case (VarType::LineAngle):
                    CopyVar<VLineAngle>(data, i.key());
                    break;
                case (VarType::Unknown):
                default:
                    qDebug()<<"Don't know how copy this type.";
                    break;
            }
        }
    }

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
 * @brief GetDetail return detail by id
 * @param id id of detail
 * @return detail
 */
const VDetail VContainer::GetDetail(quint32 id) const
{
    if (details.contains(id))
    {
        return details.value(id);
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
    SCASSERT(point != nullptr);
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
 * @brief AddLengthArc add length of arc to container
 * @param id id of arc
 */
void VContainer::AddLengthArc(const quint32 &id)
{
    const VArc * arc = GeometricObject<const VArc *>(id);
    AddVariable(arc->name(), new VLengthArc(id, arc));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear data in container. Id will be 0.
 */
void VContainer::Clear()
{
    _id = 0;

    details.clear();
    ClearVariables();
    ClearGObjects();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearObject points, splines, arcs, spline paths will be cleared.
 */
void VContainer::ClearGObjects()
{
    qDeleteAll(gObjects);
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
            if (i.value()->getMode() == Draw::Calculation)
            {
                delete i.value();
                gObjects.remove(i.key());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearVariables(const VarType &type)
{
    if (variables.size()>0)
    {
        if (type == VarType::Unknown)
        {
            qDeleteAll(variables);
            variables.clear();
        }
        else
        {
            QHashIterator<QString, VInternalVariable*> i(variables);
            while (i.hasNext())
            {
                i.next();
                if (i.value()->GetType() == type)
                {
                    delete i.value();
                    variables.remove(i.key());
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
    const VPointF *first = GeometricObject<const VPointF *>(firstPointId);
    const VPointF *second = GeometricObject<const VPointF *>(secondPointId);

    VLengthLine *length = new VLengthLine(first, firstPointId, second, secondPointId);
    AddVariable(length->GetName(), length);

    VLineAngle *angle = new VLineAngle(first, firstPointId, second, secondPointId);
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
qreal VContainer::GetTableValue(const QString &name) const
{
    VVariable *m = GetVariable<VVariable*>(name);
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
    delete variables.value(name);
    variables.remove(name);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VMeasurement*> VContainer::DataMeasurements() const
{
    return DataVar<VMeasurement>(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VIncrement *> VContainer::DataIncrements() const
{
    return DataVar<VIncrement>(VarType::Increment);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VLengthLine *> VContainer::DataLengthLines() const
{
    return DataVar<VLengthLine>(VarType::LengthLine);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VLengthSpline *> VContainer::DataLengthSplines() const
{
    return DataVar<VLengthSpline>(VarType::LengthSpline);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VLengthArc *> VContainer::DataLengthArcs() const
{
    return DataVar<VLengthArc>(VarType::LengthArc);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, VLineAngle *> VContainer::DataAngleLines() const
{
    return DataVar<VLineAngle>(VarType::LineAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VariableExist check if exist variable this same name.
 * @param name name of row
 * @return true if contains
 */
bool VContainer::VariableExist(const QString &name)
{
    return variables.contains(name);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::CopyGObject(const VContainer &data, const quint32 &id)
{
    T *obj = new T(*data.GeometricObject<const T *>(id));
    UpdateGObject(id, obj);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::CopyVar(const VContainer &data, const QString &name)
{
    T *var = new T(*data.GetVariable<T*>(name));
    AddVariable(name, var);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
const QMap<QString, T *> VContainer::DataVar(const VarType &type) const
{
    QHashIterator<QString, VInternalVariable*> i(variables);
    QMap<QString, T*> map;
    //Sorting QHash by id
    while (i.hasNext())
    {
        i.next();
        if(i.value()->GetType() == type)
        {
            T *var = GetVariable<T *>(i.key());
            map.insert(qApp->VarToUser(i.key()), var);
        }
    }
    return map;
}

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

#include "variables.h"
#include "../geometry/vdetail.h"
#include "../geometry/vgobject.h"
#include "../exception/vexceptionbadid.h"
#include "../geometry/vabstractcurve.h"

#include <QCoreApplication>
#include <QHash>

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
    const VDetail      GetDetail(quint32 id) const;
    qreal              GetTableValue(const QString& name) const;
    template <typename T>
    /**
     * @brief GetVariable return varible by name
     * @param name variable's name
     * @return variable
     */
    T GetVariable(QString name) const
    {
        SCASSERT(name.isEmpty()==false);
        if (variables.contains(name))
        {
            try
            {
                T value = dynamic_cast<T>(variables.value(name));
                SCASSERT(value != nullptr);
                return value;
            }
            catch (const std::bad_alloc &)
            {
                throw VExceptionBadId(tr("Can't cast object"), name);
                return nullptr;
            }
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object"), name);
            return nullptr;
        }
    }

    static quint32     getId(){return _id;}
    static quint32     getNextId();
    static void        UpdateId(quint32 newId);

    quint32            AddGObject(VGObject *obj);
    quint32            AddDetail(VDetail detail);
    void               AddLine(const quint32 &firstPointId, const quint32 &secondPointId);

    template <typename TLength>
    /**
     * @brief AddCurveLength add length of curve type to the container
     * @param id id of variables
     * @param parentId if of parent object.
     *
     * Parent id - id of cutting point
     */
    void               AddCurveLength(const quint32 &id, const quint32 &parentId = 0)
    {
        const VAbstractCurve *var = GeometricObject<const VAbstractCurve *>(id);
        AddVariable(var->name(), new TLength(id, parentId, var));
    }

    template <typename T>
    void               AddVariable(const QString& name, T var)
    {
        if(variables.contains(name))
        {
            if(variables.value(name)->GetType() == var->GetType())
            {
                T v = dynamic_cast<T>(variables.value(name));
                SCASSERT(v != nullptr);
                *v = *var;
                delete var;
            }
            else
            {
                throw VExceptionBadId(tr("Can't find object. Type mismatch."), name);
            }
        }
        else
        {
            variables[name] = var;
        }
    }

    void               UpdateGObject(quint32 id, VGObject* obj);
    void               UpdateDetail(quint32 id, const VDetail &detail);

    void               Clear();
    void               ClearGObjects();
    void               ClearCalculationGObjects();
    void               ClearVariables(const VarType &type = VarType::Unknown);
    void               ClearDetails();

    void               SetSize(qreal size);
    void               SetSizeName(const QString &name);
    void               SetHeight(qreal height);
    void               SetHeightName(const QString &name);
    qreal              size() const;
    QString            SizeName()const;
    qreal              height() const;
    QString            HeightName()const;

    bool               VariableExist(const QString& name);

    void               RemoveIncrement(const QString& name);

    const QHash<quint32, VGObject*>          *DataGObjects() const;
    const QHash<quint32, VDetail>            *DataDetails() const;
    const QHash<QString, VInternalVariable*> *DataVariables() const;

    const QMap<QString, VMeasurement *>  DataMeasurements() const;
    const QMap<QString, VIncrement *>    DataIncrements() const;
    const QMap<QString, VLengthLine *>   DataLengthLines() const;
    const QMap<QString, VLengthSpline *> DataLengthSplines() const;
    const QMap<QString, VLengthArc *>    DataLengthArcs() const;
    const QMap<QString, VLineAngle *>    DataAngleLines() const;


private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32 _id;
    qreal          _size;
    QString        sizeName;
    qreal          _height;
    QString        heightName;
    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<quint32, VGObject*> gObjects;

    /**
     * @brief variables container for measurements, increments, lines lengths, lines angles, arcs lengths, curve lengths
     */
    QHash<QString, VInternalVariable*> variables;
    /**
     * @brief details container of details
     */
    QHash<quint32, VDetail> details;

    template <typename key, typename val>
    // cppcheck-suppress functionStatic
    const val GetObject(const QHash<key, val> &obj, key id) const;

    template <typename val>
    void UpdateObject(QHash<quint32, val > &obj, const quint32 &id, val point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val> &obj, val value);

    template <typename T>
    void CopyGObject(const VContainer &data, const quint32 &id);

    template <typename T>
    void CopyVar(const VContainer &data, const QString &name);

    template <typename T>
    const QMap<QString, T*> DataVar(const VarType &type) const;
};

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
 * @brief data container with datagObjects return container of gObjects
 * @return pointer on container of gObjects
 */
inline const QHash<quint32, VGObject *> *VContainer::DataGObjects() const
{
    return &gObjects;
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

//---------------------------------------------------------------------------------------------------------------------
inline const QHash<QString, VInternalVariable *> *VContainer::DataVariables() const
{
    return &variables;
}

#endif // VCONTAINER_H

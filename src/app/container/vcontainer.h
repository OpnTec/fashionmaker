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
#include <QSet>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VContainerData : public QSharedData
{
public:

    VContainerData()
        :sizeName(size_M), heightName(height_M), gObjects(QHash<quint32, QSharedPointer<VGObject> >()),
          variables(QHash<QString, QSharedPointer<VInternalVariable> > ()), details(QHash<quint32, VDetail>())
    {}

    VContainerData(const VContainerData &data)
        :QSharedData(data), sizeName(data.sizeName), heightName(data.heightName), gObjects(data.gObjects),
          variables(data.variables), details(data.details)
    {}

    virtual ~VContainerData() {}

    QString        sizeName;
    QString        heightName;
    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<quint32, QSharedPointer<VGObject> > gObjects;

    /**
     * @brief variables container for measurements, increments, lines lengths, lines angles, arcs lengths, curve lengths
     */
    QHash<QString, QSharedPointer<VInternalVariable>> variables;
    /**
     * @brief details container of details
     */
    QHash<quint32, VDetail> details;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

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
    const QSharedPointer<T> GeometricObject(const quint32 &id) const
    {
        QSharedPointer<VGObject> gObj = QSharedPointer<VGObject>();
        if (d->gObjects.contains(id))
        {
            gObj = d->gObjects.value(id);
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object"), id);
        }

        try
        {
            QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(gObj);
            SCASSERT(obj.isNull() == false);
            return obj;
        }
        catch (const std::bad_alloc &)
        {
            throw VExceptionBadId(tr("Can't cast object"), id);
        }
        return QSharedPointer<T>();
    }

    const QSharedPointer<VGObject> GetGObject(quint32 id) const;
    const VDetail      GetDetail(quint32 id) const;
    qreal              GetTableValue(const QString& name) const;
    template <typename T>
    /**
     * @brief GetVariable return varible by name
     * @param name variable's name
     * @return variable
     */
    QSharedPointer<T> GetVariable(QString name) const
    {
        SCASSERT(name.isEmpty()==false);
        if (d->variables.contains(name))
        {
            try
            {
                QSharedPointer<T> value = qSharedPointerDynamicCast<T>(d->variables.value(name));
                SCASSERT(value.isNull() == false);
                return value;
            }
            catch (const std::bad_alloc &)
            {
                throw VExceptionBadId(tr("Can't cast object"), name);
            }
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object"), name);
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
        const QSharedPointer<VAbstractCurve> var = GeometricObject<VAbstractCurve>(id);
        AddVariable(var->name(), new TLength(id, parentId, var.data()));
    }

    template <typename T>
    void               AddVariable(const QString& name, T *var)
    {
        if (d->variables.contains(name))
        {
            if (d->variables.value(name)->GetType() == var->GetType())
            {
                d->variables[name].clear();
            }
            else
            {
                throw VExceptionBadId(tr("Can't find object. Type mismatch."), name);
            }
        }
        d->variables[name] = QSharedPointer<T>(var);
        uniqueNames.insert(name);
    }

    void               UpdateGObject(quint32 id, VGObject* obj);
    void               UpdateDetail(quint32 id, const VDetail &detail);

    void               Clear();
    void               ClearGObjects();
    void               ClearCalculationGObjects();
    void               ClearVariables(const VarType &type = VarType::Unknown);
    void               ClearDetails();
    static void        ClearUniqueNames();

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

    const QHash<quint32, QSharedPointer<VGObject> >         *DataGObjects() const;
    const QHash<quint32, VDetail>                           *DataDetails() const;
    const QHash<QString, QSharedPointer<VInternalVariable>> *DataVariables() const;

    const QMap<QString, QSharedPointer<VMeasurement> >  DataMeasurements() const;
    const QMap<QString, QSharedPointer<VIncrement> >    DataIncrements() const;
    const QMap<QString, QSharedPointer<VLengthLine> >   DataLengthLines() const;
    const QMap<QString, QSharedPointer<VSplineLength> > DataLengthSplines() const;
    const QMap<QString, QSharedPointer<VArcLength> >    DataLengthArcs() const;
    const QMap<QString, QSharedPointer<VLineAngle> >    DataAngleLines() const;

    static bool        IsUnique(const QString &name);

private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32 _id;
    static qreal   _size;
    static qreal   _height;
    static QSet<const QString> uniqueNames;

    QSharedDataPointer<VContainerData> d;

    template <class T>
    uint qHash( const QSharedPointer<T> &p )
    {
        return qHash( p.data() );
    }

    template <typename key, typename val>
    // cppcheck-suppress functionStatic
    const val GetObject(const QHash<key, val> &obj, key id) const;

    template <typename val>
    void UpdateObject(QHash<quint32, val > &obj, const quint32 &id, val point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val> &obj, val value);

    template <typename T>
    const QMap<QString, QSharedPointer<T> > DataVar(const VarType &type) const;
};

#endif // VCONTAINER_H

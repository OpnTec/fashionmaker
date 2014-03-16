/************************************************************************
 **
 **  @file   vcontainer.cpp
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

#include "vcontainer.h"

#include <QDebug>
#include <QtAlgorithms>

quint32 VContainer::_id = 0;

VContainer::VContainer()
    :_size(50), sizeName("Сг"), _height(176), heightName("P"), gObjects(QHash<quint32, VGObject *>()),
      measurements(QHash<QString, VMeasurement>()), incrementTable(QHash<QString, VIncrementTableRow>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<quint32, VDetail>())
{
}

VContainer &VContainer::operator =(const VContainer &data)
{
    setData(data);
    return *this;
}

VContainer::VContainer(const VContainer &data)
    :_size(50), sizeName("Сг"), _height(176), heightName("P"), gObjects(QHash<quint32, VGObject *>()),
      measurements(QHash<QString, VMeasurement>()), incrementTable(QHash<QString, VIncrementTableRow>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<quint32, VDetail>())
{
    setData(data);
}

VContainer::~VContainer()
{
    qDeleteAll(gObjects);
    gObjects.clear();
}

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
            {
                CopyGObject<VArc>(data, i.key());
                break;
            }
            case (GObject::Point):
            {
                CopyGObject<VPointF>(data, i.key());
                break;
            }
            case (GObject::Spline):
            {
                CopyGObject<VSpline>(data, i.key());
                break;
            }
            case (GObject::SplinePath):
            {
                CopyGObject<VSplinePath>(data, i.key());
                break;
            }
            default:
            qWarning()<<"Don't know how copy this type.";
        }
    }
    measurements = *data.DataMeasurements();
    incrementTable = *data.DataIncrementTable();
    lengthLines = *data.DataLengthLines();
    lineAngles = *data.DataLineAngles();
    lengthSplines = *data.DataLengthSplines();
    lengthArcs = *data.DataLengthArcs();
    details = *data.DataDetails();
}

const VGObject *VContainer::GetGObject(quint32 id)const
{
    return GetObject(gObjects, id);
}

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

const VMeasurement VContainer::GetMeasurement(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(measurements, name);
}

const VIncrementTableRow VContainer::GetIncrementTableRow(const QString& name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(incrementTable, name);
}

qreal VContainer::GetLine(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthLines, name);
}

qreal VContainer::GetLengthArc(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthArcs, name);
}

qreal VContainer::GetLengthSpline(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lengthSplines, name);
}

qreal VContainer::GetLineAngle(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(lineAngles, name);
}

const VDetail VContainer::GetDetail(quint32 id) const
{
    return GetVariable(details, id);
}

quint32 VContainer::AddGObject(VGObject *obj)
{
    return AddObject(gObjects, obj);
}

quint32 VContainer::AddDetail(VDetail detail)
{
    quint32 id = getNextId();
    details[id] = detail;
    return id;
}

void VContainer::AddIncrementTableRow(const QString &name, VIncrementTableRow row)
{
    incrementTable[name] = row;
}

quint32 VContainer::getNextId()
{
    _id++;
    return _id;
}

void VContainer::UpdateId(quint32 newId)
{
    if (newId > _id)
    {
       _id = newId;
    }
}

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

void VContainer::AddLengthSpline(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthSplines[name] = value;
}

void VContainer::AddLengthArc(const quint32 &id)
{
    const VArc * arc = GeometricObject<const VArc *>(id);
    lengthArcs[arc->name()] = fromPixel(arc->GetLength());
}

void VContainer::AddLineAngle(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lineAngles[name] = value;
}

qreal VContainer::GetValueStandardTableRow(const QString& name) const
{
    const VMeasurement m = GetMeasurement(name);
    if (patternType == Pattern::Individual)
    {
        return m.GetValue();
    }
    else
    {
        return m.GetValue(size(), height());
    }
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const
{
    const VIncrementTableRow row =  GetIncrementTableRow(name);
    const qreal k_size    = ( size() - 50.0 ) / 2.0;
    const qreal k_growth  = ( height() - 176.0 ) / 6.0;
    const qreal value = row.getBase() + k_size * row.getKsize() + k_growth * row.getKgrowth();
    return value;
}

void VContainer::Clear()
{
    _id = 0;
    measurements.clear();
    incrementTable.clear();
    lengthLines.clear();
    lengthArcs.clear();
    lineAngles.clear();
    details.clear();
    lengthSplines.clear();
    ClearGObjects();
}

void VContainer::ClearGObjects()
{
    if (gObjects.size()>0)
    {
        qDeleteAll(gObjects);
    }
    gObjects.clear();
}

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
    if (incrementTable.contains(name))
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

void VContainer::AddLine(const quint32 &firstPointId, const quint32 &secondPointId)
{
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    const VPointF *first = GeometricObject<const VPointF *>(firstPointId);
    const VPointF *second = GeometricObject<const VPointF *>(secondPointId);
    AddLengthLine(nameLine, fromPixel(QLineF(first->toQPointF(), second->toQPointF()).length()));
    nameLine = GetNameLineAngle(firstPointId, secondPointId);
    AddLineAngle(nameLine, QLineF(first->toQPointF(), second->toQPointF()).angle());
}

template <typename key, typename val>
quint32 VContainer::AddObject(QHash<key, val> &obj, val value)
{
    Q_CHECK_PTR(value);
    quint32 id = getNextId();
    value->setId(id);
    obj[id] = value;
    return id;
}

QString VContainer::GetNameLine(const quint32 &firstPoint, const quint32 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString("Line_%1_%2").arg(first->name(), second->name());
}

QString VContainer::GetNameLineAngle(const quint32 &firstPoint, const quint32 &secondPoint) const
{
    const VPointF *first = GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = GeometricObject<const VPointF *>(secondPoint);

    return QString("AngleLine_%1_%2").arg(first->name(), second->name());
}

void VContainer::UpdateGObject(quint32 id, VGObject* obj)
{
    UpdateObject(gObjects, id, obj);
}

void VContainer::UpdateDetail(quint32 id, const VDetail &detail)
{
    Q_ASSERT_X(id > 0, Q_FUNC_INFO, "id <= 0");
    details[id] = detail;
    UpdateId(id);
}

void VContainer::AddLengthLine(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lengthLines[name] = value;
}

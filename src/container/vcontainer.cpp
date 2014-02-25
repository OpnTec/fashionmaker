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
    :base(QHash<QString, qint32>()),  gObjects(QHash<quint32, VGObject *>()),
      standardTable(QHash<QString, VStandardTableRow>()), incrementTable(QHash<QString, VIncrementTableRow>()),
      lengthLines(QHash<QString, qreal>()), lineAngles(QHash<QString, qreal>()), lengthSplines(QHash<QString, qreal>()),
      lengthArcs(QHash<QString, qreal>()), details(QHash<quint32, VDetail>())
{
    SetSize(500);
    SetGrowth(1760);
    CreateManTableIGroup ();
}

VContainer &VContainer::operator =(const VContainer &data)
{
    setData(data);
    return *this;
}

VContainer::VContainer(const VContainer &data)
    :base(QHash<QString, qint32>()), gObjects(QHash<quint32, VGObject *>()),
      standardTable(QHash<QString, VStandardTableRow>()), incrementTable(QHash<QString, VIncrementTableRow>()),
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
    base = *data.DataBase();

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
    standardTable = *data.DataStandardTable();
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

const VStandardTableRow VContainer::GetStandardTableCell(const QString &name) const
{
    Q_ASSERT(name.isEmpty()==false);
    return GetVariable(standardTable, name);
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
    lengthArcs[arc->name()] = toMM(arc->GetLength());
}

void VContainer::AddLineAngle(const QString &name, const qreal &value)
{
    Q_ASSERT(name.isEmpty() == false);
    lineAngles[name] = value;
}

qreal VContainer::GetValueStandardTableCell(const QString& name) const
{
    VStandardTableRow cell =  GetStandardTableCell(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.GetBase() + k_size*cell.GetKsize() + k_growth*cell.GetKgrowth();
    return value;
}

qreal VContainer::GetValueIncrementTableRow(const QString& name) const
{
    VIncrementTableRow cell =  GetIncrementTableRow(name);
    qreal k_size    = ( static_cast<qreal> (size()/10.0) - 50.0 ) / 2.0;
    qreal k_growth  = ( static_cast<qreal> (growth()/10.0) - 176.0 ) / 6.0;
    qreal value = cell.getBase() + k_size*cell.getKsize() + k_growth*cell.getKgrowth();
    return value;
}

void VContainer::Clear()
{
    _id = 0;
    standardTable.clear();
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
            if (i.value()->getMode() == Draw::Calculation)
            {
                delete i.value();
                gObjects.remove(i.key());
            }
        }
    }
}

qreal VContainer::FindVar(const QString &name, bool *ok)const
{
    if (base.contains(name))
    {
        *ok = true;
        return base.value(name);
    }

    if (standardTable.contains(name))
    {
        *ok = true;
        return GetValueStandardTableCell(name);
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
    AddLengthLine(nameLine, toMM(QLineF(first->toQPointF(), second->toQPointF()).length()));
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

void VContainer::CreateManTableIGroup ()
{
    AddStandardTableCell("Pkor", VStandardTableRow(84, 0, 3));
    AddStandardTableCell("Vtos", VStandardTableRow(1450, 2, 51));
    AddStandardTableCell("Vtosh", VStandardTableRow(1506, 2, 54));
    AddStandardTableCell("Vpt", VStandardTableRow(1438, 3, 52));
    AddStandardTableCell("Vst", VStandardTableRow(1257, -1, 49));
    AddStandardTableCell("Vlt", VStandardTableRow(1102, 0, 43));
    AddStandardTableCell("Vk", VStandardTableRow(503, 0, 22));
    AddStandardTableCell("Vsht", VStandardTableRow(1522, 2, 54));
    AddStandardTableCell("Vzy", VStandardTableRow(1328, 0, 49));
    AddStandardTableCell("Vlop", VStandardTableRow(1320, 0, 49));
    AddStandardTableCell("Vps", VStandardTableRow(811, -1, 36));
    AddStandardTableCell("Ssh", VStandardTableRow(202, 4, 1));
    AddStandardTableCell("SgI", VStandardTableRow(517, 18, 2));
    AddStandardTableCell("SgII", VStandardTableRow(522, 19, 1));
    AddStandardTableCell("SgIII", VStandardTableRow(500, 20, 0));
    AddStandardTableCell("SbI", VStandardTableRow(482, 12, 6));
    AddStandardTableCell("Obed", VStandardTableRow(566, 18, 6));
    AddStandardTableCell("Ok", VStandardTableRow(386, 8, 8));
    AddStandardTableCell("Oi", VStandardTableRow(380, 8, 6));
    AddStandardTableCell("Osch", VStandardTableRow(234, 4, 4));
    AddStandardTableCell("Dsb", VStandardTableRow(1120, 0, 44));
    AddStandardTableCell("Dsp", VStandardTableRow(1110, 0, 43));
    AddStandardTableCell("Dn", VStandardTableRow(826, -3, 37));
    AddStandardTableCell("Dps", VStandardTableRow(316, 4, 7));
    AddStandardTableCell("Dpob", VStandardTableRow(783, 14, 15));
    AddStandardTableCell("Ds", VStandardTableRow(260, 1, 6));
    AddStandardTableCell("Op", VStandardTableRow(316, 12, 0));
    AddStandardTableCell("Ozap", VStandardTableRow(180, 4, 0));
    AddStandardTableCell("Pkis", VStandardTableRow(250, 4, 0));
    AddStandardTableCell("SHp", VStandardTableRow(160, 1, 4));
    AddStandardTableCell("Dlych", VStandardTableRow(500, 2, 15));
    AddStandardTableCell("Dzap", VStandardTableRow(768, 2, 24));
    AddStandardTableCell("DIIIp", VStandardTableRow(970, 2, 29));
    AddStandardTableCell("Vprp", VStandardTableRow(214, 3, 3));
    AddStandardTableCell("Vg", VStandardTableRow(262, 8, 3));
    AddStandardTableCell("Dtp", VStandardTableRow(460, 7, 9));
    AddStandardTableCell("Dp", VStandardTableRow(355, 5, 5));
    AddStandardTableCell("Vprz", VStandardTableRow(208, 3, 5));
    AddStandardTableCell("Dts", VStandardTableRow(438, 2, 10));
    AddStandardTableCell("DtsI", VStandardTableRow(469, 2, 10));
    AddStandardTableCell("Dvcht", VStandardTableRow(929, 9, 19));
    AddStandardTableCell("SHg", VStandardTableRow(370, 14, 4));
    AddStandardTableCell("Cg", VStandardTableRow(224, 6, 0));
    AddStandardTableCell("SHs", VStandardTableRow(416, 10, 2));
    AddStandardTableCell("dpzr", VStandardTableRow(121, 6, 0));
    AddStandardTableCell("Ogol", VStandardTableRow(576, 4, 4));
    AddStandardTableCell("Ssh1", VStandardTableRow(205, 5, 0));

    //TODO Posible duplicate. Need check.
    //AddStandardTableCell("St", VStandardTableRow(410, 20, 0));
    AddStandardTableCell("St", VStandardTableRow(390, 20, 0));

    AddStandardTableCell("Drzap", VStandardTableRow(594, 3, 19));
    AddStandardTableCell("DbII", VStandardTableRow(1020, 0, 44));

    //TODO Posible duplicate. Need check.
    //AddStandardTableCell("Sb", VStandardTableRow(504, 15, 4));
    AddStandardTableCell("Sb", VStandardTableRow(492, 15, 5));
}

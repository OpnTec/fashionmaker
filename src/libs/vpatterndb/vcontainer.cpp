/************************************************************************
 **
 **  @file   vcontainer.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include <limits.h>
#include <QVector>
#include <QtDebug>
#include <QUuid>
#include <QLoggingCategory>

#include "../ifc/exception/vexception.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/vabstractapplication.h"
#include "variables/varcradius.h"
#include "variables/vcurveangle.h"
#include "variables/vcurvelength.h"
#include "variables/vcurveclength.h"
#include "variables/vincrement.h"
#include "variables/vlineangle.h"
#include "variables/vlinelength.h"
#include "variables/vmeasurement.h"
#include "variables/vvariable.h"
#include "vtranslatevars.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vCon, "v.container")

QT_WARNING_POP

QMap<QString, quint32> VContainer::_id = QMap<QString, quint32>();
QMap<QString, qreal> VContainer::_size = QMap<QString, qreal>();
QMap<QString, qreal> VContainer::_height = QMap<QString, qreal>();
QMap<QString, QSet<QString>> VContainer::uniqueNames = QMap<QString, QSet<QString>>();
QMap<QString, quint32> VContainer::copyCounter = QMap<QString, quint32>();

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create empty container
 */
VContainer::VContainer(const VTranslateVars *trVars, const Unit *patternUnit, const QString &nspace)
    :d(new VContainerData(trVars, patternUnit, nspace))
{
    if (nspace.isEmpty())
    {
        qFatal("Namesapce is empty.");
    }

    if (VContainer::_id.contains(nspace))
    {
        qFatal("Namespace is not unique.");
    }

    if (not _id.contains(d->nspace))
    {
        _id[d->nspace] = NULL_ID;
    }

    if (not _size.contains(d->nspace))
    {
        _size[d->nspace] = 50;
    }

    if (not _height.contains(d->nspace))
    {
        _height[d->nspace] = 176;
    }

    if (not uniqueNames.contains(d->nspace))
    {
        uniqueNames[d->nspace] = QSet<QString>();
    }

    if (not copyCounter.contains(d->nspace))
    {
        copyCounter[d->nspace] = 1;
    }
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
    d = data.d;
    ++copyCounter[d->nspace];
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VContainer::VContainer(const VContainer &&data) Q_DECL_NOTHROW
    :d(data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VContainer &VContainer::operator=(VContainer &&data) Q_DECL_NOTHROW
{
    std::swap(d, data.d);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VContainer create container from another container
 * @param data container
 */
VContainer::VContainer(const VContainer &data)
    :d(data.d)
{
    ++copyCounter[d->nspace];
}

//---------------------------------------------------------------------------------------------------------------------
VContainer::~VContainer()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VContainer::UniqueNamespace()
{
    QString candidate;
    do
    {
        candidate = QUuid::createUuid().toString();
    }
    while(_size.contains(candidate));

    return candidate;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearNamespace(const QString &nspace)
{
    _id.remove(nspace);
    _size.remove(nspace);
    _height.remove(nspace);
    uniqueNames.remove(nspace);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetGObject returns a point by id
 * @param id id of point
 * @return point
 */
// cppcheck-suppress unusedFunction
const QSharedPointer<VGObject> VContainer::GetGObject(quint32 id) const
{
    if (d->calculationObjects.contains(id))
    {
        return d->calculationObjects.value(id);
    }
    else if (d->modelingObjects->contains(id))
    {
        return d->modelingObjects->value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
const QSharedPointer<VGObject> VContainer::GetFakeGObject(quint32 id)
{
    VGObject *obj = new VGObject();
    obj->setId(id);
    QSharedPointer<VGObject> pointer(obj);
    return pointer;
}

//---------------------------------------------------------------------------------------------------------------------
VPiece VContainer::GetPiece(quint32 id) const
{
    if (d->pieces->contains(id))
    {
        return d->pieces->value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath VContainer::GetPiecePath(quint32 id) const
{
    if (d->piecePaths->contains(id))
    {
        return d->piecePaths->value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::GetPieceForPiecePath(quint32 id) const
{
    auto i = d->pieces->constBegin();
    while (i != d->pieces->constEnd())
    {
        if (i.value().GetInternalPaths().contains(id))
        {
            return i.key();
        }
        ++i;
    }

    return NULL_ID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddGObject add new GObject to container
 * @param obj new object
 * @return return id of new object in container
 */
quint32 VContainer::AddGObject(VGObject *obj)
{
    SCASSERT(obj != nullptr)
    QSharedPointer<VGObject> pointer(obj);
    return AddGObject(pointer);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::AddGObject(const QSharedPointer<VGObject> &obj)
{
    SCASSERT(not obj.isNull())

    if (obj->getMode() == Draw::Layout)
    {
        qWarning("Can't add an object with mode 'Layout'");
        return NULL_ID;
    }

    uniqueNames[d->nspace].insert(obj->name());
    const quint32 id = getNextId();
    obj->setId(id);

    if (obj->getMode() == Draw::Calculation)
    {
        d->calculationObjects.insert(id, obj);
    }
    else if (obj->getMode() == Draw::Modeling)
    {
        d->modelingObjects->insert(id, obj);
    }

    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::AddPiece(const VPiece &detail)
{
    const quint32 id = getNextId();
    d->pieces->insert(id, detail);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::AddPiecePath(const VPiecePath &path)
{
    const quint32 id = getNextId();
    d->piecePaths->insert(id, path);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::getId() const
{
    return _id.value(d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNextId generate next unique id
 * @return next unique id
 */
quint32 VContainer::getNextId() const
{
    //TODO. Current count of ids are very big and allow us save time before someone will reach its max value.
    //Better way, of cource, is to seek free ids inside the set of values and reuse them.
    //But for now better to keep it as it is now.
    if (_id.value(d->nspace) == UINT_MAX)
    {
        qCritical()<<(tr("Number of free id exhausted."));
    }
    _id[d->nspace]++;
    return _id.value(d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::UpdateId(quint32 newId, const QString &nspace)
{
    if (_id.contains(nspace))
    {
        if (newId > _id.value(nspace))
        {
           _id[nspace] = newId;
        }
    }
    else
    {
        throw VException(QStringLiteral("Unknown namespace"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateId update id. If new id bigger when current save new like current.
 * @param newId id
 */
void VContainer::UpdateId(quint32 newId) const
{
    VContainer::UpdateId(newId, d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear data in container. Id will be 0.
 */
void VContainer::Clear()
{
    qCDebug(vCon, "Clearing container data.");
    _id[d->nspace] = NULL_ID;

    d->pieces->clear();
    d->piecePaths->clear();
    ClearVariables();
    ClearGObjects();
    ClearUniqueNames();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearForFullParse()
{
    qCDebug(vCon, "Clearing container data for full parse.");
    _id[d->nspace] = NULL_ID;

    d->pieces->clear();
    d->piecePaths->clear();
    Q_STATIC_ASSERT_X(static_cast<int>(VarType::Unknown) == 9, "Check that you used all types");
    ClearVariables(QVector<VarType>({VarType::Increment,
                                     VarType::IncrementSeparator,
                                     VarType::LineAngle,
                                     VarType::LineLength,
                                     VarType::CurveLength,
                                     VarType::CurveCLength,
                                     VarType::ArcRadius,
                                     VarType::CurveAngle}));
    ClearGObjects();
    ClearUniqueNames();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearObject points, splines, arcs, spline paths will be cleared.
 */
void VContainer::ClearGObjects()
{
    d->calculationObjects.clear();
    d->modelingObjects->clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearCalculationGObjects()
{
    d->calculationObjects.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearVariables(const VarType &type)
{
    ClearVariables(QVector<VarType>({type}));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearVariables(const QVector<VarType> &types)
{
    if (not d->variables.isEmpty()) //-V807
    {
        if (types.isEmpty() || types.contains(VarType::Unknown))
        {
            d->variables.clear();
        }
        else
        {
            QHash<QString, QSharedPointer<VInternalVariable> >::iterator i;
            for (i = d->variables.begin(); i != d->variables.end();)
            {
                if (types.contains(i.value()->GetType()))
                {
                    i = d->variables.erase(i);
                }
                else
                {
                    ++i;
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

    AddVariable(new VLengthLine(first.data(), firstPointId, second.data(), secondPointId, *GetPatternUnit()));
    AddVariable(new VLineAngle(first.data(), firstPointId, second.data(), secondPointId));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddArc(const QSharedPointer<VAbstractCurve> &arc, const quint32 &id, const quint32 &parentId)
{
    AddCurve(arc, id, parentId);

    if (arc->getType() == GOType::Arc)
    {
        const QSharedPointer<VArc> casted = arc.staticCast<VArc>();

        AddVariable(new VArcRadius(id, parentId, casted.data(), *GetPatternUnit()));
    }
    else if (arc->getType() == GOType::EllipticalArc)
    {
        const QSharedPointer<VEllipticalArc> casted = arc.staticCast<VEllipticalArc>();

        AddVariable(new VArcRadius(id, parentId, casted.data(), 1, *GetPatternUnit()));
        AddVariable(new VArcRadius(id, parentId, casted.data(), 2, *GetPatternUnit()));
        AddVariable(new VEllipticalArcRotation(id, parentId, casted.data()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddCurve(const QSharedPointer<VAbstractCurve> &curve, const quint32 &id, quint32 parentId)
{
    const GOType curveType = curve->getType();
    if (curveType != GOType::Spline      && curveType != GOType::SplinePath &&
        curveType != GOType::CubicBezier && curveType != GOType::CubicBezierPath &&
        curveType != GOType::Arc         && curveType != GOType::EllipticalArc)
    {
        throw VException(tr("Can't create a curve with type '%1'").arg(static_cast<int>(curveType)));
    }

    AddVariable(new VCurveLength(id, parentId, curve.data(), *GetPatternUnit()));
    AddVariable(new VCurveAngle(id, parentId, curve.data(), CurveAngle::StartAngle));
    AddVariable(new VCurveAngle(id, parentId, curve.data(), CurveAngle::EndAngle));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddSpline(const QSharedPointer<VAbstractBezier> &curve, quint32 id, quint32 parentId)
{
    AddCurve(curve, id, parentId);
    AddVariable(new VCurveCLength(id, parentId, curve.data(), CurveCLength::C1, *GetPatternUnit()));
    AddVariable(new VCurveCLength(id, parentId, curve.data(), CurveCLength::C2, *GetPatternUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddCurveWithSegments(const QSharedPointer<VAbstractCubicBezierPath> &curve, const quint32 &id,
                                      quint32 parentId)
{
    AddSpline(curve, id, parentId);

    for (qint32 i = 1; i <= curve->CountSubSpl(); ++i)
    {
        const VSpline spl = curve->GetSpline(i);

        AddVariable(new VCurveLength(id, parentId, curve->name(), spl, *GetPatternUnit(), i));
        AddVariable(new VCurveAngle(id, parentId, curve->name(), spl, CurveAngle::StartAngle, i));
        AddVariable(new VCurveAngle(id, parentId, curve->name(), spl, CurveAngle::EndAngle, i));
        AddVariable(new VCurveCLength(id, parentId, curve->name(), spl, CurveCLength::C1, *GetPatternUnit(), i));
        AddVariable(new VCurveCLength(id, parentId, curve->name(), spl, CurveCLength::C2, *GetPatternUnit(), i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::RemoveVariable(const QString &name)
{
    d->variables.remove(name);
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::RemovePiece(quint32 id)
{
    d->pieces->remove(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::UpdatePiece(quint32 id, const VPiece &detail)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0"); //-V654 //-V712
    d->pieces->insert(id, detail);
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::UpdatePiecePath(quint32 id, const VPiecePath &path)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0"); //-V654 //-V712
    d->piecePaths->insert(id, path);
    UpdateId(id);
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
const QMap<QString, QSharedPointer<VIncrement> > VContainer::DataIncrementsWithSeparators() const
{
    QMap<QString, QSharedPointer<VIncrement> > increments = DataVar<VIncrement>(VarType::Increment);
    QMap<QString, QSharedPointer<VIncrement> > separators = DataVar<VIncrement>(VarType::IncrementSeparator);

    QMap<QString, QSharedPointer<VIncrement>>::const_iterator i = separators.constBegin();
    while (i != separators.constEnd())
    {
        increments.insert(i.key(), i.value());
        ++i;
    }

    return increments;
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VLengthLine> > VContainer::DataLengthLines() const
{
    return DataVar<VLengthLine>(VarType::LineLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VCurveLength> > VContainer::DataLengthCurves() const
{
    return DataVar<VCurveLength>(VarType::CurveLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VCurveCLength> > VContainer::DataCurvesCLength() const
{
    return DataVar<VCurveCLength>(VarType::CurveCLength);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VLineAngle> > VContainer::DataAngleLines() const
{
    return DataVar<VLineAngle>(VarType::LineAngle);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VArcRadius> > VContainer::DataRadiusesArcs() const
{
    return DataVar<VArcRadius>(VarType::ArcRadius);
}

//---------------------------------------------------------------------------------------------------------------------
const QMap<QString, QSharedPointer<VCurveAngle> > VContainer::DataAnglesCurves() const
{
    return DataVar<VCurveAngle>(VarType::CurveAngle);
}

//---------------------------------------------------------------------------------------------------------------------
bool VContainer::IsUnique(const QString &name) const
{
    return VContainer::IsUnique(name, d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
bool VContainer::IsUnique(const QString &name, const QString &nspace)
{
    if (uniqueNames.contains(nspace))
    {
        return (!uniqueNames.value(nspace).contains(name) && !builInFunctions.contains(name));
    }
    else
    {
        throw VException(QStringLiteral("Unknown namespace"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VContainer::AllUniqueNames() const
{
    return AllUniqueNames(d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VContainer::AllUniqueNames(const QString &nspace)
{
    if (uniqueNames.contains(nspace))
    {
        QStringList names = builInFunctions;
        names.append(uniqueNames.value(nspace).values());
        return names;
    }
    else
    {
        throw VException(QStringLiteral("Unknown namespace"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
const Unit *VContainer::GetPatternUnit() const
{
    return d->patternUnit;
}

//---------------------------------------------------------------------------------------------------------------------
const VTranslateVars *VContainer::GetTrVars() const
{
    return d->trVars;
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
            map.insert(d->trVars->VarToUser(i.key()), var);
        }
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearUniqueNames() const
{
    uniqueNames[d->nspace].clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearUniqueIncrementNames() const
{
    const QList<QString> list = uniqueNames.value(d->nspace).values();
    ClearUniqueNames();

    for(auto &name : list)
    {
        if (not name.startsWith('#'))
        {
            uniqueNames[d->nspace].insert(name);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSize set value of size
 * @param size value of size
 */
void VContainer::SetSize(qreal size) const
{
    _size[d->nspace] = size;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetGrowth set value of growth
 * @param height value of height
 */
void VContainer::SetHeight(qreal height) const
{
    _height[d->nspace] = height;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief size return size
 * @return size in mm
 */
qreal VContainer::size() const
{
    return VContainer::size(d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VContainer::size(const QString &nspace)
{
    if (_size.contains(nspace))
    {
        return _size.value(nspace);
    }
    else
    {
        throw VException(QStringLiteral("Unknown namespace"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief height return height
 * @return height in pattern units
 */
qreal VContainer::height() const
{
    return VContainer::height(d->nspace);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VContainer::height(const QString &nspace)
{
    if (_height.contains(nspace))
    {
        return _height.value(nspace);
    }
    else
    {
        throw VException(QStringLiteral("Unknown namespace"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with datagObjects return container of gObjects
 * @return pointer on container of gObjects
 */
const QHash<quint32, QSharedPointer<VGObject> > *VContainer::CalculationGObjects() const
{
    return &d->calculationObjects;
}

//---------------------------------------------------------------------------------------------------------------------
const QHash<quint32, VPiece> *VContainer::DataPieces() const
{
    return d->pieces.data();
}

//---------------------------------------------------------------------------------------------------------------------
const QHash<QString, QSharedPointer<VInternalVariable> > *VContainer::DataVariables() const
{
    return &d->variables;
}

//---------------------------------------------------------------------------------------------------------------------
VContainerData::~VContainerData()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if (ref.loadRelaxed() == 0)
#else
    if (ref.load() == 0)
#endif
    {
        --VContainer::copyCounter[nspace];
    }

    if (VContainer::copyCounter.value(nspace) == 0)
    {
        VContainer::ClearNamespace(nspace);
    }
}

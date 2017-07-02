/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vtoolmove.h"

#include <limits.h>
#include <qiterator.h>
#include <QColor>
#include <QDomNode>
#include <QDomNodeList>
#include <QMapIterator>
#include <QPoint>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QUndoStack>
#include <new>

#include "../../../dialogs/tools/dialogtool.h"
#include "../../../dialogs/tools/dialogmove.h"
#include "../../../visualization/line/operation/vistoolmove.h"
#include "../../../visualization/visualization.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/logging.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../ifc/ifcdef.h"
#include "../ifc/exception/vexception.h"
#include "../vwidgets/vabstractsimple.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../vabstracttool.h"
#include "../../vdatatool.h"
#include "../vdrawtool.h"

template <class T> class QSharedPointer;

const QString VToolMove::ToolType = QStringLiteral("moving");

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogMove> dialogTool = m_dialog.objectCast<DialogMove>();
    SCASSERT(not dialogTool.isNull())
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetLength(formulaLength);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove *VToolMove::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                             VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogMove> dialogTool = dialog.objectCast<DialogMove>();
    SCASSERT(not dialogTool.isNull())
    QString angle = dialogTool->GetAngle();
    QString length = dialogTool->GetLength();
    const QString suffix = dialogTool->GetSuffix();
    const QVector<quint32> source = dialogTool->GetObjects();
    VToolMove* operation = Create(0, angle, length, suffix, source, QVector<DestinationItem>(), scene, doc, data,
                                    Document::FullParse, Source::FromGui);
    if (operation != nullptr)
    {
        operation->m_dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove *VToolMove::Create(quint32 _id, QString &formulaAngle, QString &formulaLength,
                                 const QString &suffix, const QVector<quint32> &source,
                                 const QVector<DestinationItem> &destination, VMainGraphicsScene *scene,
                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                 const Source &typeCreation)
{
    qreal calcAngle = 0;
    qreal calcLength = 0;

    calcAngle = CheckFormula(_id, formulaAngle, data);
    calcLength = qApp->toPixel(CheckFormula(_id, formulaLength, data));

    QVector<DestinationItem> dest = destination;

    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        dest.clear();// Try to avoid mistake, value must be empty

        id = VContainer::getNextId();//Just reserve id for tool

        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    dest.append(CreatePoint(id, idObject, calcAngle, calcLength, suffix, data));
                    break;
                case GOType::Arc:
                    dest.append(CreateArc<VArc>(id, idObject, calcAngle, calcLength, suffix, data));
                    break;
                case GOType::EllipticalArc:
                    dest.append(CreateArc<VEllipticalArc>(id, idObject, calcAngle, calcLength, suffix, data));
                    break;
                case GOType::Spline:
                    dest.append(CreateCurve<VSpline>(id, idObject, calcAngle, calcLength, suffix, data));
                    break;
                case GOType::SplinePath:
                    dest.append(CreateCurveWithSegments<VSplinePath>(id, idObject, calcAngle, calcLength, suffix,
                                                                     data));
                    break;
                case GOType::CubicBezier:
                    dest.append(CreateCurve<VCubicBezier>(id, idObject, calcAngle, calcLength, suffix, data));
                    break;
                case GOType::CubicBezierPath:
                    dest.append(CreateCurveWithSegments<VCubicBezierPath>(id, idObject, calcAngle, calcLength, suffix,
                                                                          data));
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
    }
    else
    {
        for (int i = 0; i < source.size(); ++i)
        {
            const quint32 idObject = source.at(i);
            const QSharedPointer<VGObject> obj = data->GetGObject(idObject);

            // This check helps to find missed objects in the switch
            Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
            switch(static_cast<GOType>(obj->getType()))
            {
                case GOType::Point:
                    UpdatePoint(id, idObject, calcAngle, calcLength, suffix, data, dest.at(i).id, dest.at(i).mx,
                                dest.at(i).my);
                    break;
                case GOType::Arc:
                    UpdateArc<VArc>(id, idObject, calcAngle, calcLength, suffix, data, dest.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    UpdateArc<VEllipticalArc>(id, idObject, calcAngle, calcLength, suffix, data, dest.at(i).id);
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(id, idObject, calcAngle, calcLength, suffix, data, dest.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(id, idObject, calcAngle, calcLength, suffix, data,
                                                         dest.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(id, idObject, calcAngle, calcLength, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(id, idObject, calcAngle, calcLength, suffix, data,
                                                              dest.at(i).id);
                    break;
                case GOType::Unknown:
                    break;
            }
QT_WARNING_POP
        }
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::Move, doc);
        VToolMove *tool = new VToolMove(doc, data, id, formulaAngle, formulaLength, suffix, source, dest,
                                            typeCreation);
        scene->addItem(tool);
        InitOperationToolConnections(scene, tool);
        VAbstractPattern::AddTool(id, tool);
        for (int i = 0; i < source.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(source.at(i))->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolMove::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolMove::GetFormulaLength() const
{
    VFormula fLength(formulaLength, getData());
    fLength.setCheckZero(true);
    fLength.setToolId(id);
    fLength.setPostfix(UnitsToStr(qApp->patternUnit()));
    return fLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetFormulaLength(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaLength = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolMove>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolMove *visual = qobject_cast<VisToolMove *>(vis);
        SCASSERT(visual != nullptr)

        visual->SetObjects(source);
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->SetLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SaveDialog(QDomElement &domElement)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogMove> dialogTool = m_dialog.objectCast<DialogMove>();
    SCASSERT(not dialogTool.isNull())

    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    QString length = dialogTool->GetLength();
    doc->SetAttribute(domElement, AttrLength, length);
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::ReadToolAttributes(const QDomElement &domElement)
{
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, "0");
    formulaLength = doc->GetParametrString(domElement, AttrLength, "0");
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrSuffix, suffix);

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogMove>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolMove::VToolMove(VAbstractPattern *doc, VContainer *data, quint32 id,
                         const QString &formulaAngle, const QString &formulaLength, const QString &suffix,
                         const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                         const Source &typeCreation, QGraphicsItem *parent)
    : VAbstractOperation(doc, data, id, suffix, source, destination, parent),
      formulaAngle(formulaAngle),
      formulaLength(formulaLength)
{
    InitOperatedObjects();
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VToolMove::CreatePoint(quint32 idTool, quint32 idItem, qreal angle,
                                         qreal length, const QString &suffix, VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF moved = point->Move(length, angle, suffix);
    moved.setIdObject(idTool);

    DestinationItem item;
    item.mx = moved.mx();
    item.my = moved.my();
    item.id = data->AddGObject(new VPointF(moved));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, const QString &suffix,
                                       VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, suffix, data);
    data->AddArc(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolMove::UpdatePoint(quint32 idTool, quint32 idItem, qreal angle, qreal length, const QString &suffix,
                              VContainer *data, quint32 id, qreal mx, qreal my)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF moved = point->Move(length, angle, suffix);
    moved.setIdObject(idTool);
    moved.setMx(mx);
    moved.setMy(my);
    data->UpdateGObject(id, new VPointF(moved));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateArc(quint32 idTool, quint32 idItem, qreal angle, qreal length, const QString &suffix,
                            VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, suffix, data, id);
    data->AddArc(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateItem(quint32 idTool, quint32 idItem, qreal angle,
                                        qreal length, const QString &suffix, VContainer *data)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item moved = i->Move(length, angle, suffix);
    moved.setIdObject(idTool);

    DestinationItem item;
    item.mx = INT_MAX;
    item.my = INT_MAX;
    item.id = data->AddGObject(new Item(moved));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length,
                                         const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, suffix, data);
    data->AddSpline(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolMove::CreateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle,
                                                     qreal length, const QString &suffix,
                                                     VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, angle, length, suffix, data);
    data->AddCurveWithSegments(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateItem(quint32 idTool, quint32 idItem, qreal angle, qreal length, const QString &suffix,
                             VContainer *data, quint32 id)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item moved = i->Move(length, angle, suffix);
    moved.setIdObject(idTool);
    data->UpdateGObject(id, new Item(moved));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateCurve(quint32 idTool, quint32 idItem, qreal angle, qreal length, const QString &suffix,
                              VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, suffix, data, id);
    data->AddSpline(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolMove::UpdateCurveWithSegments(quint32 idTool, quint32 idItem, qreal angle, qreal length,
                                          const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, angle, length, suffix, data, id);
    data->AddCurveWithSegments(data->GeometricObject<Item>(id), id);
}

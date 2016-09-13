/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#include "vtoolflippingbyline.h"

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

#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../dialogs/tools/dialogflippingbyline.h"
#include "../../../../undocommands/label/rotationmovelabel.h"
#include "../../../../visualization/line/operation/vistoolflippingbyline.h"
#include "../../../../visualization/visualization.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
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
#include "../../../vabstracttool.h"
#include "../../../vdatatool.h"
#include "../../vdrawtool.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
template <class T> class QSharedPointer;

const QString VToolFlippingByLine::ToolType = QStringLiteral("flippingByLine");

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine::~VToolFlippingByLine()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->SetFirstLinePointId(m_firstLinePointId);
    dialogTool->SetSecondLinePointId(m_secondLinePointId);
    dialogTool->SetSuffix(suffix);
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                 VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 firstLinePointId = dialogTool->GetFirstLinePointId();
    const quint32 secondLinePointId = dialogTool->GetSecondLinePointId();
    const QString suffix = dialogTool->GetSuffix();
    const QVector<quint32> source = dialogTool->GetObjects();
    VToolFlippingByLine* operation = Create(0, firstLinePointId, secondLinePointId, suffix, source,
                                            QVector<DestinationItem>(), scene, doc, data, Document::FullParse,
                                            Source::FromGui);
    if (operation != nullptr)
    {
        operation->dialog = dialogTool;
    }
    return operation;
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine *VToolFlippingByLine::Create(const quint32 _id, quint32 firstLinePointId, quint32 secondLinePointId,
                                                 const QString &suffix, const QVector<quint32> &source,
                                                 const QVector<DestinationItem> &destination, VMainGraphicsScene *scene,
                                                 VAbstractPattern *doc, VContainer *data, const Document &parse,
                                                 const Source &typeCreation)
{
    const auto firstPoint = *data->GeometricObject<VPointF>(firstLinePointId);
    const QPointF fPoint = firstPoint;

    const auto secondPoint = *data->GeometricObject<VPointF>(secondLinePointId);
    const QPointF sPoint = secondPoint;

    QVector<DestinationItem> dest = destination;

    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        dest.clear();// Try to avoid mistake, value must be empty

        id = data->getNextId();//Just reserve id for tool

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
                    dest.append(CreatePoint(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::Arc:
                    dest.append(CreateArc(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::EllipticalArc:
                    //dest.append(CreateItem<VEllipticalArc>(id, idObject, fPoint, sPoint, suffix));
                    break;
                case GOType::Spline:
                    dest.append(CreateCurve<VSpline>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::SplinePath:
                    dest.append(CreateCurveWithSegments<VSplinePath>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::CubicBezier:
                    dest.append(CreateCurve<VCubicBezier>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::CubicBezierPath:
                    dest.append(CreateCurveWithSegments<VCubicBezierPath>(id, idObject, fPoint, sPoint, suffix, data));
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
                    UpdatePoint(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id, dest.at(i).mx,
                                dest.at(i).my);
                    break;
                case GOType::Arc:
                    UpdateArc(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::EllipticalArc:
                    //dest.append(UpdateItem<VEllipticalArc>(id, idObject, fPoint, sPoint, suffix, data));
                    break;
                case GOType::Spline:
                    UpdateCurve<VSpline>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::SplinePath:
                    UpdateCurveWithSegments<VSplinePath>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezier:
                    UpdateCurve<VCubicBezier>(id, idObject, fPoint, sPoint, suffix, data, dest.at(i).id);
                    break;
                case GOType::CubicBezierPath:
                    UpdateCurveWithSegments<VCubicBezierPath>(id, idObject, fPoint, sPoint, suffix, data,
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

    VDrawTool::AddRecord(id, Tool::FlippingByLine, doc);
    if (parse == Document::FullParse)
    {
        VToolFlippingByLine *tool = new VToolFlippingByLine(doc, data, id, firstLinePointId, secondLinePointId, suffix,
                                                            source, dest, typeCreation);
        scene->addItem(tool);
        InitOperationToolConnections(scene, tool);
        doc->AddTool(id, tool);
        doc->IncrementReferens(firstPoint.getIdTool());
        doc->IncrementReferens(secondPoint.getIdTool());
        for (int i = 0; i < source.size(); ++i)
        {
            doc->IncrementReferens(data->GetGObject(source.at(i))->getIdTool());
        }
        return tool;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolFlippingByLine>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolFlippingByLine *visual = qobject_cast<VisToolFlippingByLine *>(vis);
        SCASSERT(visual != nullptr);

        visual->SetObjects(source);
        visual->SetFirstLinePointId(m_firstLinePointId);
        visual->SetSecondLinePointId(m_secondLinePointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogFlippingByLine *dialogTool = qobject_cast<DialogFlippingByLine*>(dialog);
    SCASSERT(dialogTool != nullptr);

    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetFirstLinePointId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetSecondLinePointId()));
    doc->SetAttribute(domElement, AttrSuffix, dialogTool->GetSuffix());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::ReadToolAttributes(const QDomElement &domElement)
{
    m_firstLinePointId = doc->GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
    m_secondLinePointId = doc->GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
    suffix = doc->GetParametrString(domElement, AttrSuffix);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrP1Line, QString().setNum(m_firstLinePointId));
    doc->SetAttribute(tag, AttrP2Line, QString().setNum(m_secondLinePointId));
    doc->SetAttribute(tag, AttrSuffix, suffix);

    SaveSourceDestination(tag);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogFlippingByLine>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolFlippingByLine::VToolFlippingByLine(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 firstLinePointId,
                                         quint32 secondLinePointId, const QString &suffix,
                                         const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                         const Source &typeCreation, QGraphicsItem *parent)
    : VAbstractOperation(doc, data, id, suffix, source, destination, parent),
      m_firstLinePointId(firstLinePointId),
      m_secondLinePointId(secondLinePointId)
{
    InitOperatedObjects();
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VToolFlippingByLine::CreatePoint(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                                 const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);

    DestinationItem item;
    item.mx = rotated.mx();
    item.my = rotated.my();
    item.id = data->AddGObject(new VPointF(rotated));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
DestinationItem VToolFlippingByLine::CreateArc(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                               const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<VArc>(idTool, idItem, firstPoint, secondPoint, suffix, data);
    data->AddArc(data->GeometricObject<VArc>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::UpdatePoint(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                      const QPointF &secondPoint, const QString &suffix, VContainer *data, quint32 id,
                                      qreal mx, qreal my)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(idItem);
    VPointF rotated = point->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);
    rotated.setMx(mx);
    rotated.setMy(my);
    data->UpdateGObject(id, new VPointF(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolFlippingByLine::UpdateArc(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                    const QPointF &secondPoint, const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<VArc>(idTool, idItem, firstPoint, secondPoint, suffix, data, id);
    data->AddArc(data->GeometricObject<VArc>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolFlippingByLine::CreateItem(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                                const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item rotated = i->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);

    DestinationItem item;
    item.mx = INT_MAX;
    item.my = INT_MAX;
    item.id = data->AddGObject(new Item(rotated));
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolFlippingByLine::CreateCurve(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                                 const QPointF &secondPoint, const QString &suffix, VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data);
    data->AddCurve(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
DestinationItem VToolFlippingByLine::CreateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                                             const QPointF &secondPoint, const QString &suffix,
                                                             VContainer *data)
{
    const DestinationItem item = CreateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data);
    data->AddCurveWithSegments(data->GeometricObject<Item>(item.id), item.id);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolFlippingByLine::UpdateItem(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                     const QPointF &secondPoint, const QString &suffix, VContainer *data, quint32 id)
{
    const QSharedPointer<Item> i = data->GeometricObject<Item>(idItem);
    Item rotated = i->Flip(QLineF(firstPoint, secondPoint), suffix);
    rotated.setIdObject(idTool);
    data->UpdateGObject(id, new Item(rotated));
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolFlippingByLine::UpdateCurve(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                      const QPointF &secondPoint, const QString &suffix, VContainer *data, quint32 id)
{
    UpdateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data, id);
    data->AddCurve(data->GeometricObject<Item>(id), id);
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
void VToolFlippingByLine::UpdateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &firstPoint,
                                                  const QPointF &secondPoint, const QString &suffix, VContainer *data,
                                                  quint32 id)
{
    UpdateItem<Item>(idTool, idItem, firstPoint, secondPoint, suffix, data, id);
    data->AddCurveWithSegments(data->GeometricObject<Item>(id), id);
}

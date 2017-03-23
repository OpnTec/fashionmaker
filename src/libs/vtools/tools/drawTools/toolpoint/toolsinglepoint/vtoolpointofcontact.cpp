/************************************************************************
 **
 **  @file   vtoolpointofcontact.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "vtoolpointofcontact.h"

#include <QLineF>
#include <QMessageLogger>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>
#include <new>

#include "../../../../dialogs/tools/dialogpointofcontact.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../../visualization/line/vistoolpointofcontact.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

const QString VToolPointOfContact::ToolType = QStringLiteral("pointOfContact");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolPointOfContact constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param radius string with formula radius arc.
 * @param center id center arc point.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolPointOfContact::VToolPointOfContact(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                         const QString &radius, const quint32 &center,
                                         const quint32 &firstPointId, const quint32 &secondPointId,
                                         const Source &typeCreation, QGraphicsItem *parent)
    : VToolSinglePoint(doc, data, id, parent), arcRadius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolPointOfContact::setDialog()
{
    SCASSERT(dialog != nullptr)
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setRadius(arcRadius);
    dialogTool->setCenter(center);
    dialogTool->SetFirstPoint(firstPointId);
    dialogTool->SetSecondPoint(secondPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint return point intersection line and arc.
 * @param radius string with formula arc radius.
 * @param center center arc point.
 * @param firstPoint first line point.
 * @param secondPoint second line point.
 * @return point intersection.
 */
QPointF VToolPointOfContact::FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                       const QPointF &secondPoint)
{
    QPointF p1, p2;
    qint32 res = VGObject::LineIntersectCircle(center, radius, QLineF(firstPoint, secondPoint), p1, p2);
    switch (res)
    {
        case 0:
            return QPointF();
        case 1:
            return p1;
        case 2:
        {
            const bool flagP1 = VGObject::IsPointOnLineSegment (p1, firstPoint, secondPoint);
            const bool flagP2 = VGObject::IsPointOnLineSegment (p2, firstPoint, secondPoint);
            if ((flagP1 == true && flagP2 == true) ||
                (flagP1 == false && flagP2 == false)/*In case we have something wrong*/)
            {
                // We don't have options for choosing correct point. Use closest to segment first point.
                if (QLineF(firstPoint, p1).length() <= QLineF(firstPoint, p2).length())
                {
                    return p1;
                }
                else
                {
                    return p2;
                }
            }
            else
            { // In this case we have one real and one theoretical intersection.
                if (flagP1)
                {
                    return p1;
                }
                else
                {
                    return p2;
                }
            }
        }
        default:
            qDebug() << "Unxpected value" << res;
            return QPointF();
    }
    return QPointF();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolPointOfContact* VToolPointOfContact::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                 VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr)
    QString radius = dialogTool->getRadius();
    const quint32 center = dialogTool->getCenter();
    const quint32 firstPointId = dialogTool->GetFirstPoint();
    const quint32 secondPointId = dialogTool->GetSecondPoint();
    const QString pointName = dialogTool->getPointName();
    VToolPointOfContact *point = Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, scene, doc,
                                        data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param radius string with formula radius arc.
 * @param center id center arc point.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolPointOfContact* VToolPointOfContact::Create(const quint32 _id, QString &radius, const quint32 &center,
                                                 const quint32 &firstPointId, const quint32 &secondPointId,
                                                 const QString &pointName, const qreal &mx, const qreal &my,
                                                 VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                                 const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> centerP = data->GeometricObject<VPointF>(center);
    const QSharedPointer<VPointF> firstP = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondP = data->GeometricObject<VPointF>(secondPointId);

    const qreal result = CheckFormula(_id, radius, data);

    QPointF fPoint = VToolPointOfContact::FindPoint(qApp->toPixel(result), *centerP, *firstP, *secondP);
    quint32 id =  _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::PointOfContact, doc);
        VToolPointOfContact *point = new VToolPointOfContact(doc, data, id, radius, center,
                                                             firstPointId, secondPointId, typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        VAbstractPattern::AddTool(id, point);
        doc->IncrementReferens(centerP->getIdTool());
        doc->IncrementReferens(firstP->getIdTool());
        doc->IncrementReferens(secondP->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfContact::ArcCenterPointName() const
{
    return VAbstractTool::data.GetGObject(center)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfContact::FirstPointName() const
{
    return VAbstractTool::data.GetGObject(firstPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfContact::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(secondPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolPointOfContact::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogPointOfContact>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolPointOfContact::RemoveReferens()
{
    const auto c = VAbstractTool::data.GetGObject(center);
    const auto firstPoint = VAbstractTool::data.GetGObject(firstPointId);
    const auto secondPoint = VAbstractTool::data.GetGObject(secondPointId);

    doc->DecrementReferens(c->getIdTool());
    doc->DecrementReferens(firstPoint->getIdTool());
    doc->DecrementReferens(secondPoint->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolPointOfContact::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr)
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrRadius, dialogTool->getRadius());
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->getCenter()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPoint()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrRadius, arcRadius);
    doc->SetAttribute(tag, AttrCenter, center);
    doc->SetAttribute(tag, AttrFirstPoint, firstPointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::ReadToolAttributes(const QDomElement &domElement)
{
    arcRadius = doc->GetParametrString(domElement, AttrRadius, "");
    center = doc->GetParametrUInt(domElement, AttrCenter, NULL_ID_STR);
    firstPointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolPointOfContact *visual = qobject_cast<VisToolPointOfContact *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(firstPointId);
        visual->setLineP2Id(secondPointId);
        visual->setRadiusId(center);
        visual->setRadius(qApp->TrVars()->FormulaToUser(arcRadius, qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfContact::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SetSecondPointId(const quint32 &value)
{
    secondPointId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfContact>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfContact::GetFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::SetFirstPointId(const quint32 &value)
{
    firstPointId = value;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfContact::getCenter() const
{
    return center;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::setCenter(const quint32 &value)
{
    if (value != NULL_ID)
    {
        center = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolPointOfContact::getArcRadius() const
{
    VFormula radius(arcRadius, this->getData());
    radius.setCheckZero(true);
    radius.setToolId(id);
    radius.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));

    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::setArcRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        arcRadius = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

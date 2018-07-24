/************************************************************************
 **
 **  @file   vtoolpointofcontact.cpp
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
#include "../ifc/exception/vexceptionobjecterror.h"
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
 * @param initData init data.
 * @param parent parent object.
 */
VToolPointOfContact::VToolPointOfContact(const VToolPointOfContactInitData &initData, QGraphicsItem *parent)
    : VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      arcRadius(initData.radius),
      center(initData.center),
      firstPointId(initData.firstPointId),
      secondPointId(initData.secondPointId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolPointOfContact::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfContact> dialogTool = qobject_cast<DialogPointOfContact *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
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
 * @param intersectionPoint point of intersection.
 * @return true if intersection exists.
 */
bool VToolPointOfContact::FindPoint(qreal radius, const QPointF &center, const QPointF &firstPoint,
                                    const QPointF &secondPoint, QPointF *intersectionPoint)
{
    SCASSERT(intersectionPoint != nullptr)

    QPointF p1, p2;
    qint32 res = VGObject::LineIntersectCircle(center, radius, QLineF(firstPoint, secondPoint), p1, p2);
    switch (res)
    {
        case 0:
            return false;
        case 1:
            *intersectionPoint = p1;
            return true;
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
                    *intersectionPoint = p1;
                    return true;
                }
                else
                {
                    *intersectionPoint = p2;
                    return true;
                }
            }
            else
            { // In this case we have one real and one theoretical intersection.
                if (flagP1)
                {
                    *intersectionPoint = p1;
                    return true;
                }
                else
                {
                    *intersectionPoint = p2;
                    return true;
                }
            }
        }
        default:
            qDebug() << "Unxpected value" << res;
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolPointOfContact* VToolPointOfContact::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                                 VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogPointOfContact> dialogTool = qobject_cast<DialogPointOfContact *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolPointOfContactInitData initData;
    initData.radius = dialogTool->getRadius();
    initData.center = dialogTool->getCenter();
    initData.firstPointId = dialogTool->GetFirstPoint();
    initData.secondPointId = dialogTool->GetSecondPoint();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointOfContact *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 */
VToolPointOfContact* VToolPointOfContact::Create(VToolPointOfContactInitData &initData)
{
    const QSharedPointer<VPointF> centerP = initData.data->GeometricObject<VPointF>(initData.center);
    const QSharedPointer<VPointF> firstP = initData.data->GeometricObject<VPointF>(initData.firstPointId);
    const QSharedPointer<VPointF> secondP = initData.data->GeometricObject<VPointF>(initData.secondPointId);

    const qreal result = CheckFormula(initData.id, initData.radius, initData.data);

    QPointF fPoint;
    const bool success = VToolPointOfContact::FindPoint(qApp->toPixel(result), static_cast<QPointF>(*centerP),
                                                        static_cast<QPointF>(*firstP), static_cast<QPointF>(*secondP),
                                                        &fPoint);

    if (not success)
    {
        const QString errorMsg = tr("Error calculating point '%1'. Circle with center '%2' and radius '%3' doesn't have "
                                    "intersection with line (%4;%5)")
                .arg(initData.name, centerP->name()).arg(result).arg(firstP->name(), secondP->name());
        qApp->IsPedantic() ? throw VExceptionObjectError(errorMsg) : qWarning() << errorMsg;
    }

    VPointF *p = new VPointF(fPoint, initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        initData.data->AddLine(initData.firstPointId, initData.id);
        initData.data->AddLine(initData.secondPointId, initData.id);
        initData.data->AddLine(initData.center, initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        initData.data->AddLine(initData.firstPointId, initData.id);
        initData.data->AddLine(initData.secondPointId, initData.id);
        initData.data->AddLine(initData.center, initData.id);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::PointOfContact, initData.doc);
        VToolPointOfContact *point = new VToolPointOfContact(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(centerP->getIdTool());
        initData.doc->IncrementReferens(firstP->getIdTool());
        initData.doc->IncrementReferens(secondP->getIdTool());
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
void VToolPointOfContact::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                     QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfContact> dialogTool = qobject_cast<DialogPointOfContact *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, center);
    AddDependence(oldDependencies, firstPointId);
    AddDependence(oldDependencies, secondPointId);
    AddDependence(newDependencies, dialogTool->getCenter());
    AddDependence(newDependencies, dialogTool->GetFirstPoint());
    AddDependence(newDependencies, dialogTool->GetSecondPoint());

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
    arcRadius = doc->GetParametrString(domElement, AttrRadius, QString());
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
QString VToolPointOfContact::MakeToolTip() const
{
    const QSharedPointer<VPointF> p1 = VAbstractTool::data.GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> p2 = VAbstractTool::data.GeometricObject<VPointF>(secondPointId);
    const QSharedPointer<VPointF> centerP = VAbstractTool::data.GeometricObject<VPointF>(center);
    const QSharedPointer<VPointF> current = VAbstractTool::data.GeometricObject<VPointF>(m_id);

    const QLineF p1ToCur(static_cast<QPointF>(*p1), static_cast<QPointF>(*current));
    const QLineF p2ToCur(static_cast<QPointF>(*p2), static_cast<QPointF>(*current));
    const QLineF centerToCur(static_cast<QPointF>(*centerP), static_cast<QPointF>(*current));

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%10:</b> %11</td> </tr>"
                                    "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%4:</b> %5 %3</td> </tr>"
                                    "<tr> <td><b>%6:</b> %7 %3</td> </tr>"
                                    "<tr> <td><b>%8:</b> %9°</td> </tr>"
                                    "</table>")
            .arg(QString("%1->%2").arg(p1->name(), current->name()))
            .arg(qApp->fromPixel(p1ToCur.length()))
            .arg(UnitsToStr(qApp->patternUnit(), true), QString("%1->%2").arg(p2->name(), current->name()))
            .arg(qApp->fromPixel(p2ToCur.length()))
            .arg(QString("%1 %2->%3").arg(tr("Length"), centerP->name(), current->name()))
            .arg(qApp->fromPixel(centerToCur.length()))
            .arg(QString("%1 %2->%3").arg(tr("Angle"), centerP->name(), current->name()))
            .arg(centerToCur.angle())
            .arg(tr("Label"), current->name());
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfContact>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointOfContact>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolPointOfContact::getArcRadius() const
{
    VFormula radius(arcRadius, this->getData());
    radius.setCheckZero(true);
    radius.setToolId(m_id);
    radius.setPostfix(UnitsToStr(qApp->patternUnit()));
    radius.Eval();

    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfContact::setArcRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        arcRadius = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

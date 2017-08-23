/************************************************************************
 **
 **  @file   vtoolshoulderpoint.cpp
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

#include "vtoolshoulderpoint.h"

#include <QLineF>
#include <QMessageLogger>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtDebug>
#include <new>

#include "../../../../../dialogs/tools/dialogshoulderpoint.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolshoulderpoint.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString VToolShoulderPoint::ToolType = QStringLiteral("shoulder");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolShoulderPoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formula string with formula length.
 * @param p1Line id first line point.
 * @param p2Line id second line point.
 * @param pShoulder id shoulder point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolShoulderPoint::VToolShoulderPoint(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &typeLine,
                                       const QString &lineColor, const QString &formula, const quint32 &p1Line,
                                       const quint32 &p2Line, const quint32 &pShoulder, const Source &typeCreation,
                                       QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, formula, p1Line, 0, parent), p2Line(p2Line),
      pShoulder(pShoulder)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolShoulderPoint::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = m_dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetP1Line(basePointId);
    dialogTool->SetP2Line(p2Line);
    dialogTool->SetP3(pShoulder);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find point.
 * @param p1Line first line point.
 * @param p2Line second line point.
 * @param pShoulder shoulder point.
 * @param length length form shoulder point to our.
 * @return point.
 */
QPointF VToolShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                      const qreal &length)
{
    QPointF shoulderPoint = p2Line; // Position if result was not found
    if (length <= 0)
    {
        return shoulderPoint;
    }

    QLineF line = QLineF(p1Line, p2Line);
    const qreal baseLength = line.length();
    const int baseAngle = qRound(line.angle());
    line.setLength(length*2);

    QPointF p1;
    QPointF p2;

    const qint32 res = VGObject::LineIntersectCircle(pShoulder, length, line, p1, p2);

    if (res == 1 || res == 2)
    {
        const QLineF line1 = QLineF(p1Line, p1);
        const QLineF line2 = QLineF(p1Line, p2);
        if (line1.length() > baseLength && baseAngle == qRound(line1.angle()))
        {
            shoulderPoint = p1;
        }
        else if (res == 2 && line2.length() > baseLength && baseAngle == qRound(line2.angle()))
        {
            shoulderPoint = p2;
        }
    }

    return shoulderPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(not dialogTool.isNull())
    QString formula = dialogTool->GetFormula();
    const quint32 p1Line = dialogTool->GetP1Line();
    const quint32 p2Line = dialogTool->GetP2Line();
    const quint32 pShoulder = dialogTool->GetP3();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    const QString pointName = dialogTool->getPointName();
    VToolShoulderPoint * point = Create(0, formula, p1Line, p2Line, pShoulder, typeLine, lineColor, pointName, 5,
                                        10, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param formula string with formula length.
 * @param p1Line id first line point.
 * @param p2Line id second line point.
 * @param pShoulder id shoulder point.
 * @param typeLine line type.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolShoulderPoint* VToolShoulderPoint::Create(const quint32 _id, QString &formula, const quint32 &p1Line,
                                               const quint32 &p2Line, const quint32 &pShoulder, const QString &typeLine,
                                               const QString &lineColor, const QString &pointName, const qreal &mx,
                                               const qreal &my, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(p1Line);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(p2Line);
    const QSharedPointer<VPointF> shoulderPoint = data->GeometricObject<VPointF>(pShoulder);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = VToolShoulderPoint::FindPoint(static_cast<QPointF>(*firstPoint),
                                                   static_cast<QPointF>(*secondPoint),
                                                   static_cast<QPointF>(*shoulderPoint), qApp->toPixel(result));
    quint32 id =  _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(p1Line, id);
        data->AddLine(p2Line, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(p1Line, id);
        data->AddLine(p2Line, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::ShoulderPoint, doc);
        VToolShoulderPoint *point = new VToolShoulderPoint(doc, data, id, typeLine, lineColor, formula,
                                                           p1Line, p2Line, pShoulder,
                                                           typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        VAbstractPattern::AddTool(id, point);
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
        doc->IncrementReferens(shoulderPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(p2Line)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::ShoulderPointName() const
{
    return VAbstractTool::data.GetGObject(pShoulder)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogShoulderPoint>(this, event);
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
void VToolShoulderPoint::RemoveReferens()
{
    const auto p2 = VAbstractTool::data.GetGObject(p2Line);
    const auto pS = VAbstractTool::data.GetGObject(pShoulder);

    doc->DecrementReferens(p2->getIdTool());
    doc->DecrementReferens(pS->getIdTool());
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolShoulderPoint::SaveDialog(QDomElement &domElement)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogShoulderPoint> dialogTool = m_dialog.objectCast<DialogShoulderPoint>();
    SCASSERT(not dialogTool.isNull())
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->GetP1Line()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->GetP2Line()));
    doc->SetAttribute(domElement, AttrPShoulder, QString().setNum(dialogTool->GetP3()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrP1Line, basePointId);
    doc->SetAttribute(tag, AttrP2Line, p2Line);
    doc->SetAttribute(tag, AttrPShoulder, pShoulder);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, "");
    basePointId = doc->GetParametrUInt(domElement, AttrP1Line, NULL_ID_STR);
    p2Line = doc->GetParametrUInt(domElement, AttrP2Line, NULL_ID_STR);
    pShoulder = doc->GetParametrUInt(domElement, AttrPShoulder, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolShoulderPoint *visual = qobject_cast<VisToolShoulderPoint *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(pShoulder);
        visual->setLineP1Id(basePointId);
        visual->setLineP2Id(p2Line);
        visual->setLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolShoulderPoint::MakeToolTip() const
{
    const QSharedPointer<VPointF> first = VAbstractTool::data.GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> second = VAbstractTool::data.GeometricObject<VPointF>(p2Line);
    const QSharedPointer<VPointF> current = VAbstractTool::data.GeometricObject<VPointF>(id);

    const QLineF firstToCur(static_cast<QPointF>(*first), static_cast<QPointF>(*current));
    const QLineF secondToCur(static_cast<QPointF>(*second), static_cast<QPointF>(*current));

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%4:</b> %5°</td> </tr>"
                                    "<tr> <td><b>%6:</b> %7 %3</td> </tr>"
                                    "</table>")
            .arg(tr("Length"))
            .arg(qApp->fromPixel(firstToCur.length()))
            .arg(UnitsToStr(qApp->patternUnit(), true))
            .arg(tr("Angle"))
            .arg(firstToCur.angle())
            .arg(QString("%1->%2").arg(second->name(), current->name()))
            .arg(qApp->fromPixel(secondToCur.length()));
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VToolShoulderPoint::getPShoulder() const
{
    return pShoulder;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VToolShoulderPoint::setPShoulder(const quint32 &value)
{
    if (value != NULL_ID)
    {
        pShoulder = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolShoulderPoint>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolShoulderPoint::GetP2Line() const
{
    return p2Line;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolShoulderPoint::SetP2Line(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p2Line = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

/************************************************************************
 **
 **  @file   vtoolarcwithlength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vtoolarcwithlength.h"

#include <QPen>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>
#include <new>

#include "../../../dialogs/tools/dialogarcwithlength.h"
#include "../../../visualization/path/vistoolarcwithlength.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "tools/drawTools/toolcurve/../../../dialogs/tools/dialogtool.h"
#include "tools/drawTools/toolcurve/../../../visualization/path/../visualization.h"
#include "tools/drawTools/toolcurve/../../vabstracttool.h"
#include "tools/drawTools/toolcurve/../vdrawtool.h"
#include "tools/drawTools/toolcurve/vabstractspline.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;

const QString VToolArcWithLength::ToolType = QStringLiteral("arcWithLength");

//---------------------------------------------------------------------------------------------------------------------
VToolArcWithLength::VToolArcWithLength(VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                                       QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::Arc;

    this->setPath(ToolPath());
    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogArcWithLength *dialogTool = qobject_cast<DialogArcWithLength*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    dialogTool->SetCenter(arc->GetCenter().id());
    dialogTool->SetF1(arc->GetFormulaF1());
    dialogTool->SetLength(arc->GetFormulaLength());
    dialogTool->SetRadius(arc->GetFormulaRadius());
    dialogTool->SetColor(arc->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
VToolArcWithLength *VToolArcWithLength::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogArcWithLength *dialogTool = qobject_cast<DialogArcWithLength*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 center = dialogTool->GetCenter();
    QString radius = dialogTool->GetRadius();
    QString f1 = dialogTool->GetF1();
    QString length = dialogTool->GetLength();
    const QString color = dialogTool->GetColor();
    VToolArcWithLength* point = Create(0, center, radius, f1, length, color, scene, doc, data, Document::FullParse,
                                       Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolArcWithLength *VToolArcWithLength::Create(const quint32 _id, const quint32 &center, QString &radius, QString &f1,
                                               QString &length, const QString &color, VMainGraphicsScene *scene,
                                               VAbstractPattern *doc, VContainer *data, const Document &parse,
                                               const Source &typeCreation)
{
    qreal calcRadius = 0, calcF1 = 0, calcLength = 0;

    calcRadius = qApp->toPixel(CheckFormula(_id, radius, data));
    calcLength = qApp->toPixel(CheckFormula(_id, length, data));
    calcF1 = CheckFormula(_id, f1, data);

    const VPointF c = *data->GeometricObject<VPointF>(center);
    VArc *arc = new VArc(calcLength, length, c, calcRadius, radius, calcF1, f1);
    arc->SetColor(color);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(arc);
        data->AddArc(data->GeometricObject<VArc>(id), id);
    }
    else
    {
        data->UpdateGObject(id, arc);
        data->AddArc(data->GeometricObject<VArc>(id), id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::ArcWithLength, doc);
    if (parse == Document::FullParse)
    {
        VToolArcWithLength *toolArc = new VToolArcWithLength(doc, data, id, typeCreation);
        scene->addItem(toolArc);
        InitArcToolConnections(scene, toolArc);
        doc->AddTool(id, toolArc);
        doc->IncrementReferens(c.getIdTool());
        return toolArc;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolArcWithLength::getTagName() const
{
    return VAbstractPattern::TagArc;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolArcWithLength::getCenter() const
{
    QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    SCASSERT(arc.isNull() == false);

    return arc->GetCenter().id();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::setCenter(const quint32 &value)
{
    if (value != NULL_ID)
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        QSharedPointer<VArc> arc = qSharedPointerDynamicCast<VArc>(obj);

        QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(value);
        arc->SetCenter(*point.data());
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolArcWithLength::GetFormulaRadius() const
{
    QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    SCASSERT(arc.isNull() == false);

    VFormula radius(arc->GetFormulaRadius(), getData());
    radius.setCheckZero(true);
    radius.setToolId(id);
    radius.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SetFormulaRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        if (value.getDoubleValue() > 0)// Formula don't check this, but radius can't be 0 or negative
        {
            QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
            QSharedPointer<VArc> arc = qSharedPointerDynamicCast<VArc>(obj);
            arc->SetFormulaRadius(value.GetFormula(FormulaType::FromUser), value.getDoubleValue());
            SaveOption(obj);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolArcWithLength::GetFormulaF1() const
{
    QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    SCASSERT(arc.isNull() == false);

    VFormula f1(arc->GetFormulaF1(), getData());
    f1.setCheckZero(false);
    f1.setToolId(id);
    f1.setPostfix(degreeSymbol);
    return f1;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SetFormulaF1(const VFormula &value)
{
    if (value.error() == false)
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        QSharedPointer<VArc> arc = qSharedPointerDynamicCast<VArc>(obj);

        if (not VFuzzyComparePossibleNulls(value.getDoubleValue(), arc->GetEndAngle()))// Angles can't be equal
        {
            arc->SetFormulaF1(value.GetFormula(FormulaType::FromUser), value.getDoubleValue());
            SaveOption(obj);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolArcWithLength::GetFormulaLength() const
{
    QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    SCASSERT(arc.isNull() == false);

    VFormula radius(arc->GetFormulaLength(), getData());
    radius.setCheckZero(true);
    radius.setToolId(id);
    radius.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SetFormulaLength(const VFormula &value)
{
    if (value.error() == false)
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        QSharedPointer<VArc> arc = qSharedPointerDynamicCast<VArc>(obj);
        arc->SetFormulaLength(value.GetFormula(FormulaType::FromUser), value.getDoubleValue());
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolArcWithLength>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogArcWithLength>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::RemoveReferens()
{
    const auto arc = VAbstractTool::data.GeometricObject<VArc>(id);
    doc->DecrementReferens(arc->GetCenter().getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogArcWithLength *dialogTool = qobject_cast<DialogArcWithLength*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->GetCenter()));
    doc->SetAttribute(domElement, AttrRadius, dialogTool->GetRadius());
    doc->SetAttribute(domElement, AttrAngle1, dialogTool->GetF1());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetLength());
    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    QSharedPointer<VArc> arc = qSharedPointerDynamicCast<VArc>(obj);
    SCASSERT(arc.isNull() == false);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrCenter, arc->GetCenter().id());
    doc->SetAttribute(tag, AttrRadius, arc->GetFormulaRadius());
    doc->SetAttribute(tag, AttrAngle1, arc->GetFormulaF1());
    doc->SetAttribute(tag, AttrLength, arc->GetFormulaLength());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::SetVisualization()
{
    if (vis != nullptr)
    {
        const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
        VisToolArcWithLength *visual = qobject_cast<VisToolArcWithLength *>(vis);
        SCASSERT(visual != nullptr);

        const VTranslateVars *trVars = qApp->TrVars();
        visual->setObject1Id(arc->GetCenter().id());
        visual->setRadius(trVars->FormulaToUser(arc->GetFormulaRadius(), qApp->Settings()->GetOsSeparator()));
        visual->setF1(trVars->FormulaToUser(arc->GetFormulaF1(), qApp->Settings()->GetOsSeparator()));
        visual->setLength(trVars->FormulaToUser(arc->GetFormulaLength(), qApp->Settings()->GetOsSeparator()));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolArcWithLength::RefreshGeometry()
{
    QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
    this->setPen(QPen(CorrectColor(arc->GetColor()),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setPath(ToolPath());

    SetVisualization();
}

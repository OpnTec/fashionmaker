/************************************************************************
 **
 **  @file   vtoolcubicbezier.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
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

#include "vtoolcubicbezier.h"
#include "../vgeometry/vcubicbezier.h"
#include "../../../dialogs/tools/dialogcubicbezier.h"
#include "../../../undocommands/movespline.h"
#include "../../../visualization/path/vistoolcubicbezier.h"

const QString VToolCubicBezier::ToolType = QStringLiteral("cubicBezier");

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier::VToolCubicBezier(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &color,
                                   const Source &typeCreation, QGraphicsItem *parent)
    :VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::Spline;
    lineColor = color;

    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setPath(ToolPath());
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier::~VToolCubicBezier()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::setDialog()
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogCubicBezier*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const auto spl = VAbstractTool::data.GeometricObject<VCubicBezier>(id);
    dialogTool->SetSpline(*spl);
    dialogTool->SetColor(lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier *VToolCubicBezier::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                           VContainer *data)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogCubicBezier*>(dialog);
    SCASSERT(dialogTool != nullptr);

    auto spl = Create(0, new VCubicBezier(dialogTool->GetSpline()), dialogTool->GetColor(), scene, doc, data,
                      Document::FullParse, Source::FromGui);

    if (spl != nullptr)
    {
        spl->dialog=dialogTool;
    }
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier *VToolCubicBezier::Create(const quint32 _id, VCubicBezier *spline, const QString &color,
                                           VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                           const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(spline);
        data->AddCurve(data->GeometricObject<VAbstractCurve>(id), id);
    }
    else
    {
        data->UpdateGObject(id, spline);
        data->AddCurve(data->GeometricObject<VAbstractCurve>(id), id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::CubicBezier, doc);
    if (parse == Document::FullParse)
    {
        auto _spl = new VToolCubicBezier(doc, data, id, color, typeCreation);
        scene->addItem(_spl);
        InitSplineToolConnections(scene, _spl);
        doc->AddTool(id, _spl);
        doc->IncrementReferens(spline->GetP1().getIdTool());
        doc->IncrementReferens(spline->GetP1().getIdTool());
        doc->IncrementReferens(spline->GetP1().getIdTool());
        doc->IncrementReferens(spline->GetP4().getIdTool());
        return _spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier VToolCubicBezier::getSpline() const
{
    auto spline = VAbstractTool::data.GeometricObject<VCubicBezier>(id);
    return *spline.data();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::setSpline(const VCubicBezier &spl)
{
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    QSharedPointer<VCubicBezier> spline = qSharedPointerDynamicCast<VCubicBezier>(obj);
    *spline.data() = spl;
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCubicBezier>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogCubicBezier>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::RemoveReferens()
{
    const auto spl = VAbstractTool::data.GeometricObject<VCubicBezier>(id);
    doc->DecrementReferens(spl->GetP1().getIdTool());
    doc->DecrementReferens(spl->GetP2().getIdTool());
    doc->DecrementReferens(spl->GetP3().getIdTool());
    doc->DecrementReferens(spl->GetP4().getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogCubicBezier*>(dialog);
    SCASSERT(dialogTool != nullptr);

    const VCubicBezier spl = dialogTool->GetSpline();

    SetSplineAttributes(domElement, spl);
    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    auto spl = qSharedPointerDynamicCast<VCubicBezier>(obj);
    SCASSERT(spl.isNull() == false);
    SetSplineAttributes(tag, *spl);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SetVisualization()
{
    if (vis != nullptr)
    {
        auto visual = qobject_cast<VisToolCubicBezier *>(vis);
        SCASSERT(visual != nullptr);

        const QSharedPointer<VCubicBezier> spl = VAbstractTool::data.GeometricObject<VCubicBezier>(id);
        visual->setObject1Id(spl->GetP1().id());
        visual->setObject2Id(spl->GetP2().id());
        visual->setObject3Id(spl->GetP3().id());
        visual->setObject4Id(spl->GetP4().id());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::RefreshGeometry()
{
    this->setPen(QPen(CorrectColor(lineColor),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    if (isHovered || detailsMode)
    {
        this->setPath(ToolPath(PathDirection::Show));
    }
    else
    {
        this->setPath(ToolPath());
    }

    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SetSplineAttributes(QDomElement &domElement, const VCubicBezier &spl)
{
    SCASSERT(doc != nullptr);

    doc->SetAttribute(domElement, AttrType,    ToolType);
    doc->SetAttribute(domElement, AttrPoint1,  spl.GetP1().id());
    doc->SetAttribute(domElement, AttrPoint2,  spl.GetP2().id());
    doc->SetAttribute(domElement, AttrPoint3,  spl.GetP3().id());
    doc->SetAttribute(domElement, AttrPoint4,  spl.GetP4().id());

    if (spl.GetDuplicate() > 0)
    {
        doc->SetAttribute(domElement, AttrDuplicate, spl.GetDuplicate());
    }
    else
    {
        if (domElement.hasAttribute(AttrDuplicate))
        {
            domElement.removeAttribute(AttrDuplicate);
        }
    }
}

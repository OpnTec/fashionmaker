/************************************************************************
 **
 **  @file   vtoolcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
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

#include "vtoolcubicbezierpath.h"

#include <QDomElement>
#include <QPen>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>
#include <new>

#include "../../../dialogs/tools/dialogcubicbezierpath.h"
#include "../../../visualization/path/vistoolcubicbezierpath.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "tools/drawTools/toolcurve/../../../dialogs/tools/dialogtool.h"
#include "tools/drawTools/toolcurve/../../../visualization/path/../visualization.h"
#include "tools/drawTools/toolcurve/../../vabstracttool.h"
#include "tools/drawTools/toolcurve/../vdrawtool.h"
#include "tools/drawTools/toolcurve/vabstractspline.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;

const QString VToolCubicBezierPath::ToolType = QStringLiteral("cubicBezierPath");

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezierPath::VToolCubicBezierPath(VAbstractPattern *doc, VContainer *data, quint32 id,
                                           const Source &typeCreation, QGraphicsItem *parent)
    : VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::SplinePath;

    this->setPath(ToolPath());
    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezierPath::~VToolCubicBezierPath()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::setDialog()
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogCubicBezierPath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VCubicBezierPath> splPath = VAbstractTool::data.GeometricObject<VCubicBezierPath>(id);
    dialogTool->SetPath(*splPath);
    dialogTool->SetColor(splPath->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezierPath *VToolCubicBezierPath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                   VContainer *data)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogCubicBezierPath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    auto path = new VCubicBezierPath(dialogTool->GetPath());
    const QString color = dialogTool->GetColor();
    for (qint32 i = 0; i < path->CountPoints(); ++i)
    {
        doc->IncrementReferens((*path)[i].getIdTool());
    }
    VToolCubicBezierPath* spl = Create(0, path, color, scene, doc, data, Document::FullParse, Source::FromGui);
    if (spl != nullptr)
    {
        spl->dialog=dialogTool;
    }
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezierPath *VToolCubicBezierPath::Create(const quint32 _id, VCubicBezierPath *path, const QString &color,
                                                   VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                                   const Document &parse, const Source &typeCreation)
{
    quint32 id = _id;
    path->SetColor(color);
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(path);
        data->AddCurveWithSegments(data->GeometricObject<VAbstractCubicBezierPath>(id), id);
    }
    else
    {
        data->UpdateGObject(id, path);
        data->AddCurveWithSegments(data->GeometricObject<VAbstractCubicBezierPath>(id), id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::CubicBezierPath, doc);
    if (parse == Document::FullParse)
    {
        VToolCubicBezierPath *spl = new VToolCubicBezierPath(doc, data, id, typeCreation);
        scene->addItem(spl);
        InitSplinePathToolConnections(scene, spl);
        doc->AddTool(id, spl);
        return spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::UpdatePathPoints(VAbstractPattern *doc, QDomElement &element, const VCubicBezierPath &path)
{
    VDomDocument::RemoveAllChildren(element);
    for (qint32 i = 0; i < path.CountPoints(); ++i)
    {
        AddPathPoint(doc, element, path.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath VToolCubicBezierPath::getSplinePath() const
{
    QSharedPointer<VCubicBezierPath> splPath = VAbstractTool::data.GeometricObject<VCubicBezierPath>(id);
    return *splPath.data();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::setSplinePath(const VCubicBezierPath &splPath)
{
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    QSharedPointer<VCubicBezierPath> splinePath = qSharedPointerDynamicCast<VCubicBezierPath>(obj);
    *splinePath.data() = splPath;
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCubicBezierPath>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogCubicBezierPath>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::RemoveReferens()
{
    const QSharedPointer<VCubicBezierPath> splPath = VAbstractTool::data.GeometricObject<VCubicBezierPath>(id);
    for (qint32 i = 0; i < splPath->CountSubSpl(); ++i)
    {
        doc->DecrementReferens(splPath->at(i).getIdTool());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    const auto dialogTool = qobject_cast<DialogCubicBezierPath*>(dialog);
    SCASSERT(dialogTool != nullptr);

    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
    SetSplinePathAttributes(domElement, dialogTool->GetPath());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    QSharedPointer<VCubicBezierPath> splPath = qSharedPointerDynamicCast<VCubicBezierPath>(obj);
    SCASSERT(splPath.isNull() == false);

    SetSplinePathAttributes(tag, *splPath);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::SetVisualization()
{
    if (vis != nullptr)
    {
        auto visual = qobject_cast<VisToolCubicBezierPath *>(vis);
        SCASSERT(visual != nullptr);

        QSharedPointer<VCubicBezierPath> splPath = VAbstractTool::data.GeometricObject<VCubicBezierPath>(id);
        visual->setPath(*splPath.data());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::RefreshGeometry()
{
    isHovered || detailsMode ? setPath(ToolPath(PathDirection::Show)) : setPath(ToolPath());

    QSharedPointer<VCubicBezierPath> splPath = VAbstractTool::data.GeometricObject<VCubicBezierPath>(id);
    this->setPen(QPen(CorrectColor(splPath->GetColor()),
                      qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));

    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::AddPathPoint(VAbstractPattern *doc, QDomElement &domElement, const VPointF &splPoint)
{
    SCASSERT(doc != nullptr);
    QDomElement pathPoint = doc->createElement(AttrPathPoint);
    doc->SetAttribute(pathPoint, AttrPSpline, splPoint.id());
    domElement.appendChild(pathPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezierPath::SetSplinePathAttributes(QDomElement &domElement, const VCubicBezierPath &path)
{
    doc->SetAttribute(domElement, AttrType, ToolType);

    if (path.GetDuplicate() > 0)
    {
        doc->SetAttribute(domElement, AttrDuplicate, path.GetDuplicate());
    }
    else
    {
        if (domElement.hasAttribute(AttrDuplicate))
        {
            domElement.removeAttribute(AttrDuplicate);
        }
    }

    UpdatePathPoints(doc, domElement, path);
}

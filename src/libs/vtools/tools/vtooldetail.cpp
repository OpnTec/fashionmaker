/************************************************************************
 **
 **  @file   vtooldetail.cpp
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

#include "vtooldetail.h"

#include <QAction>
#include <QBrush>
#include <QDialog>
#include <QDomElement>
#include <QEvent>
#include <QFlags>
#include <QFont>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QHash>
#include <QIcon>
#include <QKeyEvent>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QMessageLogger>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QString>
#include <QStringData>
#include <QStringDataPtr>
#include <QStringList>
#include <QUndoStack>
#include <QVariant>
#include <Qt>
#include <QtDebug>
#include <new>

#include "../dialogs/tools/dialogdetail.h"
#include "../dialogs/tools/dialogtool.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../undocommands/adddet.h"
#include "../undocommands/deletedetail.h"
#include "../undocommands/movedetail.h"
#include "../undocommands/savedetailoptions.h"
#include "../undocommands/toggledetailinlayout.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vdetail.h"
#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vpatterndb/calculator.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vwidgets/vnobrushscalepathitem.h"
#include "../tools/vtooldetail.h"
#include "vabstracttool.h"
#include "nodeDetails/vabstractnode.h"
#include "nodeDetails/vnodearc.h"
#include "nodeDetails/vnodepoint.h"
#include "nodeDetails/vnodespline.h"
#include "nodeDetails/vnodesplinepath.h"
#include "vtextgraphicsitem.h"
#include "vnodedetail.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QStyleOptionGraphicsItem;
class QWidget;
class VDataTool;

const QString VToolDetail::TagNode            = QStringLiteral("node");

const QString VToolDetail::AttrSupplement     = QStringLiteral("supplement");
const QString VToolDetail::AttrClosed         = QStringLiteral("closed");
const QString VToolDetail::AttrForbidFlipping = QStringLiteral("forbidFlipping");
const QString VToolDetail::AttrWidth          = QStringLiteral("width");
const QString VToolDetail::AttrHeight         = QStringLiteral("height");
const QString VToolDetail::AttrNodeType       = QStringLiteral("nodeType");
const QString VToolDetail::AttrReverse        = QStringLiteral("reverse");
const QString VToolDetail::AttrFont           = QStringLiteral("fontSize");
const QString VToolDetail::AttrRotation       = QStringLiteral("rotation");

const QString VToolDetail::NodeTypeContour    = QStringLiteral("Contour");
const QString VToolDetail::NodeTypeModeling   = QStringLiteral("Modeling");

const QString VToolDetail::NodeArc            = QStringLiteral("NodeArc");
const QString VToolDetail::NodePoint          = QStringLiteral("NodePoint");
const QString VToolDetail::NodeSpline         = QStringLiteral("NodeSpline");
const QString VToolDetail::NodeSplinePath     = QStringLiteral("NodeSplinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail constructor.
 * @param doc dom document container
 * @param data container with variables
 * @param id object id in container
 * @param typeCreation way we create this tool.
 * @param scene pointer to scene.
 * @param parent parent object
 */
VToolDetail::VToolDetail(VAbstractPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                         VMainGraphicsScene *scene, const QString &drawName, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), VNoBrushScalePathItem(parent), dialog(nullptr), sceneDetails(scene),
      drawName(drawName), seamAllowance(new VNoBrushScalePathItem(this)), dataLabel(new VTextGraphicsItem(this)),
      patternInfo(new VTextGraphicsItem(this)), grainLine(new VGrainlineItem(this))
{
    VDetail detail = data->GetDetail(id);
    for (int i = 0; i< detail.CountNode(); ++i)
    {
        switch (detail.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                VNodePoint *tool = InitTool<VNodePoint>(scene, detail.at(i));
                connect(tool, &VNodePoint::ShowContextMenu, this, &VToolDetail::contextMenuEvent);
                break;
            }
            case (Tool::NodeArc):
                doc->IncrementReferens(detail.at(i).getId());
                break;
            case (Tool::NodeSpline):
                doc->IncrementReferens(detail.at(i).getId());
                break;
            case (Tool::NodeSplinePath):
                doc->IncrementReferens(detail.at(i).getId());
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();

    this->setBrush(QBrush(Qt::Dense7Pattern));
    seamAllowance->setBrush(QBrush(Qt::FDiagPattern));

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus

    connect(scene, &VMainGraphicsScene::EnableToolMove, this, &VToolDetail::EnableToolMove);
    connect(scene, &VMainGraphicsScene::ItemClicked, this, &VToolDetail::ResetChildren);
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        AddToFile();
        if (typeCreation != Source::FromTool)
        {
            qApp->getUndoStack()->endMacro();
        }
    }
    setAcceptHoverEvents(true);

    connect(dataLabel, &VTextGraphicsItem::SignalMoved, this, &VToolDetail::SaveMoveDetail);
    connect(dataLabel, &VTextGraphicsItem::SignalResized, this, &VToolDetail::SaveResizeDetail);
    connect(dataLabel, &VTextGraphicsItem::SignalRotated, this, &VToolDetail::SaveRotationDetail);

    connect(patternInfo, &VTextGraphicsItem::SignalMoved, this, &VToolDetail::SaveMovePattern);
    connect(patternInfo, &VTextGraphicsItem::SignalResized, this, &VToolDetail::SaveResizePattern);
    connect(patternInfo, &VTextGraphicsItem::SignalRotated, this, &VToolDetail::SaveRotationPattern);

    connect(grainLine, &VGrainlineItem::SignalMoved, this, &VToolDetail::SaveMoveGrainline);
    connect(grainLine, &VGrainlineItem::SignalResized, this, &VToolDetail::SaveResizeGrainline);
    connect(grainLine, &VGrainlineItem::SignalRotated, this, &VToolDetail::SaveRotateGrainline);

    connect(doc, &VAbstractPattern::patternChanged, this, &VToolDetail::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolDetail::UpdateLabel);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolDetail::UpdatePatternInfo);
    connect(doc, &VAbstractPattern::CheckLayout, this, &VToolDetail::UpdateGrainline);

    connect(sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolDetail::UpdateLabel);
    connect(sceneDetails, &VMainGraphicsScene::DimensionsChanged, this, &VToolDetail::UpdatePatternInfo);
    connect(sceneDetails, &VMainGraphicsScene::LanguageChanged, this, &VToolDetail::retranslateUi);

    UpdateLabel();
    UpdatePatternInfo();
    UpdateGrainline();
}

//---------------------------------------------------------------------------------------------------------------------
VToolDetail::~VToolDetail()
{
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolDetail::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    dialogTool->setDetail(VAbstractTool::data.GetDetail(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
void VToolDetail::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail detail = dialogTool->getDetail();
    VDetail det;
    qApp->getUndoStack()->beginMacro("add detail");
    for (int i = 0; i< detail.CountNode(); ++i)
    {
        quint32 id = 0;
        const VNodeDetail &nodeD = detail.at(i);
        switch (nodeD.getTypeTool())
        {
            case (Tool::NodePoint):
            {
                id = CreateNode<VPointF>(data, nodeD.getId());
                VNodePoint::Create(doc, data, scene, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeArc):
            {
                id = CreateNode<VArc>(data, nodeD.getId());
                VNodeArc::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSpline):
            {
                const auto obj = data->GetGObject(nodeD.getId());
                if (obj->getType() == GOType::Spline)
                {
                    id = CreateNode<VSpline>(data, nodeD.getId());
                }
                else
                {
                    id = CreateNode<VCubicBezier>(data, nodeD.getId());
                }
                VNodeSpline::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            case (Tool::NodeSplinePath):
            {
                const auto obj = data->GetGObject(nodeD.getId());
                if (obj->getType() == GOType::SplinePath)
                {
                    id = CreateNode<VSplinePath>(data, nodeD.getId());
                }
                else
                {
                    id = CreateNode<VCubicBezierPath>(data, nodeD.getId());
                }
                VNodeSplinePath::Create(doc, data, id, nodeD.getId(), Document::FullParse, Source::FromGui);
            }
            break;
            default:
                qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        VNodeDetail node(id, nodeD.getTypeTool(), NodeDetail::Contour, nodeD.getMx(), nodeD.getMy(),
                         nodeD.getReverse());
        det.append(node);
    }
    det.setName(detail.getName());
    det.setWidth(detail.getWidth());
    det.setClosed(detail.getClosed());
    det.setSeamAllowance(detail.getSeamAllowance());
    det.setForbidFlipping(detail.getForbidFlipping());
    det.SetPatternPieceData(detail.GetPatternPieceData());
    det.SetPatternInfo(detail.GetPatternInfo());
    Create(0, det, scene, doc, data, Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param newDetail detail what we want show.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
void VToolDetail::Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene *scene, VAbstractPattern *doc,
                         VContainer *data, const Document &parse, const Source &typeCreation, const QString &drawName)
{
    quint32 id = _id;
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        id = data->AddDetail(newDetail);
    }
    else
    {
        data->UpdateDetail(id, newDetail);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::Detail, doc);
    if (parse == Document::FullParse)
    {
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene, drawName);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::EnableDetailItemHover, detail, &VToolDetail::AllowHover);
        connect(scene, &VMainGraphicsScene::EnableDetailItemSelection, detail, &VToolDetail::AllowSelecting);
        connect(scene, &VMainGraphicsScene::HighlightDetail, detail, &VToolDetail::Highlight);
        doc->AddTool(id, detail);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Remove full delete detail.
 */
void VToolDetail::Remove(bool ask)
{
    try
    {
        DeleteTool(ask);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolDetail::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiOk refresh tool data after change in options.
 * @param result keep result working dialog.
 */
void VToolDetail::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SCASSERT(dialog != nullptr);
        DialogDetail *dialogTool = qobject_cast<DialogDetail*>(dialog);
        SCASSERT(dialogTool != nullptr);
        const VDetail newDet = dialogTool->getDetail();
        const VDetail oldDet = VAbstractTool::data.GetDetail(id);

        qDebug() << "VToolDetail Position" << newDet.GetPatternPieceData().GetPos();
        SaveDetailOptions *saveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
        connect(saveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveCommand);
        UpdateLabel();
    }
    delete dialog;
    dialog = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail::paint draws a bounding box around detail, if one of its text or grainline items is not idle.
 */
void VToolDetail::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (scene()->views().count() > 0)
    {
        const QPoint pt0 = scene()->views().at(0)->mapFromScene(0, 0);
        const QPoint pt = scene()->views().at(0)->mapFromScene(0, 100);

        const QPoint p = pt - pt0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
        const qreal dScale = qSqrt(QPoint::dotProduct(p, p));
#else
        const qreal dScale = qSqrt(p.x() * p.x() + p.y() * p.y());
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
        grainLine->SetScale(100/dScale);
        //qDebug() << "SCALE" << dScale << 10/dScale;
    }

    if (dataLabel->IsIdle() == false || patternInfo->IsIdle() == false || grainLine->IsIdle() == false)
    {
        painter->save();
        painter->setPen(QPen(Qt::black, 3, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));
        painter->restore();
    }
    VNoBrushScalePathItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolDetail::AddToFile()
{
    const VDetail detail = VAbstractTool::data.GetDetail(id);
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrName, detail.getName());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(detail.getMx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(detail.getMy()));
    doc->SetAttribute(domElement, AttrSupplement, static_cast<quint8>(detail.getSeamAllowance()));
    doc->SetAttribute(domElement, AttrClosed, static_cast<quint8>(detail.getClosed()));
    doc->SetAttribute(domElement, AttrWidth, detail.getWidth());
    doc->SetAttribute(domElement, AttrForbidFlipping, static_cast<quint8>(detail.getForbidFlipping()));

    // detail data
    QDomElement domData = doc->createElement(VAbstractPattern::TagData);
    const VPatternPieceData& data = detail.GetPatternPieceData();
    doc->SetAttribute(domData, VAbstractPattern::AttrLetter, data.GetLetter());
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, data.IsVisible() == true? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, data.GetPos().x());
    doc->SetAttribute(domData, AttrMy, data.GetPos().y());
    doc->SetAttribute(domData, AttrWidth, data.GetLabelWidth());
    doc->SetAttribute(domData, AttrHeight, data.GetLabelHeight());
    doc->SetAttribute(domData, AttrFont, data.GetFontSize());
    doc->SetAttribute(domData, AttrRotation, data.GetRotation());

    for (int i = 0; i < data.GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = data.GetMCP(i);
        QDomElement domMCP = doc->createElement(VAbstractPattern::TagMCP);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrMaterial, int(mcp.m_eMaterial));
        if (mcp.m_eMaterial == MaterialType::mtUserDefined)
        {
            doc->SetAttribute(domMCP, VAbstractPattern::AttrUserDefined, mcp.m_qsMaterialUserDef);
        }
        doc->SetAttribute(domMCP, VAbstractPattern::AttrCutNumber, mcp.m_iCutNumber);
        doc->SetAttribute(domMCP, VAbstractPattern::AttrPlacement, int(mcp.m_ePlacement));
        domData.appendChild(domMCP);
    }
    domElement.appendChild(domData);

    // pattern info
    domData = doc->createElement(VAbstractPattern::TagPatternInfo);
    const VPatternInfoGeometry& geom = detail.GetPatternInfo();
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, geom.IsVisible() == true? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, geom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, geom.GetPos().y());
    doc->SetAttribute(domData, AttrWidth, geom.GetLabelWidth());
    doc->SetAttribute(domData, AttrHeight, geom.GetLabelHeight());
    doc->SetAttribute(domData, AttrFont, geom.GetFontSize());
    doc->SetAttribute(domData, AttrRotation, geom.GetRotation());
    domElement.appendChild(domData);

    // grainline
    domData = doc->createElement(VAbstractPattern::TagGrainline);
    const VGrainlineGeometry& glGeom = detail.GetGrainlineGeometry();
    doc->SetAttribute(domData, VAbstractPattern::AttrVisible, glGeom.IsVisible() == true? trueStr : falseStr);
    doc->SetAttribute(domData, AttrMx, glGeom.GetPos().x());
    doc->SetAttribute(domData, AttrMy, glGeom.GetPos().y());
    doc->SetAttribute(domData, AttrLength, glGeom.GetLength());
    doc->SetAttribute(domData, AttrRotation, glGeom.GetRotation());
    doc->SetAttribute(domData, VAbstractPattern::AttrArrows, int(glGeom.GetArrowType()));
    qDebug() << "XML ROTATION" << glGeom.GetRotation();

    // nodes
    for (int i = 0; i < detail.CountNode(); ++i)
    {
       AddNode(doc, domElement, detail.at(i));
    }

    AddDet *addDet = new AddDet(domElement, doc, detail, drawName);
    connect(addDet, &AddDet::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addDet);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolDetail::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        VDetail det = VAbstractTool::data.GetDetail(id);
        doc->SetAttribute(domElement, AttrName, det.getName());
        doc->SetAttribute(domElement, AttrSupplement, QString().setNum(static_cast<quint8>(det.getSeamAllowance())));
        doc->SetAttribute(domElement, AttrClosed, QString().setNum(static_cast<quint8>(det.getClosed())));
        doc->SetAttribute(domElement, AttrWidth, QString().setNum(det.getWidth()));
        doc->RemoveAllChildren(domElement);

        // detail data
        QDomElement domData = doc->createElement(VAbstractPattern::TagData);
        const VPatternPieceData& data = det.GetPatternPieceData();
        doc->SetAttribute(domData, VAbstractPattern::AttrLetter, data.GetLetter());
        doc->SetAttribute(domData, VAbstractPattern::AttrVisible, data.IsVisible() == true? trueStr : falseStr);
        doc->SetAttribute(domData, AttrMx, data.GetPos().x());
        doc->SetAttribute(domData, AttrMy, data.GetPos().y());
        doc->SetAttribute(domData, AttrWidth, data.GetLabelWidth());
        doc->SetAttribute(domData, AttrHeight, data.GetLabelHeight());
        doc->SetAttribute(domData, AttrFont, data.GetFontSize());
        doc->SetAttribute(domData, AttrRotation, data.GetRotation());

        for (int i = 0; i < data.GetMCPCount(); ++i)
        {
            MaterialCutPlacement mcp = data.GetMCP(i);
            QDomElement domMCP = doc->createElement(VAbstractPattern::TagMCP);
            doc->SetAttribute(domMCP, VAbstractPattern::AttrMaterial, int(mcp.m_eMaterial));
            if (mcp.m_eMaterial == MaterialType::mtUserDefined)
            {
                doc->SetAttribute(domMCP, VAbstractPattern::AttrUserDefined, mcp.m_qsMaterialUserDef);
            }
            else
            {
                domMCP.removeAttribute(VAbstractPattern::AttrUserDefined);
            }
            doc->SetAttribute(domMCP, VAbstractPattern::AttrCutNumber, mcp.m_iCutNumber);
            doc->SetAttribute(domMCP, VAbstractPattern::AttrPlacement, int(mcp.m_ePlacement));
            domData.appendChild(domMCP);
        }
        domElement.appendChild(domData);

        // pattern info
        domData = doc->createElement(VAbstractPattern::TagPatternInfo);
        const VPatternInfoGeometry& geom = det.GetPatternInfo();
        doc->SetAttribute(domData, VAbstractPattern::AttrVisible, geom.IsVisible() == true? trueStr : falseStr);
        doc->SetAttribute(domData, AttrMx, geom.GetPos().x());
        doc->SetAttribute(domData, AttrMy, geom.GetPos().y());
        doc->SetAttribute(domData, AttrWidth, geom.GetLabelWidth());
        doc->SetAttribute(domData, AttrHeight, geom.GetLabelHeight());
        doc->SetAttribute(domData, AttrFont, geom.GetFontSize());
        doc->SetAttribute(domData, AttrRotation, geom.GetRotation());

        // grainline
        domData = doc->createElement(VAbstractPattern::TagGrainline);
        const VGrainlineGeometry& glGeom = det.GetGrainlineGeometry();
        doc->SetAttribute(domData, VAbstractPattern::AttrVisible, glGeom.IsVisible() == true? trueStr : falseStr);
        doc->SetAttribute(domData, AttrMx, glGeom.GetPos().x());
        doc->SetAttribute(domData, AttrMy, glGeom.GetPos().y());
        doc->SetAttribute(domData, AttrLength, glGeom.GetLength());
        doc->SetAttribute(domData, AttrRotation, glGeom.GetRotation());
        doc->SetAttribute(domData, VAbstractPattern::AttrArrows, int(glGeom.GetArrowType()));

        // nodes
        for (int i = 0; i < det.CountNode(); ++i)
        {
           AddNode(doc, domElement, det.at(i));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle detail change.
 * @param change change
 * @param value value
 * @return new value.
 */
QVariant VToolDetail::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
            // Each time we move something we call recalculation scene rect. In some cases this can cause moving
            // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
            static bool changeFinished = true;
            if (changeFinished)
            {
               changeFinished = false;

               // value - this is new position.
               const QPointF newPos = value.toPointF();

               MoveDetail *moveDet = new MoveDetail(doc, newPos.x(), newPos.y(), id, scene());
               connect(moveDet, &MoveDetail::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
               qApp->getUndoStack()->push(moveDet);

               const QList<QGraphicsView *> viewList = scene()->views();
               if (not viewList.isEmpty())
               {
                   if (QGraphicsView *view = viewList.at(0))
                   {
                       const int xmargin = 50;
                       const int ymargin = 50;

                       const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                       const QRectF itemRect = mapToScene(boundingRect()|childrenBoundingRect()).boundingRect();

                       // If item's rect is bigger than view's rect ensureVisible works very unstable.
                       if (itemRect.height() + 2*ymargin < viewRect.height() &&
                           itemRect.width() + 2*xmargin < viewRect.width())
                       {
                            view->ensureVisible(itemRect, xmargin, ymargin);
                       }
                       else
                       {
                           // Ensure visible only small rect around a cursor
                           VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                           SCASSERT(currentScene);
                           const QPointF cursorPosition = currentScene->getScenePos();
                           view->ensureVisible(QRectF(cursorPosition.x()-5, cursorPosition.y()-5, 10, 10));
                       }
                   }
               }
               // Don't forget to update geometry, because first change never call full parse
               RefreshGeometry();
               changeFinished = true;
        }
    }

    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return VNoBrushScalePathItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key release events.
 * @param event key release event.
 */
void VToolDetail::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            try
            {
                DeleteTool();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e);
                return;//Leave this method immediately!!!
            }
            break;
        default:
            break;
    }
    VNoBrushScalePathItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    VNoBrushScalePathItem::mousePressEvent(event);

    // Somehow clicking on notselectable object do not clean previous selections.
    if (not (flags() & ItemIsSelectable) && scene())
    {
        scene()->clearSelection();
    }

    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }

    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        doc->SelectedDetail(id);
        emit ChoosedTool(id, SceneObject::Detail);
    }

    event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VToolDetail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //Disable cursor-arrow-closehand
        RestoreOverrideCursor(cursorArrowCloseHand);
    }
    VNoBrushScalePathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    //Disable cursor-arrow-openhand
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolDetail::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

    QAction *inLayoutOption = menu.addAction(tr("In layout"));
    inLayoutOption->setCheckable(true);
    const VDetail detail = VAbstractTool::data.GetDetail(id);
    inLayoutOption->setChecked(detail.IsInLayout());

    QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    _referens > 1 ? actionRemove->setEnabled(false) : actionRemove->setEnabled(true);

    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        dialog = new DialogDetail(getData(), id, qApp->getMainWindow());
        dialog->setModal(true);
        connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                dialog, &DialogTool::ChosenObject);
        connect(dialog, &DialogTool::DialogClosed, this, &VToolDetail::FullUpdateFromGuiOk);
        setDialog();
        dialog->show();
    }
    else if (selectedAction == inLayoutOption)
    {
        ToggleDetailInLayout *togglePrint = new ToggleDetailInLayout(id, selectedAction->isChecked(),
                                                                     &(VAbstractTool::data), doc);
        connect(togglePrint, &ToggleDetailInLayout::UpdateList, doc, &VAbstractPattern::CheckInLayoutList);
        qApp->getUndoStack()->push(togglePrint);
    }
    else if (selectedAction == actionRemove)
    {
        try
        {
            DeleteTool();
        }
        catch(const VExceptionToolWasDeleted &e)
        {
            Q_UNUSED(e);
            return;//Leave this method immediately!!!
        }
        return; //Leave this method immediately after call!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateLabel updates the text label, making it just big enough for the text to fit it
 */
void VToolDetail::UpdateLabel()
{
    const VDetail detail = VAbstractTool::data.GetDetail(id);
    const VPatternPieceData& data = detail.GetPatternPieceData();

    if (data.IsVisible() == true)
    {
        QFont fnt = qApp->font();
        {
            const int iFS = data.GetFontSize();
            iFS < MIN_FONT_SIZE ? fnt.setPixelSize(MIN_FONT_SIZE) : fnt.setPixelSize(iFS);
        }
        dataLabel->SetFont(fnt);
        dataLabel->SetSize(data.GetLabelWidth(), data.GetLabelHeight());
        dataLabel->UpdateData(detail.getName(), data);
        QPointF pt = data.GetPos();
        QRectF rectBB;
        rectBB.setTopLeft(pt);
        rectBB.setWidth(dataLabel->boundingRect().width());
        rectBB.setHeight(dataLabel->boundingRect().height());
        qreal dX;
        qreal dY;
        if (dataLabel->IsContained(rectBB, data.GetRotation(), dX, dY) == false)
        {
            pt.setX(pt.x() + dX);
            pt.setY(pt.y() + dY);
        }

        dataLabel->setPos(pt);
        dataLabel->setRotation(data.GetRotation());
        dataLabel->Update();
        dataLabel->show();
    }
    else
    {
        dataLabel->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePatternInfo updates the pattern info label
 */
void VToolDetail::UpdatePatternInfo()
{
    const VDetail detail = VAbstractTool::data.GetDetail(id);
    const VPatternInfoGeometry& geom = detail.GetPatternInfo();

    if (geom.IsVisible() == true)
    {
        QFont fnt = qApp->font();
        int iFS = geom.GetFontSize();
        if (iFS < MIN_FONT_SIZE)
        {
            iFS = MIN_FONT_SIZE;
        }
        fnt.setPixelSize(iFS);
        patternInfo->SetFont(fnt);
        patternInfo->SetSize(geom.GetLabelWidth(), geom.GetLabelHeight());
        patternInfo->UpdateData(doc, getData()->size(), getData()->height());

        QPointF pt = geom.GetPos();
        QRectF rectBB;
        rectBB.setTopLeft(pt);
        rectBB.setWidth(patternInfo->boundingRect().width());
        rectBB.setHeight(patternInfo->boundingRect().height());
        qreal dX;
        qreal dY;
        if (patternInfo->IsContained(rectBB, geom.GetRotation(), dX, dY) == false)
        {
            pt.setX(pt.x() + dX);
            pt.setY(pt.y() + dY);
        }

        patternInfo->setPos(pt);
        patternInfo->setRotation(geom.GetRotation());
        patternInfo->Update();
        if (patternInfo->GetTextLines() > 0)
        {
            patternInfo->show();
        }
        else
        {
            patternInfo->hide();
        }
    }
    else
    {
        patternInfo->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolDetail::UpdateGrainline updates the grain line item
 */
void VToolDetail::UpdateGrainline()
{
    const VDetail detail = VAbstractTool::data.GetDetail(id);
    const VGrainlineGeometry& geom = detail.GetGrainlineGeometry();

    if (geom.IsVisible() == true)
    {
        qreal dRotation;
        qreal dLength;
        try
        {
            QString qsFormula;
            qsFormula = geom.GetRotation().replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());

            Calculator cal1;
            dRotation = cal1.EvalFormula(VDataTool::data.PlainVariables(), qsFormula);

            qsFormula = geom.GetLength().replace("\n", " ");
            qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
            Calculator cal2;
            dLength = cal2.EvalFormula(VDataTool::data.PlainVariables(), qsFormula);
        }
        catch(...)
        {
            grainLine->hide();
            return;
        }

        grainLine->UpdateGeometry(geom.GetPos(), dRotation, ToPixel(dLength, *VDataTool::data.GetPatternUnit()),
                                  geom.GetArrowType());
        grainLine->show();
    }
    else
    {
        grainLine->hide();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveMoveDetail saves the move detail operation to the undo stack
 */
void VToolDetail::SaveMoveDetail(const QPointF& ptPos)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(ptPos);
    newDet.GetPatternPieceData().SetLabelWidth(dataLabel->boundingRect().width());
    newDet.GetPatternPieceData().SetLabelHeight(dataLabel->boundingRect().height());
    newDet.GetPatternPieceData().SetFontSize(dataLabel->GetFontSize());
    newDet.GetPatternPieceData().SetRotation(dataLabel->rotation());

    SaveDetailOptions* moveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    moveCommand->setText(tr("move pattern piece label"));
    connect(moveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveResizeDetail saves the resize detail label operation to the undo stack
 */
void VToolDetail::SaveResizeDetail(qreal dLabelW, int iFontSize)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternPieceData().SetLabelWidth(dLabelW);
    newDet.GetPatternPieceData().SetLabelHeight(dataLabel->boundingRect().height());
    newDet.GetPatternPieceData().SetFontSize(iFontSize);
    newDet.GetPatternPieceData().SetRotation(dataLabel->rotation());
    SaveDetailOptions* resizeCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    resizeCommand->setText(tr("resize pattern piece label"));
    connect(resizeCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveRotationDetail saves the rotation detail label operation to the undo stack
 */
void VToolDetail::SaveRotationDetail(qreal dRot)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternPieceData().SetPos(dataLabel->pos());
    newDet.GetPatternPieceData().SetLabelWidth(dataLabel->boundingRect().width());
    newDet.GetPatternPieceData().SetLabelHeight(dataLabel->boundingRect().height());
    newDet.GetPatternPieceData().SetFontSize(dataLabel->GetFontSize());
    newDet.GetPatternPieceData().SetRotation(dRot);

    SaveDetailOptions* rotateCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    rotateCommand->setText(tr("rotate pattern piece label"));
    connect(rotateCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveMovePattern saves the pattern label position
 */
void VToolDetail::SaveMovePattern(const QPointF &ptPos)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternInfo().SetPos(ptPos);
    newDet.GetPatternInfo().SetLabelWidth(patternInfo->boundingRect().width());
    newDet.GetPatternInfo().SetLabelHeight(patternInfo->boundingRect().height());
    newDet.GetPatternInfo().SetFontSize(patternInfo->GetFontSize());
    newDet.GetPatternInfo().SetRotation(patternInfo->rotation());

    SaveDetailOptions* moveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    moveCommand->setText(tr("move pattern info label"));
    connect(moveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief: SaveResizePattern saves the pattern label width and font size
 */
void VToolDetail::SaveResizePattern(qreal dLabelW, int iFontSize)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternInfo().SetLabelWidth(dLabelW);
    newDet.GetPatternInfo().SetLabelHeight(patternInfo->boundingRect().height());
    newDet.GetPatternInfo().SetFontSize(iFontSize);
    newDet.GetPatternInfo().SetRotation(patternInfo->rotation());
    SaveDetailOptions* resizeCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    resizeCommand->setText(tr("resize pattern info label"));
    connect(resizeCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::SaveRotationPattern(qreal dRot)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetPatternInfo().SetPos(patternInfo->pos());
    newDet.GetPatternInfo().SetLabelWidth(patternInfo->boundingRect().width());
    newDet.GetPatternInfo().SetLabelHeight(patternInfo->boundingRect().height());
    newDet.GetPatternInfo().SetFontSize(patternInfo->GetFontSize());
    newDet.GetPatternInfo().SetRotation(dRot);

    SaveDetailOptions* rotateCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    rotateCommand->setText(tr("rotate pattern info label"));
    connect(rotateCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}


//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::SaveMoveGrainline(const QPointF& ptPos)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;
    newDet.GetGrainlineGeometry().SetPos(ptPos);
    qDebug() << "******* new grainline pos" << ptPos;

    SaveDetailOptions* moveCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    moveCommand->setText(tr("move grainline"));
    connect(moveCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::SaveResizeGrainline(qreal dLength)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;

    dLength = FromPixel(dLength, *VDataTool::data.GetPatternUnit());
    newDet.GetGrainlineGeometry().SetLength(qApp->LocaleToString(dLength));
    SaveDetailOptions* resizeCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    resizeCommand->setText(tr("resize grainline"));
    connect(resizeCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(resizeCommand);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::SaveRotateGrainline(qreal dRot, const QPointF& ptPos)
{
    VDetail oldDet = VAbstractTool::data.GetDetail(id);
    VDetail newDet = oldDet;

    dRot = qRadiansToDegrees(dRot);
    newDet.GetGrainlineGeometry().SetRotation(qApp->LocaleToString(dRot));
    newDet.GetGrainlineGeometry().SetPos(ptPos);
    SaveDetailOptions* rotateCommand = new SaveDetailOptions(oldDet, newDet, doc, id, this->scene());
    rotateCommand->setText(tr("rotate grainline"));
    connect(rotateCommand, &SaveDetailOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(rotateCommand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddNode add node to the file.
 * @param domElement tag in xml tree.
 * @param node node of detail.
 */
void VToolDetail::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VNodeDetail &node)
{
    QDomElement nod = doc->createElement(TagNode);

    doc->SetAttribute(nod, AttrIdObject, node.getId());
    doc->SetAttribute(nod, AttrMx, qApp->fromPixel(node.getMx()));
    doc->SetAttribute(nod, AttrMy, qApp->fromPixel(node.getMy()));

    if (node.getTypeTool() != Tool::NodePoint)
    {
        doc->SetAttribute(nod, AttrReverse, static_cast<quint8>(node.getReverse()));
    }

    if (node.getTypeNode() == NodeDetail::Contour)
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
    {
        case (Tool::NodeArc):
            doc->SetAttribute(nod, AttrType, NodeArc);
            break;
        case (Tool::NodePoint):
            doc->SetAttribute(nod, AttrType, NodePoint);
            break;
        case (Tool::NodeSpline):
            doc->SetAttribute(nod, AttrType, NodeSpline);
            break;
        case (Tool::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, NodeSplinePath);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDetail::getTagName() const
{
    return VAbstractPattern::TagDetail;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object);
    Q_UNUSED(visible);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VToolDetail::RefreshGeometry()
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

    const VDetail detail = VAbstractTool::data.GetDetail(id);
    QPainterPath mainPath = detail.ContourPath(this->getData());
    this->setPath(mainPath);
    this->setPos(detail.getMx(), detail.getMy());

    if (detail.getSeamAllowance())
    {
        mainPath.addPath(detail.SeamAllowancePath(this->getData()));
        mainPath.setFillRule(Qt::OddEvenFill);
        seamAllowance->setPath(mainPath);
    }
    else
    {
        seamAllowance->setPath(QPainterPath());
    }

    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::DeleteTool(bool ask)
{
    DeleteDetail *delDet = new DeleteDetail(doc, id, VAbstractTool::data.GetDetail(id));
    if (ask)
    {
        if (ConfirmDeletion() == QMessageBox::No)
        {
            return;
        }
        /* If UnionDetails tool delete detail no need emit FullParsing.*/
        connect(delDet, &DeleteDetail::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    }
    qApp->getUndoStack()->push(delDet);

    // Throw exception, this will help prevent case when we forget to immediately quit function.
    VExceptionToolWasDeleted e("Tool was used after deleting.");
    throw e;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Tool>
//cppcheck-suppress unusedFunction
Tool* VToolDetail::InitTool(VMainGraphicsScene *scene, const VNodeDetail &node)
{
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    Tool *tool = qobject_cast<Tool*>(tools->value(node.getId()));
    SCASSERT(tool != nullptr);
    connect(tool, &Tool::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    tool->setParentItem(this);
    tool->SetParentType(ParentType::Item);
    doc->IncrementReferens(node.getId());
    return tool;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::EnableToolMove(bool move)
{
    setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::AllowHover(bool enabled)
{
    setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::AllowSelecting(bool enabled)
{
    setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::ResetChildren(QGraphicsItem *pItem)
{
    VTextGraphicsItem* pVGI = dynamic_cast<VTextGraphicsItem*>(pItem);
    if (pVGI != dataLabel)
    {
        dataLabel->Reset();
    }
    if (pVGI != patternInfo)
    {
        patternInfo->Reset();
    }
    VGrainlineItem* pGLI = dynamic_cast<VGrainlineItem*>(pItem);
    if (pGLI != grainLine)
    {
        grainLine->Reset();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::UpdateAll()
{
    sceneDetails->update();
    update();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::retranslateUi()
{
    UpdateLabel();
    UpdatePatternInfo();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDetail::Highlight(quint32 id)
{
    setSelected(this->id == id);
}

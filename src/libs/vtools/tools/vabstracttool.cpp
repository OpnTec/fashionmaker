/************************************************************************
 **
 **  @file   vabstracttool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vabstracttool.h"

#include <QBrush>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFlags>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QHash>
#include <QIcon>
#include <QLineF>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QStyle>
#include <QUndoStack>
#include <QVector>
#include <new>
#include <qnumeric.h>

#include "../vgeometry/vpointf.h"
#include "../vpropertyexplorer/checkablemessagebox.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionundo.h"
#include "../ifc/xml/vtoolrecord.h"
#include "../undocommands/deltool.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/calculator.h"
#include "../vwidgets/vgraphicssimpletextitem.h"
#include "nodeDetails/nodedetails.h"
#include "../dialogs/support/dialogundo.h"
#include "../dialogs/support/dialogeditwrongformula.h"

template <class T> class QSharedPointer;

bool VAbstractTool::m_suppressContextMenu = false;
const QString VAbstractTool::AttrInUse = QStringLiteral("inUse");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractTool container.
 * @param doc dom document container.
 * @param data container with data.
 * @param id object id in container.
 * @param parent parent object.
 */
VAbstractTool::VAbstractTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VDataTool(data, parent),
      doc(doc),
      m_id(id),
      vis(),
      selectionType(SelectionType::ByMouseRelease)
{
    SCASSERT(doc != nullptr)
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VAbstractPattern::haveLiteChange);
    connect(this->doc, &VAbstractPattern::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::LiteUpdateTree, this->doc, &VAbstractPattern::LiteParseTree);
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractTool::~VAbstractTool()
{
    if (not vis.isNull())
    {
        delete vis;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckFormula check formula.
 *
 * Try calculate formula. If find error show dialog that allow user try fix formula. If user can't throw exception. In
 * successes case return result calculation and fixed formula string. If formula ok don't touch formula.
 *
 * @param toolId [in] tool's id.
 * @param formula [in|out] string with formula.
 * @param data [in] container with variables. Need for math parser.
 * @throw QmuParserError.
 * @return result of calculation formula.
 */
qreal VAbstractTool::CheckFormula(const quint32 &toolId, QString &formula, VContainer *data)
{
    SCASSERT(data != nullptr)
    qreal result = 0;
    try
    {
        QScopedPointer<Calculator> cal(new Calculator());
        result = cal->EvalFormula(data->DataVariables(), formula);

        if (qIsInf(result) || qIsNaN(result))
        {
            qDebug() << "Invalid the formula value";
            return 0;
        }
    }
    catch (qmu::QmuParserError &e)
    {
        qDebug() << "\nMath parser error:\n"
                 << "--------------------------------------\n"
                 << "Message:     " << e.GetMsg()  << "\n"
                 << "Expression:  " << e.GetExpr() << "\n"
                 << "--------------------------------------";

        if (qApp->IsAppInGUIMode())
        {
            QScopedPointer<DialogUndo> dialogUndo(new DialogUndo(qApp->getMainWindow()));
            forever
            {
                if (dialogUndo->exec() == QDialog::Accepted)
                {
                    const UndoButton resultUndo = dialogUndo->Result();
                    if (resultUndo == UndoButton::Fix)
                    {
                        auto *dialog = new DialogEditWrongFormula(data, toolId, qApp->getMainWindow());
                        dialog->setWindowTitle(tr("Edit wrong formula"));
                        dialog->SetFormula(formula);
                        if (dialog->exec() == QDialog::Accepted)
                        {
                            formula = dialog->GetFormula();
                            /* Need delete dialog here because parser in dialog don't allow use correct separator for
                             * parsing here. */
                            delete dialog;
                            QScopedPointer<Calculator> cal1(new Calculator());
                            result = cal1->EvalFormula(data->DataVariables(), formula);

                            if (qIsInf(result) || qIsNaN(result))
                            {
                                qDebug() << "Invalid the formula value";
                                return 0;
                            }

                            break;
                        }
                        else
                        {
                            delete dialog;
                        }
                    }
                    else
                    {
                        throw VExceptionUndo(QString("Undo wrong formula %1").arg(formula));
                    }
                }
                else
                {
                    throw;
                }
            }
        }
        else
        {
            throw;
        }
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteTool full delete object form scene and file.
 */
void VAbstractTool::DeleteToolWithConfirm(bool ask)
{
    qCDebug(vTool, "Deleting abstract tool.");
    if (_referens == 0)
    {
        qCDebug(vTool, "No children.");
        emit qApp->getSceneView()->itemClicked(nullptr);
        if (ask)
        {
            qCDebug(vTool, "Asking.");
            if (ConfirmDeletion() == QMessageBox::No)
            {
                qCDebug(vTool, "User said no.");
                return;
            }
        }

        PerformDelete();

        // Throw exception, this will help prevent case when we forget to immediately quit function.
        VExceptionToolWasDeleted e("Tool was used after deleting.");
        throw e;
    }
    else
    {
        qCDebug(vTool, "Can't delete, tool has children.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::PerformDelete()
{
    qCDebug(vTool, "Begin deleting.");
    DelTool *delTool = new DelTool(doc, m_id);
    connect(delTool, &DelTool::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(delTool);
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractTool::ConfirmDeletion()
{
    if (false == qApp->Settings()->GetConfirmItemDelete())
    {
        return QMessageBox::Yes;
    }

    Utils::CheckableMessageBox msgBox(qApp->getMainWindow());
    msgBox.setWindowTitle(tr("Confirm deletion"));
    msgBox.setText(tr("Do you really want to delete?"));
    msgBox.setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
    msgBox.setDefaultButton(QDialogButtonBox::No);
    msgBox.setIconPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32, 32) );

    int dialogResult = msgBox.exec();

    if (dialogResult == QDialog::Accepted)
    {
        qApp->Settings()->SetConfirmItemDelete(not msgBox.isChecked());
    }

    return dialogResult == QDialog::Accepted ? QMessageBox::Yes : QMessageBox::No;
}

//---------------------------------------------------------------------------------------------------------------------
const QStringList VAbstractTool::Colors()
{
    const QStringList colors = QStringList() << ColorBlack          << ColorGreen           << ColorBlue
                                             << ColorDarkRed        << ColorDarkGreen       << ColorDarkBlue
                                             << ColorYellow         << ColorLightSalmon     << ColorGoldenRod
                                             << ColorOrange         << ColorDeepPink        << ColorViolet
                                             << ColorDarkViolet     << ColorMediumSeaGreen  << ColorLime
                                             << ColorDeepSkyBlue    << ColorCornFlowerBlue;
    return colors;
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> VAbstractTool::ColorsList()
{
    QMap<QString, QString> map;

    const QStringList colorNames = Colors();
    for (int i = 0; i < colorNames.size(); ++i)
    {
        QString name;
        switch (i)
        {
            case 1: // ColorGreen
                name = tr("green");
                break;
            case 2: // ColorBlue
                name = tr("blue");
                break;
            case 3: // ColorDarkRed
                name = tr("dark red");
                break;
            case 4: // ColorDarkGreen
                name = tr("dark green");
                break;
            case 5: // ColorDarkBlue
                name = tr("dark blue");
                break;
            case 6: // ColorYellow
                name = tr("yellow");
                break;
            case 7: // ColorLightSalmon
                name = tr("light salmon");
                break;
            case 8: // ColorGoldenRod
                name = tr("goldenrod");
                break;
            case 9: // ColorOrange
                name = tr("orange");
                break;
            case 10: // ColorDeepPink
                name = tr("deep pink");
                break;
            case 11: // ColorViolet
                name = tr("violet");
                break;
            case 12: // ColorDarkViolet
                name = tr("dark violet");
                break;
            case 13: // ColorMediumSeaGreen
                name = tr("medium sea green");
                break;
            case 14: // ColorLime
                name = tr("lime");
                break;
            case 15: // ColorDeepSkyBlue
                name = tr("deep sky blue");
                break;
            case 16: // ColorCornFlowerBlue
                name = tr("corn flower blue");
                break;
            case 0: // ColorBlack
            default:
                name = tr("black");
                break;
        }

        map.insert(colorNames.at(i), name);
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QMap<QString, quint32> VAbstractTool::PointsList() const
{
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data.CalculationGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != m_id)
        {
            QSharedPointer<VGObject> obj = i.value();
            if (obj->getType() == GOType::Point)
            {
                const QSharedPointer<VPointF> point = data.GeometricObject<VPointF>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::ChangeLabelPosition(quint32 id, const QPointF &pos)
{
    Q_UNUSED(id)
    Q_UNUSED(pos)
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::SetLabelVisible(quint32 id, bool visible)
{
    Q_UNUSED(id)
    Q_UNUSED(visible)
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::ToolSelectionType(const SelectionType &type)
{
    selectionType = type;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::RefreshDataInFile()
{
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::ToolCreation(const Source &typeCreation)
{
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VToolRecord VAbstractTool::GetRecord(const quint32 id, const Tool &toolType, VAbstractPattern *doc)
{
    const QVector<VToolRecord> *history = doc->getHistory();
    for(auto &record : *history)
    {
        if (record.getId() == id && record.getTypeTool() == toolType)
        {
            return record;
        }
    }
    return VToolRecord();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::RemoveRecord(const VToolRecord &record, VAbstractPattern *doc)
{
    QVector<VToolRecord> *history = doc->getHistory();
    for(int i = 0; i < history->size(); ++i)
    {
        if (history->at(i) == record)
        {
            history->remove(i);
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::AddRecord(const VToolRecord &record, VAbstractPattern *doc)
{
    QVector<VToolRecord> *history = doc->getHistory();
    if (history->contains(record))
    {
        return;
    }

    quint32 cursor = doc->getCursor();
    if (cursor == NULL_ID)
    {
        history->append(record);
    }
    else
    {
        qint32 index = 0;
        for (qint32 i = 0; i<history->size(); ++i)
        {
            VToolRecord rec = history->at(i);
            if (rec.getId() == cursor)
            {
                index = i;
                break;
            }
        }
        history->insert(index+1, record);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddRecord add record about tool in history.
 * @param id object id in container
 * @param toolType tool type
 * @param doc dom document container
 */
void VAbstractTool::AddRecord(const quint32 id, const Tool &toolType, VAbstractPattern *doc)
{
    AddRecord(VToolRecord(id, toolType, doc->GetNameActivPP()), doc);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiecePath &path)
{
    if (path.CountNodes() > 0)
    {
        QDomElement nodesElement = doc->createElement(VAbstractPattern::TagNodes);
        for (int i = 0; i < path.CountNodes(); ++i)
        {
            AddNode(doc, nodesElement, path.at(i));
        }
        domElement.appendChild(nodesElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece)
{
    AddNodes(doc, domElement, piece.GetPath());
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VAbstractTool::AddSANode(VAbstractPattern *doc, const QString &tagName, const VPieceNode &node)
{
    QDomElement nod = doc->createElement(tagName);

    doc->SetAttribute(nod, AttrIdObject, node.GetId());

    const Tool type = node.GetTypeTool();
    if (type != Tool::NodePoint)
    {
        doc->SetAttribute(nod, VAbstractPattern::AttrNodeReverse, static_cast<quint8>(node.GetReverse()));
    }
    else
    {
        if (node.GetFormulaSABefore() != currentSeamAllowance)
        {
            doc->SetAttribute(nod, VAbstractPattern::AttrSABefore, node.GetFormulaSABefore());
        }

        if (node.GetFormulaSAAfter() != currentSeamAllowance)
        {
            doc->SetAttribute(nod, VAbstractPattern::AttrSAAfter, node.GetFormulaSAAfter());
        }
    }

    doc->SetAttributeOrRemoveIf(nod, VAbstractPattern::AttrNodeExcluded, node.IsExcluded(), not node.IsExcluded());
    doc->SetAttributeOrRemoveIf(nod, VAbstractPattern::AttrCheckUniqueness, node.IsCheckUniqueness(),
                                node.IsCheckUniqueness());

    switch (type)
    {
        case (Tool::NodeArc):
            doc->SetAttribute(nod, AttrType, VAbstractPattern::NodeArc);
            break;
        case (Tool::NodeElArc):
            doc->SetAttribute(nod, AttrType, VAbstractPattern::NodeElArc);
            break;
        case (Tool::NodePoint):
            doc->SetAttribute(nod, AttrType, VAbstractPattern::NodePoint);
            break;
        case (Tool::NodeSpline):
            doc->SetAttribute(nod, AttrType, VAbstractPattern::NodeSpline);
            break;
        case (Tool::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, VAbstractPattern::NodeSplinePath);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }

    {
        const unsigned char angleType = static_cast<unsigned char>(node.GetAngleType());

        if (angleType > 0)
        {
            doc->SetAttribute(nod, AttrAngle, angleType);
        }
    }

    if (type == Tool::NodePoint)
    {
        doc->SetAttribute(nod, VAbstractPattern::AttrNodePassmark, node.IsPassmark());
        doc->SetAttribute(nod, VAbstractPattern::AttrNodePassmarkLine,
                          PassmarkLineTypeToString(node.GetPassmarkLineType()));
        doc->SetAttribute(nod, VAbstractPattern::AttrNodePassmarkAngle,
                          PassmarkAngleTypeToString(node.GetPassmarkAngleType()));

        if (not node.IsPassmark()
                && node.GetPassmarkLineType() == PassmarkLineType::OneLine
                && node.GetPassmarkAngleType() == PassmarkAngleType::Straightforward)
        { // For backward compatebility.
            nod.removeAttribute(VAbstractPattern::AttrNodePassmark);
            nod.removeAttribute(VAbstractPattern::AttrNodePassmarkLine);
            nod.removeAttribute(VAbstractPattern::AttrNodePassmarkAngle);
        }
    }
    else
    { // Wrong configuration.
        nod.removeAttribute(VAbstractPattern::AttrNodePassmark);
        nod.removeAttribute(VAbstractPattern::AttrNodePassmarkLine);
        nod.removeAttribute(VAbstractPattern::AttrNodePassmarkAngle);
    }

    doc->SetAttributeOrRemoveIf(nod, VAbstractPattern::AttrNodeShowSecondPassmark, node.IsShowSecondPassmark(),
                                node.IsShowSecondPassmark());

    doc->SetAttributeOrRemoveIf(nod, VAbstractPattern::AttrManualPassmarkLength, node.IsManualPassmarkLength(),
                                not node.IsManualPassmarkLength());
    doc->SetAttributeOrRemoveIf(nod, VAbstractPattern::AttrPassmarkLength, node.GetFormulaPassmarkLength(),
                                not node.IsManualPassmarkLength());

    return nod;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::AddNode(VAbstractPattern *doc, QDomElement &domElement, const VPieceNode &node)
{
    domElement.appendChild(AddSANode(doc, VAbstractPattern::TagNode, node));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VAbstractTool::PrepareNodes(const VPiecePath &path, VMainGraphicsScene *scene,
                                                VAbstractPattern *doc, VContainer *data)
{
    QVector<VPieceNode> nodes;
    for (int i = 0; i< path.CountNodes(); ++i)
    {
        VPieceNode nodeD = path.at(i);
        const quint32 id = PrepareNode(nodeD, scene, doc, data);
        if (id > NULL_ID)
        {
            nodeD.SetId(id);
            nodes.append(nodeD);
        }
    }
    return nodes;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractTool::PrepareNode(const VPieceNode &node, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                   VContainer *data)
{
    SCASSERT(scene != nullptr)
    SCASSERT(doc != nullptr)
    SCASSERT(data != nullptr)

    VAbstractNodeInitData initData;
    initData.idObject = node.GetId();
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;
    initData.scene = scene;

    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
            initData.id = CreateNode<VPointF>(data, node.GetId());
            VNodePoint::Create(initData);
            break;
        case (Tool::NodeArc):
            initData.id = CreateNode<VArc>(data, node.GetId());
            VNodeArc::Create(initData);
            break;
        case (Tool::NodeElArc):
            initData.id = CreateNode<VEllipticalArc>(data, node.GetId());
            VNodeEllipticalArc::Create(initData);
            break;
        case (Tool::NodeSpline):
            initData.id = CreateNodeSpline(data, node.GetId());
            VNodeSpline::Create(initData);
            break;
        case (Tool::NodeSplinePath):
            initData.id = CreateNodeSplinePath(data, node.GetId());
            VNodeSplinePath::Create(initData);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    return initData.id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractTool::CreateNodeSpline(VContainer *data, quint32 id)
{
    if (data->GetGObject(id)->getType() == GOType::Spline)
    {
        return VAbstractTool::CreateNode<VSpline>(data, id);
    }
    else
    {
        return VAbstractTool::CreateNode<VCubicBezier>(data, id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractTool::CreateNodeSplinePath(VContainer *data, quint32 id)
{
    if (data->GetGObject(id)->getType() == GOType::SplinePath)
    {
        return VAbstractTool::CreateNode<VSplinePath>(data, id);
    }
    else
    {
        return VAbstractTool::CreateNode<VCubicBezierPath>(data, id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractTool::CreateNodePoint(VContainer *data, quint32 id, const QSharedPointer<VPointF> &point)
{
    const quint32 pointId = CreateNode<VPointF>(data, id);
    QSharedPointer<VPointF> p = data->GeometricObject<VPointF>(pointId);
    p->SetShowLabel(point->IsShowLabel());
    p->setMx(point->mx());
    p->setMy(point->my());
    data->UpdateGObject(pointId, point);
    return pointId;
}

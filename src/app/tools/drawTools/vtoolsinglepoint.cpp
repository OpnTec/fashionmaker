/************************************************************************
 **
 **  @file   vtoolsinglepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vtoolsinglepoint.h"
#include "../../dialogs/tools/dialogsinglepoint.h"
#include "../../widgets/vgraphicssimpletextitem.h"
#include "../../undocommands/movespoint.h"
#include "../../undocommands/addpatternpiece.h"
#include "../../geometry/vpointf.h"

const QString VToolSinglePoint::ToolType = QStringLiteral("single");

//---------------------------------------------------------------------------------------------------------------------
VToolSinglePoint::VToolSinglePoint (VPattern *doc, VContainer *data, quint32 id, const Valentina::Sources &typeCreation,
                                    const QString &namePP, const QString &mPath, QGraphicsItem * parent )
    :VToolPoint(doc, data, id, parent), namePP(namePP), mPath(mPath)
{
    baseColor = Qt::red;
    currentColor = baseColor;
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    ignoreFullUpdate = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    setColorLabel(Qt::black);
    if (typeCreation == Valentina::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setData(p->name(), p->toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::AddToFile()
{
    Q_ASSERT_X(namePP.isEmpty() == false, "AddToFile", "name pattern piece is empty");

    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement sPoint = doc->createElement(TagName);

    // Create SPoint tag
    doc->SetAttribute(sPoint, VDomDocument::AttrId, id);
    doc->SetAttribute(sPoint, AttrType, ToolType);
    doc->SetAttribute(sPoint, AttrName, point->name());
    doc->SetAttribute(sPoint, AttrX, qApp->fromPixel(point->x()));
    doc->SetAttribute(sPoint, AttrY, qApp->fromPixel(point->y()));
    doc->SetAttribute(sPoint, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(sPoint, AttrMy, qApp->fromPixel(point->my()));

    //Create pattern piece structure
    QDomElement patternPiece = doc->createElement(VPattern::TagDraw);
    doc->SetAttribute(patternPiece, AttrName, namePP);

    QDomElement calcElement = doc->createElement(VPattern::TagCalculation);
    calcElement.appendChild(sPoint);

    patternPiece.appendChild(calcElement);
    patternPiece.appendChild(doc->createElement(VPattern::TagModeling));
    patternPiece.appendChild(doc->createElement(VPattern::TagDetails));

    AddPatternPiece *addPP = new AddPatternPiece(patternPiece, doc, namePP, mPath);
    connect(addPP, &AddPatternPiece::ClearScene, doc, &VPattern::ClearScene);
    connect(addPP, &AddPatternPiece::NeedFullParsing, doc, &VPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addPP);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(point->x())));
        doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(point->y())));
        doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(point->mx())));
        doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(point->my())));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - this is new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (rect.contains(newPos) == false)
        {
            // Save element into rect of scene.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    if (change == ItemPositionHasChanged && scene())
    {
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        // value - this is new position.
        QPointF newPos = value.toPointF();

        MoveSPoint *moveSP = new MoveSPoint(doc, newPos.x(), newPos.y(), id, this->scene());
        connect(moveSP, &MoveSPoint::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(moveSP);
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::decrementReferens()
{
    if (_referens > 1)
    {
        --_referens;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QPointF p = dialogTool->getPoint();
    QString name = dialogTool->getName();
    doc->SetAttribute(domElement, AttrName, name);
    doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(p.x())));
    doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(p.y())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::setColorLabel(const Qt::GlobalColor &color)
{
    namePoint->setBrush(color);
    lineName->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
    ContextMenu<DialogSinglePoint>(this, event, false);
}

//---------------------------------------------------------------------------------------------------------------------
void  VToolSinglePoint::FullUpdateFromFile()
{
    VPointF point = *VAbstractTool::data.GeometricObject<const VPointF *>(id);
    RefreshPointGeometry(point);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::black);
    }
    else
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::gray);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*(VAbstractTool::data.GeometricObject<const VPointF *>(id)));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolSinglePoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogSinglePoint>(this, event, false);
}

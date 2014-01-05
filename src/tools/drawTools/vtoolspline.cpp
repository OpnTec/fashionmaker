/************************************************************************
 **
 **  @file   vtoolspline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vtoolspline.h"
#include "../../geometry/vspline.h"

const QString VToolSpline::TagName = QStringLiteral("spline");
const QString VToolSpline::ToolType = QStringLiteral("simple");

VToolSpline::VToolSpline(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                         QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), dialogSpline(QSharedPointer<DialogSpline>()),
      controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;

    const VSpline *spl = data->GeometricObject<const VSpline *>(id);
    QPainterPath path;
    path.addPath(spl->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);

    VControlPointSpline *controlPoint1 = new VControlPointSpline(1, SplinePoint::FirstPoint, spl->GetP2(),
                                                                 spl->GetP1().toQPointF(), this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint1, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint1, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint1);

    VControlPointSpline *controlPoint2 = new VControlPointSpline(1, SplinePoint::LastPoint, spl->GetP3(),
                                                                 spl->GetP4().toQPointF(), this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(this, &VToolSpline::RefreshLine, controlPoint2, &VControlPointSpline::RefreshLine);
    connect(this, &VToolSpline::setEnabledPoint, controlPoint2, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint2);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolSpline::setDialog()
{
    Q_ASSERT(dialogSpline.isNull() == false);
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    dialogSpline->setP1(spl->GetP1().id());
    dialogSpline->setP4(spl->GetP4().id());
    dialogSpline->setAngle1(spl->GetAngle1());
    dialogSpline->setAngle2(spl->GetAngle2());
    dialogSpline->setKAsm1(spl->GetKasm1());
    dialogSpline->setKAsm2(spl->GetKasm2());
    dialogSpline->setKCurve(spl->GetKcurve());
}

void VToolSpline::Create(QSharedPointer<DialogSpline> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data)
{
    qint64 p1 = dialog->getP1();
    qint64 p4 = dialog->getP4();
    qreal kAsm1 = dialog->getKAsm1();
    qreal kAsm2 = dialog->getKAsm2();
    qreal angle1 = dialog->getAngle1();
    qreal angle2 = dialog->getAngle2();
    qreal kCurve = dialog->getKCurve();
    Create(0, p1, p4, kAsm1, kAsm2, angle1, angle2, kCurve, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolSpline::Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                         const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                         VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                         const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VPointF point1 = *data->GeometricObject<const VPointF *>(p1);
    VPointF point4 = *data->GeometricObject<const VPointF *>(p4);
    VSpline *spline = new VSpline(point1, point4, angle1, angle2, kAsm1, kAsm2, kCurve);
    Q_ASSERT(spline != 0);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(spline);
        data->AddLengthSpline(spline->name(), toMM(spline->GetLength()));
    }
    else
    {
        data->UpdateGObject(id, spline);
        data->AddLengthSpline(spline->name(), toMM(spline->GetLength()));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::SplineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolSpline *spl = new VToolSpline(doc, data, id, typeCreation);
        scene->addItem(spl);
        connect(spl, &VToolSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, spl, &VToolSpline::SetFactor);
        doc->AddTool(id, spl);
        doc->IncrementReferens(p1);
        doc->IncrementReferens(p4);
    }
}

void VToolSpline::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolSpline::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        VPointF point1 = *VAbstractTool::data.GeometricObject<const VPointF *>(dialogSpline->getP1());
        VPointF point4 = *VAbstractTool::data.GeometricObject<const VPointF *>(dialogSpline->getP4());
        VSpline spl = VSpline (point1, point4, dialogSpline->getAngle1(), dialogSpline->getAngle2(),
                               dialogSpline->getKAsm1(), dialogSpline->getKAsm2(), dialogSpline->getKCurve());

        disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        controlPoints[0]->setPos(spl.GetP2());
        controlPoints[1]->setPos(spl.GetP3());
        connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);
        connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VToolSpline::ControlPointChangePosition);

        spl = VSpline (point1, controlPoints[0]->pos(), controlPoints[1]->pos(), point4, dialogSpline->getKCurve());
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrPoint1, spl.GetP1().id());
            domElement.setAttribute(AttrPoint4, spl.GetP4().id());
            domElement.setAttribute(AttrAngle1, spl.GetAngle1());
            domElement.setAttribute(AttrAngle2, spl.GetAngle2());
            domElement.setAttribute(AttrKAsm1, spl.GetKasm1());
            domElement.setAttribute(AttrKAsm2, spl.GetKasm2());
            domElement.setAttribute(AttrKCurve, spl.GetKcurve());
            emit FullUpdateTree();
        }
    }
    dialogSpline.clear();
}

void VToolSpline::ControlPointChangePosition(const qint32 &indexSpline, const SplinePoint::Position &position,
                                             const QPointF &pos)
{
    Q_UNUSED(indexSpline);
    const VSpline *spline = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    VSpline spl;
    if (position == SplinePoint::FirstPoint)
    {
        spl = VSpline(spline->GetP1(), pos, spline->GetP3(), spline->GetP4(), spline->GetKcurve());
    }
    else
    {
        spl = VSpline(spline->GetP1(), spline->GetP2(), pos, spline->GetP4(), spline->GetKcurve());
    }
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrAngle1, QString().setNum(spl.GetAngle1()));
        domElement.setAttribute(AttrAngle2, QString().setNum(spl.GetAngle2()));
        domElement.setAttribute(AttrKAsm1, QString().setNum(spl.GetKasm1()));
        domElement.setAttribute(AttrKAsm2, QString().setNum(spl.GetKasm2()));
        domElement.setAttribute(AttrKCurve, QString().setNum(spl.GetKcurve()));
        emit FullUpdateTree();
    }
}

void VToolSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogSpline, this, event);
}

void VToolSpline::AddToFile()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrPoint1, spl->GetP1().id());
    AddAttribute(domElement, AttrPoint4, spl->GetP4().id());
    AddAttribute(domElement, AttrAngle1, spl->GetAngle1());
    AddAttribute(domElement, AttrAngle2, spl->GetAngle2());
    AddAttribute(domElement, AttrKAsm1, spl->GetKasm1());
    AddAttribute(domElement, AttrKAsm2, spl->GetKasm2());
    AddAttribute(domElement, AttrKCurve, spl->GetKcurve());

    AddToCalculation(domElement);
}

void VToolSpline::RefreshDataInFile()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrPoint1, spl->GetP1().id());
        domElement.setAttribute(AttrPoint4, spl->GetP4().id());
        domElement.setAttribute(AttrAngle1, spl->GetAngle1());
        domElement.setAttribute(AttrAngle2, spl->GetAngle2());
        domElement.setAttribute(AttrKAsm1, spl->GetKasm1());
        domElement.setAttribute(AttrKAsm2, spl->GetKasm2());
        domElement.setAttribute(AttrKCurve, spl->GetKcurve());
    }
}

void VToolSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine/factor));
}

void VToolSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine/factor));
}

void VToolSpline::RemoveReferens()
{
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    doc->DecrementReferens(spl->GetP1().id());
    doc->DecrementReferens(spl->GetP4().id());
}

QVariant VToolSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
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

    return QGraphicsItem::itemChange(change, value);
}

void VToolSpline::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

void VToolSpline::RefreshGeometry()
{
    this->setPen(QPen(currentColor, widthHairLine/factor));
    const VSpline *spl = VAbstractTool::data.GeometricObject<const VSpline *>(id);
    QPainterPath path;
    path.addPath(spl->GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    QPointF splinePoint = VAbstractTool::data.GeometricObject<const VPointF *>(spl->GetP1().id())->toQPointF();
    QPointF controlPoint = spl->GetP2();
    emit RefreshLine(1, SplinePoint::FirstPoint, controlPoint, splinePoint);
    splinePoint = VAbstractTool::data.GeometricObject<const VPointF *>(spl->GetP4().id())->toQPointF();
    controlPoint = spl->GetP3();
    emit RefreshLine(1, SplinePoint::LastPoint, controlPoint, splinePoint);

    disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    controlPoints[0]->setPos(spl->GetP2());
    controlPoints[1]->setPos(spl->GetP3());
    connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
    connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VToolSpline::ControlPointChangePosition);
}


void VToolSpline::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, widthHairLine/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    emit setEnabledPoint(selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolSpline::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolSpline::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

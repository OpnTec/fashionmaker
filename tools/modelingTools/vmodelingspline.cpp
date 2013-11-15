/************************************************************************
 **
 **  @file vmodelingspline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#include "vmodelingspline.h"
#include "../../geometry/vspline.h"

const QString VModelingSpline::TagName = QStringLiteral("spline");
const QString VModelingSpline::ToolType = QStringLiteral("simple");

VModelingSpline::VModelingSpline(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                 QGraphicsItem *parent)
    :VModelingTool(doc, data, id), QGraphicsPathItem(parent),
    dialogSpline(QSharedPointer<DialogSpline>()), controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;
    VSpline spl = data->GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    VControlPointSpline *controlPoint1 = new VControlPointSpline(1, SplinePoint::FirstPoint, spl.GetP2(),
                                                                 spl.GetPointP1().toQPointF(), this);
    connect(controlPoint1, &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
    connect(this, &VModelingSpline::RefreshLine, controlPoint1, &VControlPointSpline::RefreshLine);
    connect(this, &VModelingSpline::setEnabledPoint, controlPoint1, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint1);

    VControlPointSpline *controlPoint2 = new VControlPointSpline(1, SplinePoint::LastPoint, spl.GetP3(),
                                                                 spl.GetPointP4().toQPointF(), this);
    connect(controlPoint2, &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
    connect(this, &VModelingSpline::RefreshLine, controlPoint2, &VControlPointSpline::RefreshLine);
    connect(this, &VModelingSpline::setEnabledPoint, controlPoint2, &VControlPointSpline::setEnabledPoint);
    controlPoints.append(controlPoint2);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingSpline::setDialog()
{
    Q_ASSERT(dialogSpline.isNull() == false);
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    dialogSpline->setP1(spl.GetP1());
    dialogSpline->setP4(spl.GetP4());
    dialogSpline->setAngle1(spl.GetAngle1());
    dialogSpline->setAngle2(spl.GetAngle2());
    dialogSpline->setKAsm1(spl.GetKasm1());
    dialogSpline->setKAsm2(spl.GetKasm2());
    dialogSpline->setKCurve(spl.GetKcurve());
}

VModelingSpline *VModelingSpline::Create(QSharedPointer<DialogSpline> &dialog, VDomDocument *doc, VContainer *data)
{
    qint64 p1 = dialog->getP1();
    qint64 p4 = dialog->getP4();
    qreal kAsm1 = dialog->getKAsm1();
    qreal kAsm2 = dialog->getKAsm2();
    qreal angle1 = dialog->getAngle1();
    qreal angle2 = dialog->getAngle2();
    qreal kCurve = dialog->getKCurve();
    return Create(0, p1, p4, kAsm1, kAsm2, angle1, angle2, kCurve, doc, data, Document::FullParse,
                  Tool::FromGui);
}

VModelingSpline *VModelingSpline::Create(const qint64 _id, const qint64 &p1, const qint64 &p4,
                                         const qreal &kAsm1, const qreal kAsm2, const qreal &angle1,
                                         const qreal &angle2, const qreal &kCurve, VDomDocument *doc,
                                         VContainer *data, const Document::Documents &parse,
                                         const Tool::Sources &typeCreation)
{
    VModelingSpline *spl = 0;
    VSpline spline = VSpline(data->DataModelingPoints(), p1, p4, angle1, angle2, kAsm1, kAsm2, kCurve);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddModelingSpline(spline);
    }
    else
    {
        data->UpdateModelingSpline(id, spline);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLengthSpline(spline.name(), toMM(spline.GetLength()));
    if (parse == Document::FullParse)
    {
        spl = new VModelingSpline(doc, data, id, typeCreation);
        doc->AddTool(id, spl);
        doc->IncrementReferens(p1);
        doc->IncrementReferens(p4);
    }
    return spl;
}

void VModelingSpline::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VModelingSpline::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        VSpline spl = VSpline (VAbstractTool::data.DataModelingPoints(), dialogSpline->getP1(),
                               dialogSpline->getP4(), dialogSpline->getAngle1(), dialogSpline->getAngle2(),
                               dialogSpline->getKAsm1(), dialogSpline->getKAsm2(), dialogSpline->getKCurve());

        disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSpline::ControlPointChangePosition);
        disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSpline::ControlPointChangePosition);
        controlPoints[0]->setPos(spl.GetP2());
        controlPoints[1]->setPos(spl.GetP3());
        connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSpline::ControlPointChangePosition);
        connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
                &VModelingSpline::ControlPointChangePosition);

        spl = VSpline (VAbstractTool::data.DataModelingPoints(), dialogSpline->getP1(),
                       controlPoints[0]->pos(), controlPoints[1]->pos(), dialogSpline->getP4(),
                       dialogSpline->getKCurve());
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrPoint1, QString().setNum(spl.GetP1()));
            domElement.setAttribute(AttrPoint4, QString().setNum(spl.GetP4()));
            domElement.setAttribute(AttrAngle1, QString().setNum(spl.GetAngle1()));
            domElement.setAttribute(AttrAngle2, QString().setNum(spl.GetAngle2()));
            domElement.setAttribute(AttrKAsm1, QString().setNum(spl.GetKasm1()));
            domElement.setAttribute(AttrKAsm2, QString().setNum(spl.GetKasm2()));
            domElement.setAttribute(AttrKCurve, QString().setNum(spl.GetKcurve()));
            emit FullUpdateTree();
        }
    }
    dialogSpline.clear();
}

void VModelingSpline::ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF &pos)
{
    Q_UNUSED(indexSpline);
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    if (position == SplinePoint::FirstPoint)
    {
        spl.ModifiSpl (spl.GetP1(), pos, spl.GetP3(), spl.GetP4(), spl.GetKcurve());
    }
    else
    {
        spl.ModifiSpl (spl.GetP1(), spl.GetP2(), pos, spl.GetP4(), spl.GetKcurve());
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

void VModelingSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogSpline, this, event);
}

void VModelingSpline::AddToFile()
{
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrPoint1, spl.GetP1());
    AddAttribute(domElement, AttrPoint4, spl.GetP4());
    AddAttribute(domElement, AttrAngle1, spl.GetAngle1());
    AddAttribute(domElement, AttrAngle2, spl.GetAngle2());
    AddAttribute(domElement, AttrKAsm1, spl.GetKasm1());
    AddAttribute(domElement, AttrKAsm2, spl.GetKasm2());
    AddAttribute(domElement, AttrKCurve, spl.GetKcurve());

    AddToModeling(domElement);
}

void VModelingSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingSpline::RemoveReferens()
{
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    doc->DecrementReferens(spl.GetP1());
    doc->DecrementReferens(spl.GetP4());
}

void VModelingSpline::RefreshGeometry()
{
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    QPointF splinePoint = VAbstractTool::data.GetModelingPoint(spl.GetP1()).toQPointF();
    QPointF controlPoint = spl.GetP2();
    emit RefreshLine(1, SplinePoint::FirstPoint, controlPoint, splinePoint);
    splinePoint = VAbstractTool::data.GetModelingPoint(spl.GetP4()).toQPointF();
    controlPoint = spl.GetP3();
    emit RefreshLine(1, SplinePoint::LastPoint, controlPoint, splinePoint);

    disconnect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
    disconnect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
    controlPoints[0]->setPos(spl.GetP2());
    controlPoints[1]->setPos(spl.GetP3());
    connect(controlPoints[0], &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
    connect(controlPoints[1], &VControlPointSpline::ControlPointChangePosition, this,
            &VModelingSpline::ControlPointChangePosition);
}

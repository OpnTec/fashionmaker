/************************************************************************
 **
 **  @file   visline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "visline.h"
#include "../container/vcontainer.h"
#include "../widgets/vapplication.h"
#include "../tools/drawTools/vdrawtool.h"
#include "../container/calculator.h"

//---------------------------------------------------------------------------------------------------------------------
VisLine::VisLine(const VContainer *data, QGraphicsItem *parent)
    :QObject(), QGraphicsLineItem(parent), data(data), factor(VDrawTool::factor), scenePos(QPointF()),
      mainColor(Qt::red), supportColor(Qt::magenta), lineStyle(Qt::SolidLine), point1Id(0), toolTip(QString())
{
    this->setZValue(1);// Show on top real tool
    this->setPen(QPen(mainColor, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
VisLine::~VisLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::SetFactor(qreal factor)
{
    VApplication::CheckFactor(this->factor, factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setLineStyle(const Qt::PenStyle &value)
{
    lineStyle = value;
    this->setPen(QPen(mainColor, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::MousePos(const QPointF &scenePos)
{
    this->scenePos = scenePos;
    RefreshGeometry();
    if (toolTip.isEmpty() == false)
    {
        emit ToolTip(toolTip);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisLine::PointRect()
{
    const qreal radius = qApp->toPixel(DefPointRadius/*mm*/, Unit::Mm);
    QRectF rec = QRectF(0, 0, radius*2/factor, radius*2/factor);
    rec.translate(-rec.center().x(), -rec.center().y());
    return rec;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisLine::FindLength(const QString &expression)
{
    qreal length = 0;
    if (expression.isEmpty())
    {
        length = 0;
    }
    else
    {
        try
        {
            // Replace line return with spaces for calc if exist
            QString formula = expression;
            formula.replace("\n", " ");
            formula = qApp->FormulaFromUser(formula);
            Calculator *cal = new Calculator(data);
            length = cal->EvalFormula(formula);
            delete cal;
        }
        catch (qmu::QmuParserError &e)
        {
            length = 0;
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    return qApp->toPixel(FindVal(expression));
}


//---------------------------------------------------------------------------------------------------------------------
qreal VisLine::FindVal(const QString &expression)
{
    qreal val = 0;
    if (expression.isEmpty())
    {
        val = 0;
    }
    else
    {
        try
        {
            // Replace line return with spaces for calc if exist
            QString formula = expression;
            formula.replace("\n", " ");
            formula = qApp->FormulaFromUser(formula);
            Calculator *cal = new Calculator(data);
            val = cal->EvalFormula(formula);
            delete cal;
        }
        catch (qmu::QmuParserError &e)
        {
            val = 0;
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::DrawLine(QGraphicsLineItem *lineItem, const QLineF &line, const QColor &color, Qt::PenStyle style)
{
    SCASSERT (lineItem != nullptr);

    lineItem->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, style));
    lineItem->setLine(line);
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::DrawPoint(QGraphicsEllipseItem *point, const QPointF &pos, const QColor &color)
{
    SCASSERT (point != nullptr);

    point->setPos(pos);
    point->setPen(QPen(color, qApp->toPixel(qApp->widthMainLine())/factor));
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VisLine::Line(const QPointF &p1, const qreal &length, const qreal &angle)
{
    QLineF line = QLineF();
    line.setP1(p1);
    line.setLength(length);
    line.setAngle(angle);
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisLine::InitPoint(const QColor &color)
{
    QGraphicsEllipseItem *point = new QGraphicsEllipseItem(this);
    point->setZValue(1);
    point->setBrush(QBrush(Qt::NoBrush));
    point->setPen(QPen(color, qApp->toPixel(qApp->widthMainLine())/factor));
    point->setRect(PointRect());
    point->setFlags(QGraphicsItem::ItemStacksBehindParent);
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsLineItem *VisLine::InitLine(const QColor &color)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(this);
    line->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
    line->setZValue(1);
    line->setFlags(QGraphicsItem::ItemStacksBehindParent);
    return line;
}

qreal VisLine::CorrectAngle(const qreal &angle) const
{
    qreal ang = angle;
    if (angle > 360)
    {
        ang = angle - 360 * qFloor(angle/360);
    }

    switch(qFloor((qAbs(ang)+22.5)/45))
    {
        case 0: // <22.5
            return 0;
        case 1: // <67.5
            return 45;
        case 2: // <112.5
            return 90;
        case 3: // <157.5
            return 135;
        case 4: // <202.5
            return 180;
        case 5: // <247.5
            return 225;
        case 6: // < 292.5
            return 270;
        case 7: // <337.5
            return 315;
        default: // <360
            return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VisLine::Ray(const QPointF &firstPoint, const qreal &angle) const
{
    if(this->scene() == nullptr)
    {
        QLineF line = QLineF(firstPoint, scenePos);
        line.setAngle(angle);
        return line.p2();// We can't find ray because item doesn't have scene. We will return cursor position on scene.
    }

    QLineF line = QLineF();
    line.setP1(firstPoint);
    line.setAngle(angle);

    QRectF scRect = this->scene()->sceneRect();
    qreal diagonal = sqrt(pow(scRect.height(), 2) + pow(scRect.width(), 2));
    line.setLength(diagonal);
    if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
    {
        line.setAngle(CorrectAngle(line.angle()));
    }
    return VAbstractTool::LineIntersectRect(scRect, line);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VisLine::Ray(const QPointF &firstPoint) const
{
    QLineF line = QLineF(firstPoint, scenePos);
    return Ray(firstPoint, line.angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setMainColor(const QColor &value)
{
    mainColor = value;
    this->setPen(QPen(mainColor, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setScenePos(const QPointF &value)
{
    scenePos = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::VisualMode(const quint32 &pointId)
{
    VMainGraphicsScene *scene = qApp->getCurrentScene();
    SCASSERT(scene != nullptr);

    this->point1Id = pointId;
    this->scenePos = scene->getScenePos();
    RefreshGeometry();

    scene->addItem(this);
    connect(scene, &VMainGraphicsScene::NewFactor, this, &VisLine::SetFactor);
    connect(scene, &VMainGraphicsScene::mouseMove, this, &VisLine::MousePos);
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setPoint1Id(const quint32 &value)
{
    point1Id = value;
}

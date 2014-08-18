/************************************************************************
 **
 **  @file   visualization.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#include "visualization.h"
#include "../tools/drawTools/vdrawtool.h"
#include "../container/calculator.h"

#include <QGraphicsEllipseItem>

//---------------------------------------------------------------------------------------------------------------------
Visualization::Visualization(const VContainer *data)
    :QObject(), data(data), factor(VDrawTool::factor), scenePos(QPointF()),
      mainColor(Qt::red), supportColor(Qt::magenta), lineStyle(Qt::SolidLine), point1Id(NULL_ID), toolTip(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
Visualization::~Visualization()
{}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setPoint1Id(const quint32 &value)
{
    point1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setLineStyle(const Qt::PenStyle &value)
{
    lineStyle = value;
    InitPen();
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setScenePos(const QPointF &value)
{
    scenePos = value;
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::VisualMode(const quint32 &pointId)
{
    VMainGraphicsScene *scene = qApp->getCurrentScene();
    SCASSERT(scene != nullptr);

    this->point1Id = pointId;
    this->scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::setMainColor(const QColor &value)
{
    mainColor = value;
    InitPen();
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::SetFactor(qreal factor)
{
    VApplication::CheckFactor(this->factor, factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::MousePos(const QPointF &scenePos)
{
    this->scenePos = scenePos;
    RefreshGeometry();
    if (toolTip.isEmpty() == false)
    {
        emit ToolTip(toolTip);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *Visualization::InitPoint(const QColor &color, QGraphicsItem *parent) const
{
    QGraphicsEllipseItem *point = new QGraphicsEllipseItem(parent);
    point->setZValue(1);
    point->setBrush(QBrush(Qt::NoBrush));
    point->setPen(QPen(color, qApp->toPixel(qApp->widthMainLine())/factor));
    point->setRect(PointRect(qApp->toPixel(DefPointRadius/*mm*/, Unit::Mm)));
    point->setFlags(QGraphicsItem::ItemStacksBehindParent);
    point->setVisible(false);
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
QRectF Visualization::PointRect(const qreal &radius) const
{
    QRectF rec = QRectF(0, 0, radius*2/factor, radius*2/factor);
    rec.translate(-rec.center().x(), -rec.center().y());
    return rec;
}

//---------------------------------------------------------------------------------------------------------------------
qreal Visualization::FindLength(const QString &expression)
{
    return qApp->toPixel(FindVal(expression));
}

//---------------------------------------------------------------------------------------------------------------------
qreal Visualization::FindVal(const QString &expression)
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
            Calculator *cal = new Calculator(Visualization::data);
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
void Visualization::DrawPoint(QGraphicsEllipseItem *point, const QPointF &pos, const QColor &color, Qt::PenStyle style)
{
    SCASSERT (point != nullptr);

    point->setPos(pos);
    point->setPen(QPen(color, qApp->toPixel(qApp->widthMainLine())/factor, style));
    point->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void Visualization::DrawLine(QGraphicsLineItem *lineItem, const QLineF &line, const QColor &color, Qt::PenStyle style)
{
    SCASSERT (lineItem != nullptr);

    lineItem->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, style));
    lineItem->setLine(line);
}

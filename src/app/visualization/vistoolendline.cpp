/************************************************************************
 **
 **  @file   vistoolendline.cpp
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

#include "vistoolendline.h"
#include "../geometry/vpointf.h"
#include "../container/vcontainer.h"
#include "../widgets/vapplication.h"
#include "../tools/vabstracttool.h"
#include "../container/calculator.h"

#include <QGraphicsScene>
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::VisToolEndLine(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), length(0), angle(0), point(nullptr)
{
    this->color = Qt::red;
    this->setZValue(1);// Show on top real tool

    point = new QGraphicsEllipseItem(this);
    point->setVisible(false);
    point->setZValue(1);
    point->setBrush(QBrush(Qt::NoBrush));
    point->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));

    const qreal radius = qApp->toPixel(DefPointRadius/*mm*/, Unit::Mm);
    QRectF rec = QRectF(0, 0, radius*2/factor, radius*2/factor);
    rec.translate(-rec.center().x(), -rec.center().y());
    point->setRect(rec);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::~VisToolEndLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::RefreshGeometry()
{
    const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
    this->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
    QLineF line;
    if (qFuzzyCompare(1 + length, 1 + 0))
    {
        QGraphicsScene *sc = this->scene();
        QPointF second;
        if(sc == nullptr)
        {
            second = scenePos;
        }
        else
        {
            line =  QLineF(first->toQPointF(), scenePos);
            QRectF scRect = sc->sceneRect();
            qreal diagonal = sqrt(pow(scRect.height(), 2) + pow(scRect.width(), 2));
            line.setLength(diagonal);
            if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
            {
                line.setAngle(CorrectAngle(line.angle()));
            }
            second = VAbstractTool::LineIntersectRect(scRect, line);
        }
        line = QLineF(first->toQPointF(), second);
    }
    else
    {
        line = QLineF();
        line.setP1(first->toQPointF());
        line.setLength(qApp->toPixel(length));
        line.setAngle(angle);

        point->setPos(line.p2());
        point->setVisible(true);
        point->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
    }
    this->setLine(line);
    toolTip = QString(tr("<b>Point at distance and angle</b>: angle = %1°; <b>Shift</b> - sticking angle, "
                         "<b>Enter</b> - finish creation")).arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::VisualMode(const quint32 &pointId, const QPointF &scenePos)
{
    this->color = Qt::black;
    this->point1Id = pointId;
    this->scenePos = scenePos;
    RefreshGeometry();
}


//---------------------------------------------------------------------------------------------------------------------
qreal VisToolEndLine::Angle() const
{
    return this->line().angle();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisToolEndLine::CorrectAngle(const qreal &angle) const
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
void VisToolEndLine::setAngle(const qreal &value)
{
    angle = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::setLength(const VContainer *data, const QString &expression)
{
    SCASSERT(data != nullptr);

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
}

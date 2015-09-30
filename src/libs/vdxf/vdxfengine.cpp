/************************************************************************
 **
 **  @file   vdxfengine.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 8, 2015
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

#include "vdxfengine.h"
#include <QDebug>
#include <QDateTime>
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
static inline QPaintEngine::PaintEngineFeatures svgEngineFeatures()
{
#ifdef Q_CC_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

    return QPaintEngine::PaintEngineFeatures(
        QPaintEngine::AllFeatures
        & ~QPaintEngine::PatternBrush
        & ~QPaintEngine::PerspectiveTransform
        & ~QPaintEngine::ConicalGradientFill
        & ~QPaintEngine::PorterDuff);

#ifdef Q_CC_CLANG
#pragma clang diagnostic pop
#endif
}

//---------------------------------------------------------------------------------------------------------------------
VDxfEngine::VDxfEngine()
    :QPaintEngine(svgEngineFeatures()),
      size(), resolution(PrintDPI), fileName(), matrix(), dxf(), dw(),
      varMeasurement(VarMeasurement::Metric), varInsunits(VarInsunits::Centimeters)
{
}

 //---------------------------------------------------------------------------------------------------------------------
VDxfEngine::~VDxfEngine()
{
}

 //---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::begin(QPaintDevice *pdev)
{
    Q_UNUSED(pdev)
    if (size.isValid() == false)
    {
        qWarning()<<"VDxfEngine::begin(), size is not valid";
        return false;
    }

    dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    QByteArray fileNameArray = getFileName().toLocal8Bit();
    dw = dxf->out(fileNameArray.data(), exportVersion);

    if (dw==NULL)
    {
        qWarning("VDxfEngine::begin(), can't open file");
        return false;
    }

    dxf->writeHeader(*dw);

    dxf->writeComment(*dw, "Valentina DXF File");

    dw->dxfString(9, "$ANGDIR"); // 1 = Clockwise angles, 0 = Counterclockwise
    dw->dxfInt(70, 0); // Qt use counterclockwise

    dw->dxfString(9, "$MEASUREMENT"); // Sets drawing units: 0 = English; 1 = Metric
    dw->dxfInt(70, static_cast<int>(varMeasurement));

    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, static_cast<int>(varInsunits));

    QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd.HHmmsszzz");
    dateTime.chop(1);// we need hundredths of a second
    dw->dxfString(9, "$TDCREATE");
    dw->dxfString(40, dateTime.toUtf8().constData());

    dw->sectionEnd();

    dw->sectionTables();
    dxf->writeVPort(*dw);
    dw->tableEnd();
    int numberOfLayers = 1;
    dw->tableLayers(numberOfLayers);

    dxf->writeLayer(*dw,
                   DL_LayerData("0", 0),
                   DL_Attributes(
                       std::string(""),      // leave empty
                       DL_Codes::black,      // default color
                       100,                  // default width
                       "CONTINUOUS",         // default line style
                       1.0));                // default line type scale
    dw->tableEnd();
    dw->sectionEnd();

    dw->sectionEntities();
    return true;
}

 //---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::end()
{
    dw->sectionEnd();
    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;
    return true;
}

 //---------------------------------------------------------------------------------------------------------------------
 // cppcheck-suppress unusedFunction
void VDxfEngine::updateState(const QPaintEngineState &state)
{
    QPaintEngine::DirtyFlags flags = state.state();

    // always stream full gstate, which is not required, but...
    flags |= QPaintEngine::AllDirty;


    if (flags & QPaintEngine::DirtyTransform)
    {
        matrix = state.matrix(); // Save new matrix for moving paths
    }
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawPath(const QPainterPath &path)
{
    QPolygonF polygon = path.toFillPolygon(matrix);
    if (polygon.size() < 3)
    {
        return;
    }

    for (int i=1; i < polygon.count(); i++)
    {
            dxf->writeLine(
            *dw,
            DL_LineData(polygon.at(i-1).x(), // start point
            getSize().height() - polygon.at(i-1).y(),
            0.0,
            polygon.at(i).x(), // end point
            getSize().height() - polygon.at(i).y(),
            0.0),
            DL_Attributes("0", getPenColor(), state->pen().width(), getPenStyle(), 1.0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawLines(const QLineF * lines, int lineCount)
{
    for (int i = 0; i < lineCount; i++)
    {
            QPointF p1 = matrix.map(lines[i].p1());
            QPointF p2 = matrix.map(lines[i].p2());

            dxf->writeLine(
            *dw,
            DL_LineData(p1.x(), // start point
            getSize().height() - p1.y(),
            0.0,
            p2.x(), // end point
            getSize().height() - p2.y(),
            0.0),
            DL_Attributes("0", getPenColor(), state->pen().width(), getPenStyle(), 1.0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawLines(const QLine * lines, int lineCount)
{
    QPaintEngine::drawLines(lines, lineCount);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    Q_UNUSED(mode)

    for (int i = 1; i < pointCount; i++)
    {
            QPointF p1 = matrix.map(points[i-1]);
            QPointF p2 = matrix.map(points[i]);

            dxf->writeLine(
            *dw,
            DL_LineData(p1.x(), // start point
            getSize().height() - p1.y(),
            0.0,
            p2.x(), // end point
            getSize().height() - p2.y(),
            0.0),
            DL_Attributes("0", getPenColor(), state->pen().width(), getPenStyle(), 1.0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawPolygon(const QPoint *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    QPaintEngine::drawPolygon(points, pointCount, mode);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawEllipse(const QRectF & rect)
{
    QRectF newRect = matrix.mapRect(rect);
    double rotationAngle = atan(matrix.m12()/matrix.m11());

    double majorX, majorY; // distanse between center and endpoint of the major axis
    double ratio; // ratio of minor axis to major axis
    if(rect.width()<= rect.height())
    {
        majorX = (rect.top() - rect.center().y())*sin(rotationAngle)*matrix.m11()/cos(rotationAngle);
        // major axis * sin(rotation angle) * x-scale-factor
        majorY = (rect.top() - rect.center().y())*matrix.m22();
        // major axis * cos(rotation angle) * y-scale-factor, where y-scale-factor = matrix.m22()/cos(rotationAngle)
        ratio  = rect.width()/rect.height();
    }
    else
    {
        majorX = (rect.right() - rect.center().x())*matrix.m11();
        // major axis * cos(rotation angle) * x-scale-factor, where y-scale-factor = matrix.m22()/cos(rotationAngle)
        majorY = (rect.right() - rect.center().x())*sin(rotationAngle)*matrix.m22()/cos(rotationAngle);
        // major axis * sin(rotation angle) * y-scale-factor
        ratio  = rect.height()/rect.width();
    }
    dxf->writeEllipse(
                *dw,
                DL_EllipseData(newRect.center().x(),                       // center X
                               getSize().height() - newRect.center().y(),  // center Y
                               0,                                          // center Z
                               majorX,
                               majorY,
                               0,
                               ratio,
                               0,6.28 // startangle and endangle of ellipse in rad
                               ),
                DL_Attributes("0", getPenColor(), state->pen().width(), getPenStyle(), 1.0));
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawEllipse(const QRect & rect)
{
    QPaintEngine::drawEllipse(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawTextItem(const QPointF & p, const QTextItem & textItem)
{
    QPointF startPoint = matrix.map(p);
    double rotationAngle = atan(matrix.m12()/matrix.m11());

    int textSize = textItem.font().pixelSize() == -1 ? textItem.font().pointSize() : textItem.font().pixelSize();
    dxf->writeText(
                *dw,
                DL_TextData(startPoint.x(),
                            getSize().height() - startPoint.y(),
                            0,
                            startPoint.x(),
                            getSize().height() - startPoint.y(),
                            0,
                            textSize * matrix.m11(),
                            1, // relative X scale factor
                            0, // flag (0 = default, 2 = Backwards, 4 = Upside down)
                            0, // Horizontal justification (0 = Left (default), 1 = Center, 2 = Right,)
                            0, // Vertical justification (0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top)
                            textItem.text().toUtf8().constData(),  // text data
                            textItem.font().family().toUtf8().constData(), // font
                            -rotationAngle
                            ),
                DL_Attributes("0", getPenColor(), state->pen().width(), getPenStyle(), 1.0));
}

 //---------------------------------------------------------------------------------------------------------------------
QPaintEngine::Type VDxfEngine::type() const
{
    return QPaintEngine::User;
}

 //---------------------------------------------------------------------------------------------------------------------
 // cppcheck-suppress unusedFunction
void VDxfEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    Q_UNUSED(r)
    Q_UNUSED(pm)
    Q_UNUSED(sr)
}

 //---------------------------------------------------------------------------------------------------------------------
QSize VDxfEngine::getSize() const
{
    return size;
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setSize(const QSize &value)
{
    Q_ASSERT(!isActive());
    size = value;
}

 //---------------------------------------------------------------------------------------------------------------------
int VDxfEngine::getResolution() const
{
    return resolution;
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setResolution(int value)
{
    Q_ASSERT(!isActive());
    resolution = value;
}

 //---------------------------------------------------------------------------------------------------------------------
QString VDxfEngine::getFileName() const
{
    return fileName;
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setFileName(const QString &value)
{
    Q_ASSERT(!isActive());
    fileName = value;
}

//---------------------------------------------------------------------------------------------------------------------
std::string VDxfEngine::getPenStyle()
{
    switch (state->pen().style())
    {
        case Qt::SolidLine:
            return "BYLAYER";
            break;
        case Qt::DashLine:
            return "DASHED";
            break;
        case Qt::DotLine:
            return "DOT";
            break;
        case Qt::DashDotLine:
            return "DASHDOT";
            break;
        case Qt::DashDotDotLine:
            return "DIVIDE";
            break;
        default:
            return "BYLAYER";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VDxfEngine::getPenColor()
{
    QColor color = state->pen().color();

    if(color == Qt::black)
    {
        return DL_Codes::black;
    }
    else if(color == Qt::white)
    {
        return DL_Codes::white;
    }
    else if(color == Qt::darkGray)
    {
        return DL_Codes::gray;
    }
    else if(color == Qt::gray)
    {
        return DL_Codes::l_gray;
    }
    else if(color == Qt::darkMagenta)
    {
        return DL_Codes::magenta;
    }
    else if(color == Qt::magenta)
    {
        return DL_Codes::l_magenta;
    }
    else if(color == Qt::cyan)
    {
        return DL_Codes::l_cyan;
    }
    else if(color == Qt::darkCyan)
    {
        return DL_Codes::cyan;
    }
    else if(color == Qt::blue)
    {
        return DL_Codes::l_blue;
    }
    else if(color == Qt::darkBlue)
    {
        return DL_Codes::blue;
    }
    else if(color == Qt::darkGreen)
    {
        return DL_Codes::green;
    }
    else if(color == Qt::green)
    {
        return DL_Codes::l_green;
    }
    else if(color == Qt::darkRed)
    {
        return DL_Codes::red;
    }
    else if(color == Qt::red)
    {
        return DL_Codes::l_red;
    }
    else if(color == Qt::yellow)
    {
        return DL_Codes::yellow;
    }
    else
    {
        return DL_Codes::bylayer;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setMeasurement(const VarMeasurement &var)
{
    Q_ASSERT(!isActive());
    varMeasurement = var;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setInsunits(const VarInsunits &var)
{
    Q_ASSERT(!isActive());
    varInsunits = var;
}

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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
static inline QPaintEngine::PaintEngineFeatures svgEngineFeatures()
{
#if defined(Q_CC_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#elif defined (Q_CC_INTEL)
#pragma warning( push )
#pragma warning( disable: 68 )
#endif

    return QPaintEngine::PaintEngineFeatures(
        QPaintEngine::AllFeatures
        & ~QPaintEngine::PatternBrush
        & ~QPaintEngine::PerspectiveTransform
        & ~QPaintEngine::ConicalGradientFill
        & ~QPaintEngine::PorterDuff);

#if defined(Q_CC_CLANG)
#pragma clang diagnostic pop
#elif defined(Q_CC_INTEL)
#pragma warning( pop )
#endif
}

//---------------------------------------------------------------------------------------------------------------------
VDxfEngine::VDxfEngine()
    :QPaintEngine(svgEngineFeatures()),
      size(),
      resolution(static_cast<int>(PrintDPI)),
      fileName(),
      matrix(),
      dxf(nullptr),
      dw(nullptr),
      varMeasurement(VarMeasurement::Metric),
      varInsunits(VarInsunits::Centimeters)
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

    if (dw==nullptr)
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

    dw->dxfString(9, "$DIMSCALE");
    dw->dxfReal(40, 1.0);

    // Official documentation says that initial value is 1.0, however LibreCAD has trouble if not set this value
    // explicitly.
    dw->dxfString(9, "$DIMLFAC");
    dw->dxfReal(40, 1.0);

    QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd.HHmmsszzz");
    dateTime.chop(1);// we need hundredths of a second
    dw->dxfString(9, "$TDCREATE");
    dw->dxfString(40, dateTime.toUtf8().constData());

    dw->sectionEnd();

    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(25);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("ACAD_ISO02W100", "ACAD_ISO02W100", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("ACAD_ISO03W100", "ACAD_ISO03W100", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("ACAD_ISO04W100", "ACAD_ISO04W100", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("ACAD_ISO05W100", "ACAD_ISO05W100", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDER", "BORDER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDER2", "BORDER2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDERX2", "BORDERX2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTER", "CENTER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTER2", "CENTER2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTERX2", "CENTERX2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT", "DASHDOT", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT2", "DASHDOT2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOTX2", "DASHDOTX2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHED", "DASHED", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHED2", "DASHED2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHEDX2", "DASHEDX2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE", "DIVIDE", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE2", "DIVIDE2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDEX2", "DIVIDEX2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOT", "DOT", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOT2", "DOT2", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOTX2", "DOTX2", 0, 0, 0.0));
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

    dw->tableStyle(1);
    DL_StyleData style("Standard", 0, 0.0, 1.0, 0.0, 0, 2.5, "txt", "");
    style.bold = false;
    style.italic = false;
    dxf->writeStyle(*dw, style);
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);
    dw->tableAppid(1);
    dw->tableAppidEntry(0x12);
    dw->dxfString(2, "ACAD");
    dw->dxfInt(70, 0);
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "layout");
    dw->tableEnd();

    dw->sectionEnd();

    dw->sectionBlocks();
    dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw, DL_BlockData("layout", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "layout");

    dw->sectionEnd();

    dw->sectionEntities();
    return true;
}

 //---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::end()
{
    dw->sectionEnd();
    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);
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
    const QList<QPolygonF> subpaths = path.toSubpathPolygons(matrix);

    for (int j=0; j < subpaths.size(); ++j)
    {
        const QPolygonF polygon = subpaths.at(j);
        if (polygon.isEmpty())
        {
            return;
        }

        dxf->writePolyline(*dw,
                           DL_PolylineData(polygon.size(), 0, 0, 0),
                           DL_Attributes("0", getPenColor(), -1, getPenStyle(), 1.0));

        for (int i=0; i < polygon.count(); ++i)
        {
            dxf->writeVertex(*dw, 
                             DL_VertexData(FromPixel(polygon.at(i).x(), varInsunits), 
                                           FromPixel(getSize().height() - polygon.at(i).y(), varInsunits), 0, 0));
        }

        dxf->writePolylineEnd(*dw);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawLines(const QLineF * lines, int lineCount)
{
    for (int i = 0; i < lineCount; ++i)
    {
        const QPointF p1 = matrix.map(lines[i].p1());
        const QPointF p2 = matrix.map(lines[i].p2());

        dxf->writeLine(*dw,
                       DL_LineData(FromPixel(p1.x(), varInsunits), // start point
                                   FromPixel(getSize().height() - p1.y(), varInsunits),
                                   FromPixel(0.0, varInsunits),
                                   FromPixel(p2.x(), varInsunits), // end point
                                   FromPixel(getSize().height() - p2.y(), varInsunits),
                                   FromPixel(0.0, varInsunits)),
                       DL_Attributes("0", getPenColor(), -1, getPenStyle(), 1.0));
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

    if (pointCount <= 0)
    {
        return;
    }

    dxf->writePolyline(*dw,
                       DL_PolylineData(pointCount, 0, 0, 0),
                       DL_Attributes("0", getPenColor(), -1, getPenStyle(), 1.0));

    for (int i = 0; i < pointCount; ++i)
    {
        const QPointF p = matrix.map(points[i]);
        dxf->writeVertex(*dw, 
                         DL_VertexData(FromPixel(p.x(), varInsunits), 
                                       FromPixel(getSize().height() - p.y(), varInsunits), 0, 0));
    }

    dxf->writePolylineEnd(*dw);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawPolygon(const QPoint *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    QPaintEngine::drawPolygon(points, pointCount, mode);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawEllipse(const QRectF & rect)
{
    const QRectF newRect = matrix.mapRect(rect);
    const double rotationAngle = atan(matrix.m12()/matrix.m11());

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

    dxf->writeEllipse(*dw,
                      DL_EllipseData(FromPixel(newRect.center().x(), varInsunits),                      // center X
                                     FromPixel(getSize().height() - newRect.center().y(), varInsunits), // center Y
                                     FromPixel(0, varInsunits),                                         // center Z
                                     FromPixel(majorX, varInsunits),
                                     FromPixel(majorY, varInsunits),
                                     FromPixel(0, varInsunits),
                                     FromPixel(majorY, varInsunits),
                                     0, 6.28 // startangle and endangle of ellipse in rad
                                     ),
                      DL_Attributes("0", getPenColor(), -1, getPenStyle(), 1.0));
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawEllipse(const QRect & rect)
{
    QPaintEngine::drawEllipse(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawTextItem(const QPointF & p, const QTextItem & textItem)
{
    const QPointF startPoint = matrix.map(p);
    const double rotationAngle = atan(matrix.m12()/matrix.m11());

    const QFont f = textItem.font();
    const int textSize = f.pixelSize() == -1 ? f.pointSize() : f.pixelSize();
    dxf->writeText(*dw,
                   DL_TextData(FromPixel(startPoint.x(), varInsunits),
                               FromPixel(getSize().height() - startPoint.y(), varInsunits),
                               FromPixel(0, varInsunits),
                               FromPixel(startPoint.x(), varInsunits),
                               FromPixel(getSize().height() - startPoint.y(), varInsunits),
                               FromPixel(0, varInsunits),
                               textSize * matrix.m11(),
                               1, // relative X scale factor
                               0, // flag (0 = default, 2 = Backwards, 4 = Upside down)
                               0, // Horizontal justification (0 = Left (default), 1 = Center, 2 = Right,)
                               0, // Vertical justification (0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top)
                               textItem.text().toUtf8().constData(),  // text data
                               f.family().toUtf8().constData(), // font
                               -rotationAngle
                               ),
                   DL_Attributes("0", getPenColor(), -1, getPenStyle(), 1.0));
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
    Q_ASSERT(not isActive());
    size = value;
}

 //---------------------------------------------------------------------------------------------------------------------
double VDxfEngine::getResolution() const
{
    return resolution;
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setResolution(double value)
{
    Q_ASSERT(not isActive());
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
    Q_ASSERT(not isActive());
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
    Q_ASSERT(not isActive());
    varMeasurement = var;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::setInsunits(const VarInsunits &var)
{
    Q_ASSERT(not isActive());
    varInsunits = var;
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_CC_GNU)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-default"
#endif

double VDxfEngine::FromPixel(double pix, const VarInsunits &unit) const
{
    switch (unit)
    {
        case VarInsunits::Millimeters:
            return pix / resolution * 25.4;
        case VarInsunits::Centimeters:
            return pix / resolution * 25.4 / 10.0;
        case VarInsunits::Inches:
            return pix / resolution;
    }
    return 0;
}

#if defined(Q_CC_GNU)
    #pragma GCC diagnostic pop
#endif

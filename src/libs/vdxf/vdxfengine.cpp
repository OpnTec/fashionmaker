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

#include <QByteArray>
#include <QColor>
#include <QDateTime>
#include <QFlag>
#include <QFlags>
#include <QFont>
#include <QLineF>
#include <QList>
#include <QMessageLogger>
#include <QPaintEngineState>
#include <QPainterPath>
#include <QPen>
#include <QPolygonF>
#include <QTextItem>
#include <Qt>
#include <QtDebug>

#include "../vmisc/def.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/vmath.h"
#include "dxiface.h"

//---------------------------------------------------------------------------------------------------------------------
static inline QPaintEngine::PaintEngineFeatures svgEngineFeatures()
{
QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wsign-conversion")
QT_WARNING_DISABLE_INTEL(68)

    return QPaintEngine::PaintEngineFeatures(
        QPaintEngine::AllFeatures
        & ~QPaintEngine::PatternBrush
        & ~QPaintEngine::PerspectiveTransform
        & ~QPaintEngine::ConicalGradientFill
        & ~QPaintEngine::PorterDuff);

QT_WARNING_POP
}

//---------------------------------------------------------------------------------------------------------------------
VDxfEngine::VDxfEngine()
    :QPaintEngine(svgEngineFeatures()),
      size(),
      resolution(static_cast<int>(PrintDPI)),
      fileName(),
      m_version(DRW::AC1014),
      m_binary(false),
      matrix(),
      input(),
      varMeasurement(VarMeasurement::Metric),
      varInsunits(VarInsunits::Centimeters)
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

    input = QSharedPointer<dx_iface>(new dx_iface(fileName.toStdString(), varMeasurement, varInsunits));
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::end()
{
    const bool res = input->fileExport(m_version, m_binary);
    return res;
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
            continue;
        }

        if (m_version > DRW::AC1009)
        { // Use lwpolyline
            DRW_LWPolyline *poly = new DRW_LWPolyline();
            poly->layer = "0";
            poly->color = getPenColor();
            poly->lWeight = DRW_LW_Conv::widthByLayer;
            poly->lineType = getPenStyle();

            if (polygon.size() > 1 && polygon.first() == polygon.last())
            {
                poly->flags |= 0x1; // closed
            }

            poly->flags |= 0x80; // plinegen

            for (int i=0; i < polygon.count(); ++i)
            {
                poly->addVertex(DRW_Vertex2D(FromPixel(polygon.at(i).x(), varInsunits),
                                             FromPixel(getSize().height() - polygon.at(i).y(), varInsunits), 0));
            }

            input->AddEntity(poly);
        }
        else
        { // Use polyline
            DRW_Polyline *poly = new DRW_Polyline();
            poly->layer = "0";
            poly->color = getPenColor();
            poly->lWeight = DRW_LW_Conv::widthByLayer;
            poly->lineType = getPenStyle();
            if (polygon.size() > 1 && polygon.first() == polygon.last())
            {
                poly->flags |= 0x1; // closed
            }

            poly->flags |= 0x80; // plinegen

            for (int i=0; i < polygon.count(); ++i)
            {
                poly->addVertex(DRW_Vertex(FromPixel(polygon.at(i).x(), varInsunits),
                                           FromPixel(getSize().height() - polygon.at(i).y(), varInsunits), 0, 0));
            }

            input->AddEntity(poly);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawLines(const QLineF * lines, int lineCount)
{
    for (int i = 0; i < lineCount; ++i)
    {
        const QPointF p1 = matrix.map(lines[i].p1());
        const QPointF p2 = matrix.map(lines[i].p2());

        DRW_Line *line = new DRW_Line();
        line->basePoint = DRW_Coord(FromPixel(p1.x(), varInsunits),
                                    FromPixel(getSize().height() - p1.y(), varInsunits), 0);
        line->secPoint =  DRW_Coord(FromPixel(p2.x(), varInsunits),
                                    FromPixel(getSize().height() - p2.y(), varInsunits), 0);
        line->layer = "0";
        line->color = getPenColor();
        line->lWeight = DRW_LW_Conv::widthByLayer;
        line->lineType = getPenStyle();

        input->AddEntity(line);
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

    if (m_version > DRW::AC1009)
    { // Use lwpolyline
        DRW_LWPolyline *poly = new DRW_LWPolyline();
        poly->layer = "0";
        poly->color = getPenColor();
        poly->lWeight = DRW_LW_Conv::widthByLayer;
        poly->lineType = getPenStyle();

        if (pointCount > 1 && points[0] == points[pointCount])
        {
            poly->flags |= 0x1; // closed
        }

        poly->flags |= 0x80; // plinegen

        for (int i = 0; i < pointCount; ++i)
        {
            const QPointF p = matrix.map(points[i]);
            poly->addVertex(DRW_Vertex2D(FromPixel(p.x(), varInsunits),
                                         FromPixel(getSize().height() - p.y(), varInsunits), 0));
        }

        input->AddEntity(poly);
    }
    else
    { // Use polyline
        DRW_Polyline *poly = new DRW_Polyline();
        poly->layer = "0";
        poly->color = getPenColor();
        poly->lWeight = DRW_LW_Conv::widthByLayer;
        poly->lineType = getPenStyle();

        if (pointCount > 1 && points[0] == points[pointCount])
        {
            poly->flags |= 0x1; // closed
        }

        poly->flags |= 0x80; // plinegen

        for (int i = 0; i < pointCount; ++i)
        {
            const QPointF p = matrix.map(points[i]);
            poly->addVertex(DRW_Vertex(FromPixel(p.x(), varInsunits),
                                       FromPixel(getSize().height() - p.y(), varInsunits), 0, 0));
        }

        input->AddEntity(poly);
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

    DRW_Ellipse *ellipse = new DRW_Ellipse();
    ellipse->basePoint = DRW_Coord(FromPixel(newRect.center().x(), varInsunits),
                                   FromPixel(getSize().height() - newRect.center().y(), varInsunits), 0);
    ellipse->secPoint = DRW_Coord(FromPixel(majorX, varInsunits), FromPixel(majorY, varInsunits), 0);
    ellipse->ratio = ratio;
    ellipse->staparam = 0;
    ellipse->endparam = 2*M_PI;

    ellipse->layer = "0";
    ellipse->color = getPenColor();
    ellipse->lWeight = DRW_LW_Conv::widthByLayer;
    ellipse->lineType = getPenStyle();

    input->AddEntity(ellipse);
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

    if (m_version > DRW::AC1009)
    { // Use MTEXT
        DRW_MText *text = new DRW_MText();
        text->basePoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                    FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        text->secPoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                   FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        text->height = textSize * matrix.m11();
        text->text = textItem.text().toStdString();
        text->style = f.family().toStdString();
        text->angle = -rotationAngle;

        text->layer = "0";
        text->color = getPenColor();
        text->lWeight = DRW_LW_Conv::widthByLayer;
        text->lineType = getPenStyle();

        input->AddEntity(text);
    }
    else
    { // Use TEXT
        DRW_Text *text = new DRW_Text();
        text->basePoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                    FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        text->secPoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                   FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        text->height = textSize * matrix.m11();
        text->text = textItem.text().toStdString();
        text->style = f.family().toStdString();
        text->angle = -rotationAngle;

        text->layer = "0";
        text->color = getPenColor();
        text->lWeight = DRW_LW_Conv::widthByLayer;
        text->lineType = getPenStyle();

        input->AddEntity(text);
    }
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
DRW::Version VDxfEngine::GetVersion() const
{
    return m_version;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::SetVersion(DRW::Version version)
{
    Q_ASSERT(not isActive());
    m_version = version;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::SetBinaryFormat(bool binary)
{
    m_binary = binary;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::IsBinaryFormat() const
{
    return m_binary;
}

//---------------------------------------------------------------------------------------------------------------------
std::string VDxfEngine::getPenStyle()
{
    switch (state->pen().style())
    {
        case Qt::DashLine:
            return "DASHED";
        case Qt::DotLine:
            return "DOT";
        case Qt::DashDotLine:
            return "DASHDOT2";
        case Qt::DashDotDotLine:
            return "DIVIDE2";
        case Qt::SolidLine:
        default:
            return "BYLAYER";
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VDxfEngine::getPenColor()
{
    QColor color = state->pen().color();

    if(color == Qt::black)
    {
        return DRW::black;
    }
    else if(color == Qt::white)
    {
        return DRW::white;
    }
    else if(color == Qt::darkGray)
    {
        return DRW::gray;
    }
    else if(color == Qt::gray)
    {
        return DRW::l_gray;
    }
    else if(color == Qt::darkMagenta)
    {
        return DRW::magenta;
    }
    else if(color == Qt::magenta)
    {
        return DRW::l_magenta;
    }
    else if(color == Qt::cyan)
    {
        return DRW::l_cyan;
    }
    else if(color == Qt::darkCyan)
    {
        return DRW::cyan;
    }
    else if(color == Qt::blue)
    {
        return DRW::l_blue;
    }
    else if(color == Qt::darkBlue)
    {
        return DRW::blue;
    }
    else if(color == Qt::darkGreen)
    {
        return DRW::green;
    }
    else if(color == Qt::green)
    {
        return DRW::l_green;
    }
    else if(color == Qt::darkRed)
    {
        return DRW::red;
    }
    else if(color == Qt::red)
    {
        return DRW::l_red;
    }
    else if(color == Qt::yellow)
    {
        return DRW::yellow;
    }
    else
    {
        return DRW::ColorByLayer;
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
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

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

QT_WARNING_POP

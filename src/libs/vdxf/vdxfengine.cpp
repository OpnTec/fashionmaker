 /************************************************************************
 **
 **  @file   vdxfengine.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 8, 2015
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
#include <QTextCodec>
#include <QTextItem>
#include <Qt>
#include <QtDebug>

#include "../vmisc/def.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/vmath.h"
#include "dxiface.h"
#include "../vlayout/vlayoutpiece.h"
#include "../vgeometry/vgeometrydef.h"

static const qreal AAMATextHeight = 2.5;

namespace
{
QVector<QPointF> PieceOutline(const VLayoutPiece &detail)
{
    QVector<QPointF> outline;
    if (detail.IsSeamAllowance() && not detail.IsSeamAllowanceBuiltIn())
    {
        outline = detail.GetMappedSeamAllowancePoints();
    }
    else
    {
        outline = detail.GetMappedContourPoints();
    }
    return outline;
}
}

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
      varInsunits(VarInsunits::Millimeters),
      textBuffer(new DRW_Text())
{
}

//---------------------------------------------------------------------------------------------------------------------
VDxfEngine::~VDxfEngine()
{
    delete textBuffer;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::begin(QPaintDevice *pdev)
{
    Q_UNUSED(pdev)

    if (isActive())
    {
        qWarning("VDxfEngine::begin(), the engine was alredy activated");
        return false;
    }

    if (size.isValid() == false)
    {
        qWarning()<<"VDxfEngine::begin(), size is not valid";
        return false;
    }

    input = QSharedPointer<dx_iface>(new dx_iface(getFileNameForLocale(), m_version, varMeasurement,
                                                  varInsunits));
    input->AddQtLTypes();
    input->AddDefLayers();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::end()
{
    const bool res = input->fileExport(m_binary);
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
        matrix = state.transform(); // Save new matrix for moving paths
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::drawPath(const QPainterPath &path)
{
    const QList<QPolygonF> subpaths = path.toSubpathPolygons(matrix);

    for (auto polygon : subpaths)
    {
        if (polygon.isEmpty())
        {
            continue;
        }

        if (m_version > DRW::AC1009)
        { // Use lwpolyline
            DRW_LWPolyline *poly = new DRW_LWPolyline();
            poly->layer = '0';
            poly->color = getPenColor();
            poly->lWeight = DRW_LW_Conv::widthByLayer;
            poly->lineType = getPenStyle();

            if (polygon.size() > 1 && polygon.first() == polygon.last())
            {
                poly->flags |= 0x1; // closed
            }

            poly->flags |= 0x80; // plinegen

            for (auto p : polygon)
            {
                poly->addVertex(DRW_Vertex2D(FromPixel(p.x(), varInsunits),
                                             FromPixel(getSize().height() - p.y(), varInsunits), 0));
            }

            input->AddEntity(poly);
        }
        else
        { // Use polyline
            DRW_Polyline *poly = new DRW_Polyline();
            poly->layer = '0';
            poly->color = getPenColor();
            poly->lWeight = DRW_LW_Conv::widthByLayer;
            poly->lineType = getPenStyle();
            if (polygon.size() > 1 && polygon.first() == polygon.last())
            {
                poly->flags |= 0x1; // closed
            }

            poly->flags |= 0x80; // plinegen

            for (auto p : polygon)
            {
                poly->addVertex(DRW_Vertex(FromPixel(p.x(), varInsunits),
                                           FromPixel(getSize().height() - p.y(), varInsunits), 0, 0));
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
        line->layer = '0';
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
        poly->layer = '0';
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
        poly->layer = '0';
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

    ellipse->layer = '0';
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
    if (textBuffer->text.size() == 0)
    {
        const QPointF startPoint = matrix.map(p);
        const double rotationAngle = qRadiansToDegrees(qAtan2(matrix.m12(), matrix.m11()));

        const QFont f = textItem.font();
        const UTF8STRING fontStyle = input->AddFont(f);

        textBuffer->basePoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                    FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        textBuffer->secPoint = DRW_Coord(FromPixel(startPoint.x(), varInsunits),
                                   FromPixel(getSize().height() - startPoint.y(), varInsunits), 0);
        textBuffer->height = FromPixel(QFontMetrics(f).height(), varInsunits);

        textBuffer->style = fontStyle;
        textBuffer->angle = -rotationAngle;

        textBuffer->layer = '0';
        textBuffer->color = getPenColor();
        textBuffer->lWeight = DRW_LW_Conv::widthByLayer;
        textBuffer->lineType = getPenStyle();
    }

    /* Because QPaintEngine::drawTextItem doesn't pass whole string per time we mark end of each string by adding
     * special placholder. */
    QString t = textItem.text();
    const bool foundEndOfString = t.contains(endStringPlaceholder);

    if (foundEndOfString)
    {
        t.replace(endStringPlaceholder, QString());
    }

    textBuffer->text += t.toStdString();

    if (foundEndOfString)
    {
        input->AddEntity(textBuffer);
        textBuffer = new DRW_Text();
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
qreal VDxfEngine::GetXScale() const
{
    return m_xscale;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::SetXScale(const qreal &xscale)
{
    m_xscale = xscale;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VDxfEngine::GetYScale() const
{
    return m_yscale;
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::SetYScale(const qreal &yscale)
{
    m_yscale = yscale;
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
    return pix;
}

//---------------------------------------------------------------------------------------------------------------------
double VDxfEngine::ToPixel(double val, const VarInsunits &unit) const
{
    switch (unit)
    {
        case VarInsunits::Millimeters:
            return (val / 25.4) * resolution;
        case VarInsunits::Centimeters:
            return ((val * 10.0) / 25.4) * resolution;
        case VarInsunits::Inches:
            return val * resolution;
    }
    return val;
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::ExportToAAMA(const QVector<VLayoutPiece> &details)
{
    if (size.isValid() == false)
    {
        qWarning()<<"VDxfEngine::begin(), size is not valid";
        return false;
    }

    input = QSharedPointer<dx_iface>(new dx_iface(getFileNameForLocale(), m_version, varMeasurement,
                                                  varInsunits));
    input->AddAAMAHeaderData();
    if (m_version > DRW::AC1009)
    {
        input->AddDefLayers();
    }
    input->AddAAMALayers();

    ExportStyleSystemText(input, details);

    for(auto detail : details)
    {
        dx_ifaceBlock *detailBlock = new dx_ifaceBlock();

        QString blockName = detail.GetName();
        if (m_version <= DRW::AC1009)
        {
            blockName.replace(' ', '_');
        }

        detailBlock->name = blockName.toStdString();
        detailBlock->layer = '1';

        detail.Scale(m_xscale, m_yscale);

        ExportAAMAOutline(detailBlock, detail);
        ExportAAMADraw(detailBlock, detail);
        ExportAAMAIntcut(detailBlock, detail);
        ExportAAMANotch(detailBlock, detail);
        ExportAAMAGrainline(detailBlock, detail);
        ExportPieceText(detailBlock, detail);
        ExportAAMADrill(detailBlock, detail);

        input->AddBlock(detailBlock);

        DRW_Insert *insert = new DRW_Insert();
        insert->name = blockName.toStdString();
        insert->layer = '1';

        input->AddEntity(insert);
    }

    return input->fileExport(m_binary);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMAOutline(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    DRW_Entity *e = AAMAPolygon(PieceOutline(detail), QChar('1'), true);
    if (e)
    {
        detailBlock->ent.push_back(e);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMADraw(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    if (not detail.IsHideMainPath() && not detail.IsSeamAllowanceBuiltIn())
    {
        if (DRW_Entity *e = AAMAPolygon(detail.GetMappedContourPoints(), QChar('8'), true))
        {
            detailBlock->ent.push_back(e);
        }
    }

    const QVector<QVector<QPointF>> drawIntCut = detail.InternalPathsForCut(false);
    for(auto &intCut : drawIntCut)
    {
        if (DRW_Entity *e = AAMAPolygon(intCut, QChar('8'), false))
        {
            detailBlock->ent.push_back(e);
        }
    }

    const QVector<VLayoutPlaceLabel> labels = detail.GetPlaceLabels();
    for(auto &label : labels)
    {
        if (label.type != PlaceLabelType::Doubletree && label.type != PlaceLabelType::Button
                && label.type != PlaceLabelType::Circle)
        {
            for(auto &p : qAsConst(label.shape))
            {
                if (DRW_Entity *e = AAMAPolygon(p, QChar('8'), false))
                {
                    detailBlock->ent.push_back(e);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMAIntcut(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    QVector<QVector<QPointF>> drawIntCut = detail.InternalPathsForCut(true);
    for(auto &intCut : drawIntCut)
    {
        if (DRW_Entity *e = AAMAPolygon(intCut, "11", false))
        {
            detailBlock->ent.push_back(e);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMANotch(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    if (detail.IsSeamAllowance())
    {
        const QVector<VLayoutPassmark> passmarks = detail.GetPassmarks();
        for(auto &passmark : passmarks)
        {
            for (auto &line : passmark.lines)
            {
                if (DRW_Entity *e = AAMALine(line, QChar('4')))
                {
                    detailBlock->ent.push_back(e);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMAGrainline(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    const QVector<QPointF> grainline = detail.GetGrainline();
    if (grainline.count() > 1)
    {
        if (DRW_Entity *e = AAMALine(QLineF(grainline.first(), grainline.last()), QChar('7')))
        {
            detailBlock->ent.push_back(e);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportPieceText(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    const QStringList list = detail.GetPieceText();
    const QPointF startPos = detail.GetPieceTextPosition();

    for (int i = 0; i < list.size(); ++i)
    {
        QPointF pos(startPos.x(), startPos.y() - ToPixel(AAMATextHeight * m_yscale, varInsunits)*(list.size() - i-1));
        detailBlock->ent.push_back(AAMAText(pos, list.at(i), QChar('1')));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportStyleSystemText(const QSharedPointer<dx_iface> &input, const QVector<VLayoutPiece> &details)
{
    for(auto &detail : details)
    {
        const QStringList strings = detail.GetPatternText();
        if (not strings.isEmpty())
        {
            for (int j = 0; j < strings.size(); ++j)
            {
                QPointF pos(0, getSize().height() -
                                   ToPixel(AAMATextHeight * m_yscale, varInsunits)*(strings.size() - j-1));
                input->AddEntity(AAMAText(pos, strings.at(j), QChar('1')));
            }
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportAAMADrill(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    const QVector<VLayoutPlaceLabel> labels = detail.GetPlaceLabels();

    for(auto &label : labels)
    {
        if (label.type == PlaceLabelType::Doubletree || label.type == PlaceLabelType::Button
                || label.type == PlaceLabelType::Circle)
        {
            const QPointF center = detail.GetMatrix().map(label.center);
            DRW_Point *point = new DRW_Point();
            point->basePoint = DRW_Coord(FromPixel(center.x(), varInsunits),
                                         FromPixel(getSize().height() - center.y(), varInsunits), 0);
            point->layer = "13";

            detailBlock->ent.push_back(point);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfEngine::ExportToASTM(const QVector<VLayoutPiece> &details)
{
    if (size.isValid() == false)
    {
        qWarning()<<"VDxfEngine::begin(), size is not valid";
        return false;
    }

    input = QSharedPointer<dx_iface>(new dx_iface(getFileNameForLocale(), m_version, varMeasurement,
                                                  varInsunits));

    input->AddAAMAHeaderData();
    if (m_version > DRW::AC1009)
    {
        input->AddDefLayers();
    }
    input->AddASTMLayers();

    ExportStyleSystemText(input, details);

    for(auto detail : details)
    {
        dx_ifaceBlock *detailBlock = new dx_ifaceBlock();

        QString blockName = detail.GetName();
        if (m_version <= DRW::AC1009)
        {
            blockName.replace(' ', '_');
        }

        detailBlock->name = blockName.toStdString();
        detailBlock->layer = '1';

        detail.Scale(m_xscale, m_yscale);

        ExportASTMPieceBoundary(detailBlock, detail);
        ExportASTMSewLine(detailBlock, detail);
        ExportASTMInternalLine(detailBlock, detail);
        ExportASTMInternalCutout(detailBlock, detail);
        ExportASTMNotch(detailBlock, detail);
        ExportAAMAGrainline(detailBlock, detail);
        ExportPieceText(detailBlock, detail);
        ExportASTMDrill(detailBlock, detail);
        ExportASTMAnnotationText(detailBlock, detail);

        input->AddBlock(detailBlock);

        DRW_Insert *insert = new DRW_Insert();
        insert->name = blockName.toStdString();
        insert->layer = '1';

        input->AddEntity(insert);
    }

    return input->fileExport(m_binary);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMPieceBoundary(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    QVector<QPointF> pieceBoundary = PieceOutline(detail);

    // Piece boundary
    DRW_Entity *e = AAMAPolygon(PieceOutline(detail), QChar('1'), true);
    if (e)
    {
        detailBlock->ent.push_back(e);
    }

    // Piece boundary quality validation curves
    DRW_Entity *q = AAMAPolygon(PieceOutline(detail), "84", true);
    if (q)
    {
        detailBlock->ent.push_back(q);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMSewLine(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    if (not detail.IsHideMainPath() && not detail.IsSeamAllowanceBuiltIn())
    {
        QVector<QPointF> sewLine = detail.GetMappedContourPoints();

        // Sew lines
        if (DRW_Entity *e = AAMAPolygon(sewLine, "14", true))
        {
            detailBlock->ent.push_back(e);
        }

        // Sew lines quality validation curves
        if (DRW_Entity *e = AAMAPolygon(sewLine, "87", true))
        {
            detailBlock->ent.push_back(e);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMInternalLine(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    const QVector<QVector<QPointF>> drawIntCut = detail.InternalPathsForCut(false);
    for(auto &intCut : drawIntCut)
    {
        // Internal line
        if (DRW_Entity *e = AAMAPolygon(intCut, QChar('8'), false))
        {
            detailBlock->ent.push_back(e);
        }

        // Internal lines quality validation curves
        if (DRW_Entity *e = AAMAPolygon(intCut, "85", false))
        {
            detailBlock->ent.push_back(e);
        }
    }

    const QVector<VLayoutPlaceLabel> labels = detail.GetPlaceLabels();
    for(auto &label : labels)
    {
        if (label.type != PlaceLabelType::Doubletree && label.type != PlaceLabelType::Button
            && label.type != PlaceLabelType::Circle)
        {
            for(auto &p : qAsConst(label.shape))
            {
                // Internal line (placelabel)
                if (DRW_Entity *e = AAMAPolygon(p, QChar('8'), false))
                {
                    detailBlock->ent.push_back(e);
                }

                // Internal lines quality validation curves
                if (DRW_Entity *e = AAMAPolygon(p, "85", false))
                {
                    detailBlock->ent.push_back(e);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMInternalCutout(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    QVector<QVector<QPointF>> drawIntCut = detail.InternalPathsForCut(true);
    for(auto &intCut : drawIntCut)
    {
        // Internal cutout
        if (DRW_Entity *e = AAMAPolygon(intCut, "11", false))
        {
            detailBlock->ent.push_back(e);
        }

        // Internal cutouts quality validation curves
        if (DRW_Entity *e = AAMAPolygon(intCut, "86", false))
        {
            detailBlock->ent.push_back(e);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMAnnotationText(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    QString name = detail.GetName();
    QPointF textPos = detail.VLayoutPiece::DetailBoundingRect().center();

    QPointF pos(textPos.x(), textPos.y() - ToPixel(AAMATextHeight, varInsunits));
    detailBlock->ent.push_back(AAMAText(pos, name, "15"));
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMDrill(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    const QVector<VLayoutPlaceLabel> labels = detail.GetPlaceLabels();

    for(auto &label : labels)
    {
        if (label.type == PlaceLabelType::Doubletree || label.type == PlaceLabelType::Button
            || label.type == PlaceLabelType::Circle)
        {
            const QPointF center = detail.GetMatrix().map(label.center);
            DRW_Point *point = new DRW_Point();
            point->basePoint = DRW_Coord(FromPixel(center.x(), varInsunits),
                                         FromPixel(getSize().height() - center.y(), varInsunits), 0);
            point->layer = "13";

            detailBlock->ent.push_back(point);

            // TODO. Investigate drill category
//            QPointF pos(center.x(), center.y() - ToPixel(AAMATextHeight, varInsunits));
//            detailBlock->ent.push_back(AAMAText(pos, category, "13"));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfEngine::ExportASTMNotch(dx_ifaceBlock *detailBlock, const VLayoutPiece &detail)
{
    if (detail.IsSeamAllowance())
    {
        const QVector<VLayoutPassmark> passmarks = detail.GetPassmarks();
        for(auto &passmark : passmarks)
        {
            DRW_ASTMNotch *notch = new DRW_ASTMNotch();
            const QPointF center = passmark.baseLine.p1();

            notch->basePoint = DRW_Coord(FromPixel(center.x(), varInsunits),
                                         FromPixel(getSize().height() - center.y(), varInsunits),
                                         FromPixel(passmark.baseLine.length(), varInsunits));

            notch->angle = passmark.baseLine.angle();

            if (passmark.type == PassmarkLineType::OneLine || passmark.type == PassmarkLineType::TwoLines
                || passmark.type == PassmarkLineType::ThreeLines)
            { // Slit notch
                notch->layer = "4";
            }
            else if (passmark.type == PassmarkLineType::VMark || passmark.type == PassmarkLineType::VMark2)
            {
                QLineF boundaryLine = QLineF(passmark.lines.first().p2(), passmark.lines.last().p2());
                notch->thickness = FromPixel(boundaryLine.length(), varInsunits); // width

                notch->layer = "4";
            }
            else if (passmark.type == PassmarkLineType::TMark)
            {
                qreal width = FromPixel(passmark.lines.last().length(), varInsunits);
                notch->thickness = FromPixel(width, varInsunits);

                notch->layer = "80";
            }
            else if (passmark.type == PassmarkLineType::BoxMark)
            {
                QPointF start = passmark.lines.first().p1();
                QPointF end = passmark.lines.last().p2();

                notch->layer = "81";

                notch->thickness = FromPixel(QLineF(start, end).length(), varInsunits);
            }
            else if (passmark.type == PassmarkLineType::UMark)
            {
                QPointF start = passmark.lines.first().p1();
                QPointF end = passmark.lines.last().p2();

                notch->thickness = FromPixel(QLineF(start, end).length(), varInsunits);

                notch->layer = "83";
            }

            detailBlock->ent.push_back(notch);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
DRW_Entity *VDxfEngine::AAMAPolygon(const QVector<QPointF> &polygon, const QString &layer, bool forceClosed)
{
    if (polygon.isEmpty())
    {
        return nullptr;
    }

    if (m_version > DRW::AC1009)
    { // Use lwpolyline
        return CreateAAMAPolygon<DRW_LWPolyline, DRW_Vertex2D>(polygon, layer, forceClosed);
    }
    else
    { // Use polyline
        return CreateAAMAPolygon<DRW_Polyline, DRW_Vertex>(polygon, layer, forceClosed);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DRW_Entity *VDxfEngine::AAMALine(const QLineF &line, const QString &layer)
{
    DRW_Line *lineEnt = new DRW_Line();
    lineEnt->basePoint = DRW_Coord(FromPixel(line.p1().x(), varInsunits),
                                   FromPixel(getSize().height() - line.p1().y(), varInsunits), 0);
    lineEnt->secPoint =  DRW_Coord(FromPixel(line.p2().x(), varInsunits),
                                   FromPixel(getSize().height() - line.p2().y(), varInsunits), 0);
    lineEnt->layer = layer.toStdString();

    return lineEnt;
}

//---------------------------------------------------------------------------------------------------------------------
DRW_Entity *VDxfEngine::AAMAText(const QPointF &pos, const QString &text, const QString &layer)
{
    DRW_Text *textLine = new DRW_Text();

    textLine->basePoint = DRW_Coord(FromPixel(pos.x(), varInsunits),
                                    FromPixel(getSize().height() - pos.y(), varInsunits), 0);
    textLine->secPoint = DRW_Coord(FromPixel(pos.x(), varInsunits),
                                   FromPixel(getSize().height() - pos.y(), varInsunits), 0);
    textLine->height = AAMATextHeight;
    textLine->layer = layer.toStdString();
    textLine->text = text.toStdString();

    return textLine;
}

//---------------------------------------------------------------------------------------------------------------------
std::string VDxfEngine::FromUnicodeToCodec(const QString &str, QTextCodec *codec)
{
    return codec->fromUnicode(str).toStdString();
}

//---------------------------------------------------------------------------------------------------------------------
std::string VDxfEngine::getFileNameForLocale() const
{
#if defined(Q_OS_WIN)
    return VDxfEngine::FromUnicodeToCodec(fileName, QTextCodec::codecForLocale());
#else
    return fileName.toStdString();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
template<class P, class V>
P *VDxfEngine::CreateAAMAPolygon(const QVector<QPointF> &polygon, const QString &layer, bool forceClosed)
{
    P *poly = new P();
    poly->layer = layer.toStdString();

    if (forceClosed)
    {
        poly->flags |= 0x1; // closed
    }
    else
    {
        if (polygon.size() > 1 && polygon.first() == polygon.last())
        {
            poly->flags |= 0x1; // closed
        }
    }

    for (auto p : polygon)
    {
        poly->addVertex(V(FromPixel(p.x(), varInsunits),
                          FromPixel(getSize().height() - p.y(), varInsunits)));
    }

    return poly;
}

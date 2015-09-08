/************************************************************************
 **
 **  @file   vdxfengine.h
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

#ifndef VDXFENGINE_H
#define VDXFENGINE_H

#include <QPaintEngine>
#include "dxflib/dl_dxf.h"
#include "../vmisc/def.h"
#include "dxfdef.h"

class QTextStream;

class VDxfEngine : public QPaintEngine
{
public:
    VDxfEngine();
    virtual ~VDxfEngine() Q_DECL_OVERRIDE;

    virtual bool begin(QPaintDevice *pdev) Q_DECL_OVERRIDE;
    virtual bool end() Q_DECL_OVERRIDE;
    virtual void updateState(const QPaintEngineState &state) Q_DECL_OVERRIDE;
    virtual void drawPath(const QPainterPath &path) Q_DECL_OVERRIDE;
    virtual void drawLines(const QLineF * lines, int lineCount) Q_DECL_OVERRIDE;
    virtual void drawLines(const QLine * lines, int lineCount) Q_DECL_OVERRIDE;
    virtual void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode) Q_DECL_OVERRIDE;
    virtual void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode) Q_DECL_OVERRIDE;
    virtual void drawEllipse(const QRectF & rect) Q_DECL_OVERRIDE;
    virtual void drawEllipse(const QRect & rect) Q_DECL_OVERRIDE;
    virtual void drawTextItem(const QPointF & p, const QTextItem & textItem) Q_DECL_OVERRIDE;
    virtual Type type() const Q_DECL_OVERRIDE;
    virtual void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) Q_DECL_OVERRIDE;

    QSize getSize() const;
    void setSize(const QSize &value);

    int getResolution() const;
    void setResolution(int value);

    QString getFileName() const;
    void setFileName(const QString &value);

    std::string getPenStyle();
    int getPenColor();

    void setMeasurement(const VarMeasurement &var);
    void setInsunits(const VarInsunits &var);

private:
    Q_DISABLE_COPY(VDxfEngine)
    QSize            size;
    int              resolution;
    QString          fileName;
    QMatrix          matrix;
    DL_Dxf* dxf;
    DL_WriterA* dw;
    VarMeasurement varMeasurement;
    VarInsunits varInsunits;

};

#endif // VDXFENGINE_H
